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

		////�����ﰢ��, ����, ��, 2D���簢���� �׸��� ���� �� Ŭ����. �ӵ������� ����� �������Ⱑ ����.
		class PrimitiveRenderer {
		public:
			///���� ���� ��Ÿ ����� ���� �����Ͽ� ����
			static PrimitiveRenderer create(
				int vertexCapacity,
				int commandCapacity);

			void setTexture(Graphics::Texture);
			void enableDepthTest(bool);
			void enableDepthWrite(bool);
			void setBlendMode(Graphics::BlendMode);
			void setCullMode(Graphics::CullMode);
			void setTransform(const Matrix44&);

			/// �ﰢ���� ��ȯ�� �������� ����. �� p�� 4��� ����.
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
			/// �ﰢ���� ��ȯ�� ���������� ����.�ؽ�ó ����
			void addTransformedTriangle(
				const Vector4& pos0,
				const Vector4& pos1,
				const Vector4& pos2,
				unsigned color0 = 0xffffffff,
				unsigned color1 = 0xffffffff,
				unsigned color2 = 0xffffffff);
			///�ﰢ�� �׸���
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
			///�ﰢ�� �׸����ؽ�ó ����
			void addTriangle(
				const Vector3& pos0,
				const Vector3& pos1,
				const Vector3& pos2,
				unsigned color0 = 0xffffffff,
				unsigned color1 = 0xffffffff,
				unsigned color2 = 0xffffffff);
			///������ �׸���.
			void addLine(
				const Vector3& pos0,
				const Vector3& pos1,
				const Vector2& uv0,
				const Vector2& uv1,
				unsigned color0 = 0xffffffff,
				unsigned color1 = 0xffffffff);
			///������ �׸���.�ؽ�ó ����
			void addLine(
				const Vector3& pos0,
				const Vector3& pos1,
				unsigned color0 = 0xffffffff,
				unsigned color1 = 0xffffffff);
			///���� �׸���.
			void addPoint(
				const Vector3& pos,
				const Vector2& uv,
				unsigned color = 0xffffffff);
			///���� �׸���.�ؽ�ó ����
			void addPoint(
				const Vector3& pos,
				unsigned color = 0xffffffff);
			///Z=0 ��鿡 ���簢���� �׸���.��ǥ��� ��ũ�� ��ǥ.
			void addRectangle(
				const Vector2& pos0,
				const Vector2& pos1,
				const Vector2& uv0,
				const Vector2& uv1,
				unsigned color = 0xffffffff,
				float depth = 0.f);
			///Z=0 ��鿡 ���簢���� �׸���.��ǥ��� ��ũ�� ��ǥ.�ؽ�ó ����
			void addRectangle(
				const Vector2& pos0,
				const Vector2& pos1,
				unsigned color = 0xffffffff,
				float depth = 0.f);
			///�׸���
			void draw();

			//���� ����ڴ� �ǽ����� �ʴ´�
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
