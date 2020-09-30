#ifndef INCLUDED_GAMELIB_FILEIO_MANAGERIMPL_H
#define INCLUDED_GAMELIB_FILEIO_MANAGERIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/FileIO/Manager.h"
#include "GameLib/Threading/Thread.h"
#include "GameLib/Threading/Mutex.h"
#include "GameLib/Threading/Event.h"
#include "GameLib/Threading/Functions.h"
#include "FileIO/InFileImpl.h"
#include "FileIO/OutFileImpl.h"
#include "FileIO/Archive.h"
#include <map>
#include <list>
#include <string>
#include <sstream>
#include <fstream>

namespace GameLib {
	namespace FileIO {
		using namespace std;
		using namespace GameLib::Threading;

		class ManagerImpl {
		public:
			class OutRequest {
			public:
				OutRequest(
					const char* fileName,
					const char* data,
					int size,
					int id,
					bool isAuto) :
					mFileName(fileName),
					mData(0),
					mSize(size),
					mId(id),
					mIsAuto(isAuto) {
					// ��°�� ����
					mData = NEW char[size];
					for (int i = 0; i < size; ++i) {
						mData[i] = data[i];
					}
				}
				~OutRequest() {
					SAFE_DELETE_ARRAY(mData);
					mSize = 0;
				}
				int size() const {
					return mSize;
				}
				const char* data() const {
					return mData;
				}
				const char* fileName() const {
					return mFileName.c_str();
				}
				bool isAuto() const {
					return mIsAuto;
				}
				int id() const {
					return mId;
				}
				string mFileName;
				char* mData;
				int mSize;
				int mId;
				bool mIsAuto;
			};
			class LoadThread : public Thread {
			public:
				LoadThread(ManagerImpl* manager) : mManager(manager) {
				}
				~LoadThread() {
					wait(); //���Ḧ ��ٸ�
					mManager = 0; //������ ������
				}
			private:
				void operator()() {
					// ���� ��û���� �ʴ� ���� ���� ����
					while (!mManager->isEndRequested()) {
						mManager->update();
					}
				}
				ManagerImpl* mManager;
			};
			typedef map< string, InFile::Impl* > InMap;
			typedef InMap::iterator InIt;
			typedef InMap::const_iterator CInIt;

			typedef map< int, OutFile::Impl* > OutMap;
			typedef OutMap::iterator OutIt;
			typedef OutMap::const_iterator COutIt;

			typedef list< string > InList;
			typedef list< OutRequest* > OutList;

