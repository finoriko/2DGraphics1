#ifndef INCLUDED_GAMELIB_GRAPHICS_MANAGER_H
#define INCLUDED_GAMELIB_GRAPHICS_MANAGER_H

#include "GameLib/Graphics/Enum.h"

namespace GameLib {

	namespace Math {
		class Matrix44;
		class Matrix34;
		class Vector2;
		class Vector3;
	}
	using namespace Math;

	namespace Graphics {

		class Texture;
		class VertexBuffer;
		class IndexBuffer;

		class Manager {
		public:
			Manager();
			static Manager instance();

			void setVertexBuffer(VertexBuffer);
			void setIndexBuffer(IndexBuffer);
			void setTexture(Texture);

			void enableDepthTest(bool);
			void enableDepthWrite(bool);
			void setBlendMode(BlendMode);
			void setCullMode(CullMode);
			void setTextureFilter(TextureFilter);
			///���� ��ȯ �� ��ȯ ��� ��Ʈ.
			void setProjectionViewMatrix(const Matrix44&);
			///���� ��ȯ ��� ��Ʈ
			void setWorldMatrix(const Matrix34&);
			///����Ʈ ����
			void setViewport(int x, int y, int width, int height);
			///����Ʈ���(0�� �ƴ� �Ϳ��� �ֱ� ������ ���ϴ� �͸� �����͸� ��Ʈ)
			void getViewport(int* xOut, int* yOut, int* widthOut, int* heightOut);

			///������ �ؽ�ó, ������ ����, �ε��� ���۷� �׸���
			void drawIndexed(
				int offset,
				int primitiveNumber,
				PrimitiveType = PRIMITIVE_TRIANGLE);
			///������ �ؽ�ó, ������ ���۷� �׸���
			void draw(
				int offset,
				int primitiveNumber,
				PrimitiveType = PRIMITIVE_TRIANGLE);
			///��ü ȭ�鿡 �ؽ�ó�� ����
			void blendToScreen(Texture);
			///Ǯ��ũ��ȭ
			void enableFullScreen(bool);
			///�׸��� ���� ���� ��ȯ�ϴ�
			int width() const;
			///�׸� ���� ���̸� ��ȯ�ϴ�
			int height() const;
			///Ǯ��ũ�� ���� â ���� ��ȯ�ϴ�
			int fullScreenWindowWidth() const;
			///Ǯ��ũ�� ���� ���� ���̸� ��ȯ�ϴ�
			int fullScreenWindowHeight() const;
			///������ ��ȣ�� ��ȯ�ϴ�
			unsigned frameId() const;
			///��ũ������ ��´�(Ȯ���ڷ� DDS, TGA�ڵ��Ǻ�). ����Ÿ�̹��� �׸��� ���� ��.
			void captureScreen(const char* filename);

			//�����ð���
			///�ȼ� ���̴��� ��ȿȭ�ϴ�
			void setLightingMode(LightingMode);
			///����Ʈ ��ġ
			void setLightPosition(int index, const Vector3& position);
			///����Ʈ ����(100�̸� 100m �Ÿ��� 1�� ��Ⱑ �ȴ�.���� I�μ� ���� 'I/�Ÿ�'�� ������)
			void setLightIntensity(int index, float intensity);
			///����Ʈ��
			void setLightColor(int index, const Vector3&);
			///������ġ
			void setEyePosition(const Vector3& position);
			///��ǻ���
			void setDiffuseColor(const Vector3&);
			///����ŧ�� ��
			void setSpecularColor(const Vector3&);
			///�߱���
			void setEmissionColor(const Vector3&);
			///�ں�ǻ�
			void setAmbientColor(const Vector3&);
			///��������
			void setTransparency(float);
			///����ŧ���� ��ī�ο�(ũ�� Ŭ���� ��ī�Ӵ�)������ �ǹ̴� �ٸ� å�� ����)
			void setSpecularSharpness(float);

			//���ϴ� ���� �ȴ�
			static void create(
				void* windowHandle,
				int width,
				int height,
				bool fullScreen,
				bool vSync,
				bool antiAlias);
			static void destroy();
			void beginDraw();
			void endDraw();
			bool canRender() const; //����̽� �ν�Ʈ(device lost)�ϰ� ������ false�� ��ȯ�ȴ�.
			void restore(); //����̽��ν�Ʈ���� �����ϴ�
			void getPointerModifier(float* scale, Vector2* offset) const;
		};

	} //namespace Graphics
} //namespace GameLib

#endif
