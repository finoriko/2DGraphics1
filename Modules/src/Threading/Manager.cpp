#include "GameLib/GameLib.h"
#include "GameLib/Threading/Manager.h"
#include "GameLib/WindowCreator/WindowCreator.h"

#include "Threading/ManagerImpl.h"

namespace GameLib 
{
	namespace Threading 
	{
		ManagerImpl* gManagerImpl = 0; //Impl�� ��ü

		Manager::Manager() 
		{
			//������ ������κ����� ȣ���� ������� �ʴ´�
			ASSERT(WindowCreator::isMainThread() && "you must call from MAIN thread");
		}
		Manager Manager::instance() 
		{
			return Manager();
		}

		int Manager::getCoreNumber() const 
		{
			return gManagerImpl->getCoreNumber();
		}

		void Manager::create(int additionalThreadNumber) 
		{
			STRONG_ASSERT(!gManagerImpl);
			gManagerImpl = NEW ManagerImpl(additionalThreadNumber);
		}

	
		void Manager::destroy() 
		{
			SAFE_DELETE(gManagerImpl);
		}


	} //namespace Threading
} //namespace GameLib
