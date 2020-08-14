#ifndef INCLUDED_GAMELIB_SCENE_FONTIMPLOS_H
#define INCLUDED_GAMELIB_SCENE_FONTIMPLOS_H

#include "Scene/FontImpl.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/WindowCreator/WindowCreator.h"
#include <map> //�����ʾ� �������� �ٲٰ� �ʹ�...

#include <windows.h>
#undef min
#undef max

namespace GameLib {
	namespace Scene {
		using namespace std;

		using namespace GameLib::Graphics;
		using namespace GameLib::Math;

		namespace { //����

		//Windows���� ��Ʈ �����͸� ���
			class FontImplOS : public Font::Impl {
				struct Char;
			public:
				FontImplOS(
					const char* fontName,
					int charHeightHint, //�̷��� �ȴٰ�� �� �� ����
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
					//�ؽ�ó ����. �ϴ� �� ��
					Texture t = Texture::create(mTextureWidth, mTextureHeight, false);
					//�ʿ� �ݳ�
					mTextures.insert(make_pair(0, t));

					//�۲ø��� UNICODE ��ȯ
					DWORD pitch = (proportional) ? VARIABLE_PITCH : FIXED_PITCH;

					//��Ʈ ����
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
					//HDC����
					mDeviceContextHandle = CreateCompatibleDC(NULL);
					//�ؽ�Ʈ ��Ʈ���� ���
					//font���ë�
					SelectObject(mDeviceContextHandle, mFontHandle);
					//���� ��Ʈ���� ����
					TEXTMETRIC tm;
					GetTextMetrics(mDeviceContextHandle, &tm);
					mCharHeight = tm.tmHeight; //������ ������ �����
					mCharAscent = tm.tmAscent; //���̽����� ���� ġ���� �ʿ���
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
					//�ڵ� ���̺� ������ ����Ѵ�.
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
					//font��Ʈ
					SelectObject(mDeviceContextHandle, mFontHandle);

					GLYPHMETRICS gm; // ����
					MAT2 mat = {
						{ 0, 1 },
						{ 0, 0 },
						{ 0, 0 },
						{ 0, 1 }
					};
					DWORD size;
					const UINT format = (mAntiAliased) ? GGO_GRAY8_BITMAP : GGO_BITMAP;
					// �켱 �ʿ��� ���� ����� ȹ��
					size = GetGlyphOutlineA(
						mDeviceContextHandle,
						static_cast<UINT>(code),
						format,
						&gm, 0, NULL, &mat);
					//ġ�� ���
					int cellW = static_cast<int>(gm.gmCellIncX);
					int cellH = static_cast<int>(mCharHeight); //IncY�� ������ �� �ǰ�
					int glyphW = static_cast<int>(gm.gmBlackBoxX);
					int glyphH = static_cast<int>(gm.gmBlackBoxY);
					int glyphX = static_cast<int>(gm.gmptGlyphOrigin.x);
					int glyphY = static_cast<int>(mCharAscent - gm.gmptGlyphOrigin.y);
					if (glyphW + glyphX > cellW) { //���� ���ۿ� ����
						cellW = glyphW + glyphX;
					}
					if (glyphH + glyphY > cellH) { //���� ���ۿ� ����
						cellH = glyphH + glyphY;
					}
					//���� �ؽ�ó ���
					TextureIt tit = mTextures.find(mCurrentPage);
					ASSERT(tit != mTextures.end());
					Texture* tex = &(tit->second);
					int tw = tex->width();
					int th = tex->height();
					//���ƾ��!
					if (mCurrentX + cellW >= tw) {
						mCurrentX = 0;
						mCurrentY += mCurrentLineHeight;
						mCurrentLineHeight = 0;
						//���� ���ƾ��!
						if (mCurrentY + cellH >= th) {
							mCurrentY = 0;
							++mCurrentPage;
							Texture t = Texture::create(mTextureWidth, mTextureHeight, false);
							//�ʿ� �ݳ�
							mTextures.insert(make_pair(mCurrentPage, t));
							//�ؽ��� �ٽ� �ϱ�
							TextureIt tit = mTextures.find(mCurrentPage);
							ASSERT(tit != mTextures.end());
							tex = &(tit->second);
						}
					}
					//�ؽ�ó��(�κ����)
					unsigned* dst;
					int dstPitch;
					tex->lock(
						&dst,
						&dstPitch,
						mCurrentX,
						mCurrentY,
						cellW,
						cellH);

					if (size == 0) { //�����ŭ ���� 0�޿��
						for (int y = 0; y < cellH; ++y) {
							for (int x = 0; x < cellW; ++x) {
								dst[x] = 0x00ffffff;
							}
							dst += dstPitch / 4;
						}
					}
					else {
						// ���� Ȯ��
						BYTE* data = NEW BYTE[size];
						int srcPitch = (glyphW + 3) & ~(3); //4����Ʈ�˶���

						// �� ��° ȣ���� ��¥.������ ���.
						size = GetGlyphOutlineA(
							mDeviceContextHandle,
							static_cast<UINT>(code),
							format,
							&gm, size, data, &mat);
						ASSERT(size != GDI_ERROR);

						//1bit�϶��� �ʹ� �����ؼ� ����Ʈ�迭�� ����
						if (mAntiAliased) { //��Ƽ�� �ִٸ� 64->255���� ��ȯ
							for (DWORD i = 0; i < size; ++i) {
								data[i] = data[i] * 255 / 64;
							}
						}
						else {
							BYTE* newData = NEW BYTE[size * 8]; //8�踸 ������ ������ݾ�
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
							//�� �迭�� deletel|
							SAFE_DELETE_ARRAY(data);
							//�� �迭�� ��ü
							data = newData;
						}
						BYTE* src = data;
						//��� ������ ����
						int y = 0;
						for (; y < glyphY; ++y) {
							for (int x = 0; x < cellW; ++x) {
								dst[x] = 0x00ffffff;
							}
							dst += dstPitch / 4;
						}
						//�۸��� ��ü Z
						for (; y < glyphY + glyphH; ++y) {
							LONG x = 0;
							//���� ����
							for (; x < glyphX; ++x) {
								dst[x] = 0x00ffffff;
							}
							//�۸��� ��ü
							for (; x < glyphX + glyphW; ++x) {
								int a = src[x - glyphX];
								dst[x] = (a << 24) | 0xffffff;
							}
							//���� ����
							for (; x < cellW; ++x) {
								dst[x] = 0x00ffffff;
							}
							dst += dstPitch / 4;
							src += srcPitch;
						}
						//�Ϻ� ������ ����
						for (; y < cellH; ++y) {
							for (int x = 0; x < cellW; ++x) {
								dst[x] = 0x00ffffff;
							}
							dst += dstPitch / 4;
						}
						SAFE_DELETE_ARRAY(data);
					}
					tex->unlock(&dst, 0);
					//����Ʈ�� �߰�
					Char c;
					c.mTexturePage = static_cast<unsigned char>(mCurrentPage);
					c.mWidth = static_cast<unsigned char>(cellW);
					c.mHeight = static_cast<unsigned char>(cellH);
					c.mX = static_cast<unsigned short>(mCurrentX);
					c.mY = static_cast<unsigned short>(mCurrentY);
					mCurrentX += cellW; //���� ��ġ�� �����ϴ�
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
				void operator=(const FontImplOS&); //����

				struct Char {
					unsigned char mTexturePage; //�ؽ�ó ������
					unsigned char mHeight; //���� ����(�ȼ�)
					unsigned char mWidth; //�� ����(�ȼ�)
					unsigned short mX; //���� ��
					unsigned short mY; //���
				};
				typedef map< unsigned short, Char > CharMap;
				typedef CharMap::iterator CharIt;
				typedef map< int, Texture > TextureMap;
				typedef TextureMap::iterator TextureIt;

				int mCharHeight;
				int mCharAscent; //TEXTMETRIX���� ���
				bool mAntiAliased;
				HFONT mFontHandle;
				HDC mDeviceContextHandle;
				CharMap mChars;
				TextureMap mTextures;
				int mCurrentX; //���� ������ ���� ��� X
				int mCurrentY; // ���� ������ ���� ��� Y
				int mCurrentLineHeight; //���� ���� �ִ� ����
				int mCurrentPage; //���� ���� ���� �ؽ�ó ��ȣ
				const int mTextureWidth;
				const int mTextureHeight;
			};

		} //namespace {}

	} //Scene:
} //GameLib
#endif
