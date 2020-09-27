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
//#include "GameLib/Base/Array.h"

//���� ���̺귯����
#include "GameLib/Input/Keyboard.h"

#include "Framework.h"
#include "FontTextureGenerated.h"

#include <string>

using namespace std;

namespace GameLib
{
	namespace //����
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
				mLoadMode(FileIO::Manager::MODE_DIRECT_FIRST),
				mPreviousFrameInterval(0),
				mFrameRate(0),
				mEndRequested(false),
				mStarted(false) 
			{
			//Count �ʱ�ȭ
				unsigned t = time();
				//������ �̷� ����
				for (int i = 0; i < TIME_HISTORY_SIZE; ++i) {
					mTimeHistory[i] = t;
				}

				mVideoMemoryWithPadding.setSize(mWidth * (mHeight + 2));
				//0�ʱ�ȭ
				for (int i = 0; i < mWidth * (mHeight + 2); ++i) {
					mVideoMemoryWithPadding[i] = 0;
				}
				for (int i = 0; i < mWidth; ++i) {
					mVideoMemoryWithPadding[i] = MAGIC_NUMBER;
					mVideoMemoryWithPadding[mWidth * (mHeight + 1) + i] = MAGIC_NUMBER;
				}
			}
			~Impl() {
				if (mArchiveNames) {
					SAFE_DELETE_ARRAY(mArchiveNames);
				}
				mDebugStringRenderer.release();
				mDebugFont.release();
				Graphics::Manager::destroy();
				Input::Manager::destroy();
				Sound::Manager::destroy();
				FileIO::Manager::destroy();
				Threading::Manager::destroy();
				//cout����
				cout.end();
			}
			void start(void* windowHandle) {
				//������ �ý��� �ʱ�ȭ
				Threading::Manager::create(0);
				//���� �δ� �ʱ�ȭ
				if (mArchiveNumber > 0) {
					Array< const char* > names(mArchiveNumber);
					for (int i = 0; i < mArchiveNumber; ++i) {
						names[i] = mArchiveNames[i].c_str();
					}
					FileIO::Manager::create(&names[0], mArchiveNumber, mLoadMode);
					//��ī�̺� �̸� �迭�� ���ʿ��ϹǷ� ���
					SAFE_DELETE_ARRAY(mArchiveNames);
					mArchiveNumber = 0;
				}
				else {
					FileIO::Manager::create();
				}
				//�� �ʱ�ȭ
				Sound::Manager::create(windowHandle);
				//�Է� �ʱ�ȭ
				Input::Manager::create(windowHandle);
				//�׸��� �ʱ�ȭ
				Graphics::Manager::create(windowHandle, mWidth * 2, mHeight * 2, mFullScreen, mVSync, mAntiAlias);
				// ���� �׸���
				// �ƽ�Ű �κ��� ǥ(�ƽ�Ű(126-32+1=95��+63�ڷ� 158��)
				Array< Scene::Font::BitmapChar > chars(158);
				// �ƽ�Ű �κ� ����
				int pos = 0;
				for (int i = 0x20; i <= 0x7E; ++i) {
					Scene::Font::BitmapChar& c = chars[pos];
					c.mCode = static_cast<unsigned short>(i);
					c.mX = static_cast<unsigned short>((i % 16) * 8);
					c.mY = static_cast<unsigned short>((i / 16) * 16);
					c.mTexturePage = 0;
					c.mWidth = 8;
					c.mHeight = 16;
					++pos;
				}
				//�ݰ� ������
				for (int i = 0xA1; i <= 0xDF; ++i) {
					Scene::Font::BitmapChar& c = chars[pos];
					c.mCode = static_cast<unsigned short>(i);
					c.mX = static_cast<unsigned short>((i % 16) * 8);
					c.mY = static_cast<unsigned short>((i / 16) * 16);
					c.mTexturePage = 0;
					c.mWidth = 8;
					c.mHeight = 16;
					++pos;
				}

				ASSERT(pos == 158);
				int textureSize = sizeof(gFontTga);
				const char* fontDataArray[1];
				fontDataArray[0] = gFontTga;
				mDebugFont = Scene::Font::create(fontDataArray, &textureSize, 1, &(chars[0]), 158);
				mDebugStringRenderer = Scene::StringRenderer::create(2048, 128); //�̷� �����ε� ����ϰ���?
				mDebugStringRenderer.setFont(mDebugFont);
				//2D���̾�
				m2dTexture = Graphics::Texture::create(mWidth, mHeight, false);
				Graphics::Manager().setTextureFilter(Graphics::TEXTURE_FILTER_POINT);

				mStarted = true;
			}
			unsigned time() const {
				return WindowCreator().time();
			}
			void preUpdate() {
				//������ �ð� ����
				unsigned currentTime = time();
				mPreviousFrameInterval = currentTime - mTimeHistory[TIME_HISTORY_SIZE - 1];
				unsigned frameIntervalSum = currentTime - mTimeHistory[0];
				mFrameRate = TIME_HISTORY_SIZE * 1000 / frameIntervalSum;
				//�̷� ����
				for (int i = 0; i < TIME_HISTORY_SIZE - 1; ++i) {
					mTimeHistory[i] = mTimeHistory[i + 1];
				}
				mTimeHistory[TIME_HISTORY_SIZE - 1] = currentTime;

				Graphics::Manager().beginDraw();
				float pointerScale;
				Vector2 pointerOffset;
				Graphics::Manager().getPointerModifier(&pointerScale, &pointerOffset);
				Input::Manager().update(pointerScale, pointerOffset);
			}
			void postUpdate() {
				//--2D ó��
				// �ı�üũ
				for (int i = 0; i < mWidth; ++i) {
					ASSERT(mVideoMemoryWithPadding[i] == MAGIC_NUMBER && "VRAM RANGE ERROR!");
					ASSERT(mVideoMemoryWithPadding[mWidth * (mHeight + 1) + i] == MAGIC_NUMBER && "VRAM RANGE ERROR!");
				}
				//VRAM����
				unsigned* dst;
				int pitch;
				m2dTexture.lock(&dst, &pitch);
				const unsigned* src = &mVideoMemoryWithPadding[mWidth];
				for (int y = 0; y < mHeight; ++y) {
					for (int x = 0; x < mWidth; ++x) {
						dst[x] = 0xff000000 | src[x]; //���� ���� ff
					}
					src += mWidth;
					dst += pitch / 4;
				}
				src = 0; //��볡
				m2dTexture.unlock(&dst);
				Graphics::Manager().blendToScreen(m2dTexture);

				//�����ռ�
				mDebugStringRenderer.draw();
				Graphics::Manager().endDraw();
			}
			//���� ���̺귯�� ��
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
			FileIO::Manager::AccessMode mLoadMode;
			static const int TIME_HISTORY_SIZE = 60;
			unsigned mTimeHistory[TIME_HISTORY_SIZE];
			int mPreviousFrameInterval;
			int mFrameRate;
			bool mEndRequested;
			bool mStarted;
			Scene::StringRenderer mDebugStringRenderer;
			Scene::Font mDebugFont;
		};
		Impl* gImpl = 0;
	}
	Framework::Framework()
	{
		ASSERT(WindowCreator::isMainThread() && "you must call from MAIN thread");
	}

	void Framework::create() {
		ASSERT(!gImpl);
		gImpl = NEW Impl();
	}
	
	void Framework::destroy() {
		SAFE_DELETE(gImpl);
	}

	Framework Framework::instance()
	{
		return Framework();
	}

	void Framework::start(void* windowHandle) {
		gImpl->start(windowHandle);
	}

	void Framework::preUpdate() {
		gImpl->preUpdate();
	}

	void Framework::postUpdate() {
		gImpl->postUpdate();
	}

	int Framework::width() const {
		return gImpl->mWidth;
	}

	int Framework::height() const {
		return gImpl->mHeight;
	}

	void Framework::requestEnd() {
		gImpl->mEndRequested = true;
	}

	bool Framework::isEndRequested() const {
		return gImpl->mEndRequested;
	}

	// ���� ���̺귯���� �Լ�
	unsigned* Framework::videoMemory()
	{
		return &gImpl->mVideoMemoryWithPadding[gImpl->mWidth];
	}

	//Window Procedure ����� ���� �Լ�
	void WindowCreator::configure(Configuration* config) {
		Framework::create();
		//����� ����
		Framework f;
		//	Framework::Configuration fwConfig;
		//	f.configure( &fwConfig );
		config->setWidth(f.width() * 2);
		config->setHeight(f.height() * 2);
		config->setTitle("2D Graphics1 samples");
		config->enableFullScreen(false);
		config->forbidFullScreen(true);
	}

	void WindowCreator::update() {
		WindowCreator wc;
		Graphics::Manager gm;
		Framework f;
		//Window Creator ���¿� ���� ���� ������ �ϴ�
		bool sleepFlag = false;
		//�ּ�ȭ �Ǿ� ������ �ܴ�
		if (wc.isMinimized()) {
			sleepFlag = true;
		}
		//����̽� �ν�Ʈ�� �ִ� �� ������ ���͸� �õ�������, �⺻������ �ܴ�.
	/*
		if ( !gm.canRender() ){
			gm.restore();
			sleepFlag = true;
		}
		//Window���� ��� ��ȯ ��ȣ
		bool wcFullScreen = wc.isFullScreen();
		if ( f.isFullScreen() != wcFullScreen ){
			f.enableFullScreen( wcFullScreen );
		}
	*/
		if (!sleepFlag) {
			f.preUpdate();
		}
		//���(����ư ��)���� �������� �Դ��� üũ
		if (isEndRequested()) { //�� ������ Framework�� ���� ����� ����
			f.requestEnd();
		}
		if (sleepFlag) {
			Threading::sleep(50); //50ms �ڴ�
		}
		else {
			f.update();
			f.postUpdate();
		}
		//��������
		if (f.isEndRequested()) {
			Framework::destroy();
			end(); //������Ե� �ҽ���
		}
	}

	void WindowCreator::start(void* windowHandle) {
		Framework().start(windowHandle);
	}

	//StandardInput
	StandardInput cin;

	StandardInput::StandardInput() : mPrevEnterOn(false) {
	}
	
	//���⼭�� window ���� �θ���
