#ifndef INCLUDED_GAMELIB_SCENE_FONTIMPLOS_H
#define INCLUDED_GAMELIB_SCENE_FONTIMPLOS_H

#include "Scene/FontImpl.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/WindowCreator/WindowCreator.h"
#include <map> //머지않아 자작으로 바꾸고 싶다...

#include <windows.h>
#undef min
#undef max

namespace GameLib {
	namespace Scene {
		using namespace std;

		using namespace GameLib::Graphics;
		using namespace GameLib::Math;

		namespace { //무명

		//Windows에서 폰트 데이터를 취득
			class FontImplOS : public Font::Impl {
				struct Char;
			public:
				FontImplOS(
					const char* fontName,
					int charHeightHint, //이렇게 된다고는 할 수 없다
					bool bold,
					bool italic,
					bool proportional,
					bool antialias,
					int textureWidth,
					int textureHeight) :
					mCharHeight(0),
					mCharAscent(0),
					mAntiAliased(antialias),
					mFontHandle(0),
					mDeviceContextHandle(0),
					mCurrentX(0),
					mCurrentY(0),
					mCurrentLineHeight(0),
					mCurrentPage(0),
					mTextureWidth(textureWidth),
					mTextureHeight(textureHeight) {
					//텍스처 생성. 일단 한 장
					Texture t = Texture::create(mTextureWidth, mTextureHeight, false);
					//맵에 격납
					mTextures.insert(make_pair(0, t));

					//글꼴명을 UNICODE 변환
					DWORD pitch = (proportional) ? VARIABLE_PITCH : FIXED_PITCH;

					//폰트 생성
					mFontHandle = CreateFontA(
						charHeightHint,
						0,
						0,
						0,
						(bold) ? FW_BOLD : FW_DONTCARE,
						(italic) ? TRUE : FALSE,
						FALSE,
						FALSE,
						SHIFTJIS_CHARSET,
						OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY,
						pitch | FF_DONTCARE,
						fontName);
					ASSERT(mFontHandle);
					//HDC생성
					mDeviceContextHandle = CreateCompatibleDC(NULL);
					//텍스트 매트릭스 취득
					//fontセット
					SelectObject(mDeviceContextHandle, mFontHandle);
					//문자 매트릭스 생성
					TEXTMETRIC tm;
					GetTextMetrics(mDeviceContextHandle, &tm);
					mCharHeight = tm.tmHeight; //실제의 값으로 덮어쓰기
					mCharAscent = tm.tmAscent; //베이스라인 위의 치수가 필요함
				}
				~FontImplOS() {
					DeleteDC(mDeviceContextHandle);
					mDeviceContextHandle = 0;

					DeleteObject(mFontHandle);
					mFontHandle = 0;
				}
				bool getChar(
					Texture* texture,
					Vector2* uvTopLeft,
					Vector2* uvBottomRight,
					int code) {
					//코드 테이블에 있으면 사용한다.
					CharIt cit = mChars.find(static_cast<unsigned short>(code));
					const Char* c = 0;
					if (cit != mChars.end()) {
						c = &cit->second;
					}
					else {
						c = createGlyph(code);
					}
					TextureIt tit = mTextures.find(c->mTexturePage);
					ASSERT(tit != mTextures.end());
					*texture = tit->second;
					float tw = static_cast<float>(texture->width());
					float th = static_cast<float>(texture->height());
					float w = static_cast<float>(c->mWidth);
					float h = static_cast<float>(c->mHeight);
					float x = static_cast<float>(c->mX);
					float y = static_cast<float>(c->mY);
					uvTopLeft->set(x / tw, y / th);
					uvBottomRight->set(
						(x + w) / tw,
						(y + h) / th);
					return true;
				}
				const Char* createGlyph(int code) {
					//font세트
					SelectObject(mDeviceContextHandle, mFontHandle);

					GLYPHMETRICS gm; // 정보
					MAT2 mat = {
						{ 0, 1 },
						{ 0, 0 },
						{ 0, 0 },
						{ 0, 1 }
					};
					DWORD size;
					const UINT format = (mAntiAliased) ? GGO_GRAY8_BITMAP : GGO_BITMAP;
					// 우선 필요한 버퍼 사이즈를 획득
					size = GetGlyphOutlineA(
						mDeviceContextHandle,
						static_cast<UINT>(code),
						format,
						&gm, 0, NULL, &mat);
					//치수 취득
					int cellW = static_cast<int>(gm.gmCellIncX);
					int cellH = static_cast<int>(mCharHeight); //IncY는 도움이 안 되고
					int glyphW = static_cast<int>(gm.gmBlackBoxX);
					int glyphH = static_cast<int>(gm.gmBlackBoxY);
					int glyphX = static_cast<int>(gm.gmptGlyphOrigin.x);
					int glyphY = static_cast<int>(mCharAscent - gm.gmptGlyphOrigin.y);
					if (glyphW + glyphX > cellW) { //있을 수밖에 없다
						cellW = glyphW + glyphX;
					}
					if (glyphH + glyphY > cellH) { //있을 수밖에 없다
						cellH = glyphH + glyphY;
					}
					//기입 텍스처 취득
					TextureIt tit = mTextures.find(mCurrentPage);
					ASSERT(tit != mTextures.end());
					Texture* tex = &(tit->second);
					int tw = tex->width();
					int th = tex->height();
					//넘쳤어요!
					if (mCurrentX + cellW >= tw) {
						mCurrentX = 0;
						mCurrentY += mCurrentLineHeight;
						mCurrentLineHeight = 0;
						//세로 넘쳤어요!
						if (mCurrentY + cellH >= th) {
							mCurrentY = 0;
							++mCurrentPage;
							Texture t = Texture::create(mTextureWidth, mTextureHeight, false);
							//맵에 격납
							mTextures.insert(make_pair(mCurrentPage, t));
							//텍스쳐 다시 하기
							TextureIt tit = mTextures.find(mCurrentPage);
							ASSERT(tit != mTextures.end());
							tex = &(tit->second);
						}
					}
					//텍스처록(부분잠금)
					unsigned* dst;
					int dstPitch;
					tex->lock(
						&dst,
						&dstPitch,
						mCurrentX,
						mCurrentY,
						cellW,
						cellH);

					if (size == 0) { //사이즈만큼 단지 0메우기
						for (int y = 0; y < cellH; ++y) {
							for (int x = 0; x < cellW; ++x) {
								dst[x] = 0x00ffffff;
							}
							dst += dstPitch / 4;
						}
					}
					else {
						// 버퍼 확보
						BYTE* data = NEW BYTE[size];
						int srcPitch = (glyphW + 3) & ~(3); //4바이트알라인

						// 두 번째 호출이 진짜.데이터 취득.
						size = GetGlyphOutlineA(
							mDeviceContextHandle,
							static_cast<UINT>(code),
							format,
							&gm, size, data, &mat);
						ASSERT(size != GDI_ERROR);

						//1bit일때가 너무 불편해서 바이트배열에 복사
						if (mAntiAliased) { //앤티가 있다면 64->255계조 변환
							for (DWORD i = 0; i < size; ++i) {
								data[i] = data[i] * 255 / 64;
							}
						}
						else {
							BYTE* newData = NEW BYTE[size * 8]; //8배만 있으면 충분하잖아
							const BYTE* src = data;
							BYTE* dst = newData;
							int srcPitch = (((glyphW + 7) / 8) + 3) & (~3);
							int dstPitch = (glyphW + 3) & (~3);
							for (int y = 0; y < glyphH; ++y) {
								for (int x = 0; x < glyphW; ++x) {
									int shift = 7 - (x % 8);
									int mask = 0x1 << shift;
									int bit = (src[x / 8] & mask) >> shift;
									dst[x] = static_cast<BYTE>(bit * 255);
								}
								dst += dstPitch;
								src += srcPitch;
							}
							//구 배열을 deletel|
							SAFE_DELETE_ARRAY(data);
							//새 배열로 대체
							data = newData;
						}
						BYTE* src = data;
						//상부 공백의 도색
						int y = 0;
						for (; y < glyphY; ++y) {
							for (int x = 0; x < cellW; ++x) {
								dst[x] = 0x00ffffff;
							}
							dst += dstPitch / 4;
						}
						//글리프 본체 Z
						for (; y < glyphY + glyphH; ++y) {
							LONG x = 0;
							//좌측 공백
							for (; x < glyphX; ++x) {
								dst[x] = 0x00ffffff;
							}
							//글리프 본체
							for (; x < glyphX + glyphW; ++x) {
								int a = src[x - glyphX];
								dst[x] = (a << 24) | 0xffffff;
							}
							//우측 공백
							for (; x < cellW; ++x) {
								dst[x] = 0x00ffffff;
							}
							dst += dstPitch / 4;
							src += srcPitch;
						}
						//하부 공백의 도색
						for (; y < cellH; ++y) {
							for (int x = 0; x < cellW; ++x) {
								dst[x] = 0x00ffffff;
							}
							dst += dstPitch / 4;
						}
						SAFE_DELETE_ARRAY(data);
					}
					tex->unlock(&dst, 0);
					//리스트에 추가
					Char c;
					c.mTexturePage = static_cast<unsigned char>(mCurrentPage);
					c.mWidth = static_cast<unsigned char>(cellW);
					c.mHeight = static_cast<unsigned char>(cellH);
					c.mX = static_cast<unsigned short>(mCurrentX);
					c.mY = static_cast<unsigned short>(mCurrentY);
					mCurrentX += cellW; //기입 위치로 진행하다
					if (mCurrentLineHeight < cellH) {
						mCurrentLineHeight = cellH;
					}

					CharIt it = mChars.insert(make_pair(code, c)).first;
					return &(it->second);
				}
				int charHeight() const {
					return mCharHeight;
				}
			private:
				void operator=(const FontImplOS&); //금지

				struct Char {
					unsigned char mTexturePage; //텍스처 페이지
					unsigned char mHeight; //높이 정보(픽셀)
					unsigned char mWidth; //폭 정보(픽셀)
					unsigned short mX; //왼쪽 끝
					unsigned short mY; //상단
				};
				typedef map< unsigned short, Char > CharMap;
				typedef CharMap::iterator CharIt;
				typedef map< int, Texture > TextureMap;
				typedef TextureMap::iterator TextureIt;

				int mCharHeight;
				int mCharAscent; //TEXTMETRIX에서 취득
				bool mAntiAliased;
				HFONT mFontHandle;
				HDC mDeviceContextHandle;
				CharMap mChars;
				TextureMap mTextures;
				int mCurrentX; //다음 문자의 왼쪽 상단 X
				int mCurrentY; // 다음 문자의 왼쪽 상단 Y
				int mCurrentLineHeight; //현재 행의 최대 높이
				int mCurrentPage; //현재 기입 중인 텍스처 번호
				const int mTextureWidth;
				const int mTextureHeight;
			};

		} //namespace {}

	} //Scene:
} //GameLib
#endif
