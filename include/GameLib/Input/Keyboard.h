#ifndef INCLUDED_GAMELIB_INPUT_KEYBOARD_H
#define INCLUDED_GAMELIB_INPUT_KEYBOARD_H

namespace GameLib
{
	namespace Input
	{
		class Keyboard
		{
		public :
			enum  Key
			{

			};
			bool isOn(int i) const;
			bool isTriggered(int i) const;
		};
		
	} //namespace Input
} //namespace GameLib

#endif