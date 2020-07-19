#ifndef INCLUDED_GAMELIB_INPUT_MANAGERIMPL_H
#define INCLUDED_GAMELIB_INPUT_MANAGERIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Input/Mouse.h"
#include "GameLib/Input/Joystick.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Threading/Functions.h"
#include "GameLib/WindowCreator/WindowCreator.h"
using namespace GameLib::Math;

#include <dinput.h>
#undef min
#undef max

#include "JoystickImpl.h"

namespace GameLib {
	namespace Input {

		class ManagerImpl {
		public:
			ManagerImpl(HWND hwnd) :
				mJoystickNumber(0),
				mJoysticks(0),
				mX(0),
				mY(0),
				mVelocityX(0),
				mVelocityY(0),
				mWheel(0),

				mEnumeratedJoystick(0),
				mDirectInput(0),
				mWindowHandle(hwnd),
				mDoubleBufferIndex(0) {
				HRESULT hr;
				HINSTANCE hi = GetModuleHandle(0);
				STRONG_ASSERT(hi);
				hr = DirectInput8Create(hi, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, 0);
				STRONG_ASSERT(SUCCEEDED(hr));
				//우선 세어 확보
				hr = mDirectInput->EnumDevices(DI8DEVCLASS_ALL, countCallback, this, DIEDFL_ATTACHEDONLY);
				STRONG_ASSERT(SUCCEEDED(hr));
				if (mJoystickNumber > 0) {
					mJoysticks = static_cast<Joystick::Impl*>(OPERATOR_NEW(sizeof(Joystick::Impl) * mJoystickNumber));
				}
				hr = mDirectInput->EnumDevices(DI8DEVCLASS_ALL, createCallback, this, DIEDFL_ATTACHEDONLY);
				STRONG_ASSERT(SUCCEEDED(hr));
				// 마우스 버튼 초기화
				for (int i = 0; i < BUTTON_MAX; ++i) {
					mButtons[0][i] = false;
					mButtons[1][i] = false;
				}
				// 키 초기화
				for (int i = 0; i < KEY_MAX; ++i) {
					mKeys[0][i] = false;
					mKeys[1][i] = false;
				}
			}
			~ManagerImpl() {
				for (int i = 0; i < mJoystickNumber; ++i) {
					mJoysticks[i].~Impl();
				}
				OPERATOR_DELETE(mJoysticks);

				mDirectInput->Release();
				mDirectInput = 0;
			}
			void update(float pointerScale, const Vector2& pointerOffset) {
				mDoubleBufferIndex = 1 - mDoubleBufferIndex;

				// 마우스 커서를 받아서 위치를 변환한 후 마우스에 셋팅
				POINT pos;
				GetCursorPos(&pos);
				ScreenToClient(mWindowHandle, &pos);
				Vector2 p(
					static_cast<float>(pos.x),
					static_cast<float>(pos.y));
				p += pointerOffset; //역변환이므로 이동이 먼저
				p *= pointerScale;
				int x = static_cast<int>(p.x + 0.5f);
				int y = static_cast<int>(p.y + 0.5f);
				mVelocityX = x - mX;
				mVelocityY = y - mY;
				mX = x;
				mY = y;

				if (WindowCreator().isActive()) {
					//키보드
					unsigned char keys[KEY_MAX];
					GetKeyboardState(keys);
					for (int i = 0; i < KEY_MAX; ++i) {
						mKeys[mDoubleBufferIndex][i] = ((keys[i] & 0x80) != 0) ? true : false;
					}
					//마우스
					mButtons[mDoubleBufferIndex][0] = (GetAsyncKeyState(VK_LBUTTON) & 0xf000) ? true : false;
					mButtons[mDoubleBufferIndex][1] = (GetAsyncKeyState(VK_RBUTTON) & 0xf000) ? true : false;
					mButtons[mDoubleBufferIndex][2] = (GetAsyncKeyState(VK_MBUTTON) & 0xf000) ? true : false;
					mWheel = WindowCreator().getAndResetMouseWheel();
				}
				else {
					//키보드
					for (int i = 0; i < KEY_MAX; ++i) {
						mKeys[mDoubleBufferIndex][i] = false;
					}
					//마우스
					for (int i = 0; i < BUTTON_MAX; ++i) {
						mButtons[mDoubleBufferIndex][i] = false;
					}
					mWheel = 0;
				}
				for (int i = 0; i < mJoystickNumber; ++i) {
					mJoysticks[i].update();
				}
			}
			bool isButtonOn(Mouse::Button i) const {
				ASSERT(i < BUTTON_MAX);
				return mButtons[mDoubleBufferIndex][i];
			}
			bool isButtonTriggered(Mouse::Button i) const {
				ASSERT(i < BUTTON_MAX);
				int idx = mDoubleBufferIndex;
				return mButtons[idx][i] && (!mButtons[1 - idx][i]);
			}
			bool isKeyOn(int i) const
			{
				ASSERT(i < KEY_MAX);
				int idx = mDoubleBufferIndex;
				int k = convertKeyID(i);
				return mKeys[idx][k];
			}
			bool isKeyTriggered(int i) const
			{
				ASSERT(i < KEY_MAX);
				int idx = mDoubleBufferIndex;
				int k = convertKeyID(i);
				return mKeys[idx][k] & (!mKeys[1 - idx][k]);
			}
			int mJoystickNumber;
			Joystick::Impl* mJoysticks;

