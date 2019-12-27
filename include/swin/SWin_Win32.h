#ifndef SWin_Win32_h
#define SWin_Win32_h

#include <swin/SWin.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define TYPE_SWIN_WIN32_VIEW            0x00010000
#define TYPE_SWIN_WIN32_WINDOW          0x00020000
#define TYPE_SWIN_WIN32_OPENGLCONTEXT   0x00030000
#define TYPE_SWIN_WIN32_THREAD          0x00040000
#define TYPE_SWIN_WIN32_MUTEX           0x00050000

#define VIEW_TYPE_PLAIN                 0x00000001
#define VIEW_TYPE_BUTTON                0x00000002
#define VIEW_TYPE_LABEL                 0x00000003
#define VIEW_TYPE_TEXTFIELD             0x00000004

typedef struct SWin_Win32_Time {
	uint8_t hasPC;
	uint64_t frequency;
} SWin_Win32_Time;

typedef struct SWin_Win32_View {
	SType type;
	HWND hWnd;
	HINSTANCE hInstance;
	void* window;
	pfnSMouseMovedCallback mouseMovedCallback;
	pfnSMouseDownCallback mouseDownCallback;
	pfnSMouseUpCallback mouseUpCallback;
	pfnSMouseScrollCallback mouseScrollCallback;
} SWin_Win32_View;

typedef struct SWin_Win32_Window {
	SType type;
	SWin_Win32_View view;
	const char* title;
	MSG msg;
	int close;
} SWin_Win32_Window;

typedef struct SWin_Win32_OpenGLContext {
	SType type;
	HGLRC hRc;
	HDC hDc;
} SWin_Win32_OpenGLContext;

typedef struct SWin_Win32_RootPointer {
	SWin_Win32_Window* window;
} SWin_Win32_RootPointer;

typedef void(*buttonCallback)(void*);

typedef struct SWin_Win32_Button {
	buttonCallback callback;
	void* userPointer;
	HWND hWnd;
} SWin_Win32_Button;

typedef struct SWin_Win32_Thread {
	SType type;
	HANDLE handle;
	DWORD   threadId;
} SWin_Win32_Thread;

typedef struct SWin_Win32_Mutex {
	SType type;
	HANDLE handle;
} SWin_Win32_Mutex;

inline LRESULT CALLBACK SWin_Win32_Thread_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

extern SWin_Win32_Time __sWin_Win32_Time;
extern uint32_t __sWin_Win32_viewID;
extern HFONT __sWin_Win32_font;
extern HMODULE __sWin_Win32_libGL;

#endif