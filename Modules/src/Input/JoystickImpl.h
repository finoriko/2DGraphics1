#ifndef INCLUDED_GAMELIB_INPUT_JOYSTICKIMPL_H
#define INCLUDED_GAMELIB_INPUT_JOYSTICKIMPL_H

#include <dinput.h>
#undef min
#undef max

namespace GameLib 
{
	namespace Input 
	{
		class Joystick::Impl
		{
		public:
			Impl(IDirectInputDevice8* device, HWND windowHandle) :
				mDevice(device),
				mDoubleBufferIndex(0),
				mEnabled(false) {
				HRESULT hr;
				hr = mDevice->SetDataFormat(&c_dfDIJoystick2);
				STRONG_ASSERT(SUCCEEDED(hr));
				hr = mDevice->SetCooperativeLevel(windowHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
				STRONG_ASSERT(SUCCEEDED(hr));
				for (int i = 0; i < BUTTON_MAX; ++i) {
					mButtons[0][i] = false;
					mButtons[1][i] = false;
				}
				for (int i = 0; i < ANALOG_MAX; ++i) {
					mAnalogs[i] = 0;
				}

			}
			~Impl() {
				mDevice->Unacquire();
				mDevice->Release();
				mDevice = 0;
			}
		};
	}
}
#endif