			//마우스
			int mX;
			int mY;
			int mVelocityX;
			int mVelocityY;
			int mWheel;
		private:
			static BOOL CALLBACK countCallback(const DIDEVICEINSTANCE* instance, VOID* arg) {
				int type = instance->dwDevType & 0xff; //하위바이트 유형
				ManagerImpl* self = static_cast<ManagerImpl*>(arg);
				if (type == DI8DEVTYPE_JOYSTICK) {
					++(self->mJoystickNumber);
				}
				return DIENUM_CONTINUE;
			}
			static BOOL CALLBACK createCallback(const DIDEVICEINSTANCE* instance, VOID* arg) {
				ManagerImpl* self = static_cast<ManagerImpl*>(arg);
				self->createDevice(instance);
				return DIENUM_CONTINUE;
			}
			void createDevice(const DIDEVICEINSTANCE* instance) {
				int type = instance->dwDevType & 0xff; //하위 바이트 타입
				IDirectInputDevice8* device;
				HRESULT hr = mDirectInput->CreateDevice(instance->guidInstance, &device, 0);
				STRONG_ASSERT(SUCCEEDED(hr));

				if (type == DI8DEVTYPE_JOYSTICK) {
					new (&mJoysticks[mEnumeratedJoystick]) Joystick::Impl(device, mWindowHandle);
					++mEnumeratedJoystick;
				}
			}
			static int convertKeyID(int a) {
				typedef Keyboard K;
				int r = 0;
				if (a >= 'a' && a <= 'z') { //대문자로 변환
					a = 'A' + (a - 'a');
				}
				bool isAlpha = (a >= 'A' && a <= 'Z');
				bool isNum = (a >= '0' && a <= '9');
				if (isAlpha || isNum) {
					r = a;
				}
				if (r == 0) {
					switch (a) {
					case ' ': r = VK_SPACE; break;
					case '+': r = VK_OEM_PLUS; break;
					case ',': r = VK_OEM_COMMA; break;
					case '-': r = VK_OEM_MINUS; break;
					case '.': r = VK_OEM_PERIOD; break;
					case K::KEY_RETURN: r = VK_RETURN; break;
					case K::KEY_TAB: r = VK_TAB; break;
					case K::KEY_ESC: r = VK_ESCAPE; break;
					case K::KEY_SHIFT: r = VK_SHIFT; break;
					case K::KEY_CONTROL: r = VK_CONTROL; break;
					case K::KEY_ALT: r = VK_MENU; break;
					case K::KEY_F1: r = VK_F1; break;
					case K::KEY_F2: r = VK_F2; break;
					case K::KEY_F3: r = VK_F3; break;
					case K::KEY_F4: r = VK_F4; break;
					case K::KEY_F5: r = VK_F5; break;
					case K::KEY_F6: r = VK_F6; break;
					case K::KEY_F7: r = VK_F7; break;
					case K::KEY_F8: r = VK_F8; break;
					case K::KEY_F9: r = VK_F9; break;
					case K::KEY_F10: r = VK_F10; break;
					case K::KEY_F11: r = VK_F11; break;
					case K::KEY_F12: r = VK_F12; break;
					case K::KEY_UP: r = VK_UP; break;
					case K::KEY_DOWN: r = VK_DOWN; break;
					case K::KEY_LEFT: r = VK_LEFT; break;
					case K::KEY_RIGHT: r = VK_RIGHT; break;
					case K::KEY_NUM0: r = VK_NUMPAD0; break;
					case K::KEY_NUM1: r = VK_NUMPAD1; break;
					case K::KEY_NUM2: r = VK_NUMPAD2; break;
					case K::KEY_NUM3: r = VK_NUMPAD3; break;
					case K::KEY_NUM4: r = VK_NUMPAD4; break;
					case K::KEY_NUM5: r = VK_NUMPAD5; break;
					case K::KEY_NUM6: r = VK_NUMPAD6; break;
					case K::KEY_NUM7: r = VK_NUMPAD7; break;
					case K::KEY_NUM8: r = VK_NUMPAD8; break;
					case K::KEY_NUM9: r = VK_NUMPAD9; break;
					}
				}
				return r;
			}
			void operator=(const ManagerImpl&); //금지

			int mEnumeratedJoystick;
			IDirectInput8* mDirectInput;
			const HWND mWindowHandle;
			int mDoubleBufferIndex;

			//마우스
			static const int BUTTON_MAX = 3;
			bool mButtons[2][BUTTON_MAX];
			//키보드
			static const int KEY_MAX = 256;
			bool mKeys[2][KEY_MAX];

		};
		extern ManagerImpl* gManagerImpl; //유일 인스턴스

	} //nemespace Input
} //namespace GameLib

#endif