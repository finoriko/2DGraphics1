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
				mCoreNumber(1) { //최소한 1
				mQueueSemaphore = Semaphore::create(0, 0x7fffffff); //최대치는 무한하다
				mLock = Mutex::create();

				// 코어 수 취득
#ifdef NDEBUG //Debug 버전에서는 자동으로 한 개만 만든다.개발 머신으로 움직여 그 밖에 움직이지 않는, 것을 막기 위해.
				HANDLE process = GetCurrentProcess();
				DWORD processMask;
				DWORD systemMask;
				BOOL succeeded = GetProcessAffinityMask(process, &processMask, &systemMask);
				if (succeeded != 0) { //실패해도 곤란하지만...
					for (int i = 1; i < 32; ++i) {
						if (processMask & (1 << i)) {
							++mCoreNumber;
						}
					}
				}
#endif
				mThreadNumber = additionalThreadNumber + mCoreNumber + 1; //최소한 코어수+1(로더용)은 만든다
				mThreads = new HANDLE[mThreadNumber];
				//기동
				for (int i = 0; i < mThreadNumber; ++i) {
					unsigned dummy;
					mThreads[i] = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, execute, this, 0, &dummy));
					SetThreadPriority(mThreads[i], THREAD_PRIORITY_BELOW_NORMAL); //우선도 조금 아래
				}
			}
			~ManagerImpl() {
				//종료 대기
				mLock.lock();
				mEndRequest = true;
				mLock.unlock();

				//스레드 종료 대기
				for (int i = 0; i < mThreadNumber; ++i) {
					while (true) {
						//끝나지 않는 동안 세마포를 늘리며 빙빙 기다린다.decrease에서 기다리고 있기 때문에 increase 하지 않으면 일어나지 않는다.
						mQueueSemaphore.increase(); //큐에 무엇인가 들어 있는 것처럼 꾸며 깨우다
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
					mQueueSemaphore.decrease(); //큐 추가를 기다리다

					mLock.lock();
					if (mEndRequest) {
						mLock.unlock();
						break;
					}
					Entry e = mQueue.front();
					mQueue.pop_front(); //큐에서 꺼내다
					mLock.unlock();

					(*(e.mThread))(); //실행
					e.mFinished->set(); //종료 플래그를 세우다
				}
			}
			void add(Thread* t, Event* finishFlag) {
				Entry e(t, finishFlag);

				mLock.lock();
				mQueue.push_back(e);
				mLock.unlock();

				mQueueSemaphore.increase(); //더했습니다
			}
			int getCoreNumber() const {
				return mCoreNumber;
			}
		private:
			HANDLE* mThreads; //스레드 핸들 배열
			int mThreadNumber;
			bool mEndRequest;
			Semaphore mQueueSemaphore;
			Mutex mLock;
			list< Entry > mQueue; //요구 대기 행렬
			int mCoreNumber;
		};
		extern ManagerImpl* gManagerImpl;

	} //namespace Threading
} //namespace GameLib

#endif
