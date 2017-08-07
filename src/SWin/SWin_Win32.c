#include <swin/SWin.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

inline LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

typedef HGLRC(WINAPI * PFNWGLCREATECONTEXT)(HDC hdc);
typedef BOOL(WINAPI * PFNWGLMAKECURRENT)(HDC hdc, HGLRC hglrc);

PFNWGLCREATECONTEXT swin_win32_createContext;
PFNWGLMAKECURRENT swin_win32_makecurrent;

typedef struct SWin_Win32_Time {
	uint8_t hasPC;
	uint64_t frequency;
} SWin_Win32_Time;

typedef struct SWin_Win32_Window {
	HINSTANCE instance;
	HWND hWnd;
	const char* title;
	MSG msg;
	int close;
} SWin_Win32_Window;

typedef struct SWin_Win32_OpenGLView {
	HGLRC hRc;
	HDC hDc;
	HPALETTE hPalette;
	HWND hWnd;
} SWin_Win32_OpenGLView;

typedef void(*buttonCallback)(void*);

typedef struct SWin_Win32_Button {
	buttonCallback callback;
	void* userPointer;
	HWND hWnd;
} SWin_Win32_Button;

SWin_Win32_Time __sWin_Win32_Time;
uint32_t viewID;
HFONT font;
HMODULE libGL;

void* userInfos;

void swInit() {
	uint64_t frequency;

	if (QueryPerformanceFrequency((LARGE_INTEGER*)&frequency)) {
		__sWin_Win32_Time.hasPC = 1;
		__sWin_Win32_Time.frequency = frequency;
	} else {
		__sWin_Win32_Time.hasPC = 0;
		__sWin_Win32_Time.frequency = 1000;
	}

	viewID = 1;
	userInfos = malloc(sizeof(SWin_Win32_Button) * viewID);

	font = CreateFont(0, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE, TEXT("Segoe UI"));

	libGL = LoadLibraryW(L"opengl32.dll");

	swin_win32_createContext = GetProcAddress(libGL, "wglCreateContext");
	swin_win32_makecurrent = GetProcAddress(libGL, "wglMakeCurrent");
}

SWindow* swCreateWindow(int width, int height, const char* title) {
	SWin_Win32_Window* window = malloc(sizeof(SWin_Win32_Window));

	int n;
	WNDCLASSEX wc;
	LOGPALETTE* lpPal;

	window->instance = GetModuleHandle(NULL);

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = window->instance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = title;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "RegisterClassEx() failed", "Error", MB_OK);
		printf("Problem!\n");
		return;
	}

	int nStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;

	window->hWnd = CreateWindowEx(WS_EX_LAYERED, title, title, WS_SYSMENU, 100, 100, width, height, HWND_DESKTOP, NULL, window->instance, NULL);
	if (window->hWnd == NULL) {
		MessageBox(NULL, "CreateWindowEx() failed", "Error", MB_OK);
		PostQuitMessage(0);
		printf("Problem!\n");
		return;
	}

	RECT contentSize;
	contentSize.bottom = height;
	contentSize.top = 0;
	contentSize.left = 0;
	contentSize.right = width;

	AdjustWindowRectEx(&contentSize, WS_SYSMENU, FALSE, WS_EX_LAYERED);

	ZeroMemory(&window->msg, sizeof(MSG));
	window->close = 0;

	SetWindowLongPtr(window->hWnd, GWLP_USERDATA, (LONG_PTR)window);

	SetLayeredWindowAttributes(window->hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	ShowWindow(window->hWnd, SW_SHOW);
	UpdateWindow(window->hWnd);
	SetForegroundWindow(window->hWnd);
	SetFocus(window->hWnd);

	SendMessage(window->hWnd, WM_SETFONT, font, TRUE);

	return window;
}

void swPollEvents() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void swDraw(SWin_Win32_Window* window) {
	
}

uint8_t swCloseRequested(SWin_Win32_Window* window) {
	return window->close;
}

void swCloseWindow(SWin_Win32_Window* window) {
	PostQuitMessage(0);
	DestroyWindow(window->hWnd);

	window->instance = NULL;
}

SView* swGetRootView(SWin_Win32_Window* window) {
	return (SView*)window->hWnd;
}

SView* swCreateView(HWND parent, SRect* bounds) {
	return NULL;
}

