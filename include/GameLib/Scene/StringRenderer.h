#ifndef INCLUDED_GAMELIB_SCENE_STRINGRENDERER_H
#define INCLUDED_GAMELIB_SCENE_STRINGRENDERER_H

namespace GameLib {

	namespace Graphics {
		class Texture;
	}
	namespace Math {
		class Vector2;
	}
	using namespace Math;

	namespace Scene {

		class Font;

		///문자열 묘화 클래스우선도 지정 있음.자동으로 정렬합니다.
		class StringRenderer {
		public:
			///최대 합계 문자수, 최대 요구 발행수 지정.넘으면 assert
			static StringRenderer create(int charCapacity, int requestCapacity);
			///픽셀 좌표로 지정.z만큼 float이고[0,1]인 것에 주의
			void add(
				int x,
				int y,
				const char* string,
				unsigned color = 0xffffffff,
				bool wrapAround = false,
				float z = 0.f);
			///픽셀이지만 float으로 위치를 지정(x=[0,w], y = [0,h], z = [0,1] )
			void add(
				const Vector2& position,
				const char* string,
				unsigned color = 0xffffffff,
				bool wrapAroung = false,
				float z = 0.f);
			///밀려든 요구를 한꺼번에 그리기
			void draw();
			///폰트를 세트.최소 한 번은 이걸 해야 글씨를 쓸 수 있어.
			void setFont(Font&);

			//이하 의식하지 않아도 된다
			StringRenderer();
			~StringRenderer();
			StringRenderer(const StringRenderer&);
			operator void* () const;
			StringRenderer& operator=(const StringRenderer&);
			void release();
			bool operator==(const StringRenderer&) const;
			bool operator!=(const StringRenderer&) const;
		private:
			class Impl;
			Impl* mImpl;
		};

	} //namespace Scene
} //namespace GameLib

#endif
