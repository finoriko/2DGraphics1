#ifndef INCLUDED_GAMELIB_THREADING_MANAGERIMPL_H
#define INCLUDED_GAMELIB_THREADING_MANAGERIMPL_H

#include <windows.h>
#include <process.h>
#undef min
#undef max
#include <list>

#include "GameLib/GameLib.h"
#include "GameLib/Threading/Manager.h"
#include "GameLib/Threading/Thread.h"
#include "GameLib/Threading/Event.h"
#include "GameLib/Threading/Mutex.h"
#include "GameLib/Threading/Semaphore.h"

namespace GameLib {
	namespace Threading {
		using namespace std;

		class ManagerImpl {
		public:
			struct Entry {
				Entry() : mThread(0), mFinished(0) {}
				Entry(Thread* t, Event* e) : mThread(t), mFinished(e) {}
				~Entry() {
					mThread = 0;
					mFinished = 0;
				}
				Thread* mThread;
				Event* mFinished;
			};
			ManagerImpl(int additionalThreadNumber) :
				mThreads(0),
				mThreadNumber(0),
				mEndRequest(false),
				mCoreNumber(1) { //�ּ��� 1
				mQueueSemaphore = Semaphore::create(0, 0x7fffffff); //�ִ�ġ�� �����ϴ�
				mLock = Mutex::create();

				// �ھ� �� ���
#ifdef NDEBUG //Debug ���������� �ڵ����� �� ���� �����.���� �ӽ����� ������ �� �ۿ� �������� �ʴ�, ���� ���� ����.
				HANDLE process = GetCurrentProcess();
				DWORD processMask;
				DWORD systemMask;
				BOOL succeeded = GetProcessAffinityMask(process, &processMask, &systemMask);
				if (succeeded != 0) { //�����ص� ���������...
					for (int i = 1; i < 32; ++i) {
						if (processMask & (1 << i)) {
							++mCoreNumber;
						}
					}
				}
#endif
				mThreadNumber = additionalThreadNumber + mCoreNumber + 1; //�ּ��� �ھ��+1(�δ���)�� �����
				mThreads = new HANDLE[mThreadNumber];
				//�⵿
				for (int i = 0; i < mThreadNumber; ++i) {
					unsigned dummy;
					mThreads[i] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, execute, this, 0, &dummy));
					SetThreadPriority(mThreads[i], THREAD_PRIORITY_BELOW_NORMAL); //�켱�� ���� �Ʒ�
				}
			}
			~ManagerImpl() {
				//���� ���
				mLock.lock();
				mEndRequest = true;
				mLock.unlock();

				//������ ���� ���
				for (int i = 0; i < mThreadNumber; ++i) {
					while (true) {
						//������ �ʴ� ���� �������� �ø��� ���� ��ٸ���.decrease���� ��ٸ��� �ֱ� ������ increase ���� ������ �Ͼ�� �ʴ´�.
						mQueueSemaphore.increase(); //ť�� �����ΰ� ��� �ִ� ��ó�� �ٸ� �����
						if (WAIT_OBJECT_0 == WaitForSingleObject(mThreads[i], 1)) {
							CloseHandle(mThreads[i]);
							mThreads[i] = 0;
							break;
						}
					}
				}
				SAFE_DELETE_ARRAY(mThreads);
			}
			static unsigned WINAPI execute(void* p) {
				ManagerImpl* m = reinterpret_cast<ManagerImpl*>(p);
				m->execute();
				return 0;
			}
			void execute() {
				while (true) {
					mQueueSemaphore.decrease(); //ť �߰��� ��ٸ���

					mLock.lock();
					if (mEndRequest) {
						mLock.unlock();
						break;
					}
					Entry e = mQueue.front();
					mQueue.pop_front(); //ť���� ������
					mLock.unlock();

					(*(e.mThread))(); //����
					e.mFinished->set(); //���� �÷��׸� �����
				}
			}
			void add(Thread* t, Event* finishFlag) {
				Entry e(t, finishFlag);

				mLock.lock();
				mQueue.push_back(e);
				mLock.unlock();

				mQueueSemaphore.increase(); //���߽��ϴ�
			}
			int getCoreNumber() const {
				return mCoreNumber;
			}
		private:
			HANDLE* mThreads; //������ �ڵ� �迭
			int mThreadNumber;
			bool mEndRequest;
			Semaphore mQueueSemaphore;
			Mutex mLock;
			list< Entry > mQueue; //�䱸 ��� ���
			int mCoreNumber;
		};
		extern ManagerImpl* gManagerImpl;

	} //namespace Threading
} //namespace GameLib

#endif
