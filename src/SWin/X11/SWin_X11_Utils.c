#include "../../../include/swin/SWin.h"
#include "../../../include/swin/SWin_X11.h"
#include <unistd.h>
#include <time.h>

Display* __sWin_X11_display;
int __sWin_X11_screen;
unsigned long __sWin_X11_black;
unsigned long __sWin_X11_white;
Atom __sWin_X11_wmDeleteMessage;
XContext __sWin_X11_context;

long __sWin_X11_milliSeconds;
time_t __sWin_X11_seconds;
struct timespec __sWin_X11_timespec;

double __sWin_X11_startTime = 0;

double _swGetRawTime();

void swInit() {
    __sWin_X11_display = XOpenDisplay(NULL);
    __sWin_X11_screen = DefaultScreen(__sWin_X11_display);
    __sWin_X11_black = BlackPixel(__sWin_X11_display, __sWin_X11_screen);
    __sWin_X11_white = WhitePixel(__sWin_X11_display, __sWin_X11_screen);
    __sWin_X11_wmDeleteMessage = XInternAtom(__sWin_X11_display, "WM_DELETE_WINDOW", False);
    __sWin_X11_context = XUniqueContext();

	__sWin_X11_startTime = _swGetRawTime();
}

double _swGetRawTime() {
	clock_gettime(CLOCK_REALTIME, &__sWin_X11_timespec);

	__sWin_X11_seconds  = __sWin_X11_timespec.tv_sec;
	__sWin_X11_milliSeconds = __sWin_X11_timespec.tv_nsec;
	if (__sWin_X11_milliSeconds > 999999999) {
		__sWin_X11_seconds++;
		__sWin_X11_milliSeconds = 0;
	}

	return (double)(__sWin_X11_seconds + (((double)__sWin_X11_milliSeconds)/1000000000.0));
}

double swGetTime() {
	return _swGetRawTime() - __sWin_X11_startTime;
}

void swSleep(uint32_t milliSeconds) {
	usleep(milliSeconds * 1000);
}


void swPopup(char* title, char* text) {

}

void swTerminate() {

}