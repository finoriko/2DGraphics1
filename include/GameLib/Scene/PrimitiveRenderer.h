#ifndef INCLUDED_GAMELIB_SCENE_PRIMITIVERENDERER_H
#define INCLUDED_GAMELIB_SCENE_PRIMITIVERENDERER_H

#include "GameLib/Graphics/Enum.h"

namespace GameLib {

	namespace Graphics {
		class Texture;
	}

	namespace Math {
		class Matrix44;
		class Vector4;
		class Vector3;
		class Vector2;
	}
	using namespace Math;

	namespace Scene {

		////직접삼각형, 선분, 점, 2D직사각형을 그리기 위한 편리 클래스. 속도적으로 상당히 군더더기가 많음.
		class PrimitiveRenderer {
		public:
			///정점 수와 기타 명령의 수를 지정하여 구축
			static PrimitiveRenderer create(
				int vertexCapacity,
				int commandCapacity);

			void setTexture(Graphics::Texture);
			void enableDepthTest(bool);
			void enableDepthWrite(bool);
			void setBlendMode(Graphics::BlendMode);
			void setCullMode(Graphics::CullMode);
			void setTransform(const Matrix44&);

			/// 삼각형을 변환된 정점으로 쓴다. 즉 p는 4요소 벡터.
			void addTransformedTriangle(
				const Vector4& pos0,
				const Vector4& pos1,
				const Vector4& pos2,
				const Vector2& uv0,
				const Vector2& uv1,
				const Vector2& uv2,
				unsigned color0 = 0xffffffff,
				unsigned color1 = 0xffffffff,
				unsigned color2 = 0xffffffff);
			/// 삼각형을 변환된 꼭짓점으로 쓴다.텍스처 없음
			void addTransformedTriangle(
				const Vector4& pos0,
				const Vector4& pos1,
				const Vector4& pos2,
				unsigned color0 = 0xffffffff,
				unsigned color1 = 0xffffffff,
				unsigned color2 = 0xffffffff);
			///삼각형 그리기
			void addTriangle(
				const Vector3& pos0,
				const Vector3& pos1,
				const Vector3& pos2,
				const Vector2& uv0,
				const Vector2& uv1,
				const Vector2& uv2,
				unsigned color0 = 0xffffffff,
				unsigned color1 = 0xffffffff,
				unsigned color2 = 0xffffffff);
			///삼각형 그리기텍스처 없음
			void addTriangle(
				const Vector3& pos0,
				const Vector3& pos1,
				const Vector3& pos2,
				unsigned color0 = 0xffffffff,
				unsigned color1 = 0xffffffff,
				unsigned color2 = 0xffffffff);
			///선분을 그린다.
			void addLine(
				const Vector3& pos0,
				const Vector3& pos1,
				const Vector2& uv0,
				const Vector2& uv1,
				unsigned color0 = 0xffffffff,
				unsigned color1 = 0xffffffff);
			///선분을 그린다.텍스처 없음
			void addLine(
				const Vector3& pos0,
				const Vector3& pos1,
				unsigned color0 = 0xffffffff,
				unsigned color1 = 0xffffffff);
			///점을 그리다.
			void addPoint(
				const Vector3& pos,
				const Vector2& uv,
				unsigned color = 0xffffffff);
			///점을 그리다.텍스처 없음
			void addPoint(
				const Vector3& pos,
				unsigned color = 0xffffffff);
			///Z=0 평면에 직사각형을 그린다.좌표계는 스크린 좌표.
			void addRectangle(
				const Vector2& pos0,
				const Vector2& pos1,
				const Vector2& uv0,
				const Vector2& uv1,
				unsigned color = 0xffffffff,
				float depth = 0.f);
			///Z=0 평면에 직사각형을 그린다.좌표계는 스크린 좌표.텍스처 없음
			void addRectangle(
				const Vector2& pos0,
				const Vector2& pos1,
				unsigned color = 0xffffffff,
				float depth = 0.f);
			///그린다
			void draw();

			//이하 사용자는 의식하지 않는다
			PrimitiveRenderer();
			PrimitiveRenderer(const PrimitiveRenderer&);
			~PrimitiveRenderer();
			operator void* () const;
			PrimitiveRenderer& operator=(const PrimitiveRenderer&);
			void release();
			bool operator==(const PrimitiveRenderer&) const;
			bool operator!=(const PrimitiveRenderer&) const;
		private:
			class Impl;
			Impl* mImpl;
		};

	} //namespace Scene

} //namespace GameLib


#endif
