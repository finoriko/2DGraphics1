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
			Vector4 mPosition; //��ġ
			Vector3 mNormal; //����
			unsigned mColor; //��������
			Vector2 mUv; //�ؽ�ó ��ǥ
		};

	} //namespace Graphics
} //namespace GameLib

#endif
