#ifndef INCLUDED_GAMELIB_SCENE_FONTIMPLBITMAP_H
#define INCLUDED_GAMELIB_SCENE_FONTIMPLBITMAP_H

#include "GameLib/Scene/Font.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Graphics/Manager.h"
#include "Scene/FontImpl.h"

namespace GameLib {
	namespace Scene {

		namespace { //무명

			class FontImplBitmap : public Font::Impl {
			public:
				FontImplBitmap(
					const char** textureFileNames,
					int textureNumber,
					Font::BitmapChar* bitmapChars,
					int bitmapCharNumber) :
					mCharHeight(0),
					mTextures(0),
					mTextureNumber(textureNumber),
					mCharNumber(bitmapCharNumber) {
					// 텍스처를 일거에 생성합니다.
					mTextures = NEW Texture[mTextureNumber];
					for (int i = 0; i < mTextureNumber; ++i) {
						mTextures[i] = Texture::create(textureFileNames[i]);
					}
					// 문자 테이블 생성. 내친김에 height 최대값 구하기
					mChars = NEW Char[mCharNumber];
					for (int i = 0; i < mCharNumber; ++i) {
						Char& src = mChars[i];
						const Font::BitmapChar& dst = bitmapChars[i];
						src.mCode = dst.mCode;
						src.mX = dst.mX;
						src.mY = dst.mY;
						src.mTexturePage = dst.mTexturePage;
						src.mHeight = dst.mHeight;
						src.mWidth = dst.mWidth;
						if (src.mHeight > mCharHeight) {
							mCharHeight = src.mHeight;
						}
					}
				}
				FontImplBitmap(
					const char** textureFileData,
					const int* textureFileSizes,
					int textureNumber,
					Font::BitmapChar* bitmapChars,
					int bitmapCharNumber) :
					mCharHeight(0),
					mTextures(0),
					mTextureNumber(textureNumber),
					mCharNumber(bitmapCharNumber) {
					// 텍스처를 일거에 생성합니다.
					mTextures = NEW Texture[mTextureNumber];
					for (int i = 0; i < mTextureNumber; ++i) {
						mTextures[i] = Texture::create(textureFileData[i], textureFileSizes[i]);
					}
					//문자 테이블 생성. 내친김에 height 최대값 구하기
					mChars = NEW Char[mCharNumber];
					for (int i = 0; i < mCharNumber; ++i) {
						Char& src = mChars[i];
						const Font::BitmapChar& dst = bitmapChars[i];
						src.mCode = dst.mCode;
						src.mX = dst.mX;
						src.mY = dst.mY;
						src.mTexturePage = dst.mTexturePage;
						src.mHeight = dst.mHeight;
						src.mWidth = dst.mWidth;
						if (src.mHeight > mCharHeight) {
							mCharHeight = src.mHeight;
						}
					}
				}
				~FontImplBitmap() {
					SAFE_DELETE_ARRAY(mTextures);
					SAFE_DELETE_ARRAY(mChars);
				}

				bool getChar(
					Texture* texture,
					Vector2* uvTopLeft,
					Vector2* uvBottomRight,
					int code) {
					//둘로 나누어 검색색합니다
					int first = 0;
					int last = mCharNumber - 1;
					while (last > first) {
						int middle = (first + last) / 2;
						int t = mChars[middle].mCode;
						if (t < code) {
							first = middle + 1;
						}
						else if (t > code) {
							last = middle - 1;
						}
						else {
							first = last = middle;
						}
					}
					if (mChars[first].mCode == code) {
						//발견
						Char& c = mChars[first];
						*texture = mTextures[c.mTexturePage];
						float tw = static_cast<float>(texture->width());
						float th = static_cast<float>(texture->height());
						float w = static_cast<float>(c.mWidth);
						float h = static_cast<float>(c.mHeight);
						float x = static_cast<float>(c.mX);
						float y = static_cast<float>(c.mY);
						uvTopLeft->set(x / tw, y / th);
						uvBottomRight->set(
							(x + w) / tw,
							(y + h) / th);
						return true;
					}
					else {
						return false;
					}
				}
				int charHeight() const {
					return mCharHeight;
				}
			private:
				struct Char {
					bool operator<(const Char& a) const {
						return (mCode < a.mCode);
					}
					unsigned char mTexturePage; //텍스처 페이지
					unsigned char mHeight; //높이 정보(픽셀)
					unsigned char mWidth; //폭 정보(픽셀)
					unsigned short mCode; //문자코드(SHIFT_JIS한정)
					unsigned short mX; //왼쪽끝
					unsigned short mY; //상단
				};
				int mCharHeight;
				Texture* mTextures;
				int mTextureNumber;
				Char* mChars;
				int mCharNumber;
			};

		} //namespace{}

	} //Scene
} //GameLib
#endif
