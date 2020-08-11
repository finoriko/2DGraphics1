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
			///투시 변환 뷰 변환 행렬 세트.
			void setProjectionViewMatrix(const Matrix44&);
			///월드 변환 행렬 세트
			void setWorldMatrix(const Matrix34&);
			///뷰포트 설정
			void setViewport(int x, int y, int width, int height);
			///뷰포트취득(0이 아닌 것에만 넣기 때문에 원하는 것만 포인터를 세트)
			void getViewport(int* xOut, int* yOut, int* widthOut, int* heightOut);

			///설정한 텍스처, 꼭짓점 버퍼, 인덱스 버퍼로 그리기
			void drawIndexed(
				int offset,
				int primitiveNumber,
				PrimitiveType = PRIMITIVE_TRIANGLE);
			///설정한 텍스처, 꼭짓점 버퍼로 그리기
			void draw(
				int offset,
				int primitiveNumber,
				PrimitiveType = PRIMITIVE_TRIANGLE);
			///전체 화면에 텍스처를 부착
			void blendToScreen(Texture);
			///풀스크린화
			void enableFullScreen(bool);
			///그리기 버퍼 폭을 반환하다
			int width() const;
			///그림 버퍼 높이를 반환하다
			int height() const;
			///풀스크린 시의 창 폭을 반환하다
			int fullScreenWindowWidth() const;
			///풀스크린 시의 윈도 높이를 반환하다
			int fullScreenWindowHeight() const;
			///프레임 번호를 반환하다
			unsigned frameId() const;
			///스크린샷을 찍는다(확장자로 DDS, TGA자동판별). 저장타이밍은 그리기 종료 후.
			void captureScreen(const char* filename);

			//라이팅관계
			///픽셀 셰이더를 유효화하다
			void setLightingMode(LightingMode);
			///라이트 위치
			void setLightPosition(int index, const Vector3& position);
			///라이트 강도(100이면 100m 거리에 1의 밝기가 된다.강도 I로서 밝기는 'I/거리'로 정해짐)
			void setLightIntensity(int index, float intensity);
			///러이트색
			void setLightColor(int index, const Vector3&);
			///시점위치
			void setEyePosition(const Vector3& position);
			///디퓨즈색
			void setDiffuseColor(const Vector3&);
			///스페큘라 색
			void setSpecularColor(const Vector3&);
			///발광색
			void setEmissionColor(const Vector3&);
			///앰비션색
			void setAmbientColor(const Vector3&);
			///투명도설정
			void setTransparency(float);
			///스페큘러의 날카로움(크면 클수록 날카롭다)숫자의 의미는 다른 책을 읽자)
			void setSpecularSharpness(float);

			//이하는 몰라도 된다
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
			bool canRender() const; //디바이스 로스트(device lost)하고 있으면 false가 반환된다.
			void restore(); //디바이스로스트에서 복귀하다
			void getPointerModifier(float* scale, Vector2* offset) const;
		};

	} //namespace Graphics
} //namespace GameLib

#endif
