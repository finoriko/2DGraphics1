#ifndef INCLUDED_GAMELIB_SOUND_MANAGER_H
#define INCLUDED_GAMELIB_SOUND_MANAGER_H

namespace GameLib {
	namespace Sound {

		class Manager {
		public:
			Manager();
			static Manager instance();

			//이하 사용자가 부르는 일은 없다
			static void create(void* windowHandle);
			static void destroy();
		};

	} //namespace Sound
} //namespace GameLib

#endif
