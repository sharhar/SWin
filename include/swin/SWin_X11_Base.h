#ifndef SWin_POSIX_h
#define SWin_POSIX_h

#include <swin/SWin.h>
#include <X11/Xlib.h>
#include <time.h>

typedef enum SWin_X11_View_Type {
	SWIN_X11_VIEW_TYPE_VIEW = 1,
	SWIN_X11_VIEW_TYPE_OPENGL = 2,
	SWIN_X11_VIEW_TYPE_BUTTON = 3,
} SWin_X11_View_Type;

typedef struct SWin_X11_Base_View {
	Window xWindow;
	SWin_X11_View_Type type;
} SWin_X11_Base_View;

extern Display* __sWin_X11_display;
extern int __sWin_X11_screen;
extern double __sWin_X11_startTime;

double _swGetRawTime();

#endif
