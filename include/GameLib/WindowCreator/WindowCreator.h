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
			void setTitle(const char* title);
			void forbidFullScreen(bool f); //이것을 불러두면 alt-enter가 무시된다
			void enableFullScreen(bool f);
			void enableDragAndDrop(bool = true);
		};
		WindowCreator();
		//사용자 구현함수
		void configure(Configuration*);
		void start(void* windowHandle);
		void update();

		const char* title() const;
		int titleLength() const;
		int width() const;
		int height() const;
		bool isFullScreen() const;
		bool isMinimized() const;
		bool isActive() const;
		void enableFullScreen(bool);
		//문자열 입력받기
		const char* commandLineString() const;
		///드럭앤드롭은 유효합니까?
		bool isDragAndDropEnabled() const;
		///드러그앤드롭수취득
		int droppedItemNumber() const;
		///드래그 앤드 드롭 취득
		const char* droppedItem(int index) const;
		///드럭앤드롭클리어
		void clearDroppedItem();
		///밀리초 단위시각을 반환합니다.
		unsigned time() const;

		static bool isMainThread();
		static WindowCreator instance();
		void requestEnd(); //끝을 요구하다
		void end(); ////끝을 통지한다.
		bool isEndRequested() const; ///끝은 요구하시나요?
		int getAndResetMouseWheel();
	};
}
#endif
