#include <swin/SWin.h>
#include <windows.h>

inline LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

typedef struct SWin_Win32_Time {
	uint8_t hasPC;
	uint64_t frequency;
} SWin_Win32_Time;

SWin_Win32_Time __sWin_Win32_Time;

void swInit() {
	uint64_t frequency;

	if (QueryPerformanceFrequency((LARGE_INTEGER*)&frequency)) {
		__sWin_Win32_Time.hasPC = 1;
		__sWin_Win32_Time.frequency = frequency;
	} else {
		__sWin_Win32_Time.hasPC = 0;
		__sWin_Win32_Time.frequency = 1000;
	}
}

typedef struct SWin_Win32_Window {
	HINSTANCE instance;
	HWND hWnd;
	HDC hDc;
	HGLRC hRc;
	HPALETTE hPalette;
	const char* title;
	MSG msg;
	int close;
} SWin_Win32_Window;

SWindow* swCreateWindow(int width, int height, const char* title) {
	SWin_Win32_Window* window = malloc(sizeof(SWin_Win32_Window));

	int n, pf;
	WNDCLASSEX wc;
	LOGPALETTE* lpPal;
	PIXELFORMATDESCRIPTOR pfd;

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

	//wc.lpszClassName = "GLClass";

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

	//ShowWindow(hwndButton, SW_SHOW);
	//UpdateWindow(hwndButton);
	//SetForegroundWindow(hwndButton);
	//SetFocus(hwndButton);

	SetLayeredWindowAttributes(window->hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	ShowWindow(window->hWnd, SW_SHOW);
	UpdateWindow(window->hWnd);
	SetForegroundWindow(window->hWnd);
	SetFocus(window->hWnd);

	WNDCLASSEX wc2;

	wc2.cbSize = sizeof(WNDCLASSEX);
	wc2.style = 0;
	wc2.lpfnWndProc = WndProc;
	wc2.cbClsExtra = 0;
	wc2.cbWndExtra = 0;
	wc2.hInstance = window->instance;
	wc2.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc2.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc2.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc2.lpszMenuName = NULL;
	wc2.lpszClassName = "GLCLass";
	wc2.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc2)) {
		MessageBox(NULL, "RegisterClassEx() failed", "Error", MB_OK);
		printf("Problem!\n");
		return;
	}

	HWND subView = CreateWindowEx(0, TEXT("GLCLass"),
		(LPCTSTR)NULL,
		WS_CHILD | WS_BORDER | WS_VISIBLE,
		200, 0, 600, 600,
		window->hWnd,
		(HMENU)2,
		window->instance,
		NULL);

	
	window->hDc = GetDC(subView);

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	pf = ChoosePixelFormat(window->hDc, &pfd);
	if (pf == 0) {
		MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return NULL;
	}

	if (SetPixelFormat(window->hDc, pf, &pfd) == FALSE) {
		MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
		return NULL;
	}

	window->hRc = wglCreateContext(window->hDc);
	wglMakeCurrent(window->hDc, window->hRc);
	
	CreateWindow(
		TEXT("BUTTON"), TEXT("OK"),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | WS_BORDER,
		0, 0, 200, 50,
		window->hWnd,
		(HMENU)1, GetModuleHandle(NULL), NULL);

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
	wglMakeCurrent(NULL, NULL);
	//ReleaseDC(window->hWnd, window->hDc);
	wglDeleteContext(window->hRc);
	
	window->instance = NULL;
}

SView* swGetRootView(SWin_Win32_Window* window) {
	return NULL;
}

SView* swCreateView(SRect* bounds) {
	return NULL;
}

void swSubView(SView* view, SView* sub) {
	
}

SOpenGLView* swCreateOpenGLView(SRect* bounds) {
	return NULL;
}

void swMakeContextCurrent(SOpenGLView* view) {
	
}

void swSwapBufers(SWin_Win32_Window* window) {
	SwapBuffers(window->hDc);
}

SButton* swCreateButton(SRect* bounds, const char* title, void* callback, void* userData) {
	return NULL;
}

SLabel* swCreateLabel(SRect* bounds, const char* text) {
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
	} else if (message == WM_CLOSE) {
		SWin_Win32_Window* window = GetWindowLongPtr(hWnd, GWLP_USERDATA);
		PostQuitMessage(0);
		DestroyWindow(hWnd);
		window->close = 1;
	} else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}