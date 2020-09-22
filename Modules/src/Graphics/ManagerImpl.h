#ifndef INCLUDED_GAMELIB_GRAPHICS_MANAGERIMPL_H
#define INCLUDED_GAMELIB_GRAPHICS_MANAGERIMPL_H

// 코드화된 쉐이더 객체
#include "Graphics/NoLightingVsGenerated.h"
#include "Graphics/VertexLightingVsGenerated.h"
#include "Graphics/PixelLightingVsGenerated.h"
#include "Graphics/PixelLightingPsGenerated.h"

#include "GameLib/Math/Functions.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Graphics/Enum.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Graphics/VertexBuffer.h"
#include "GameLib/Graphics/IndexBuffer.h"
#include "GameLib/Threading/Manager.h"
#include "Graphics/TextureImpl.h"
#include "Graphics/VertexBufferImpl.h"
#include "Graphics/IndexBufferImpl.h"

namespace GameLib
{
	using namespace Math;
	namespace Graphics
	{
		class ManagerImpl
		{
		public:
			ManagerImpl(HWND wh, int w, int h, bool fullScreen, bool vSync, bool antiAlias) :
				mDirect3d(0),
				mDevice(0),
				mWidth(w),
				mHeight(h),
				mFullScreen(fullScreen),
				mVSync(vSync),
				mPixelShaderReady(false),
				mLightingMode(LIGHTING_NONE),
				mFullScreenWindowWidth(0),
				mFullScreenWindowHeight(0),
				mFrameId(0),
				mCanRender(true),
				mWindowHandle(wh),
				mFullScreenQuadVertexBuffer(0),
				mVertexDeclaration(0),
				mNoLightingVertexShader(0),
				mVertexLightingVertexShader(0),
				mPixelLightingVertexShader(0),
				mPixelLightingPixelShader(0),
				mWhiteTexture(0),
				mCurrentTexture(0),
				mCurrentVertexBuffer(0),
				mCurrentIndexBuffer(0),
				mCurrentDepthTest(TRUE),
				mCurrentDepthWrite(TRUE),
				mCurrentCullMode(D3DCULL_CW),
				mCurrentAlphaBlend(FALSE),
				mCurrentAlphaTest(FALSE),
				mCurrentDestBlend(D3DBLEND_ZERO),
				mCurrentMinFilter(D3DTEXF_POINT),
				mCurrentMagFilter(D3DTEXF_POINT),
				mCurrentMipFilter(D3DTEXF_NONE),
				mBestMinFilter(D3DTEXF_POINT),
				mBestMagFilter(D3DTEXF_POINT),
				mBestMipFilter(D3DTEXF_NONE),
				mMaxAnisotropy(1),
				mMsaaQualityWindowed(0),
				mMsaaQualityFullScreen(0),
				mLightPositionXs(0.f),
				mLightPositionYs(0.f),
				mLightPositionZs(0.f),
				mEyePosition(0.f, 0.f, 0.f),
				mDiffuseColor(1.f),
				mSpecularColor(1.f),
				mEmissionColor(0.f),
				mAmbientColor(1.f),
				mLightChanged(true),//첫회의 draw는 라이트 정보 보내줘
				mMatricesChanged(true) { //첫회의 draw는 행렬도 보내줘
				for (int i = 0; i < 4; ++i) {
					mLightColors[i].set(0.f, 0.f, 0.f);
					mLightIntensities[i] = 0.f;
				}
				mProjectionViewMatrix.setIdentity();
				mWorldMatrix.setIdentity();
				HRESULT hr;
				//Direct3D작성
				mDirect3d = Direct3DCreate9(D3D_SDK_VERSION);
				STRONG_ASSERT(mDirect3d && "Direct3D creation failed.");

				//caps get
				D3DCAPS9 caps;
				mDirect3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
				//꼭지점 쉐이더 1.1행 맞나?
				bool vs11 = (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1));
				//픽셀 2.0 할줄 알아?
				mPixelShaderReady = (caps.PixelShaderVersion >= D3DPS_VERSION(2, 0));
				//비등방성(anisotropy)는 몇배나 되는가?
				mMaxAnisotropy = caps.MaxAnisotropy;
				//최강 텍스처 필터 획득
				DWORD tfCaps = caps.TextureFilterCaps;
				if (tfCaps & D3DPTFILTERCAPS_MIPFLINEAR) {
					mBestMipFilter = D3DTEXF_LINEAR;
				}
				else if (tfCaps & D3DPTFILTERCAPS_MIPFPOINT) {
					mBestMipFilter = D3DTEXF_POINT;
				}
				if (tfCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC) {
					mBestMagFilter = D3DTEXF_ANISOTROPIC;
				}
				else if (tfCaps & D3DPTFILTERCAPS_MAGFLINEAR) {
					mBestMagFilter = D3DTEXF_LINEAR;
				}
				if (tfCaps & D3DPTFILTERCAPS_MINFANISOTROPIC) {
					mBestMinFilter = D3DTEXF_ANISOTROPIC;
				}
				else if (tfCaps & D3DPTFILTERCAPS_MINFLINEAR) {
					mBestMinFilter = D3DTEXF_LINEAR;
				}
				//MSAA를 몇배로 갈수있니?
				if (antiAlias) {
					hr = mDirect3d->CheckDeviceMultiSampleType(
						D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						D3DFMT_X8R8G8B8,
						TRUE,
						D3DMULTISAMPLE_NONMASKABLE,
						&mMsaaQualityWindowed);
					STRONG_ASSERT(hr != D3DERR_INVALIDCALL && "CheckDeviceMultiSampleType : INVALID CALL");
					STRONG_ASSERT(hr != D3DERR_INVALIDDEVICE && "CheckDeviceMultiSampleType : INVALID CALL");
					if (FAILED(hr)) {
						mMsaaQualityWindowed = 0;
					}
					hr = mDirect3d->CheckDeviceMultiSampleType(
						D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						D3DFMT_X8R8G8B8,
						TRUE,
						D3DMULTISAMPLE_NONMASKABLE,
						&mMsaaQualityFullScreen);
					STRONG_ASSERT(hr != D3DERR_INVALIDCALL && "CheckDeviceMultiSampleType : INVALID CALL");
					STRONG_ASSERT(hr != D3DERR_INVALIDDEVICE && "CheckDeviceMultiSampleType : INVALID CALL");
					if (FAILED(hr)) {
						mMsaaQualityFullScreen = 0;
					}
				}
			}
			void setTexture(Texture::Impl* o) {
				if (mCurrentTexture == o) {
					return;
				}
				HRESULT hr;
				IDirect3DTexture9* dxObj = (o) ? o->mDxObject : 0;
				hr = mDevice->SetTexture(0, dxObj);
				STRONG_ASSERT(SUCCEEDED(hr) && "SetTexture : INVALID CALL");
				if (mCurrentTexture) {
					mCurrentTexture->release(); //카운트 줄이기
					if (mCurrentTexture->referenceCount() == 0) {
						SAFE_DELETE(mCurrentTexture);
					}
				}
				mCurrentTexture = o;
				if (o) {
					o->refer();
				}
			}
			void setVertexBuffer(VertexBuffer::Impl* o) {
				if (mCurrentVertexBuffer == o) {
					return;
				}
				HRESULT hr;
				IDirect3DVertexBuffer9* dxObj = (o) ? o->mDxObject : 0;
				hr = mDevice->SetStreamSource(0, dxObj, 0, sizeof(Vertex)); //Strideきめうち
				STRONG_ASSERT(SUCCEEDED(hr) && "SetStreamSource : INVALID CALL");
				if (mCurrentVertexBuffer) {
					mCurrentVertexBuffer->release();
					if (mCurrentVertexBuffer->referenceCount() == 0) {
						SAFE_DELETE(mCurrentVertexBuffer);
					}
				}
				mCurrentVertexBuffer = o;
				if (o) {
					o->refer();
				}
			}
			void setIndexBuffer(IndexBuffer::Impl* o) {
				if (mCurrentIndexBuffer == o) {
					return;
				}
				HRESULT hr;
				IDirect3DIndexBuffer9* dxObj = (o) ? o->mDxObject : 0;
				hr = mDevice->SetIndices(dxObj);
				STRONG_ASSERT(SUCCEEDED(hr) && "SetIndices : INVALID CALL");
				if (mCurrentIndexBuffer) {
					mCurrentIndexBuffer->release();
					if (mCurrentIndexBuffer->referenceCount() == 0) {
						SAFE_DELETE(mCurrentIndexBuffer);
					}
				}
				mCurrentIndexBuffer = o;
				if (o) {
					o->refer();
				}
			}
			void enableDepthTest(bool f) {
				BOOL tf = (f) ? TRUE : FALSE;
				if (mCurrentDepthTest == tf) {
					return;
				}
				HRESULT hr;
				hr = mDevice->SetRenderState(D3DRS_ZENABLE, tf);
				STRONG_ASSERT(SUCCEEDED(hr) && "SetRenderState error : INVALID CALL");
				mCurrentDepthTest = tf;
			}
			void enableDepthWrite(bool f) {
				BOOL tf = (f) ? TRUE : FALSE;
				if (mCurrentDepthWrite == tf) {
					return;
				}
				HRESULT hr;
				hr = mDevice->SetRenderState(D3DRS_ZWRITEENABLE, tf);
				STRONG_ASSERT(SUCCEEDED(hr) && "SetRenderState error : INVALID CALL");
				mCurrentDepthWrite = tf;
			}
			void setBlendMode(BlendMode b) {
				BOOL alphaBlend;
				D3DBLEND dstBlend;
				BOOL alphaTest;
				if (b == BLEND_OPAQUE) {
					alphaBlend = FALSE;
					dstBlend = D3DBLEND_INVSRCALPHA;
					alphaTest = TRUE;
				}
				else {
					alphaBlend = TRUE;
					alphaTest = FALSE;
					if (b == BLEND_LINEAR) {
						dstBlend = D3DBLEND_INVSRCALPHA;
					}
					else {
						dstBlend = D3DBLEND_ONE;
					}
				}
				HRESULT hr;
				if (mCurrentAlphaBlend != alphaBlend) {
					hr = mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, alphaBlend);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetRenderState : INVALID CALL");
					mCurrentAlphaBlend = alphaBlend;
				}
				if (mCurrentDestBlend != dstBlend) {
					hr = mDevice->SetRenderState(D3DRS_DESTBLEND, dstBlend);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetRenderState : INVALID CALL");
					mCurrentDestBlend = dstBlend;
				}
				if (mCurrentAlphaTest != alphaTest) {
					hr = mDevice->SetRenderState(D3DRS_ALPHATESTENABLE, alphaTest);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetRenderState : INVALID CALL");
					mCurrentAlphaTest = alphaTest;
				}
			}
			void setTextureFilter(TextureFilter f) {
				D3DTEXTUREFILTERTYPE dxMinF = D3DTEXF_NONE;
				D3DTEXTUREFILTERTYPE dxMagF = D3DTEXF_NONE;
				D3DTEXTUREFILTERTYPE dxMipF = D3DTEXF_NONE;
				if (f == TEXTURE_FILTER_POINT) {
					dxMinF = D3DTEXF_POINT;
					dxMagF = D3DTEXF_POINT;
					dxMipF = D3DTEXF_NONE;
				}
				else if (f == TEXTURE_FILTER_LINEAR) { //리니어라고 말하지만 안이소?GO!
					dxMinF = mBestMinFilter;
					dxMagF = mBestMagFilter;
					dxMipF = mBestMipFilter;
				}
				HRESULT hr;
				if (mCurrentMinFilter != dxMinF) {
					mCurrentMinFilter = dxMinF;
					hr = mDevice->SetSamplerState(0, D3DSAMP_MINFILTER, dxMinF);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetSamplerState : INVALID CALL");
				}
				if (mCurrentMagFilter != dxMagF) {
					mCurrentMagFilter = dxMagF;
					hr = mDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, dxMagF);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetSamplerState : INVALID CALL");
				}
				if (mCurrentMipFilter != dxMipF) {
					mCurrentMipFilter = dxMipF;
					hr = mDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, dxMipF);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetSamplerState : INVALID CALL");
				}
			}
			void setCullMode(CullMode c) {
				D3DCULL mode = D3DCULL_NONE;
				switch (c) {
				case CULL_NONE: mode = D3DCULL_NONE; break;
				case CULL_BACK: mode = D3DCULL_CW; break;
				case CULL_FRONT: mode = D3DCULL_CCW; break;
				}
				if (mCurrentCullMode == mode) {
					return;
				}
				HRESULT hr;
				hr = mDevice->SetRenderState(D3DRS_CULLMODE, mode);
				STRONG_ASSERT(SUCCEEDED(hr) && "SetRenderState : INVALID CALL");

				mCurrentCullMode = mode;
			}
			void setProjectionViewMatrix(const Matrix44& m) {
				if (mProjectionViewMatrix != m) { //조금이라도 다르다면
					mProjectionViewMatrix = m;
					mMatricesChanged = true;
				}
			}
			void setWorldMatrix(const Matrix34& m) {
				if (mWorldMatrix != m) { //조금이라도 다르다면
					mWorldMatrix = m;
					mMatricesChanged = true;
				}
			}
			void sendMatrices() {
				HRESULT hr;
				//DX용 0.5픽셀 밀링이 있는 최종 변환 행렬
				/*
				x,y를 어긋나게 하는 행렬 O를 만들고
				O*PV*W
				과 곱하여 최종 행렬로 삼는다.

				그러나 O는 03, 13의 두 요소에 의미가 있고 다른 것은 의미가 없는 행렬이다.
				따라서 행렬연산을 풀세트하는 것은 어리석다.

				1 0 0 X    a b c d   a+Xm b+Xn c+Xo d+Xp
				0 1 0 Y  * e f g h = e+Ym f+Yn g+Yo h+Yp
				0 0 1 0    i j k l   i    j    k    l
				0 0 0 1    m n o p   m    n    o    p
				*/
				Matrix44 pvwm;
				pvwm.setMul(mProjectionViewMatrix, mWorldMatrix);
				float x = -1.f / static_cast<float>(mPresentParameters.BackBufferWidth);
				float y = 1.f / static_cast<float>(mPresentParameters.BackBufferHeight);
				pvwm.m00 += x * pvwm.m30;
				pvwm.m01 += x * pvwm.m31;
				pvwm.m02 += x * pvwm.m32;
				pvwm.m03 += x * pvwm.m33;
				pvwm.m10 += y * pvwm.m30;
				pvwm.m11 += y * pvwm.m31;
				pvwm.m12 += y * pvwm.m32;
				pvwm.m13 += y * pvwm.m33;
				//법선 변환용 월드 역행렬 전치
				Matrix34 itwm; //inverseTransposedWorldMatrix
				itwm.setInverse(mWorldMatrix);
				itwm.transpose33();

				//전송
				hr = mDevice->SetVertexShaderConstantF(0, &(pvwm.m00), 4);
				STRONG_ASSERT(SUCCEEDED(hr) && "SetVertexShaderConstantF : INVALID CALL");
				hr = mDevice->SetVertexShaderConstantF(4, &(mWorldMatrix.m00), 3);
				STRONG_ASSERT(SUCCEEDED(hr) && "SetVertexShaderConstantF : INVALID CALL");
				hr = mDevice->SetVertexShaderConstantF(7, &(itwm.m00), 3);
				STRONG_ASSERT(SUCCEEDED(hr) && "SetVertexShaderConstantF : INVALID CALL");

				mMatricesChanged = false;
			}
			void draw(int offset, int primitiveNumber, PrimitiveType prim = PRIMITIVE_TRIANGLE) {
				if (mLightChanged) {
					sendLightingParameters();
				}
				if (mMatricesChanged) {
					sendMatrices();
				}
				STRONG_ASSERT(mCurrentVertexBuffer && "VertexBuffer is not set.");
				if (!mCurrentTexture) { //텍스처가 없으면 더미를 꽂는다.
					setTexture(mWhiteTexture);
				}
				D3DPRIMITIVETYPE dxPrim = D3DPT_TRIANGLELIST;
				switch (prim) {
				case PRIMITIVE_TRIANGLE: dxPrim = D3DPT_TRIANGLELIST; break;
				case PRIMITIVE_LINE: dxPrim = D3DPT_LINELIST; break;
				case PRIMITIVE_POINT: dxPrim = D3DPT_POINTLIST; break;
				default: STRONG_ASSERT(0); break;
				}
				HRESULT hr;
				hr = mDevice->DrawPrimitive(
					dxPrim,
					offset,
					primitiveNumber);
				STRONG_ASSERT(SUCCEEDED(hr) && "DrawPrimitive : INVALID CALL");
			}
			void drawIndexed(int offset, int primitiveNumber, PrimitiveType prim = PRIMITIVE_TRIANGLE) {
				if (mLightChanged) {
					sendLightingParameters();
				}
				if (mMatricesChanged) {
					sendMatrices();
				}
				STRONG_ASSERT(mCurrentIndexBuffer && "IndexBuffer is not set.");
				STRONG_ASSERT(mCurrentVertexBuffer && "VertexBuffer is not set.");
				if (!mCurrentTexture) { //텍스처가 없으면 더미를 꽂는다.
					setTexture(mWhiteTexture);
				}
				int vertexNumber = mCurrentVertexBuffer->mVertexNumber;
				D3DPRIMITIVETYPE dxPrim = D3DPT_TRIANGLELIST;
				switch (prim) {
				case PRIMITIVE_TRIANGLE: dxPrim = D3DPT_TRIANGLELIST; break;
				case PRIMITIVE_LINE: dxPrim = D3DPT_LINELIST; break;
				case PRIMITIVE_POINT: dxPrim = D3DPT_POINTLIST; break;
				default: STRONG_ASSERT(0); break;
				}
				HRESULT hr;
				hr = mDevice->DrawIndexedPrimitive(
					dxPrim,
					0,
					0,
					vertexNumber,
					offset,
					primitiveNumber);
				STRONG_ASSERT(SUCCEEDED(hr) && "DrawPrimitive : INVALID CALL");
			}
			void setViewport(int x, int y, int w, int h) {
				mViewport.X = static_cast<DWORD>(x);
				mViewport.Y = static_cast<DWORD>(y);
				mViewport.Width = static_cast<DWORD>(w);
				mViewport.Height = static_cast<DWORD>(h);
				//이제, 진짜 뷰포트를 만들까?
				float dw = static_cast<float>(mPresentParameters.BackBufferWidth);
				float dh = static_cast<float>(mPresentParameters.BackBufferHeight);
				float rw = static_cast<float>(mWidth);
				float rh = static_cast<float>(mHeight);
				float wRatio = dw / rw;
				float hRatio = dh / rh;
				float newX, newY, newW, newH;
				if (wRatio > hRatio) { //세로로 맞추다. 가로가 남다.
					newX = (dw - hRatio * rw) * 0.5f + static_cast<float>(x) * hRatio;
					newY = 0.f;
					newW = static_cast<float>(w) * hRatio;
					newH = dh;
				}
				else { //옆을 맞추다
					newX = 0.f;
					newY = (dh - wRatio * rh) * 0.5f + static_cast<float>(y) * wRatio;
					newW = dw;
					newH = static_cast<float>(h) * wRatio;
				}
				D3DVIEWPORT9 vp;
				vp.MaxZ = mViewport.MaxZ;
				vp.MinZ = mViewport.MinZ;
				vp.Width = static_cast<DWORD>(newW + 0.5f);
				vp.Height = static_cast<DWORD>(newH + 0.5f);
				vp.X = static_cast<DWORD>(newX);
				vp.Y = static_cast<DWORD>(newY);
				HRESULT hr = mDevice->SetViewport(&vp);
				STRONG_ASSERT(hr != D3DERR_INVALIDCALL && "SetViewport : INVALID CALL");
			}
			void getViewport(int* x, int* y, int* w, int* h) {
				if (x) {
					*x = mViewport.X;
				}
				if (y) {
					*y = mViewport.Y;
				}
				if (w) {
					*w = mViewport.Width;
				}
				if (h) {
					*h = mViewport.Height;
				}
			}
			void blendToScreen(Texture::Impl* t) {
				setTexture(t);
				setBlendMode(BLEND_LINEAR);
				enableDepthTest(false);
				enableDepthWrite(false);
				Matrix44 pvm;
				pvm.setIdentity();
				setProjectionViewMatrix(pvm);
				Matrix34 wm;
				wm.setIdentity();
				setWorldMatrix(wm);
				setVertexBuffer(mFullScreenQuadVertexBuffer);
				draw(0, 1);
				setTexture(0); //뒷정리. set한 사람이 되돌리는 약속으로 한다.
			}
			void enableFullScreen(bool f) {
				if (f != mFullScreen) {
					mFullScreen = f;
					mCanRender = false; //의도적으로 디바이스 로스트 상태에
				}
			}
			void getPointerModifier(float* scale, Vector2* offset) {
				float dw = static_cast<float>(mPresentParameters.BackBufferWidth);
				float dh = static_cast<float>(mPresentParameters.BackBufferHeight);
				float rw = static_cast<float>(mWidth);
				float rh = static_cast<float>(mHeight);
				float wRatio = rw / dw;
				float hRatio = rh / dh;
				if (wRatio > hRatio) { //옆에 맞추다.
					*scale = wRatio;
					offset->x = 0.f;
					offset->y = (dh - (rh / wRatio)) * -0.5f;
				}
				else { //세로로 맞추다
					*scale = hRatio;
					offset->x = (dw - (rw / hRatio)) * -0.5f;
					offset->y = 0.f;
				}
			}
			void setLightPosition(int index, const Vector3& position) {
				mLightPositionXs[index] = position.x;
				mLightPositionYs[index] = position.y;
				mLightPositionZs[index] = position.z;
				mLightChanged = true;
			}
			void setLightIntensity(int index, float intensity) {
				mLightIntensities[index] = intensity;
				mLightChanged = true;
			}
			void setLightColor(int index, const Vector3& color) {
				mLightColors[index] = color;
				mLightChanged = true;
			}
			void setEyePosition(const Vector3& position) {
				mEyePosition = position;
				mLightChanged = true;
			}
			void setDiffuseColor(const Vector3& c) {
				mDiffuseColor.x = c.x;
				mDiffuseColor.y = c.y;
				mDiffuseColor.z = c.z;
				mLightChanged = true;
			}
			void setSpecularColor(const Vector3& c) {
				mSpecularColor.x = c.x;
				mSpecularColor.y = c.y;
				mSpecularColor.z = c.z;
				mLightChanged = true;
			}
			void setEmissionColor(const Vector3& c) {
				mEmissionColor.x = c.x;
				mEmissionColor.y = c.y;
				mEmissionColor.z = c.z;
				mLightChanged = true;
			}
			void setAmbientColor(const Vector3& c) {
				mAmbientColor.x = c.x;
				mAmbientColor.y = c.y;
				mAmbientColor.z = c.z;
				mLightChanged = true;
			}
			void setTransparency(float t) {
				mDiffuseColor.w = t;
				mLightChanged = true;
			}
			void setSpecularSharpness(float e) {
				mSpecularColor.w = e;
				mLightChanged = true;
			}
			void sendLightingParameters() {
				//라이트 색은 매번 강도와 색을 곱한다.
				Vector4 lc[4];
				for (int i = 0; i < 4; ++i) {
					lc[i].x = mLightColors[i].x * mLightIntensities[i];
					lc[i].y = mLightColors[i].y * mLightIntensities[i];
					lc[i].z = mLightColors[i].z * mLightIntensities[i];
					lc[i].w = 0.f;
				}
				//디퓨즈컬러는 언제든지 보내겠다
				HRESULT hr;
				hr = mDevice->SetVertexShaderConstantF(10, &mDiffuseColor.x, 1);
				STRONG_ASSERT(SUCCEEDED(hr) && "SetVertexShaderConstantF : INVALID CALL");
				if (mLightingMode == LIGHTING_PER_PIXEL) {
					hr = mDevice->SetPixelShaderConstantF(0, &mAmbientColor.x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetPixelShaderConstantF : INVALID CALL");
					hr = mDevice->SetPixelShaderConstantF(1, &mLightPositionXs.x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetPixelShaderConstantF : INVALID CALL");
					hr = mDevice->SetPixelShaderConstantF(2, &mLightPositionYs.x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetPixelShaderConstantF : INVALID CALL");
					hr = mDevice->SetPixelShaderConstantF(3, &mLightPositionZs.x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetPixelShaderConstantF : INVALID CALL");
					hr = mDevice->SetPixelShaderConstantF(4, &mEyePosition.x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetPixelShaderConstantF : INVALID CALL");
					//스페큘러색은 강도를 조정2+e/2pi
					Vector4 sc;
					const float rcpPi2 = 1.f / (3.1415926535897932384626433832795f * 2.f);
					float f = (2.f + mSpecularColor.w) * rcpPi2;
					sc.x = mSpecularColor.x * f;
					sc.y = mSpecularColor.y * f;
					sc.z = mSpecularColor.z * f;
					sc.w = mSpecularColor.w;

					hr = mDevice->SetPixelShaderConstantF(5, &sc.x, 1);
					//			hr = mDevice->SetPixelShaderConstantF( 5, &mSpecularColor.x, 1 );
					STRONG_ASSERT(SUCCEEDED(hr) && "SetPixelShaderConstantF : INVALID CALL");
					hr = mDevice->SetPixelShaderConstantF(6, &lc[0].x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetPixelShaderConstantF : INVALID CALL");
					hr = mDevice->SetPixelShaderConstantF(7, &lc[1].x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetPixelShaderConstantF : INVALID CALL");
					hr = mDevice->SetPixelShaderConstantF(8, &lc[2].x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetPixelShaderConstantF : INVALID CALL");
					hr = mDevice->SetPixelShaderConstantF(9, &lc[3].x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetPixelShaderConstantF : INVALID CALL");
					hr = mDevice->SetPixelShaderConstantF(10, &mEmissionColor.x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetPixelShaderConstantF : INVALID CALL");
				}
				else if (mLightingMode == LIGHTING_PER_VERTEX) {
					hr = mDevice->SetVertexShaderConstantF(11, &mAmbientColor.x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetVertexShaderConstantF : INVALID CALL");
					hr = mDevice->SetVertexShaderConstantF(12, &mLightPositionXs.x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetVertexShaderConstantF : INVALID CALL");
					hr = mDevice->SetVertexShaderConstantF(13, &mLightPositionYs.x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetVertexShaderConstantF : INVALID CALL");
					hr = mDevice->SetVertexShaderConstantF(14, &mLightPositionZs.x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetVertexShaderConstantF : INVALID CALL");
					hr = mDevice->SetVertexShaderConstantF(15, &lc[0].x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetVertexShaderConstantF : INVALID CALL");
					hr = mDevice->SetVertexShaderConstantF(16, &lc[1].x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetVertexShaderConstantF : INVALID CALL");
					hr = mDevice->SetVertexShaderConstantF(17, &lc[2].x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetVertexShaderConstantF : INVALID CALL");
					hr = mDevice->SetVertexShaderConstantF(18, &lc[3].x, 1);
					STRONG_ASSERT(SUCCEEDED(hr) && "SetVertexShaderConstantF : INVALID CALL");
				}
				mLightChanged = false;
			}
			void setLightingMode(LightingMode lm) {
				if (!mPixelShaderReady && (lm == LIGHTING_PER_PIXEL)) {
					cout << "Graphics::enablePixelShader() : This hardware doesn't support pixel shader( > 2.0 ). do per vertex." << endl;
					lm = LIGHTING_PER_VERTEX;
				}
				if (lm != mLightingMode) {
					mLightChanged = true;
					mLightingMode = lm;
					setShader();
				}
			}
			IDirect3DDevice9* dxDevice() {
				return mDevice;
			}
			IDirect3D9* mDirect3d;
			IDirect3DDevice9* mDevice;
			D3DVIEWPORT9 mViewport;
			D3DPRESENT_PARAMETERS mPresentParameters;
			int mWidth;
			int mHeight;
			bool mFullScreen;
			bool mVSync;
			bool mPixelShaderReady;
			LightingMode mLightingMode;
			int mFullScreenWindowWidth;
			int mFullScreenWindowHeight;
			unsigned mFrameId;
			bool mCanRender;
			HWND mWindowHandle;

			IDirect3DVertexDeclaration9* mVertexDeclaration;
			IDirect3DVertexShader9* mNoLightingVertexShader;
			IDirect3DVertexShader9* mVertexLightingVertexShader;
			IDirect3DVertexShader9* mPixelLightingVertexShader;
			IDirect3DPixelShader9* mPixelLightingPixelShader;

			VertexBuffer::Impl* mFullScreenQuadVertexBuffer;
			Texture::Impl* mWhiteTexture;

			//상태 변수군
			Texture::Impl* mCurrentTexture;
			VertexBuffer::Impl* mCurrentVertexBuffer;
			IndexBuffer::Impl* mCurrentIndexBuffer;
			BOOL mCurrentDepthTest;
			BOOL mCurrentDepthWrite;
			D3DCULL mCurrentCullMode;
			BOOL mCurrentAlphaTest;
			BOOL mCurrentAlphaBlend;
			D3DBLEND mCurrentDestBlend;
			D3DTEXTUREFILTERTYPE mCurrentMinFilter;
			D3DTEXTUREFILTERTYPE mCurrentMagFilter;
			D3DTEXTUREFILTERTYPE mCurrentMipFilter;
			D3DTEXTUREFILTERTYPE mBestMinFilter;
			D3DTEXTUREFILTERTYPE mBestMagFilter;
			D3DTEXTUREFILTERTYPE mBestMipFilter;
			DWORD mMaxAnisotropy;
			DWORD mMsaaQualityFullScreen;
			DWORD mMsaaQualityWindowed;

			Vector4 mLightPositionXs;
			Vector4 mLightPositionYs;
			Vector4 mLightPositionZs;
			Vector3 mLightColors[4];
			float mLightIntensities[4];
			Vector4 mEyePosition;
			Vector4 mDiffuseColor;
			Vector4 mSpecularColor;
			Vector4 mEmissionColor;
			Vector4 mAmbientColor;
			bool mLightChanged;

			Matrix44 mProjectionViewMatrix;
			Matrix34 mWorldMatrix;
			bool mMatricesChanged;
			bool mCaptureRequest;
			string mCaptureFilename;
		};
		extern ManagerImpl* gManagerImpl; //유일한 인스턴스
	}
}

#endif