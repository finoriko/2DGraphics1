#ifndef INCLUDED_GAMELIB_FILEIO_ARCHIVE_H
#define INCLUDED_GAMELIB_FILEIO_ARCHIVE_H

#include "GameLib/Base/HashMap.h"
#include "GameLib/base/Compressor.h"
#include <fstream>

namespace GameLib
{
	namespace FileIO
	{
		using namespace std;

		class Archive
		{
		public:
			Archive(const char* name) :
				mFileNumber(0),
				mBlockSize(0),
				mStream(0),
				mDummy(false) {
				if (!name) {
					mDummy = true; //더미 아카이브직접 파일로부터 불러옵니다.
					return;
				}
				//파일을 열어서 멤버에게 가져간다.
				setlocale(LC_ALL, ""); //이것이 없으면 일본어 파일명을 받을 수 없다.
				mStream = NEW ifstream(name, ifstream::binary);
				if (!mStream->good()) {
					cout << "can't open archive: " << name << endl;
					SAFE_DELETE(mStream);
					return;
				}
				// 말미에서 4바이트 전으로 이동
				// 파일 사이즈를 한번 재볼까?에러 체크를 위해서 말이야.
				mStream->seekg(0, ifstream::end);
				streamsize fileSize = mStream->tellg();
				if (fileSize < 8) { //우선 있을 수 없네
					cout << "archive size is illegal ( too small ): " << name << endl;
					SAFE_DELETE(mStream);
					return;
				}
				mStream->seekg(-8, ifstream::end);
				mBlockSize = getUnsigned();
				streamsize tableBegin = getUnsigned() * mBlockSize;
				//테이블 선두로 이동
				if (tableBegin + 12 >= fileSize) { //있을수 없는 일
					cout << "archive size is illegal ( wrong table ): " << name << endl;
					SAFE_DELETE(mStream);
					return;
				}
				mStream->seekg(tableBegin, ifstream::beg);
				//4바이트를 읽으면 파일 수
				mFileNumber = getUnsigned();
				if (fileSize < mFileNumber * 16) { //있을수 없는 일
					cout << "archive size is illegal ( wrong file number ): " << name << endl;
					SAFE_DELETE(mStream);
					return;
				}
				// 해시맵 크기 확보
				mEntries.setCapacity(mFileNumber);
				//나머지는 루프로 돌리면서 읽는다.
				for (int i = 0; i < mFileNumber; ++i) {
					Entry e;
					e.mPosition = getUnsigned();
					e.mSize = getUnsigned();
					e.mOriginalSize = getUnsigned();
					e.mNecessaryBufferSize = getUnsigned();
					int nameLength = getUnsigned();
					//이름은 일단 일시 배열에 넣는다.곧 delete 하는데.
					char* name = NEW char[nameLength + 1]; //종단 NULL로 +1
					mStream->read(name, nameLength);
					name[nameLength] = '\0'; //종단 NULL
					//mEntries는map< char*, Entry >
					mEntries.add(name, e); //map에 저장
					// 정보를 토해보자.맞는가?
					SAFE_DELETE_ARRAY(name);
				}
			}
			~Archive() {
				SAFE_DELETE(mStream);
			}
			void open(
				ifstream** streamOut,
				int* entryIndex,
				const char* name) {
				if (mDummy) {
					*entryIndex = -1;
					setlocale(LC_ALL, ""); //이것이 없으면 일본어 파일명을 받을 수 없다.
					*streamOut = NEW ifstream(name, ifstream::binary);
					if (!((*streamOut)->good())) {
						SAFE_DELETE(*streamOut); //없다
					}
				}
				else {
					if (mFileNumber == 0) { //없는 아카이브(archive)라면 언제나 발견되지 않는다.
						*entryIndex = -1;
						*streamOut = 0;
					}
					else {
						//슬래시와 백슬래시를 모두 접수하기 위해 백슬래시화
						string tName = name;
						for (size_t i = 0; i < tName.size(); ++i) {
							if (tName[i] == '/') {
								tName[i] = '\\';
							}
						}
						int it = mEntries.find(tName);
						if (!mEntries.isEnd(it)) {
							*entryIndex = it;
							*streamOut = mStream;
							const Entry& e = *mEntries.value(*entryIndex);
							streamsize pos = e.mPosition * mBlockSize;
							mStream->seekg(pos, ifstream::beg);
						}
						else {
							*entryIndex = -1;
							*streamOut = 0;
						}
					}
				}
			}
			void getFileSize(streamsize* readSize, streamsize* originalSize, int entryIndex, ifstream* stream) const {
				if (mDummy) {
					stream->seekg(0, ifstream::end);
					streamsize r = stream->tellg();
					stream->seekg(0, ifstream::beg);
					*readSize = *originalSize = r;
				}
				else {
					STRONG_ASSERT(entryIndex >= 0);
					STRONG_ASSERT(stream == mStream);
					const Entry& e = *mEntries.value(entryIndex);
					*readSize = e.mSize;
					*originalSize = e.mOriginalSize;
				}
			}
			void allocate(char** dataOut, int size, int entryIndex) {
				if (mDummy) {
					STRONG_ASSERT(entryIndex == -1);
					*dataOut = NEW char[size + 1]; //친절 설계 NULL 종단.
				}
				else {
					STRONG_ASSERT(entryIndex >= 0);
					const Entry& e = *mEntries.value(entryIndex);
					int allocSize = max(e.mNecessaryBufferSize, e.mOriginalSize + 1); //전개에 필요한 버퍼 사이즈가 있다면 그것도 적는다.
					*dataOut = NEW char[allocSize];
				}
			}
			void read(
				bool* errorOut,
				char* data,
				int size,
				int entryIndex,
				ifstream* stream) {
				bool compressed = false;
				int originalSize = size;
				int necessaryBufferSize = size;
				if (!mDummy) {
					STRONG_ASSERT(stream == mStream);
					STRONG_ASSERT(entryIndex >= 0);
					const Entry& e = *mEntries.value(entryIndex);
					STRONG_ASSERT(e.mSize == size);
					if (e.mOriginalSize != size) {
						compressed = true;
						originalSize = e.mOriginalSize;
						necessaryBufferSize = e.mNecessaryBufferSize;
					}
				}
				int readOffset = 0;
				if (compressed) {
					readOffset = necessaryBufferSize - size; //뒤로 로드. 전개에 여분으로 필요한 경우가 있으므로 original Size가 아니다.
				}
				stream->read(data + readOffset, size);
				if (stream->gcount() != size) { //에러야?
					*errorOut = true;
					return;
				}
				if (compressed) {
					int outSize;
					Compressor::decompress(
						data,
						&outSize,
						data + readOffset,
						size);
					if (outSize != originalSize) {
						cout << "Decompression Failed!" << endl;
						*errorOut = true;
					}
				}
			}
			void close(ifstream** stream) {
				if (mDummy) {
					SAFE_DELETE(*stream);
				}
				else {
					*stream = 0;
				}
			}
		private:
			class Entry {
			public:
				unsigned mPosition;
				int mSize;
				int mOriginalSize;
				int mNecessaryBufferSize;
			};
			unsigned getUnsigned() {
				unsigned char buffer[4];
				mStream->read(reinterpret_cast<char*>(buffer), 4);
				unsigned r = buffer[0];
				r |= (buffer[1] << 8);
				r |= (buffer[2] << 16);
				r |= (buffer[3] << 24);
				return r;
			}
			int mFileNumber;
			unsigned mBlockSize;
			ifstream* mStream;
			HashMap< string, Entry > mEntries;
			bool mDummy;
		};
	};

}
#endif