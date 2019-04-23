#include "../../../include/swin/SWin.h"
#include "../../../include/swin/SWin_X11.h"
#include <unistd.h>

Display* __sWin_X11_display;
int __sWin_X11_screen;
unsigned long __sWin_X11_black;
unsigned long __sWin_X11_white;
Atom __sWin_X11_wmDeleteMessage;
XContext __sWin_X11_context;

void swInit() {
    __sWin_X11_display = XOpenDisplay(NULL);
    __sWin_X11_screen = DefaultScreen(__sWin_X11_display);
    __sWin_X11_black = BlackPixel(__sWin_X11_display, __sWin_X11_screen);
    __sWin_X11_white = WhitePixel(__sWin_X11_display, __sWin_X11_screen);
    __sWin_X11_wmDeleteMessage = XInternAtom(__sWin_X11_display, "WM_DELETE_WINDOW", False);
    __sWin_X11_context = XUniqueContext();
}

double swGetTime() {

}

void swSleep(uint32_t milliSeconds) {
	usleep(milliSeconds * 1000);
}


void swPopup(char* title, char* text) {

}

void swTerminate() {

}