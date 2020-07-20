#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x400 //WM_MOUSEWHEEL������
#endif

#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>
#include "resource.h"
#include <string>
#undef min
#undef max

#include "GameLib/GameLib.h"
#include "GameLib/WindowCreator/WindowCreator.h"
#include "GameLib/Threading/Functions.h"

using namespace std;
using namespace GameLib;

//WindowCreator �����Լ�

namespace GameLib
{
	namespace //����
	{
		class Impl
		{
		public:
			Impl() :
				mWidth(640),
				mHeight(480),
				mFullScreen(false),
				mMinimized(false),
				mActive(false),
				mTitle("A GameLib Application"),
				mCommandLine(""),
				mEndRequested(false),
				mEnded(false),
				mStarted(false),
				mFullScreenForbidden(false),
				mDragAndDropEnabled(false),
				mMouseWheel(0),
				mTimeBias(0) {
				//get Time()�� ���̾ �� ���
				unsigned time = timeGetTime();
				mTimeBias = 0xffffffff - time;
				mTimeBias -= 60000; //60�ʸ� ����

				//������ID���
				mThreadId = GetCurrentThreadId();
			}
			~Impl() {
			}
			void enableFullScreen(bool f) {
				if (mFullScreen != f) {
					if (mStarted) {
						LONG style;
						if (f) {
							style = WS_POPUP | WS_VISIBLE;
							// �ִ�ȭ�� ������ ������ ��ġ�� ����ϰڽ��ϴ�.
							GetWindowRect(mWindowHandle, &mWindowRect);
							SetWindowLong(mWindowHandle, GWL_STYLE, style);
							//��ġ�� 0,0
							SetWindowPos(mWindowHandle, NULL,
								0, 0,
								mWidth, mHeight,
								SWP_SHOWWINDOW);
						}
						else {
							style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;
							SetWindowLong(mWindowHandle, GWL_STYLE, style);
							// ���� ��ġ �ǵ�����
							SetWindowPos(mWindowHandle, NULL,
								mWindowRect.left, mWindowRect.top,
								mWindowRect.right - mWindowRect.left,
								mWindowRect.bottom - mWindowRect.top,
								SWP_SHOWWINDOW);
						}
					}
					mFullScreen = f;
				}
			}
			void createDefaultWindowPos() {
				mWindowRect.left = 0;
				mWindowRect.top = 0;
				mWindowRect.right = mWidth;
				mWindowRect.bottom = mHeight;
				LONG style;
				style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;
				int oldLeft = mWindowRect.left;
				int oldTop = mWindowRect.top;
				AdjustWindowRect(&mWindowRect, style, FALSE);
				int diffX = mWindowRect.left - oldLeft;
				int diffY = mWindowRect.top - oldTop;
				mWindowRect.left -= diffX;
				mWindowRect.right -= diffX;
				mWindowRect.top -= diffY;
				mWindowRect.bottom -= diffY;
			}
			int mWidth;
			int mHeight;
			bool mFullScreen;
			bool mMinimized;
			bool mActive;
			string mTitle;
			string mCommandLine;
			bool mEndRequested;
			bool mEnded;
			bool mStarted;
			bool mFullScreenForbidden;
			bool mDragAndDropEnabled;
			int mMouseWheel;
			unsigned mTimeBias; //timeGetTime
			unsigned mThreadId;
			Array< string > mDroppedItems;
			HWND mWindowHandle;
			RECT mWindowRect;
		};
		Impl* gImpl = 0;//������ �ν��Ͻ�
	}
	WindowCreator::WindowCreator() {
		//�ٸ� ������κ��� ȣ���� ������� �ʴ´�
		ASSERT(isMainThread() && "you must call from MAIN thread");
	}
	const char* WindowCreator::title() const {
		return gImpl->mTitle.c_str();
	}

	const char* WindowCreator::commandLineString() const {
		return gImpl->mCommandLine.c_str();
	}

	int WindowCreator::titleLength() const {
		return static_cast<int>(gImpl->mTitle.size());
	}

	int WindowCreator::height() const {
		return gImpl->mHeight;
	}

	int WindowCreator::width() const {
		return gImpl->mWidth;
	}

	bool WindowCreator::isFullScreen() const {
		return gImpl->mFullScreen;
	}

	WindowCreator WindowCreator::instance() {
		return WindowCreator(); //// ���� �� �Լ� �ʿ����
	}

	bool WindowCreator::isEndRequested() const {
		return gImpl->mEndRequested;
	}

	void WindowCreator::requestEnd() {
		gImpl->mEndRequested = true;
	}

	void WindowCreator::end() {
		gImpl->mEnded = true;
	}

	unsigned WindowCreator::time() const {
		return timeGetTime() + gImpl->mTimeBias;
	}

	bool WindowCreator::isMinimized() const
	{
		return gImpl->mMinimized;
	}

	bool WindowCreator::isActive() const
	{
		return gImpl->mActive;
	}
	bool WindowCreator::isMainThread() {
		return (GetCurrentThreadId() == gImpl->mThreadId);
	}

