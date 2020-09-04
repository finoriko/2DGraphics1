#ifndef INCLUDED_GAMELIB_THREADING_MANAGER_H
#define INCLUDED_GAMELIB_THREADING_MANAGER_H

namespace GameLib {
	namespace Threading {

		class Manager {
		public:
			Manager();
			static Manager instance();

			//코어 수 취득(논리 코어 수이므로 실체와는 다를 수 있음)
			int getCoreNumber() const;

			//이하 사용자가 부르는 일은 없다
			static void create(int additionalThreadNumber);
			static void destroy();
		};

	} //namespace Threading
} //namespace GameLib

#endif
