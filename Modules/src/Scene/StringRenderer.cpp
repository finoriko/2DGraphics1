#include "GameLib/GameLib.h"
#include "GameLib/Scene/StringRenderer.h"
#include "GameLib/Scene/Font.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Scene/PrimitiveRenderer.h"
#include "GameLib/Base/Stack.h"
#include "GameLib/Base/Array.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include <algorithm>
#include <string>
using namespace std;

namespace GameLib {
	namespace Scene {

		using namespace GameLib::Math;
		using namespace GameLib::Graphics;

		//���Ŀ� �� Ŭ����
		template< class T > struct PointerLess {
			bool operator()(const T* a, const T* b) {
				return (*a < *b);
			}
		};

		class StringRenderer::Impl : public ReferenceType {
		public:
			Impl(int charCapacity, int requestCapacity) :
				mCharCapacity(charCapacity),
				mRequests(requestCapacity) { //������Ʈ Ȯ��
					//TODO:������Ʈ ���� ����� �ִ� ���� �ϴ� ���� �� ��.�ʹ� ����ġ��.
				mPrimitiveRenderer = PrimitiveRenderer::create(charCapacity * 2, charCapacity + 4);
				//draw �� �� �θ��µ� ��ó�ϱ� ���� ����
				mPreviousFrameId = Manager().frameId() - 1; //������ ����
			}
			~Impl() {
			}
			void add(const Vector2& p, const char* s, unsigned c, bool wrap, float depth) {
				STRONG_ASSERT(mCurrentFont && "Font hasn't been set.");
				mRequests.push(); //�� Ǫ��
				Request& r = *mRequests.get();
				r.mPosition = p;
				r.mDepth = depth;
				r.mString = s;
				r.mColor = c;
				r.mFont = mCurrentFont;
				r.mWrap = wrap;
			}
			void draw() {
				//������ idüũ
				unsigned fid = Manager().frameId();
				STRONG_ASSERT(mPreviousFrameId != fid && "StringRenderer::draw() : you can't draw() twice in a frame!");
				mPreviousFrameId = fid;

				if (mRequests.size() == 0) { //�Ұ��� ����
					return;
				}
				//TriangleRenderer�� ǥ�� ������Ʈ�� ����
				mPrimitiveRenderer.setBlendMode(Graphics::BLEND_LINEAR);
				mPrimitiveRenderer.setCullMode(Graphics::CULL_NONE);
				mPrimitiveRenderer.enableDepthTest(false);
				mPrimitiveRenderer.enableDepthWrite(false);
				// ���� ����Ʈ ������ floatȭ
				Vector2 screenSize;
				int tw, th;
				Graphics::Manager().getViewport(0, 0, &tw, &th);
				screenSize.set(
					static_cast<float>(tw),
					static_cast<float>(th));

				// ���� ���� �غ�
				Array< Char > chars(mCharCapacity);
				int charPosition = 0;
				// ��û�� ������ ó��.
				while (mRequests.size() > 0) {
					Request& r = *mRequests.get();
					Vector2 pos = r.mPosition;
					float charHeight = static_cast<float>(r.mFont.charHeight());
					//�� ���ھ� ������.unsigned char �� �� �ʿ� ����
					int n = static_cast<int>(r.mString.size());
					const unsigned char* s = reinterpret_cast<const unsigned char*>(r.mString.c_str());
					for (int i = 0; i < n; ++i) {
						//Char�غ�
						int code = s[i];
						//ShiftJis �����ΰ�?
						if ((code >= 0x81 && code <= 0x9F) || (code >= 0xE0 && code <= 0xFC)) {
							if ((i + 1 < n)) {
								code <<= 8;
								code |= s[i + 1]; //2����Ʈ �ڵ�
								++i;
							}
						}
						if (code == '\n') { //���๮���Դϴ�
							pos.x = r.mPosition.x;
							pos.y += charHeight;
							continue;
						}
						//���� ��
						Char& c = chars[charPosition];
						bool found = r.mFont.getChar(
							&c.mTexture,
							&c.mUv0,
							&c.mUv1,
							code);
						if (found) { //���� �ִٸ�
							// ���� ��ġ�� �ȼ��� ���ذ���.
							c.mSize.setSub(c.mUv1, c.mUv0);
							// �ؽ��ķκ��� ���� ���̸� �޾� �ȼ��� ��ġ�� �� ���Ѵ�.
							c.mSize.x *= static_cast<float>(c.mTexture.width());
							c.mSize.y *= static_cast<float>(c.mTexture.height());
						}
						else { //���ڰ����ٸ顣�κΰ� ���Դ�(��:�׸� �κζ�� �ϴµ�)
							c.mSize.set(charHeight, charHeight); //������
						}
						//�ø��� ��ȯ ���
						//��ȯ�� �����̶� ����� �ݺ��Ѵ�
						if (r.mWrap) {
							if (pos.x + c.mSize.x > screenSize.x) {
								pos.x = r.mPosition.x;
								pos.y += charHeight;
							}
						}
						else { //��ȯ���� ������ ������ ��ĥ �� �߶� ������
							if (pos.x > screenSize.x) {
								break;
							}
						}
						//���ΰ� ������ ��ģ ��쿡�� ����.
						if (pos.y > screenSize.y) {
							break;
						}
						//��Ʈ���߰�
						c.mColor = r.mColor;
						c.mPosition.set(pos.x, pos.y, r.mDepth);
						++charPosition;
						//���ں�����
						pos.x += c.mSize.x;
					}
					mRequests.pop();
				}
				if (charPosition == 0) { //���ڰ� ���ٸ�
					return;
				}
				// ó���� ���� ���۸� Z�� �ؽ�ó�� ����
				Array< Char* > charPointers(charPosition);
				for (int i = 0; i < charPosition; ++i) {
					charPointers[i] = &chars[i];
				}
				sort(&charPointers[0], &charPointers[0] + charPosition, PointerLess< Char >());
				//���ĵ� ������� ���� ���۸� Triangle Buffer�� add
				Texture lastTexture;
				for (int i = 0; i < charPosition; ++i) {
					Char& c = *charPointers[i];
					//�ؽ�ó�� ���� �ٸ��� ��Ʈ
					if (lastTexture != c.mTexture) {
						mPrimitiveRenderer.setTexture(c.mTexture);
						lastTexture = c.mTexture;
					}
					//�������� ����� �簢���� �׸���
					Vector2 p[2];
					p[0].set(c.mPosition.x, c.mPosition.y); //�»���
					p[1].set(c.mPosition.x + c.mSize.x, c.mPosition.y + c.mSize.y); //����
					//�׸���
					if (c.mTexture) { //�ؽ�ó ������
						mPrimitiveRenderer.addRectangle(
							p[0], p[1],
							c.mUv0, c.mUv1,
							c.mColor);
					}
					else { //���׿䡣�κΰ� ���Դ�(��:�׸� �κζ�� �ϴµ�)
						mPrimitiveRenderer.addRectangle(
							p[0], p[1],
							c.mColor);
					}
				}
				// �� ������ Triangle Renderer �׸���
				mPrimitiveRenderer.draw();
			}
			void setFont(Font& font) {
				mCurrentFont = font;
			}
		private:
			struct Request {
				Vector2 mPosition;
				float mDepth;
				string mString;
				unsigned mColor;
				Font mFont;
				bool mWrap;
			};
			// ���� ����ü
			struct Char {
				//Z�� �ؽ�ó�� ��.Z�� ������ ����
				bool operator<(const Char& a) const {
					if (mPosition.z > a.mPosition.z) {
						return true;
					}
					else if (mPosition.z < a.mPosition.z) {
						return false;
					}
					else if (mTexture < a.mTexture) {
						return true;
					}
					else {
						return false;
					}
				}
				Vector3 mPosition; //�»���
				Vector2 mUv0; //�»�UV
				Vector2 mUv1; //����UV
				Vector2 mSize;
				Texture mTexture; //�ؽ���
				unsigned mColor;
			};
			int mCharCapacity;
			Stack< Request > mRequests;
			PrimitiveRenderer mPrimitiveRenderer;
			Font mCurrentFont;
			unsigned mPreviousFrameId;
		};

		StringRenderer StringRenderer::create(int charCapacity, int requestCapacity) {
			StringRenderer r;
			r.mImpl = NEW Impl(charCapacity, requestCapacity);
			return r;
		}

		void StringRenderer::add(int x, int y, const char* s, unsigned c, bool wrap, float z) {
			Vector2 p(
				static_cast<float>(x),
				static_cast<float>(y));
			mImpl->add(p, s, c, wrap, z);
		}

		void StringRenderer::add(const Vector2& p, const char* s, unsigned c, bool wrap, float z) {
			mImpl->add(p, s, c, wrap, z);
		}

		void StringRenderer::draw() {
			mImpl->draw();
		}

		void StringRenderer::setFont(Font& font) {
			mImpl->setFont(font);
		}

#define TYPE StringRenderer
#include "GameLib/Base/Impl/ReferenceTypeTemplate.h"

	} //namespace Scene
} //namespace GameLib
