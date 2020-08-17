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