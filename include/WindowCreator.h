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
			void forbidFullScreen(bool = true); //이것을 불러두면 alt-enter가 무시된다
			void enableFullScreen(bool = true);
			void enableDragAndDrop(bool = true);
		};
	};
}

#endif
