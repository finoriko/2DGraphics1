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
			void forbidFullScreen(bool = true); //�̰��� �ҷ��θ� alt-enter�� ���õȴ�
			void enableFullScreen(bool = true);
			void enableDragAndDrop(bool = true);
		};
		WindowCreator();
		//����� �����Լ�
		void configure(Configuration*);
		void start(void* windowHandle);
		void update();

		const char* title() const;
		//���ڿ� �Է¹ޱ�
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
