#include <swin/SWin.h>
#include <swin/SWin_Win32.h>

SWin_Win32_Time __sWin_Win32_Time;
HFONT __sWin_Win32_font;

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

	__sWin_Win32_viewID = 1;

	__sWin_Win32_font = CreateFont(0, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE, TEXT("Segoe UI"));
}

double swGetTime() {
	uint64_t value;

	QueryPerformanceCounter((LARGE_INTEGER*)&value);

	double result = (double)value;

	result = result / __sWin_Win32_Time.frequency;

	return result;
}

void swSleep(uint32_t milliSeconds) {
	SleepEx((DWORD) milliSeconds, FALSE);
}

void swPopup(char* title, char* text) {
	MessageBox(NULL, text, title, MB_OK);
}

void swTerminate() {
	
}
