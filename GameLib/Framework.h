#ifndef INCLUDED_GAMELIB_FRAMEWORK_H
#define INCLUDED_GAMELIB_FRAMEWORK_H
namespace GameLib {
	class Framework
	{
	public:
		Framework();
		//사용자 정의 함수.이것을 쓴다.
		void update();
		///인스턴스 받기
		static Framework instance();
		///화면폭 가져오기
		int width() const;
		///와면 높이 가져오기
		int height() const;
		///VRAM 받아오기
		unsigned* videoMemory();
		///Framework종료명령 내리기
		void requestEnd();
		///Framework에 종료명령이 내려저 있는지 알아보기
		bool isEndRequested() const;

		//아래 라이브러리 사용자는 몰라도 된다
		void start(void* windowHandle);
		void preUpdate();
		void postUpdate();
		static void create();
		static void destroy();
	};
	class StandardInput {
	public:
		StandardInput();
		StandardInput& operator>>(char&);
	private:
		bool mPrevEnterOn;
	};
	extern StandardInput cin;

} //namespace GameLib
#include "GameLib/Base/DebugStream.h"

#endif