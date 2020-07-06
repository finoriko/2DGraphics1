#include "GameLib/Input/Manager.h"


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
				mStarted(false) {
			
			}


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