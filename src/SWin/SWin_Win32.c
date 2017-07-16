#include <swin/SWin.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

inline LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

typedef struct SWin_Win32_Time {
	uint8_t hasPC;
	uint64_t frequency;
} SWin_Win32_Time;

SWin_Win32_Time __sWin_Win32_Time;
uint32_t viewID;

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
}

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

	ZeroMemory(&window->msg, sizeof(MSG));
	window->close = 0;

	SetWindowLongPtr(window->hWnd, GWLP_USERDATA, (LONG_PTR)window);

	SetLayeredWindowAttributes(window->hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	ShowWindow(window->hWnd, SW_SHOW);
	UpdateWindow(window->hWnd);
	SetForegroundWindow(window->hWnd);
	SetFocus(window->hWnd);

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

	view->hWnd = CreateWindowEx(0, TEXT(viewIDStr),
		(LPCTSTR)NULL,
		WS_CHILD | WS_BORDER | WS_VISIBLE,
		bounds->x, bounds->y, bounds->width, bounds->height,
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

	view->hRc = wglCreateContext(view->hDc);

	SetWindowLongPtr(view->hWnd, GWLP_USERDATA, (LONG_PTR)view);

	return view->hWnd;
}

void swMakeContextCurrent(HWND view) {
	SWin_Win32_OpenGLView* glView = GetWindowLongPtr(view, GWLP_USERDATA);
	wglMakeCurrent(glView->hDc, glView->hRc);
}

void swSwapBufers(HWND view) {
	SWin_Win32_OpenGLView* glView = GetWindowLongPtr(view, GWLP_USERDATA);
	SwapBuffers(glView->hDc);
}

SButton* swCreateButton(SView* parent, SRect* bounds, const char* title, void* callback, void* userData) {
	HWND button = CreateWindow(
		TEXT("BUTTON"), TEXT(title),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER,
		bounds->x, bounds->y, bounds->width, bounds->height,
		parent,
		(HMENU)viewID, 
		GetWindowLong(parent, GWL_HINSTANCE), NULL);

	viewID++;

	int test = 777;

	SetWindowLongPtr(button, GWLP_USERDATA, (LONG_PTR)&test);

	return button;
}

SLabel* swCreateLabel(SView* parent, SRect* bounds, const char* text) {
	return NULL;
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
		int* test = GetWindowLongPtr(hWnd, GWLP_USERDATA);
		printf("Command %d\n", *test);
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}