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
			unsigned mTimeBias; //timeGetTime
			unsigned mThreadId;
			//Array< string > mDroppedItems;
			HWND mWindowHandle;
			RECT mWindowRect;
		};
		Impl* gImpl = 0;//유일한 인스턴스
	}
	WindowCreator::WindowCreator() {
		//다른 스레드로부터 호출은 허용하지 않는다
		//ASSERT(isMainThread() && "you must call from MAIN thread");
	}
	const char* WindowCreator::title() const {
		return gImpl->mTitle.c_str();
	}

	const char* WindowCreator::commandLineString() const {
		return gImpl->mCommandLine.c_str();
	}

	int WindowCreator::titleLength() const {
		return static_cast<int>(gImpl->mTitle.size());
	}

	int WindowCreator::height() const {
		return gImpl->mHeight;
	}

	int WindowCreator::width() const {
		return gImpl->mWidth;
	}

	bool WindowCreator::isFullScreen() const {
		return gImpl->mFullScreen;
	}

	bool WindowCreator::isMinimized() const
	{
		return false;
	}

	bool WindowCreator::isActive() const
	{
		return false;
	}

	void WindowCreator::enableFullScreen(bool)
	{
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