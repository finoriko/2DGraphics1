#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x400 //WM_MOUSEWHEEL때문에
#endif

#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>
#include <string>
#undef min
#undef max

#include "../include/WindowCreator.h"
using namespace std;
using namespace GameLib;

//WindowCreator 메인함수

namespace GameLib
{
	namespace
	{
		class Impl
		{
		public:
			Impl() :
				mWidth(640),
				mHeight(480),
				mFullScreen(false),
				mMinimized(false),
				mActive(false),
				mTitle("A GameLib Application"),
				mCommandLine(""),
				mEndRequested(false),
				mEnded(false),
				mStarted(false),
				mFullScreenForbidden(false),
				mDragAndDropEnabled(false),
				mMouseWheel(0),
				mTimeBias(0) {
				//get Time()용 바이어스 값 계산
				unsigned time = timeGetTime();
				mTimeBias = 0xffffffff - time;
				mTimeBias -= 60000; //60초를 빼다

				//스레드ID취득
				mThreadId = GetCurrentThreadId();
			}
			~Impl() {
			}
			int mWidth;
			int mHeight;
			bool mFullScreen;
			bool mMinimized;
			bool mActive;
			string mTitle;
			string mCommandLine;
			bool mEndRequested;
			bool mEnded;
			bool mStarted;
			bool mFullScreenForbidden;
			bool mDragAndDropEnabled;
			int mMouseWheel;
			unsigned mTimeBias; //timeGetTimeにはかせるゲタ
			unsigned mThreadId;
			//Array< string > mDroppedItems;
			HWND mWindowHandle;
			RECT mWindowRect;
		};
		
	}


	void WindowCreator::Configuration::setWidth(int width)
	{
	}

	void WindowCreator::Configuration::setHeight(int height)
	{
	}

	void WindowCreator::Configuration::forbidFullScreen(bool)
	{
	}

	void WindowCreator::Configuration::enableFullScreen(bool)
	{
	}

	void WindowCreator::Configuration::enableDragAndDrop(bool)
	{
	}

	WindowCreator::WindowCreator()
	{
	}

}