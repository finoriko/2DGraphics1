#include "GameLib/GameLib.h"
#include "GameLib/Threading/Thread.h"
#include "GameLib/Threading/Event.h"
#include "Threading/ManagerImpl.h"

namespace GameLib {
	namespace Threading {

		class Thread::Impl {
		public:
			Impl() {
				mFinished = Event::create();
			}
			~Impl() {
			}
			Event mFinished;
		};

		Thread::Thread() : mImpl(0) {
		}

		void Thread::start() {
			wait(); //앞것이 있으면 기다린다
			mImpl = new Impl;
			gManagerImpl->add(this, &mImpl->mFinished); //실행 개시
		}

		void Thread::wait() {
			if (mImpl) {
				mImpl->mFinished.wait();
				SAFE_DELETE(mImpl);
			}
		}

		bool Thread::isFinished() {
			bool r = false;
			if (mImpl) {
				if (mImpl->mFinished.isSet()) { //끝났어
					SAFE_DELETE(mImpl);
					r = true;
				}
			}
			else {
				r = true;
			}
			return r;
		}

		Thread::~Thread() {
			STRONG_ASSERT(!mImpl && "You must call wait() or isFinished() before destruction.");
		}

	} //namespace Threading
} //namespace GameLib
