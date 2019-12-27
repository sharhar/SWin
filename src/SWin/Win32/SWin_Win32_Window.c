#include <swin/SWin.h>
#include <swin/SWin_Win32.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define GWL_HINSTANCE (-6) //I don't know why, but even though this macro is defined in windows.h, it needs to be redefined here for this file to compile

uint32_t __sWin_Win32_viewID;
MSG __sWin_Win32_msg;

SWindow* swCreateWindow(int width, int height, const char* title) {
	CHECK(title, "title was NULL", NULL);
	
	SWin_Win32_Window* result = ALLOC_S(SWin_Win32_Window);
	CHECK(result, "Failed to allocate Window", NULL);

	result->type = TYPE_SWIN_WIN32_WINDOW;
	result->view.type = TYPE_SWIN_WIN32_VIEW;

	result->view.mouseDownCallback = NULL;
	result->view.mouseUpCallback = NULL;
	result->view.mouseMovedCallback = NULL;
	result->view.mouseScrollCallback = NULL;

	int n;
	WNDCLASSEX wc;
	LOGPALETTE* lpPal;

	result->view.hInstance = GetModuleHandle(NULL);

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
	wc.hInstance = result->view.hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = title;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	CHECK(RegisterClassEx(&wc), "RegisterClassEx() failed", NULL);

	int nStyle = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX;

	result->view.hWnd = CreateWindowEx(WS_EX_LAYERED, title, title, WS_SYSMENU, 
		100, 100, contentSize.right - contentSize.left, contentSize.bottom - contentSize.top, 
		HWND_DESKTOP, NULL, result->view.hInstance, NULL);

	CHECK(result->view.hWnd, "CreateWindowEx() failed", NULL);

	ZeroMemory(&result->msg, sizeof(MSG));
	result->close = 0;

	result->view.window = result;

	SetWindowLongPtr(result->view.hWnd, GWLP_USERDATA, (LONG_PTR)&result->view);

	SetLayeredWindowAttributes(result->view.hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	ShowWindow(result->view.hWnd, SW_SHOW);
	UpdateWindow(result->view.hWnd);
	SetForegroundWindow(result->view.hWnd);
	SetFocus(result->view.hWnd);

	SendMessage(result->view.hWnd, WM_SETFONT, __sWin_Win32_font, TRUE);

	return result;
}

void swPollEvents() {
	ZeroMemory(&__sWin_Win32_msg, sizeof(MSG));
	
	while (PeekMessage(&__sWin_Win32_msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&__sWin_Win32_msg);
		DispatchMessage(&__sWin_Win32_msg);
	}
}

SResult swDraw(SWindow* window) {
	CHECK(window, "window was NULL", SWIN_FAILED);

	SWin_Win32_Window* _window = (SWin_Win32_Window*)window;

	CHECK_T(_window->type, TYPE_SWIN_WIN32_WINDOW, "window was not an SWindow*", NULL);

	return SWIN_OK;
}

uint8_t swCloseRequested(SWindow* window) {
	CHECK(window, "window was NULL", 1);

	SWin_Win32_Window* _window = (SWin_Win32_Window*)window;

	CHECK_T(_window->type, TYPE_SWIN_WIN32_WINDOW, "window was not an SWindow*", NULL);

	return _window->close;
}

SResult swDestroyWindow(SWindow* window) {
	CHECK(window, "window was NULL", SWIN_FAILED);

	SWin_Win32_Window* _window = (SWin_Win32_Window*)window;

	CHECK_T(_window->type, TYPE_SWIN_WIN32_WINDOW, "window was not an SWindow*", NULL);

	DestroyWindow(_window->view.hWnd);

	DEALLOC(_window);

	return SWIN_OK;
}

SView* swGetRootView(SWindow* window) {
	CHECK(window, "window was NULL", NULL);

	SWin_Win32_Window* _window = (SWin_Win32_Window*)window;

	CHECK_T(_window->type, TYPE_SWIN_WIN32_WINDOW, "window was not an SWindow*", NULL);

	return (SView*)&_window->view;
}

SView* swCreateView(SView* parent, SRect* bounds) {
	CHECK(parent, "parent was NULL", NULL);
	CHECK(bounds, "bounds was NULL", NULL);

	SWin_Win32_View* _parent = (SWin_Win32_View*)parent;

	CHECK_T(_parent->type, TYPE_SWIN_WIN32_VIEW, "parent was not an SView*", NULL);

	SWin_Win32_View* result = ALLOC_S(SWin_Win32_View);
	CHECK(result, "Failed to allocate SWin_Win32_View", NULL);

	result->type = TYPE_SWIN_WIN32_VIEW | VIEW_TYPE_PLAIN;
	
	result->mouseDownCallback = NULL;
	result->mouseUpCallback = NULL;
	result->mouseMovedCallback = NULL;
	result->mouseScrollCallback = NULL;

	uint32_t viewID = __sWin_Win32_viewID;

	size_t len = 0;
	while (viewID != 0) {
		viewID = viewID / 10;
		len++;
	}

	char* pre_viewIDStr = ALLOC(char*, len+1);
	CHECK(pre_viewIDStr, "Failed to Allocate String", NULL);

	memset(pre_viewIDStr, 0, len+1);

	viewID = __sWin_Win32_viewID;

	for (int i = 0; i < len; i++) {
		pre_viewIDStr[i] = ((char)(viewID % 10)) + '0';
		viewID = viewID / 10;
	}

	char* viewIDStr = ALLOC(char*, len+2);
	CHECK(viewIDStr, "Failed to Allocate String", NULL);

	memset(viewIDStr, 0, len+2);

	viewIDStr[0] = 'C';

	for (int i = 0; i < len; i++) {
		viewIDStr[i + 1] = pre_viewIDStr[len - i - 1];
	}

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = SWin_Win32_Thread_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _parent->hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = viewIDStr;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	CHECK(RegisterClassEx(&wc), "RegisterClassEx() failed", NULL);

	RECT viewBounds;
	GetClientRect(_parent->hWnd, &viewBounds);

	int viewHeight = abs(viewBounds.top - viewBounds.bottom);

	result->hWnd = CreateWindowEx(0, TEXT(viewIDStr),
		(LPCTSTR)NULL,
		WS_CHILD | WS_BORDER | WS_VISIBLE,
		bounds->x, viewHeight - bounds->y - bounds->height, bounds->width, bounds->height,
		_parent->hWnd,
		(HMENU)__sWin_Win32_viewID,
		_parent->hInstance,
		NULL);

	CHECK(result->hWnd, "CreateWindowEx() failed", NULL);

	result->window = _parent->window;

	SetWindowLongPtr(result->hWnd, GWLP_USERDATA, result);

	result->hInstance = _parent->hInstance;

	__sWin_Win32_viewID++;

	CHECK_RECT(bounds);

	result->mouseDownCallback = NULL;
	result->mouseUpCallback = NULL;
	result->mouseScrollCallback = NULL;
	result->mouseMovedCallback = NULL;

	return result;
}

SResult swSetMouseMovedCallback(SView* view, pfnSMouseMovedCallback callback) {
	CHECK(view, "view was NULL", NULL);

	SWin_Win32_View* _view = (SWin_Win32_View*)view;

	CHECK_T(_view->type, TYPE_SWIN_WIN32_VIEW, "view was not an SView*", NULL);

	_view->mouseMovedCallback = callback;
}

SResult swSetMouseDownCallback(SView* view, pfnSMouseDownCallback callback) {
	CHECK(view, "view was NULL", NULL);

	SWin_Win32_View* _view = (SWin_Win32_View*)view;

	CHECK_T(_view->type, TYPE_SWIN_WIN32_VIEW, "view was not an SView*", NULL);

	_view->mouseDownCallback = callback;
}

SResult swSetMouseUpCallback(SView* view, pfnSMouseUpCallback callback) {
	CHECK(view, "view was NULL", NULL);

	SWin_Win32_View* _view = (SWin_Win32_View*)view;

	CHECK_T(_view->type, TYPE_SWIN_WIN32_VIEW, "view was not an SView*", NULL);

	_view->mouseUpCallback = callback;
}

SResult swSetMouseScrollCallback(SView* view, pfnSMouseScrollCallback callback) {
	CHECK(view, "view was NULL", NULL);

	SWin_Win32_View* _view = (SWin_Win32_View*)view;

	CHECK_T(_view->type, TYPE_SWIN_WIN32_VIEW, "view was not an SView*", NULL);

	_view->mouseScrollCallback = callback;
}

void swSetViewBackgroundColor(SView* view, SColor color) {
	CHECK(view, "view was NULL", NULL);

	SWin_Win32_View* _view = (SWin_Win32_View*)view;

	CHECK_T(_view->type, TYPE_SWIN_WIN32_VIEW, "view was not an SView*", NULL);
}

SButton* swCreateButton(SView* parent, SRect* bounds, const char* title, void* callback, void* userData) {
	CHECK(parent, "parent was NULL", NULL);
	CHECK(bounds, "bounds was NULL", NULL);
	CHECK(title, "title was NULL", NULL);
	CHECK(callback, "callback was NULL", NULL);

	SWin_Win32_View* _parent = (SWin_Win32_View*)parent;
	buttonCallback _callback = (buttonCallback)callback;

	CHECK_T(_parent->type, TYPE_SWIN_WIN32_VIEW, "parent was not an SView*", NULL);

	SWin_Win32_View* result = ALLOC_S(SWin_Win32_View);
	CHECK(result, "Failed to allocate SWin_Win32_View", NULL);

	result->type = TYPE_SWIN_WIN32_VIEW | VIEW_TYPE_BUTTON;

	result->mouseDownCallback = NULL;
	result->mouseUpCallback = NULL;
	result->mouseMovedCallback = NULL;
	result->mouseScrollCallback = NULL;

	RECT viewBounds;
	GetClientRect(_parent->hWnd, &viewBounds);

	int viewHeight = abs(viewBounds.top - viewBounds.bottom);

	result->hWnd = CreateWindow(
		TEXT("BUTTON"), TEXT(title),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		bounds->x, viewHeight - bounds->y - bounds->height, bounds->width, bounds->height,
		_parent->hWnd,
		(HMENU)__sWin_Win32_viewID,
		_parent->hInstance, NULL);
	
	CHECK(result->hWnd, "CreateWindow(\"BUTTON\") failed", NULL);

	SendMessage(result->hWnd, WM_SETFONT, __sWin_Win32_font, TRUE);

	SWin_Win32_Button* buttonInfo = ALLOC_S(SWin_Win32_Button);
	CHECK(buttonInfo, "Failed to Allocate Button", NULL);

	buttonInfo->callback = _callback;
	buttonInfo->userPointer = userData;
	buttonInfo->hWnd = result;

	SetWindowLongPtr(result->hWnd, GWLP_USERDATA, (LONG_PTR)buttonInfo);

	result->hInstance = _parent->hInstance;

	__sWin_Win32_viewID++;

	CHECK_RECT(bounds);

	return result;
}

SLabel* swCreateLabel(SView* parent, SRect* bounds, const char* text) {
	CHECK(parent, "parent was NULL", NULL);
	CHECK(bounds, "bounds was NULL", NULL);
	CHECK(text, "text was NULL", NULL);
	
	SWin_Win32_View* _parent = (SWin_Win32_View*)parent;

	CHECK_T(_parent->type, TYPE_SWIN_WIN32_VIEW, "parent was not an SView*", NULL);

	SWin_Win32_View* result = ALLOC_S(SWin_Win32_View);
	CHECK(result, "Failed to allocate SWin_Win32_View", NULL);

	result->type = TYPE_SWIN_WIN32_VIEW | VIEW_TYPE_LABEL;

	result->mouseDownCallback = NULL;
	result->mouseUpCallback = NULL;
	result->mouseMovedCallback = NULL;
	result->mouseScrollCallback = NULL;

	RECT viewBounds;
	GetClientRect(parent, &viewBounds);

	int viewHeight = abs(viewBounds.top - viewBounds.bottom);

	result->hWnd = CreateWindow(
		TEXT("STATIC"), TEXT(text),
		WS_VISIBLE | WS_CHILD | WS_BORDER,
		bounds->x, viewHeight - bounds->y - bounds->height, bounds->width, bounds->height,
		_parent->hWnd,
		(HMENU)__sWin_Win32_viewID,
		_parent->hInstance, NULL);

	CHECK(result->hWnd, "CreateWindow(\"STATIC\") failed", NULL);

	SendMessage(result, WM_SETFONT, __sWin_Win32_font, TRUE);

	BringWindowToTop(result);

	result->hInstance = _parent->hInstance;

	__sWin_Win32_viewID++;

	CHECK_RECT(bounds);

	return result;
}

STextField* swCreateTextField(SView* parent, SRect* bounds, const char* text) {
	CHECK(parent, "parent was NULL", NULL);
	CHECK(bounds, "bounds was NULL", NULL);
	CHECK(text, "text was NULL", NULL);

	SWin_Win32_View* _parent = (SWin_Win32_View*)parent;

	CHECK_T(_parent->type, TYPE_SWIN_WIN32_VIEW, "parent was not an SView*", NULL);

	SWin_Win32_View* result = ALLOC_S(SWin_Win32_View);
	CHECK(result, "Failed to allocate SWin_Win32_View", NULL);

	result->type = TYPE_SWIN_WIN32_VIEW | VIEW_TYPE_TEXTFIELD;

	result->mouseDownCallback = NULL;
	result->mouseUpCallback = NULL;
	result->mouseMovedCallback = NULL;
	result->mouseScrollCallback = NULL;

	RECT viewBounds;
	GetClientRect(_parent->hWnd, &viewBounds);

	int viewHeight = abs(viewBounds.top - viewBounds.bottom);

	result->hWnd = CreateWindow(
		TEXT("EDIT"), TEXT(text),
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
		bounds->x, viewHeight - bounds->y - bounds->height, bounds->width, bounds->height,
		_parent->hWnd,
		(HMENU)__sWin_Win32_viewID,
		_parent->hInstance, NULL);

	CHECK(result->hWnd, "CreateWindow(\"EDIT\") failed", NULL);

	SendMessage(result->hWnd, WM_SETFONT, __sWin_Win32_font, TRUE);

	result->hInstance = _parent->hInstance;

	__sWin_Win32_viewID++;

	CHECK_RECT(bounds);

	return result;
}

char* swGetTextFromTextField(STextField* textField) {
	CHECK(textField, "textField was NULL", NULL);

	SWin_Win32_View* _textField = (SWin_Win32_View*)textField;

	CHECK_T(_textField->type, TYPE_SWIN_WIN32_VIEW, "textFeild was not an STextField*", NULL);
	CHECK_K(_textField->type, VIEW_TYPE_TEXTFIELD, "textFeild was not an STextField*", NULL);

	size_t length = SendMessage(_textField->hWnd, WM_GETTEXTLENGTH, 0, 0);

	CHECK(length != -1, "SendMessage(WM_GETTEXTLENGTH) failed", NULL);

	char* result = ALLOC(char, length + 1);
	CHECK(result, "Failed to allocate String", NULL);

	SendMessage(_textField->hWnd, WM_GETTEXT, length + 1, (LPARAM)result);

	CHECK(result, "SendMessage(WM_GETTEXT) failed", NULL);

	return result;
}

inline LRESULT CALLBACK SWin_Win32_Thread_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	SWin_Win32_View* _view = GetWindowLongPtr(hWnd, GWLP_USERDATA);
	
	if (_view == NULL) {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	SWin_Win32_Window* window = (SWin_Win32_Window*)_view->window;

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
		ScreenToClient(hWnd, &pt);

		if (_view->mouseMovedCallback != NULL) {
			_view->mouseMovedCallback(pt.x, pt.y);
		}
	}
	else if (message == WM_LBUTTONDOWN) {
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);

		if (_view->mouseDownCallback != NULL) {
			_view->mouseDownCallback(pt.x, pt.y, SWIN_MOUSE_BUTTON_LEFT);
		}
	}
	else if (message == WM_LBUTTONUP) {
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);

		if (_view->mouseUpCallback != NULL) {
			_view->mouseUpCallback(pt.x, pt.y, SWIN_MOUSE_BUTTON_LEFT);
		}
	}
	else if (message == WM_MOUSEWHEEL) {
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);

		if (_view->mouseScrollCallback != NULL) {
			_view->mouseScrollCallback(pt.x, pt.y, GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
		}
	}
	else {
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
