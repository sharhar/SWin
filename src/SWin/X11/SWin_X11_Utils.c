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

XrmDatabase __sWin_X11_db;

XFontStruct* __sWin_X11_font_fixed;
XFontStruct* __sWin_X11_font_9x15;
XFontStruct* __sWin_X11_font_9x15bold;
XFontStruct* __sWin_X11_font_10x20;
XFontStruct* __sWin_X11_font_10x20bold;

XFontStruct* __sWin_X11_font_button;

unsigned long __sWin_X11_hover;
unsigned long __sWin_X11_press;
unsigned long __sWin_X11_lightBlue;
unsigned long __sWin_X11_darkBlue;

unsigned long __sWin_X11_lightGray;
unsigned long __sWin_X11_gray;
unsigned long __sWin_X11_DimGray;

double _swGetRawTime();

void swInit() {
	XInitThreads();
    __sWin_X11_display = XOpenDisplay(NULL);
    __sWin_X11_screen = DefaultScreen(__sWin_X11_display);
    __sWin_X11_black = BlackPixel(__sWin_X11_display, __sWin_X11_screen);
    __sWin_X11_white = WhitePixel(__sWin_X11_display, __sWin_X11_screen);
    __sWin_X11_wmDeleteMessage = XInternAtom(__sWin_X11_display, "WM_DELETE_WINDOW", False);
    __sWin_X11_context = XUniqueContext();

	__sWin_X11_db = XrmGetDatabase(__sWin_X11_display);

	__sWin_X11_font_fixed = XLoadQueryFont(__sWin_X11_display, "fixed");
	__sWin_X11_font_9x15 = XLoadQueryFont(__sWin_X11_display, "9x15");
	__sWin_X11_font_9x15bold = XLoadQueryFont(__sWin_X11_display, "9x15bold");
	__sWin_X11_font_10x20 = XLoadQueryFont(__sWin_X11_display, "10x20");
	__sWin_X11_font_10x20bold = XLoadQueryFont(__sWin_X11_display, "10x20bold");

	if(__sWin_X11_font_10x20 != NULL) {
		__sWin_X11_font_button = __sWin_X11_font_10x20;
	} else if (__sWin_X11_font_9x15bold != NULL) {
		__sWin_X11_font_button = __sWin_X11_font_9x15bold;
	} else if (__sWin_X11_font_9x15 != NULL) {
		__sWin_X11_font_button = __sWin_X11_font_9x15;
	} else {
		__sWin_X11_font_button = __sWin_X11_font_fixed;
	}

	Colormap cmap = DefaultColormap(__sWin_X11_display, __sWin_X11_screen);

	int fontNum = 0;
	int maxNum = 1024;
	char* pattern = "x*";

	char** fontNames = XListFonts(__sWin_X11_display, pattern, maxNum, &fontNum);

	for(int i = 0; i < fontNum; i++) {
		printf("%d. %s\n", i, fontNames[i]);
	}

	if(fontNames != NULL) {
		XFreeFontNames(fontNames);
	} else {
		printf("Found no fonts matching pattern: %s\n", pattern);
	}

	XColor col1, col2;

	XAllocNamedColor(__sWin_X11_display, cmap, "LightGray", &col1, &col2);
	__sWin_X11_lightGray = col2.pixel;

	XAllocNamedColor(__sWin_X11_display, cmap, "Gray", &col1, &col2);
	__sWin_X11_gray = col2.pixel;

	XAllocNamedColor(__sWin_X11_display, cmap, "DimGray", &col1, &col2);
	__sWin_X11_DimGray = col2.pixel;


	XAllocNamedColor(__sWin_X11_display, cmap, "SkyBlue", &col1, &col2);
	__sWin_X11_hover = col2.pixel;

	XColor xcolour;

	xcolour.red = (unsigned short)(0.48f * (256*256-1));
	xcolour.green = (unsigned short)(0.74f * (256*256-1));
	xcolour.blue = (unsigned short)(0.85f * (256*256-1));
	xcolour.flags = DoRed | DoGreen | DoBlue;
	XAllocColor(__sWin_X11_display, cmap, &xcolour);

	__sWin_X11_press = xcolour.pixel;

	XAllocNamedColor(__sWin_X11_display, cmap, "PaleTurquoise", &col1, &col2);
	__sWin_X11_lightBlue = col2.pixel;

	XAllocNamedColor(__sWin_X11_display, cmap, "SteelBlue", &col1, &col2);
	__sWin_X11_darkBlue = col2.pixel;

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