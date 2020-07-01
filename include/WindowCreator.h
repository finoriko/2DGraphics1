#ifndef INCLUDED_GAMELIB_WINDOWCREATOR_H
#define INCLUDED_GAMELIB_WINDOWCREATOR_H

namespace GameLib
{
	class WindowCreator
	{
	public:
		class Configuration
		{
		public:
			void setWidth(int width);
			void setHeight(int height);
			void forbidFullScreen(bool = true); //이것을 불러두면 alt-enter가 무시된다
			void enableFullScreen(bool = true);
			void enableDragAndDrop(bool = true);
		};
		WindowCreator();
		//사용자 구현함수
		void configure(Configuration*);
		void start(void* windowHandle);
		void update();

		const char* title() const;
		//문자열 입력받기
		const char* commandLineString() const;

		int titleLength() const;
		int width() const;
		int height() const;
		bool isFullScreen() const;
		bool isMinimized() const;
		bool isActive() const;
		void enableFullScreen(bool);
	};
}

#endif
