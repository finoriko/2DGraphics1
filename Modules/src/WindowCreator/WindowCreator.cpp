#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x400 //WM_MOUSEWHEEL때문에
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

//WindowCreator 메인함수

namespace GameLib
{
	namespace //무명
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
				//get Time()용 바이어스 값 계산
				unsigned time = timeGetTime();
				mTimeBias = 0xffffffff - time;
				mTimeBias -= 60000; //60초를 빼다

				//스레드ID취득
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
							// 최대화할 때에는 지금의 위치를 기억하겠습니다.
							GetWindowRect(mWindowHandle, &mWindowRect);
							SetWindowLong(mWindowHandle, GWL_STYLE, style);
							//위치는 0,0
							SetWindowPos(mWindowHandle, NULL,
								0, 0,
								mWidth, mHeight,
								SWP_SHOWWINDOW);
						}
						else {
							style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;
							SetWindowLong(mWindowHandle, GWL_STYLE, style);
							// 원래 위치 되돌리기
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
		Impl* gImpl = 0;//유일한 인스턴스
	}
	WindowCreator::WindowCreator() {
		//다른 스레드로부터 호출은 허용하지 않는다
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
		return WindowCreator(); //// 실은 이 함수 필요없어
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

// // 글로벌 변수:
HINSTANCE hInst;								// 현재의 인터페이스
TCHAR szTitle[MAX_LOADSTRING];					// 타이틀 바의 텍스트
TCHAR szWindowClass[MAX_LOADSTRING];			// 메인 윈도우 클래스 이름

// 이 코드 모듈에 포함되는 함수의 선언을 전송합니다.
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

	// TODO: 여기에 코드를 삽입해 주세요.
	MSG msg;
	HACCEL hAccelTable;

	//윈도우 상태 유지 클래스 생성
	
	gImpl = NEW Impl();
	gImpl->mCommandLine = lpCmdLine;
	WindowCreator wc;
	//사용자 설정 함수 호출
	WindowCreator::Configuration config;
	wc.configure(&config);

	memcpy(
		szTitle,
		wc.title(),
		wc.titleLength());
	memcpy(szWindowClass, "GameLibWindowCreator", sizeof("GameLibWindowCreator"));

	MyRegisterClass(hInstance);

	// 어플리케이션의 초기화를 실행합니다.
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWCREATOR));

	timeBeginPeriod(1);
	HWND windowHandle = gImpl->mWindowHandle;
	///부팅 전의 예외는 계속 불가능하므로 메시지 상자를 내고 끝내다
	try {
		wc.start(windowHandle);
	}
	catch (...) { //부팅시 예외는 무시할 수 없다.
		MessageBoxA(
			windowHandle,
			//"치명적인 문제가 있어, 기동할 수 없습니다.죄송하지만 종료하겠습니다.",
			"test",
			//"치명적 오류",
			"test",
			MB_OK | MB_ICONERROR);
		wc.requestEnd(); //끝이다
		wc.update(); //안에서 종료 처리
		SAFE_DELETE(gImpl);
		timeEndPeriod(1);
		return 0;
	}

	//풀스크린이면 되돌리는 창 위치 생성
	if (gImpl->mFullScreen) {
		gImpl->createDefaultWindowPos();
	}
	//드럭앤드롭활성화
	if (gImpl->mDragAndDropEnabled) {
		DragAcceptFiles(windowHandle, TRUE);
	}
	gImpl->mStarted = true; //루우프 개시 신호
	while (true) {
		//메세지가 있다면 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (GetMessage(&msg, NULL, 0, 0)) {
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else {
				break; //WM_QUIT 왔어요.루프 빠져요.
			}
		}
		//끝나면 window 파기
		if (gImpl->mEnded) { //끝나지 않으면 갱신 루프
			if (windowHandle) { //혹시 두번 올지도 모르니까 혹시 모르니까
				DestroyWindow(windowHandle);
				windowHandle = 0;
			}
		}
		else {	//실행 본체
			try {
				wc.update();
			}
			catch (Exception e) {
				if (e == EXCEPTION_EXIT) { //종료요청 예외인 경우에만 종료
					MessageBoxA(
						windowHandle,
						//"치명적인 문제가 있어 계속할 수 없습니다.죄송하지만 종료하겠습니다.",
						"test",
						//"치명적 오류",
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

//
//  함수: MyRegisterClass()
//
//  목적: 윈도우 클래스를 등록합니다
//
//  코멘트:
//
//  이 함수 및 사용법은 'Register Class Ex' 함수가 추가된
//   Windows 95 이전 Win32 시스템과 호환시킬 경우에만 필요합니다.
//   어플리케이션이 연관된
//   올바른 형식의 작은 아이콘을 얻을 수 있도록 하려면
//    이 함수를 호출해 주세요.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWCREATOR));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_WINDOWCREATOR);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수 InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 메인 창을 만듭니다.
//
//   코멘트:
//
//       이 함수로 글로벌 변수로 인스턴스 핸들을 저장하고
//       메인 프로그램 창을 작성 및 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance;// 글로벌 변수에 인스턴스 처리를 격납합니다.
//----
	DWORD style = 0;
	if (gImpl->mFullScreen) {
		style = WS_POPUP;
	}
	else {
		style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	}
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = gImpl->mWidth;
	rect.bottom = gImpl->mHeight;
	AdjustWindowRect(&rect, style, FALSE);
	rect.right -= rect.left;
	rect.bottom -= rect.top; //차분계산
	rect.left = 0;
	rect.top = 0;
	//----

	hWnd = CreateWindow(szWindowClass, szTitle,
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right, rect.bottom, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	gImpl->mWindowHandle = hWnd;

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  메인 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 애플리케이션 메뉴 처리
//  WM_PAINT	- 메인 창의 그림
//  WM_DESTROY	- 중지 메시지를 표시하고 돌아오다
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		break;
	case WM_CLOSE: //종료 요청
		gImpl->mEndRequested = true;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SYSKEYDOWN: //alt+enter 인식
		if (!gImpl->mFullScreenForbidden) {
			if (wParam == VK_RETURN) {
				if (gImpl->mFullScreen) {
					WindowCreator().enableFullScreen(false);
				}
				else {
					WindowCreator().enableFullScreen(true);
				}
			}
		}
		DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_ACTIVATE:
		if (wParam & 0xffff0000) { //최소화 되어 있으면 false
			gImpl->mActive = false;
		}
		else {
			gImpl->mActive = ((wParam & 0xffff) != 0);
		}
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_MAXIMIZE) {
			if (!gImpl->mFullScreenForbidden) {
				WindowCreator().enableFullScreen(true); // 최대화 시 DefWindow Proc 부르지 않음
			}
		}
		else if (wParam == SC_MINIMIZE) {
			gImpl->mMinimized = true;
			gImpl->mActive = false;
			DefWindowProc(hWnd, message, wParam, lParam);
		}
		else if (wParam == SC_RESTORE) {
			gImpl->mMinimized = false;
			gImpl->mActive = true;
			DefWindowProc(hWnd, message, wParam, lParam);
		}
		else {
			DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_MOUSEWHEEL:
		gImpl->mMouseWheel = (static_cast<short>(HIWORD(wParam)) > 0) ? 1 : -1;
		break;
	case WM_DROPFILES:
		if (gImpl->mDragAndDropEnabled) {
			HDROP hDrop = (HDROP)wParam;
			gImpl->mDroppedItems.clear();
			unsigned n = DragQueryFileA(hDrop, 0xffffffff, NULL, 0);
			gImpl->mDroppedItems.setSize(n);
			char item[MAX_PATH];
			for (unsigned i = 0; i < n; ++i) {
				DragQueryFileA(hDrop, i, item, MAX_PATH);
				gImpl->mDroppedItems[i] = item;
			}
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
