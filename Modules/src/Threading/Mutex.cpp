#include <windows.h>
#undef min
#undef max
#include "GameLib/GameLib.h"
#include "GameLib/Threading/Mutex.h"
#include "GameLib/Base/Impl/ReferenceType.h"

namespace GameLib {
	namespace Threading {

		// 다중 언록, 다중 록 등을 검출하기 위해서, 내용은 세마포로 해 둔다.
		//Release판에서 Critical Section으로 전환하는 것도 좋을지도 모른다.
		class Mutex::Impl : public ReferenceType {
		public:
			Impl() : mHandle(0) {
				mHandle = CreateSemaphore(NULL, 1, 1, NULL);
			}
			~Impl() {
				CloseHandle(mHandle);
			}
			void lock() {
				WaitForSingleObject(mHandle, INFINITE);
			}
			void unlock() {
				LONG prev;
				ReleaseSemaphore(mHandle, 1, &prev);
				STRONG_ASSERT(prev == 0 && "unlock() is called twice! Check lock-unlock pair.");
			}
			HANDLE mHandle;
		};

		Mutex Mutex::create() {
			Mutex r;
			r.mImpl = NEW Impl();
			return r;
		}

		void Mutex::lock() {
			mImpl->lock();
		}

		void Mutex::unlock() {
			mImpl->unlock();
		}

#define TYPE Mutex
#include "GameLib/Base/Impl/ReferenceTypeTemplate.h"


	} //namespace Threading
} //namespace GameLib
