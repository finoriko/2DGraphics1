#ifndef INCLUDED_GAMELIB_GRAPHICS_VERTEX_H
#define INCLUDED_GAMELIB_GRAPHICS_VERTEX_H

#include "GameLib/Math/Vector4.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Vector2.h"

namespace GameLib {
	using namespace Math;
	namespace Graphics {

		class Vertex {
		public:
			Vector4 mPosition; //위치
			Vector3 mNormal; //법선
			unsigned mColor; //꼭짓점색
			Vector2 mUv; //텍스처 좌표
		};

	} //namespace Graphics
} //namespace GameLib

#endif