			ManagerImpl(
				const char** archiveNames,
				int archiveNumber,
				Manager::AccessMode am) :
				mThread(0),
				mOutRequestId(0),
				mWarningLimit(0x7fffffff),
				mErrorLimit(0x7fffffff),
				mLoadedSum(0),
				mHaltOnError(true),
				mArchives(0),
				mArchiveNumber(0),
				mAccessMode(am) {
				//��ī�̺� ���� ��
				if (mAccessMode == Manager::MODE_ARCHIVE_FIRST || mAccessMode == Manager::MODE_DIRECT_FIRST) {
					mArchiveNumber = archiveNumber + 1;
				}
				else {
					mArchiveNumber = archiveNumber;
				}
				mArchives = static_cast<Archive*>(OPERATOR_NEW(sizeof(Archive) * mArchiveNumber));

				if (mAccessMode == Manager::MODE_DIRECT_FIRST) {
					//ó�� ���̸� �ִ´�
					new (&mArchives[0]) Archive(0); //����. ���� ���� �б�
					for (int i = 0; i < archiveNumber; ++i) {
						new (&mArchives[i + 1]) Archive(archiveNames[i]);
					}
				}
				else {
					for (int i = 0; i < archiveNumber; ++i) {
						new (&mArchives[i]) Archive(archiveNames[i]);
					}
					if (mAccessMode == Manager::MODE_ARCHIVE_FIRST) {
						new (&mArchives[archiveNumber]) Archive(0); //����. ���� ���� �б�
					}
				}
				mMutex = Mutex::create();
				mEndFlag = Event::create();
				mThread = NEW LoadThread(this);
				mThread->start();
			}
			~ManagerImpl() {
				STRONG_ASSERT(mInFiles.size() == 0 && "FileIO::Manager : you must destroy all files!");
				STRONG_ASSERT(mOutFiles.size() == 0 && "FileIO::Manager : writing files exist. IT MUST BE A BUG.");
				for (OutList::iterator i = mOutRequests.begin(); i != mOutRequests.end(); ++i) {
					if ((*i)->isAuto()) {
						SAFE_DELETE(*i);
					}
					else {
						HALT("FileIO::Manager : you must wait all write request!");
					}
				}
				mEndFlag.set();
				SAFE_DELETE(mThread); //����

				//��ī�̺� ����
				for (int i = 0; i < mArchiveNumber; ++i) {
					mArchives[i].~Archive();
				}
				OPERATOR_DELETE(mArchives);
			}
			InFile::Impl* createInFile(const char* filename) {
				string fn(filename); //string�� �̽�

				mMutex.lock();
				//���� map�� �ִ��� Ȯ��
				InIt it = mInFiles.find(fn);
				if (it == mInFiles.end()) { //�����Ƿ� �����
					InFile::Impl* f = NEW InFile::Impl();
					it = mInFiles.insert(make_pair(fn, f)).first;
					f->setIterator(it);
					//�ε� �䱸
					mInRequests.push_back(fn);
				}
				else {
					it->second->refer();
				}
				mMutex.unlock();
				return it->second;
			}
			void destroyInFile(InFile::Impl* f) {
				mMutex.lock();
				//�̳��� �������̶�� ����
				f->release();
				if (f->referenceCount() == 0) {
					mLoadedSum -= f->size(); //�� �뷮 ����
					mInFiles.erase(f->iterator());
					SAFE_DELETE(f);
				}
				mMutex.unlock();
			}
			OutFile::Impl* createOutFile(const char* filename, const char* data, int size) {
				mMutex.lock();
				//�ڵ����
				OutFile::Impl* f = NEW OutFile::Impl(mOutRequestId);

				//������Ʈ ����
				OutRequest* req = NEW OutRequest(filename, data, size, mOutRequestId, false);
				//�ڵ�,��û�߰�
				mOutFiles.insert(make_pair(mOutRequestId, f));
				mOutRequests.push_back(req);
				++mOutRequestId; //������ ����Ͽ� ��ũ����Ʈ
				mMutex.unlock();

				return f;
			}
			void writeFile(const char* filename, const char* data, int size) {
				mMutex.lock();
				//������Ʈ ����
				OutRequest* req = NEW OutRequest(filename, data, size, mOutRequestId, true);
				++mOutRequestId; //������ ����Ͽ� ��ũ����Ʈ
				//������Ʈ �߰�
				mOutRequests.push_back(req);
				mMutex.unlock();
			}
			void destroyOutFile(OutFile::Impl* f) {
				mMutex.lock();
				//�̳��� �������̶�� ����
				if (f->referenceCount() == 0) {
					OutIt it = mOutFiles.find(f->id());
					STRONG_ASSERT(it != mOutFiles.end());
					mOutFiles.erase(it);
					SAFE_DELETE(f);
				}
				mMutex.unlock();
			}
			void setLimit(int warning, int error) {
				mWarningLimit = warning;
				mErrorLimit = error;
			}
			void enableHaltOnError(bool f) {
				mHaltOnError = f;
			}
			void update() {
				using namespace std;
				//�� �� ������ ��� �ڰ� �����ڴ�
				if (mInRequests.size() == 0 && mOutRequests.size() == 0) {
					Threading::sleep(10);
					return;
				}
				read();
				write();
			}
			void read() {
				ostringstream oss;
				//�ε� �䱸 ������
				mMutex.lock();
				if (mInRequests.size() == 0) { //���⼭ �������� ������ �� ��
					mMutex.unlock();
					return;
				}
				string fn = mInRequests.front(); //����ī��. �� ����Ʈ���� ����� �����Ƿ�
				mInRequests.pop_front();
				mMutex.unlock();

				bool error = false;
				bool notification = false; //������ �ڵ� ������ �˷��ٱ�?

				//���� ����
				int archiveIndex = -1;
				int entryIndex = -1;
				ifstream* stream = 0;
				open(&archiveIndex, &stream, &entryIndex, fn.c_str());

				char* buffer = 0;
				int size = 0;

				if (archiveIndex == -1) { //������ ã�� �� ����
					oss.str("");
					oss << "FileIO : can't open file! ( " << fn.c_str() << " )";
					cout << oss.str().c_str() << endl;
					if (mHaltOnError) {
						HALT("FileIO : can't open file!");
					}
					error = notification = true;
				}
				else {
					//���� �ܰ�. ����� �������� �����մϴ�.
					streamsize readSize;
					streamsize originalSize;
					mArchives[archiveIndex].getFileSize(&readSize, &originalSize, entryIndex, stream);
					if (originalSize >= 0x80000000) { //int�δ� ������ �� ����!
						oss.str("");
						oss << "FileIO : file is too big! 2GB is maximum. ( " << fn.c_str() << " )";
						cout << oss.str().c_str() << endl;
						if (mHaltOnError) {
							HALT("FileIO : file is too big! 2GB is maximum.");
						}
						error = notification = true;
					}
					else if (mLoadedSum + originalSize > mErrorLimit) { //�Ѱ踦 �Ѿ���.
						oss.str("");
						oss << "FileIO : EXCEED ERROR LIMIT! this file is not loaded. ( " << fn.c_str() << " )";
						cout << oss.str().c_str() << endl;
						if (mHaltOnError) {
							HALT("FileIO : EXCEED ERROR LIMIT! this file is not loaded.");
						}
						error = notification = true;
					}
					else if (mLoadedSum + originalSize > mWarningLimit) {
						cout << "FileIO : EXCEED WARNING LIMIT! ( " << fn.c_str() << " )" << endl;
					}

					// �б� �� �ڵ��� �ִ��� üũ
					//�ڵ� �� �־�?���� �ε� �����ų� ���� �ʾ�?
					mMutex.lock();
					InIt it = mInFiles.find(fn);
					if (it == mInFiles.end()) {
						error = true;
					}
					else {
						if (it->second->isFinished()) { //�ٺ���! �����ݾ�!
							error = true;
						}
					}
					mMutex.unlock();

					if (!error) {
						size = static_cast<int>(readSize);
						//���� Ȯ��
						mArchives[archiveIndex].allocate(&buffer, size, entryIndex);
						//�б�
						bool readError = false;
						mArchives[archiveIndex].read(
							&readError,
							buffer,
							size,
							entryIndex,
							stream);
						if (readError) { //���� ���� ����. �߰��� ���� �������ٵ簡.
							oss.str("");
							oss << "FileIO : read error! can't read entire file! ( " << fn.c_str() << " )";
							cout << oss.str().c_str() << endl;
							if (mHaltOnError) {
								HALT("FileIO : read error! can't read entire file!");
							}
							error = notification = true;
						}
						if (!error) {
							//�� �о���
							size = static_cast<int>(originalSize); //�Ʊ���� ���� �� ��������ϱ�
							buffer[size] = '\0'; //�Ƚ� ����. 0����.
						}
					}
					if (stream) {
						mArchives[archiveIndex].close(&stream);
					}
				}
				//���� ó��
				mMutex.lock();
				InIt it = mInFiles.find(fn); //���⼭ �� �� �� �ִ��� Ȯ��
				if (it == mInFiles.end()) { //����
					error = true;
				}
				if (error) {
					SAFE_DELETE_ARRAY(buffer); //���� ����
					if (notification) {
						if (it != mInFiles.end()) {
							it->second->setError(); //���� �÷��� ����
						}
					}
				}
				else { //��������
					it->second->set(buffer, size);
					mLoadedSum += size; //�� �뷮 �߰�
				}
				mMutex.unlock();
			}
			void write() {
				ostringstream oss;
				//������Ʈ ������
				mMutex.lock();
				if (mOutRequests.size() == 0) { //���⼭ �������� ������ �� ��
					mMutex.unlock();
					return;
				}
				OutRequest* req = mOutRequests.front();
				mOutRequests.pop_front();
				mMutex.unlock();

				bool isAuto = req->isAuto();
				int id = req->id();
				//���� ����
				const char* fileName = req->fileName();
				setlocale(LC_ALL, ""); //�̰��� ������ �Ϻ��� ���ϸ��� ���� �� ����.
				ofstream out(fileName, ofstream::binary);
				bool error = false;
				if (!out) {
					oss.str("");
					oss << "FileIO : can't open file! ( " << fileName << " )";
					cout << oss.str().c_str() << endl;
					if (mHaltOnError) {
						HALT("FileIO : can't open file!");
					}
					error = true;
				}
				else {
					//������ ������
					int size = req->size();
					const char* data = req->data();
					const char* readPos = data;
					static const int WRITE_UNIT = 1024 * 1024; //1MB
					int rest = size;
					while (!error) {
						int writeSize = min(rest, WRITE_UNIT);
						out.write(readPos, writeSize);
						readPos += writeSize;
						rest -= writeSize;
						STRONG_ASSERT(rest >= 0); //���� �� ���� ��
						if (rest == 0) {
							break; //��������
						}
						//��¥ �� ���?
						streamsize wroteSize = out.tellp();
						if (wroteSize != (size - rest)) {
							oss.str("");
							oss << "FileIO : write error! can't write entire file! ( " << fileName << " )";
							cout << oss.str().c_str() << endl;
							if (mHaltOnError) {
								STRONG_ASSERT(0 && "FileIO : write error! can't write entire file!");
							}
							error = true;
							break;
						}
						//�ڵ����üũ(�ڵ�����������)
						if (!isAuto) {
							mMutex.lock();
							OutIt it = mOutFiles.find(id);
							if (it == mOutFiles.end()) { //����! ���� �� �Ծ��!
								error = true;
							}
							mMutex.unlock();
						}
					}
				}
				//��� ����
				if (!isAuto) {
					mMutex.lock();
					OutIt it = mOutFiles.find(id);
					if (it != mOutFiles.end()) { //����! ���� �� �Ծ��!
						it->second->setFinished();
						if (error) {
							it->second->setError();
						}
					}
					mMutex.unlock();
				}
				SAFE_DELETE(req); //������Ʈ �ı�
			}
			bool isEndRequested() const {
				return mEndFlag.isSet();
			}
			string createListString() const {
				ostringstream oss;
				CInIt i = mInFiles.begin();
				while (i != mInFiles.end()) {
					oss << i->first;
					oss << '\t' << i->second->size();
					oss << '\t' << i->second->referenceCount() << '\n';
					++i;
				}
				string ret = oss.str();
				return ret;
			}
			void open(int* archiveIndex, ifstream** streamOut, int* entryIndex, const char* name) {
				for (int i = 0; i < mArchiveNumber; ++i) {
					*archiveIndex = i;
					mArchives[i].open(streamOut, entryIndex, name);
					if (*streamOut) { //ã�Ҵ�. ������.
						break;
					}
				}
				if (!(*streamOut)) {
					*archiveIndex = -1; //�ᱹ �� ã�Ҿ��.
				}
			}
			Mutex mMutex;
			LoadThread* mThread;
			Event mEndFlag;
			int mOutRequestId;

			InMap mInFiles;
			OutMap mOutFiles;
			InList mInRequests;
			OutList mOutRequests;

			int mWarningLimit;
			int mErrorLimit;
			int mLoadedSum;
			bool mHaltOnError;

			Archive* mArchives;
			int mArchiveNumber;
			Manager::AccessMode mAccessMode;
		};
		extern ManagerImpl* gManagerImpl;

	} //namespace Sound
} //namespace GameLib

#endif