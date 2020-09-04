#include "GameLib/GameLib.h"
#include "GameLib/Threading/Manager.h"
#include "GameLib/WindowCreator/WindowCreator.h"

#include "Threading/ManagerImpl.h"

namespace GameLib {
	namespace Threading {

		ManagerImpl* gManagerImpl = 0; //Impl의 실체

		Manager::Manager() {
			//별도의 쓰레드로부터의 호출은 허용하지 않는다
			ASSERT(WindowCreator::isMainThread() && "you must call from MAIN thread");
		}

		void Manager::create(int additionalThreadNumber) {
			STRONG_ASSERT(!gManagerImpl);
			gManagerImpl = NEW ManagerImpl(additionalThreadNumber);
		}

		int Manager::getCoreNumber() const {
			return gManagerImpl->getCoreNumber();
		}

		void Manager::destroy() {
			SAFE_DELETE(gManagerImpl);
		}

		Manager Manager::instance() {
			return Manager();
		}

	} //namespace Threading
} //namespace GameLib
