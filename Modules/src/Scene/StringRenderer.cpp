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

		//정렬용 편리 클래스
		template< class T > struct PointerLess {
			bool operator()(const T* a, const T* b) {
				return (*a < *b);
			}
		};

		class StringRenderer::Impl : public ReferenceType {
		public:
			Impl(int charCapacity, int requestCapacity) :
				mCharCapacity(charCapacity),
				mRequests(requestCapacity) { //리퀘스트 확보
					//TODO:스테이트 변경 명령의 최대 수는 일단 글자 몇 분.너무 지나치다.
				mPrimitiveRenderer = PrimitiveRenderer::create(charCapacity * 2, charCapacity + 4);
				//draw 두 번 부르는데 대처하기 위한 변수
				mPreviousFrameId = Manager().frameId() - 1; //지나간 값에
			}
			~Impl() {
			}
			void add(const Vector2& p, const char* s, unsigned c, bool wrap, float depth) {
				STRONG_ASSERT(mCurrentFont && "Font hasn't been set.");
				mRequests.push(); //빈 푸시
				Request& r = *mRequests.get();
				r.mPosition = p;
				r.mDepth = depth;
				r.mString = s;
				r.mColor = c;
				r.mFont = mCurrentFont;
				r.mWrap = wrap;
			}
			void draw() {
				//프레임 id체크
				unsigned fid = Manager().frameId();
				STRONG_ASSERT(mPreviousFrameId != fid && "StringRenderer::draw() : you can't draw() twice in a frame!");
				mPreviousFrameId = fid;

				if (mRequests.size() == 0) { //할것이 없음
					return;
				}
				//TriangleRenderer에 표준 스테이트를 발행
				mPrimitiveRenderer.setBlendMode(Graphics::BLEND_LINEAR);
				mPrimitiveRenderer.setCullMode(Graphics::CULL_NONE);
				mPrimitiveRenderer.enableDepthTest(false);
				mPrimitiveRenderer.enableDepthWrite(false);
				// 현재 뷰포트 가져와 float화
				Vector2 screenSize;
				int tw, th;
				Graphics::Manager().getViewport(0, 0, &tw, &th);
				screenSize.set(
					static_cast<float>(tw),
					static_cast<float>(th));

				// 문자 버퍼 준비
				Array< Char > chars(mCharCapacity);
				int charPosition = 0;
				// 요청을 꺼내서 처리.
				while (mRequests.size() > 0) {
					Request& r = *mRequests.get();
					Vector2 pos = r.mPosition;
					float charHeight = static_cast<float>(r.mFont.charHeight());
					//한 글자씩 꺼내다.unsigned char 로 할 필요 있음
					int n = static_cast<int>(r.mString.size());
					const unsigned char* s = reinterpret_cast<const unsigned char*>(r.mString.c_str());
					for (int i = 0; i < n; ++i) {
						//Char준비
						int code = s[i];
						//ShiftJis 범위인가?
						if ((code >= 0x81 && code <= 0x9F) || (code >= 0xE0 && code <= 0xFC)) {
							if ((i + 1 < n)) {
								code <<= 8;
								code |= s[i + 1]; //2바이트 코드
								++i;
							}
						}
						if (code == '\n') { //개행문자입니다
							pos.x = r.mPosition.x;
							pos.y += charHeight;
							continue;
						}
						//문자 겟
						Char& c = chars[charPosition];
						bool found = r.mFont.getChar(
							&c.mTexture,
							&c.mUv0,
							&c.mUv1,
							code);
						if (found) { //문자 있다면
							// 현재 위치에 픽셀을 더해간다.
							c.mSize.setSub(c.mUv1, c.mUv0);
							// 텍스쳐로부터 폭과 높이를 받아 픽셀로 고치는 비를 구한다.
							c.mSize.x *= static_cast<float>(c.mTexture.width());
							c.mSize.y *= static_cast<float>(c.mTexture.height());
						}
						else { //문자가없다면。두부가 나왔다(주:네모를 두부라고 하는듯)
							c.mSize.set(charHeight, charHeight); //정방형
						}
						//컬링과 반환 계산
						//반환시 조금이라도 벗어나면 반복한다
						if (r.mWrap) {
							if (pos.x + c.mSize.x > screenSize.x) {
								pos.x = r.mPosition.x;
								pos.y += charHeight;
							}
						}
						else { //반환점이 없으면 완전히 넘칠 때 잘라 버리다
							if (pos.x > screenSize.x) {
								break;
							}
						}
						//세로가 완전히 넘친 경우에만 종료.
						if (pos.y > screenSize.y) {
							break;
						}
						//엔트리추가
						c.mColor = r.mColor;
						c.mPosition.set(pos.x, pos.y, r.mDepth);
						++charPosition;
						//문자보내기
						pos.x += c.mSize.x;
					}
					mRequests.pop();
				}
				if (charPosition == 0) { //문자가 없다면
					return;
				}
				// 처리후 문자 버퍼를 Z와 텍스처로 정렬
				Array< Char* > charPointers(charPosition);
				for (int i = 0; i < charPosition; ++i) {
					charPointers[i] = &chars[i];
				}
				sort(&charPointers[0], &charPointers[0] + charPosition, PointerLess< Char >());
				//정렬된 순서대로 문자 버퍼를 Triangle Buffer에 add
				Texture lastTexture;
				for (int i = 0; i < charPosition; ++i) {
					Char& c = *charPointers[i];
					//텍스처가 전과 다르면 세트
					if (lastTexture != c.mTexture) {
						mPrimitiveRenderer.setTexture(c.mTexture);
						lastTexture = c.mTexture;
					}
					//꼭짓점을 만들어 사각형을 그린다
					Vector2 p[2];
					p[0].set(c.mPosition.x, c.mPosition.y); //좌상점
					p[1].set(c.mPosition.x + c.mSize.x, c.mPosition.y + c.mSize.y); //우하
					//그리기
					if (c.mTexture) { //텍스처 있으면
						mPrimitiveRenderer.addRectangle(
							p[0], p[1],
							c.mUv0, c.mUv1,
							c.mColor);
					}
					else { //없네요。두부가 나왔다(주:네모를 두부라고 하는듯)
						mPrimitiveRenderer.addRectangle(
							p[0], p[1],
							c.mColor);
					}
				}
				// 다 끝나서 Triangle Renderer 그리기
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
			// 문자 구조체
			struct Char {
				//Z와 텍스처로 비교.Z는 안쪽이 먼저
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
				Vector3 mPosition; //좌상점
				Vector2 mUv0; //좌상UV
				Vector2 mUv1; //우하UV
				Vector2 mSize;
				Texture mTexture; //텍스쳐
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
