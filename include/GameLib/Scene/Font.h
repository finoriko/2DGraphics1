#ifndef INCLUDED_GAMELIB_SCENE_FONT_H
#define INCLUDED_GAMELIB_SCENE_FONT_H

namespace GameLib {

	namespace Graphics {
		class Texture;
	}
	namespace Math {
		class Vector2;
	}
	using namespace Math;

	namespace Scene {

		///폰트클래스
		class Font {
		public:
			///비트맵 글꼴 하나하나에 필요한 데이터
			struct BitmapChar {
				unsigned shortmCode; // 문자코드(SHIFT_JIS)
				unsigned shortmX; // 왼쪽 끝
				unsigned short mY; // 상단
				unsigned charmTexturePage; // 텍스처 페이지
				unsigned charmWidth; // 폭 정보(픽셀)
				unsigned charmHeight; // 높이 정보(픽셀)
			};
			////// 텍스처를 사용한 비트맵 폰트를 생성.문자의 높이는 최대치를 채용.
			static Font create(
				const char** textureFileNames, //텍스처 파일 이름 배열
				int textureNumber,
				BitmapChar* BitmapChars,
				int bitmapCharNumber);
			///텍스처를 사용한 비트맵 폰트를 생성.문자의 높이는 최대치를 채용.
			static Font create(
				const char** textureFileData, //텍스처 DDS 이미지 배열
				const int* textureFileSizes, //텍스처 데이터 크기 배열
				int textureNumber,
				BitmapChar* bitmapChars,
				int CharNumber);
			///OS에서 동적으로 폰트 데이터를 가져온다.문자의 높이, 내부 텍스처 폭 등은 지정.
			static Font create(
				const char* osFontName,
				int charHeightHint, //이건 기준이야. 실제로 몇 개는 getCharHeight를 참조하라.
				bool bold = false,
				bool italic = false,
				bool proportional = false,
				bool antialias = false,
				int internalTextureWidth = 512,
				int internalTextureHeight = 512);
			///문자 정보 가져오기.발견되지 않으면 false를 반환한다.
			bool getChar(
				Graphics::Texture* textureOut,
				Vector2* uvTopLeftOut,
				Vector2* uvBottomRightOut,
				int code);
			///글자의 최대 높이
			int charHeight() const;
			///로드 끝났나?
			bool isReady();

			//이하 사용자는 의식하지 않아도 된다
			Font();
			~Font();
			Font(const Font&);
			operator void* () const;
			Font& operator=(const Font&);
			void release();
			bool operator==(const Font&) const;
			bool operator!=(const Font&) const;

			class Impl;
		private:
			explicit Font(Impl*);
			Impl* mImpl;
		};

	} //namespace Scene
} //namespace GameLib

#endif
