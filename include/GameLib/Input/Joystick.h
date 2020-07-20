#ifndef INCLUDED_GAMELIB_INPUT_JOYSTICK_H
#define INCLUDED_GAMELIB_INPUT_JOYSTICK_H

namespace GameLib 
{
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
			int analog(int) const;// �Ƴ��αװ� ���
			//�����¿�� ���ٷ� ������.�ٸ� ��ư�� 0 ���� ���� �Դϴ�.
			bool isOn(int) const;
			bool isTriggered(int) const;
			int buttonNumber() const;
			//��ȿ�Ѱ�?
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
}
#endif