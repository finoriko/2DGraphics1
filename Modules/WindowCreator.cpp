#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x400 //WM_MOUSEWHEEL때문에
#endif

#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>

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

}