#pragma once
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
	};
}

#endif