#include <windows.h>
	StandardInput& StandardInput::operator>>(char& a) {
		a = '\0'; //�ʱⰪ
		bool entered = false;
		while (true) { //���� �����
			//�����̽�
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				a = ' ';
				entered = true;
			}
			//0-9
			for (char i = '0'; i <= '9'; ++i) {
				if (GetAsyncKeyState(i) & 0x8000) {
					a = i;
					entered = true;
				}
			}
			//a-z
			for (char i = 'A'; i <= 'Z'; ++i) {
				if (GetAsyncKeyState(i) & 0x8000) {
					if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
						a = i;
					}
					else {
						a = 'a' + (i - 'A');
					}
					entered = true;
				}
			}
			bool enterOn = ((GetAsyncKeyState(VK_RETURN) & 0x8000) != 0);
			if (entered && enterOn && !mPrevEnterOn) {
				mPrevEnterOn = enterOn;
				break;
			}
			//â���� ���� ��û�� �ް� �ִٸ� ������
			bool alt = ((GetAsyncKeyState(VK_MENU) & 0x8000) != 0);
			bool f4 = ((GetAsyncKeyState(VK_F4) & 0x8000) != 0);
			if (alt && f4) {
				Framework().requestEnd();
				break;
			}
			mPrevEnterOn = enterOn;
		}
		return *this;
	}



}