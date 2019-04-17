#include <swin/SWin.h>
#include <swin/SWin_Win32.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

uint32_t __sWin_Win32_viewID;

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
	wc.lpfnWndProc = SWin_Win32_Thread_WndProc;
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

	SWin_Win32_RootPointer* root = (SWin_Win32_RootPointer*)malloc(sizeof(SWin_Win32_RootPointer));
	root->window = window;
	SetWindowLongPtr(window->hWnd, GWLP_USERDATA, (LONG_PTR)root);

	SetLayeredWindowAttributes(window->hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	ShowWindow(window->hWnd, SW_SHOW);
	UpdateWindow(window->hWnd);
	SetForegroundWindow(window->hWnd);
	SetFocus(window->hWnd);

	SendMessage(window->hWnd, WM_SETFONT, __sWin_Win32_font, TRUE);

	window->mouseState = malloc(sizeof(SMouseState));
	window->mouseState->x = 0;
	window->mouseState->y = 0;
	window->mouseState->ldown = 0;

	window->root = window;

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
	size_t len = sizeof(char) * (size_t)ceil(log10(__sWin_Win32_viewID)) + 2;

	char* viewIDStr = malloc(len);
	memset(viewIDStr, 0, len);

	sprintf(viewIDStr + 1, "%d", __sWin_Win32_viewID);

	viewIDStr[0] = 'C';

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = SWin_Win32_Thread_WndProc;
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

	HWND hWnd = CreateWindowEx(0, TEXT(viewIDStr),
		(LPCTSTR)NULL,
		WS_CHILD | WS_BORDER | WS_VISIBLE,
		bounds->x, viewHeight - bounds->y - bounds->height, bounds->width, bounds->height,
		parent,
		(HMENU)__sWin_Win32_viewID,
		GetWindowLong(parent, GWL_HINSTANCE),
		NULL);

	SetWindowLongPtr(hWnd, GWLP_USERDATA, GetWindowLongPtr(parent, GWLP_USERDATA));

	__sWin_Win32_viewID++;

	return hWnd;
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
		(HMENU)__sWin_Win32_viewID,
		GetWindowLong(parent, GWL_HINSTANCE), NULL);

	SendMessage(button, WM_SETFONT, __sWin_Win32_font, TRUE);

	__sWin_Win32_viewID++;

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
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		bounds->x, viewHeight - bounds->y - bounds->height, bounds->width, bounds->height,
		parent,
		(HMENU)__sWin_Win32_viewID,
		GetWindowLong(parent, GWL_HINSTANCE), NULL);

	SendMessage(label, WM_SETFONT, __sWin_Win32_font, TRUE);

	//HWND cWnd = capCreateCaptureWindowA("", WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, 0, 0, 1000, 620, parent, 0);
	//if (!SendMessage(cWnd, WM_USER + 10, devId, NULL) != NULL) {
	//	SendMessage(cWnd, WM_CAP_SET_SCALE, 1, Nothing)
	//		SendMessage(cWnd, WM_CAP_SET_PREVIEWRATE, 66, Nothing)
	//		SendMessage(cWnd, WM_CAP_SET_PREVIEW, 1, Nothing)
	//}
	//else {
		//MessageBox.Show("Error connecting to capture device. Make sure your WebCam is connected and try again.");
	//}

	//SetWindowText(label, L"Text Override");

	//SetWindowPos(label, HWND_TOP, bounds->x, viewHeight - bounds->y - bounds->height, bounds->width, bounds->height, 0);

	BringWindowToTop(label);

	//SetForegroundWindow(label);

	__sWin_Win32_viewID++;

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
		(HMENU)__sWin_Win32_viewID,
		GetWindowLong(parent, GWL_HINSTANCE), NULL);

	SendMessage(textField, WM_SETFONT, __sWin_Win32_font, TRUE);

	__sWin_Win32_viewID++;

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

SMouseState* swGetMouseState(SWin_Win32_Window* window) {
	return window->mouseState;
}

inline LRESULT CALLBACK SWin_Win32_Thread_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	SWin_Win32_RootPointer* root = GetWindowLongPtr(hWnd, GWLP_USERDATA);
	SWin_Win32_Window* window = NULL;

	if (root != NULL) {
		window = root->window;;
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	if (message == WM_PAINT) {
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	else if (message == WM_CLOSE) {
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
	else if (message == WM_MOUSEMOVE) {
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(window->hWnd, &pt);

		window->mouseState->x = (float)pt.x;
		window->mouseState->y = (float)pt.y;
	}
	else if (message == WM_LBUTTONDOWN) {
		window->mouseState->ldown = 1;
	}
	else if (message == WM_LBUTTONUP) {
		window->mouseState->ldown = 0;
	}
	else if (message == WM_MOUSEWHEEL) {
		window->mouseState->scroll += GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
