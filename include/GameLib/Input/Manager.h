#ifndef INCLUDED_GAMELIB_INPUT_MANAGER_H
#define INCLUDED_GAMELIB_INPUT_MANAGER_H
namespace GmaeLib
{
	namespace Math
	{
		class Vector2;
	}
	namespace Input {
		class Keyboard;
		class Mouse;
		class Joystick;

		class Manager {
		public:
			Manager();
			static Manager instance();

			Mouse mouse() const;
			Keyboard keyboard() const;
			Joystick joystick(int i = 0) const;
			int joystickNumber() const;

			//아래는 몰라도 된다
			static void create(void* windowHandle);
			static void destroy();
			void update(float pointerScale, const Math::Vector2& pointerOffset);
		};

	} //namespace Input
}

#endif