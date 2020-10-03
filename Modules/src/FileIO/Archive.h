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
					mDummy = true; //���� ��ī�̺����� ���Ϸκ��� �ҷ��ɴϴ�.
					return;
				}
				//������ ��� ������� ��������.
				setlocale(LC_ALL, ""); //�̰��� ������ �Ϻ��� ���ϸ��� ���� �� ����.
				mStream = NEW ifstream(name, ifstream::binary);
				if (!mStream->good()) {
					cout << "can't open archive: " << name << endl;
					SAFE_DELETE(mStream);
					return;
				}
				// ���̿��� 4����Ʈ ������ �̵�
				// ���� ����� �ѹ� �纼��?���� üũ�� ���ؼ� ���̾�.
				mStream->seekg(0, ifstream::end);
				streamsize fileSize = mStream->tellg();
				if (fileSize < 8) { //�켱 ���� �� ����
					cout << "archive size is illegal ( too small ): " << name << endl;
					SAFE_DELETE(mStream);
					return;
				}
				mStream->seekg(-8, ifstream::end);
				mBlockSize = getUnsigned();
				streamsize tableBegin = getUnsigned() * mBlockSize;
				//���̺� ���η� �̵�
				if (tableBegin + 12 >= fileSize) { //������ ���� ��
					cout << "archive size is illegal ( wrong table ): " << name << endl;
					SAFE_DELETE(mStream);
					return;
				}
				mStream->seekg(tableBegin, ifstream::beg);
				//4����Ʈ�� ������ ���� ��
				mFileNumber = getUnsigned();
				if (fileSize < mFileNumber * 16) { //������ ���� ��
					cout << "archive size is illegal ( wrong file number ): " << name << endl;
					SAFE_DELETE(mStream);
					return;
				}
				// �ؽø� ũ�� Ȯ��
				mEntries.setCapacity(mFileNumber);
				//�������� ������ �����鼭 �д´�.
				for (int i = 0; i < mFileNumber; ++i) {
					Entry e;
					e.mPosition = getUnsigned();
					e.mSize = getUnsigned();
					e.mOriginalSize = getUnsigned();
					e.mNecessaryBufferSize = getUnsigned();
					int nameLength = getUnsigned();
					//�̸��� �ϴ� �Ͻ� �迭�� �ִ´�.�� delete �ϴµ�.
					char* name = NEW char[nameLength + 1]; //���� NULL�� +1
					mStream->read(name, nameLength);
					name[nameLength] = '\0'; //���� NULL
					//mEntries��map< char*, Entry >
					mEntries.add(name, e); //map�� ����
					// ������ ���غ���.�´°�?
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
					setlocale(LC_ALL, ""); //�̰��� ������ �Ϻ��� ���ϸ��� ���� �� ����.
					*streamOut = NEW ifstream(name, ifstream::binary);
					if (!((*streamOut)->good())) {
						SAFE_DELETE(*streamOut); //����
					}
				}
				else {
					if (mFileNumber == 0) { //���� ��ī�̺�(archive)��� ������ �߰ߵ��� �ʴ´�.
						*entryIndex = -1;
						*streamOut = 0;
					}
					else {
						//�����ÿ� �齽���ø� ��� �����ϱ� ���� �齽����ȭ
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
					*dataOut = NEW char[size + 1]; //ģ�� ���� NULL ����.
				}
				else {
					STRONG_ASSERT(entryIndex >= 0);
					const Entry& e = *mEntries.value(entryIndex);
					int allocSize = max(e.mNecessaryBufferSize, e.mOriginalSize + 1); //������ �ʿ��� ���� ����� �ִٸ� �װ͵� ���´�.
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
					readOffset = necessaryBufferSize - size; //�ڷ� �ε�. ������ �������� �ʿ��� ��찡 �����Ƿ� original Size�� �ƴϴ�.
				}
				stream->read(data + readOffset, size);
				if (stream->gcount() != size) { //������?
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