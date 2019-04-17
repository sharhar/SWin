#ifndef SWin_Win32_h
#define SWin_Win32_h

#include <swin/SWin.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

typedef struct SWin_Win32_Time {
	uint8_t hasPC;
	uint64_t frequency;
} SWin_Win32_Time;

typedef struct SWin_Win32_Window {
	struct SWin_Win32_Window* root;
	HINSTANCE instance;
	HWND hWnd;
	const char* title;
	MSG msg;
	int close;
	SMouseState* mouseState;
} SWin_Win32_Window;

typedef struct SWin_Win32_OpenGLContext {
	HGLRC hRc;
	HDC hDc;
	HPALETTE hPalette;
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
	HANDLE handle;
	DWORD   threadId;
} SWin_Win32_Thread;

inline LRESULT CALLBACK SWin_Win32_Thread_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

extern SWin_Win32_Time __sWin_Win32_Time;
extern uint32_t __sWin_Win32_viewID;
extern HFONT __sWin_Win32_font;
extern HMODULE __sWin_Win32_libGL;

#endif