#include "GameLib/GameLib.h"
#include "GameLib/WindowCreator/WindowCreator.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/FileIO/Manager.h"
#include "GameLib/Sound/Manager.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Scene/StringRenderer.h"
#include "GameLib/Scene/Font.h"
#include "GameLib/Threading/Functions.h"
#include "GameLib/Threading/Manager.h"

#include "GameLib/Input/Keyboard.h"

#include "Framework.h"
#include "FontTextureGenerated.h"

#include <string>

using namespace std;

namespace GameLib
{
	namespace
	{
		class Impl
		{
		public:
			Impl() :
				mWidth(320),
				mHeight(240),
				mFullScreen(false),
				mFullScreenForbidden(true),
				mVSync(false),
				mAntiAlias(false),
				mTitle("A GameLib Framework Application"),
				mArchiveNames(0),
				mArchiveNumber(0),
				//mLoadMode(FileIO::Manager::MODE_DIRECT_FIRST),
				mPreviousFrameInterval(0),
				mFrameRate(0),
				mEndRequested(false),
				mStarted(false) 
			{
			//Count 초기화
				unsigned t = time();
				//프레임 이력 리셋
				for (int i = 0; i < TIME_HISTORY_SIZE; ++i) {
					mTimeHistory[i] = t;
				}

				mVideoMemoryWithPadding.setSize(mWidth * (mHeight + 2));
				//0초기화
				for (int i = 0; i < mWidth * (mHeight + 2); ++i) {
					mVideoMemoryWithPadding[i] = 0;
				}
				for (int i = 0; i < mWidth; ++i) {
					mVideoMemoryWithPadding[i] = MAGIC_NUMBER;
					mVideoMemoryWithPadding[mWidth * (mHeight + 1) + i] = MAGIC_NUMBER;
				}
			}
			unsigned time() const {
				return WindowCreator().time();
			}
			Array< unsigned > mVideoMemoryWithPadding;
			Graphics::Texture m2dTexture;
			static const unsigned MAGIC_NUMBER = 0x12345678;
			int mWidth;
			int mHeight;
			bool mFullScreen;
			bool mFullScreenForbidden;
			bool mVSync;
			bool mAntiAlias;
			string mTitle;
			string* mArchiveNames;
			int mArchiveNumber;
			//FileIO::Manager::AccessMode mLoadMode;
			static const int TIME_HISTORY_SIZE = 60;
			unsigned mTimeHistory[TIME_HISTORY_SIZE];
			int mPreviousFrameInterval;
			int mFrameRate;
			bool mEndRequested;
			bool mStarted;
			//Scene::StringRenderer mDebugStringRenderer;
			//Scene::Font mDebugFont;
		};
		Impl* gImpl = 0;
	}
	Framework::Framework()
	{
	}

	void Framework::update()
	{
	}

	Framework Framework::instance()
	{
		return Framework();
	}

	int Framework::width() const
	{
		return 0;
	}

	int Framework::height() const
	{
		return 0;
	}

	unsigned* Framework::videoMemory()
	{
		return nullptr;
	}

	void Framework::requestEnd()
	{
	}

	bool Framework::isEndRequested() const
	{
		return false;
	}

	void Framework::start(void* windowHandle)
	{
	}

	void Framework::preUpdate()
	{
	}

	void Framework::postUpdate()
	{
		gImpl->postUpdate();
	}

	void Framework::create()
	{
	}

	void Framework::destroy()
	{
	}
	
	StandardInput::StandardInput()
	{
	}

}