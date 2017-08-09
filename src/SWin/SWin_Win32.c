#include <swin/SWin.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_FLAGS_ARB             0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

inline LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int interval);
typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef HGLRC(WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef HGLRC(WINAPI * PFNWGLCREATECONTEXT)(HDC hdc);
typedef BOOL(WINAPI * PFNWGLMAKECURRENT)(HDC hdc, HGLRC hglrc);
typedef PROC(WINAPI * PFNWGLGETPROCADDRESS)(LPCSTR lpszProc);

PFNWGLCREATECONTEXT swin_win32_createContext;
PFNWGLMAKECURRENT swin_win32_makecurrent;
PFNWGLGETPROCADDRESS swin_win32_getProcAddress;

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

void swInit() {
#ifndef _DEBUG
	FreeConsole();
#endif

	uint64_t frequency;

	if (QueryPerformanceFrequency((LARGE_INTEGER*)&frequency)) {
		__sWin_Win32_Time.hasPC = 1;
		__sWin_Win32_Time.frequency = frequency;
	} else {
		__sWin_Win32_Time.hasPC = 0;
		__sWin_Win32_Time.frequency = 1000;
	}

	viewID = 1;

	font = CreateFont(0, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE, TEXT("Segoe UI"));

	libGL = LoadLibraryW(L"opengl32.dll");

	swin_win32_createContext = GetProcAddress(libGL, "wglCreateContext");
	swin_win32_makecurrent = GetProcAddress(libGL, "wglMakeCurrent");
	swin_win32_getProcAddress = GetProcAddress(libGL, "wglGetProcAddress");
}

SWindow* swCreateWindow(int width, int height, const char* title) {
	SWin_Win32_Window* window = malloc(sizeof(SWin_Win32_Window));

	int n;
	WNDCLASSEX wc;
	LOGPALETTE* lpPal;

	window->instance = GetModuleHandle(NULL);

	RECT contentSize;
	contentSize.bottom = height;
	contentSize.top = 0;
	contentSize.left = 0;
	contentSize.right = width;

	AdjustWindowRectEx(&contentSize, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE, WS_EX_LAYERED);

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

	window->hWnd = CreateWindowEx(WS_EX_LAYERED, title, title, WS_SYSMENU, 100, 100, contentSize.right - contentSize.left, contentSize.bottom - contentSize.top, HWND_DESKTOP, NULL, window->instance, NULL);
	if (window->hWnd == NULL) {
		MessageBox(NULL, "CreateWindowEx() failed", "Error", MB_OK);
		PostQuitMessage(0);
		printf("Problem!\n");
		return;
	}

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
	size_t len = sizeof(char) * (size_t)ceil(log10(viewID)) + 2;

	char* viewIDStr = malloc(len);
	memset(viewIDStr, 0, len);

	sprintf(viewIDStr + 1, "%d", viewID);

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
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
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

	HWND hWnd = CreateWindowEx(0, TEXT(viewIDStr),
		(LPCTSTR)NULL,
		WS_CHILD | WS_BORDER | WS_VISIBLE,
		bounds->x, viewHeight - bounds->y - bounds->height, bounds->width, bounds->height,
		parent,
		(HMENU)viewID,
		GetWindowLong(parent, GWL_HINSTANCE),
		NULL);

	viewID++;

	return hWnd;
}

SOpenGLView* swCreateOpenGLView(HWND parent, SRect* bounds, SOpenGLContextAttribs* attribs) {
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
	pfd.cDepthBits = 32;

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
	
	swin_win32_makecurrent(view->hDc, view->hRc);

	PFNWGLCREATECONTEXTATTRIBSARBPROC pfnCreateContextAttribsARB = swGetProcAddress("wglCreateContextAttribsARB");
	PFNWGLSWAPINTERVALEXTPROC pfnSwapIntervalEXT = swGetProcAddress("wglSwapIntervalEXT");

	swin_win32_makecurrent(view->hDc, NULL);

	int iContextAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, attribs->major,
		WGL_CONTEXT_MINOR_VERSION_ARB, attribs->minor,
		WGL_CONTEXT_PROFILE_MASK_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, attribs->debug ? 1 : 0,
		0
	};

	view->hRc = pfnCreateContextAttribsARB(view->hDc, 0, iContextAttribs);

	swin_win32_makecurrent(view->hDc, view->hRc);

	pfnSwapIntervalEXT(attribs->swapInterval);

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
	void* result = swin_win32_getProcAddress(name);

	return result == NULL ? GetProcAddress(libGL, name) : result;
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

	SWin_Win32_Button* buttonInfo = malloc(sizeof(SWin_Win32_Button));
	buttonInfo->callback = callback;
	buttonInfo->userPointer = userData;
	buttonInfo->hWnd = button;

	SetWindowLongPtr(button, GWLP_USERDATA, (LONG_PTR)buttonInfo);

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

STextField* swCreateTextField(SView* parent, SRect* bounds, const char* text) {
	RECT viewBounds;
	GetClientRect(parent, &viewBounds);

	int viewHeight = abs(viewBounds.top - viewBounds.bottom);

	HWND textField = CreateWindow(
		TEXT("EDIT"), TEXT(text),
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
		bounds->x, viewHeight - bounds->y - bounds->height, bounds->width, bounds->height,
		parent,
		(HMENU)viewID,
		GetWindowLong(parent, GWL_HINSTANCE), NULL);

	SendMessage(textField, WM_SETFONT, font, TRUE);

	viewID++;

	return textField;
}

char* swGetTextFromTextField(STextField* textField) {
	HWND hwnd = (HWND)textField;

	int length = SendMessage(hwnd, WM_GETTEXTLENGTH, 0, 0);

	if (length == -1) {
		return "";
	}

	char* buffer = malloc(sizeof(char) * (length + 1));
	SendMessage(hwnd, WM_GETTEXT, length + 1, (LPARAM)buffer);

	return buffer;
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
		if (HIWORD(wParam) == BN_CLICKED) {
			SWin_Win32_Button* buttonInfo = GetWindowLongPtr((HWND)lParam, GWLP_USERDATA);
			buttonInfo->callback(buttonInfo->userPointer);
		}
		else if (HIWORD(wParam) == EN_CHANGE) {
			
		}
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}