	void WindowCreator::enableFullScreen(bool f) {
		gImpl->enableFullScreen(f);
	}

	bool WindowCreator::isDragAndDropEnabled() const {
		return gImpl->mDragAndDropEnabled;
	}

	int WindowCreator::droppedItemNumber() const {
		return gImpl->mDroppedItems.size();
	}

	const char* WindowCreator::droppedItem(int i) const {
		return gImpl->mDroppedItems[i].c_str();
	}

	void WindowCreator::clearDroppedItem() {
		gImpl->mDroppedItems.clear();
	}

	int WindowCreator::getAndResetMouseWheel() {
		int t = gImpl->mMouseWheel;
		gImpl->mMouseWheel = 0;
		return t;
	}

	//Configuration

	void WindowCreator::Configuration::setWidth(int w) {
		gImpl->mWidth = w;
	}

	void WindowCreator::Configuration::setHeight(int h) {
		gImpl->mHeight = h;
	}

	void WindowCreator::Configuration::setTitle(const char* title) {
		gImpl->mTitle = title;
	}

	void WindowCreator::Configuration::enableFullScreen(bool f) {
		gImpl->enableFullScreen(f);
	}

	void WindowCreator::Configuration::forbidFullScreen(bool f) {
		gImpl->mFullScreenForbidden = f;
	}

	void WindowCreator::Configuration::enableDragAndDrop(bool f) {
		gImpl->mDragAndDropEnabled = f;
	}

}
#define MAX_LOADSTRING 100

// // �۷ι� ����:
HINSTANCE hInst;								// ������ �������̽�
TCHAR szTitle[MAX_LOADSTRING];					// Ÿ��Ʋ ���� �ؽ�Ʈ
TCHAR szWindowClass[MAX_LOADSTRING];			// ���� ������ Ŭ���� �̸�

// �� �ڵ� ��⿡ ���ԵǴ� �Լ��� ������ �����մϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���⿡ �ڵ带 ������ �ּ���.
	MSG msg;
	HACCEL hAccelTable;

	//������ ���� ���� Ŭ���� ����
	gImpl = NEW Impl();
	gImpl->mCommandLine = lpCmdLine;
	WindowCreator wc;
	//����� ���� �Լ� ȣ��
	WindowCreator::Configuration config;
	wc.configure(&config);

	memcpy(
		szTitle,
		wc.title(),
		wc.titleLength());
	memcpy(szWindowClass, "GameLibWindowCreator", sizeof("GameLibWindowCreator"));

	MyRegisterClass(hInstance);

	// ���ø����̼��� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWCREATOR));

	timeBeginPeriod(1);
	HWND windowHandle = gImpl->mWindowHandle;
	///���� ���� ���ܴ� ��� �Ұ����ϹǷ� �޽��� ���ڸ� ���� ������
	try {
		wc.start(windowHandle);
	}
	catch (...) { //���ý� ���ܴ� ������ �� ����.
		MessageBoxA(
			windowHandle,
			//"ġ������ ������ �־�, �⵿�� �� �����ϴ�.�˼������� �����ϰڽ��ϴ�.",
			"test",
			//"ġ���� ����",
			"test",
			MB_OK | MB_ICONERROR);
		wc.requestEnd(); //���̴�
		wc.update(); //�ȿ��� ���� ó��
		SAFE_DELETE(gImpl);
		timeEndPeriod(1);
		return 0;
	}

	//Ǯ��ũ���̸� �ǵ����� â ��ġ ����
	if (gImpl->mFullScreen) {
		gImpl->createDefaultWindowPos();
	}
	//�巰�ص��Ȱ��ȭ
	if (gImpl->mDragAndDropEnabled) {
		DragAcceptFiles(windowHandle, TRUE);
	}
	gImpl->mStarted = true; //����� ���� ��ȣ
	while (true) {
		//�޼����� �ִٸ� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (GetMessage(&msg, NULL, 0, 0)) {
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else {
				break; //WM_QUIT �Ծ��.���� ������.
			}
		}
		//������ window �ı�
		if (gImpl->mEnded) { //������ ������ ���� ����
			if (windowHandle) { //Ȥ�� �ι� ������ �𸣴ϱ� Ȥ�� �𸣴ϱ�
				DestroyWindow(windowHandle);
				windowHandle = 0;
			}
		}
		else {	//���� ��ü
			try {
				wc.update();
			}
			catch (Exception e) {
				if (e == EXCEPTION_EXIT) { //�����û ������ ��쿡�� ����
					MessageBoxA(
						windowHandle,
						//"ġ������ ������ �־� ����� �� �����ϴ�.�˼������� �����ϰڽ��ϴ�.",
						"test",
						//"ġ���� ����",
						"test",
						MB_OK | MB_ICONERROR);
				}
			}
		}
	}
	SAFE_DELETE(gImpl);
	timeEndPeriod(1);

	CoUninitialize();
	return (int)msg.wParam;
}
