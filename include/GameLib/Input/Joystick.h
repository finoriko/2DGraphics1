#ifndef INCLUDED_GAMELIB_INPUT_JOYSTICK_H
#define INCLUDED_GAMELIB_INPUT_JOYSTICK_H

//namespace GameLib 
//{
	namespace Input 
	{
		class Joystick
		{
		public:
			enum Button {
				BUTTON_UP = 128,
				BUTTON_DOWN,
				BUTTON_LEFT,
				BUTTON_RIGHT,
			};

			int analogNumber() const;
			int analog(int) const;// 아날로그값 취득
			//상하좌우는 윗줄로 잡힌다.다른 버튼은 0 에서 숫자 입니다.
			bool isOn(int) const;
			bool isTriggered(int) const;
			int buttonNumber() const;
			//유효한가?
			bool isEnabled() const;

			Joystick();
			~Joystick();
			operator void* () const;
			class Impl;
		private:
			friend class Manager;
			Joystick(Impl*);
			Impl* mImpl;
		};
	}
//}
#endif