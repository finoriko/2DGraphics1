#include "GameLib/GameLib.h"
#include <windows.h>
#include <cstdio>


bool gThrowWhenHalt = false;

// 이 파일에서는 new하지 않도록 배려한다.
//new 안에서도 불릴 수 있기 때문이다.

namespace GameLib 
{
	void halt(const char* filename, int line, const char* message) 
	{
		char buf[8192];
		sprintf_s(buf, 8192, "%s:%d : %s", filename, line, message);
		if (gThrowWhenHalt) {
			::OutputDebugStringA(buf);
			::OutputDebugStringA("\n");
			throw EXCEPTION_IGNORE; //기본은 무시
		}
		else {
			//메시지 상자를 꺼내서
			MessageBoxA(0, buf, "Critical Error!", MB_OK | MB_ICONERROR);
			//즉사
			*reinterpret_cast<int*>(0) = 0;
		}
	}

} //namespace GameLib