SOpenGLView* swCreateOpenGLView(HWND parent, SRect* bounds) {
	SWin_Win32_OpenGLView* view = malloc(sizeof(SWin_Win32_OpenGLView));
	
	size_t len = sizeof(char) * (size_t)ceil(log10(viewID)) + 2;

	char* viewIDStr = malloc(len);
	memset(viewIDStr, 0, len);

	sprintf(viewIDStr+1, "%d", viewID);

	viewIDStr[0] = 'C';

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetWindowLong(parent, GWL_HINSTANCE);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = viewIDStr;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "RegisterClassEx() failed", "Error", MB_OK);
		printf("Problem!\n");
		return;
	}

	RECT viewBounds;
	GetClientRect(parent, &viewBounds);

	int viewHeight = abs(viewBounds.top - viewBounds.bottom);

	view->hWnd = CreateWindowEx(0, TEXT(viewIDStr),
		(LPCTSTR)NULL,
		WS_CHILD | WS_BORDER | WS_VISIBLE,
		bounds->x, viewHeight - bounds->y - bounds->height, bounds->width, bounds->height,
		parent,
		(HMENU)viewID,
		GetWindowLong(parent, GWL_HINSTANCE),
		NULL);

	viewID++;

	view->hDc = GetDC(view->hWnd);

	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	int pf = ChoosePixelFormat(view->hDc, &pfd);
	if (pf == 0) {
		MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return NULL;
	}

	if (SetPixelFormat(view->hDc, pf, &pfd) == FALSE) {
		MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
		return NULL;
	}

	view->hRc = swin_win32_createContext(view->hDc);

	SetWindowLongPtr(view->hWnd, GWLP_USERDATA, (LONG_PTR)view);

	return view->hWnd;
}

void swMakeContextCurrent(HWND view) {
	SWin_Win32_OpenGLView* glView = GetWindowLongPtr(view, GWLP_USERDATA);
	swin_win32_makecurrent(glView->hDc, glView->hRc);
}

void swSwapBufers(HWND view) {
	SWin_Win32_OpenGLView* glView = GetWindowLongPtr(view, GWLP_USERDATA);
	SwapBuffers(glView->hDc);
}

void* swGetProcAddress(const char* name) {
	return GetProcAddress(libGL, name);
}

SButton* swCreateButton(SView* parent, SRect* bounds, const char* title, buttonCallback callback, void* userData) {
	RECT viewBounds;
	GetClientRect(parent, &viewBounds);

	int viewHeight = abs(viewBounds.top - viewBounds.bottom);

	HWND button = CreateWindow(
		TEXT("BUTTON"), TEXT(title),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		bounds->x, viewHeight - bounds->y - bounds->height, bounds->width, bounds->height,
		parent,
		(HMENU)viewID, 
		GetWindowLong(parent, GWL_HINSTANCE), NULL);

	SendMessage(button, WM_SETFONT, font, TRUE);

	viewID++;
	buttonCallback* preInfos = userInfos;
	userInfos = malloc(sizeof(buttonCallback) * viewID);
	memcpy(userInfos, preInfos, sizeof(buttonCallback) * viewID - 1);

	SWin_Win32_Button* buttonInfo = malloc(sizeof(SWin_Win32_Button));
	buttonInfo->callback = callback;
	buttonInfo->userPointer = userData;
	buttonInfo->hWnd = button;

	((SWin_Win32_Button**)userInfos)[viewID - 2] = buttonInfo;

	free(preInfos);

	return button;
}

SLabel* swCreateLabel(HWND parent, SRect* bounds, const char* text) {
	RECT viewBounds;
	GetClientRect(parent, &viewBounds);

	int viewHeight = abs(viewBounds.top - viewBounds.bottom);

	HWND label = CreateWindow(
		TEXT("STATIC"), TEXT(text),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER,
		bounds->x, viewHeight - bounds->y - bounds->height, bounds->width, bounds->height,
		parent,
		(HMENU)viewID,
		GetWindowLong(parent, GWL_HINSTANCE), NULL);

	SendMessage(label, WM_SETFONT, font, TRUE);

	viewID++;

	return label;
}

double swGetTime() {
	uint64_t value;

	QueryPerformanceCounter((LARGE_INTEGER*)&value);

	double result = (double)value;

	result = result / __sWin_Win32_Time.frequency;

	return result;
}

void swTerminate() {
	
}

inline LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	if (message == WM_PAINT) {
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	else if (message == WM_CLOSE) {
		SWin_Win32_Window* window = GetWindowLongPtr(hWnd, GWLP_USERDATA);
		window->close = 1;
	}
	else if (message == WM_COMMAND) {
		SWin_Win32_Button* buttonInfo = ((SWin_Win32_Button**)userInfos)[wParam-1];
		buttonInfo->callback(buttonInfo->userPointer);
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}