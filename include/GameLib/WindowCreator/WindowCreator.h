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
			void forbidFullScreen(bool f); //�̰��� �ҷ��θ� alt-enter�� ���õȴ�
			void enableFullScreen(bool f);
			void enableDragAndDrop(bool = true);
		};
		WindowCreator();
		//����� �����Լ�
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
		//���ڿ� �Է¹ޱ�
		const char* commandLineString() const;
		///�巰�ص���� ��ȿ�մϱ�?
		bool isDragAndDropEnabled() const;
		///�巯�׾ص�Ӽ����
		int droppedItemNumber() const;
		///�巡�� �ص� ��� ���
		const char* droppedItem(int index) const;
		///�巰�ص��Ŭ����
		void clearDroppedItem();
		///�и��� �����ð��� ��ȯ�մϴ�.
		unsigned time() const;

		static bool isMainThread();
		static WindowCreator instance();
		void requestEnd(); //���� �䱸�ϴ�
		void end(); ////���� �����Ѵ�.
		bool isEndRequested() const; ///���� �䱸�Ͻó���?
		int getAndResetMouseWheel();
	};
}
#endif
