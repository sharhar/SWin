#ifndef SWin_X11_h
#define SWin_X11_h

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <malloc.h>
#include <dlfcn.h>

struct SWin_X11_Window;

typedef enum SWin_X11_View_Type {
	SWIN_X11_VIEW_TYPE_VIEW = 1,
	SWIN_X11_VIEW_TYPE_OPENGL = 2,
	SWIN_X11_VIEW_TYPE_BUTTON = 3,
} SWin_X11_View_Type;

typedef struct SWin_X11_View {
    Window window;
    GC gc;
    int width, height;
    struct SWin_X11_View** children;
    int childCount;
    SColor bg;
	SWin_X11_View_Type type;
    struct SWin_X11_Window* win;
} SWin_X11_View;

typedef struct SWin_X11_Window {
    SWin_X11_View view;
    uint8_t closeRequested;
    Colormap colormap;
    Visual* visual;
} SWin_X11_Window;

typedef void (*SWin_X11_ButtonCallback)(void*);

typedef struct SWin_X11_Button {
	SWin_X11_View view;
	XChar2b* text;
	int text_width;
	int font_ascent;
	int width, height;
	int titleLength;
	unsigned long currentColor;
	unsigned long upColor;
	unsigned long downColor;
	void *cbdata;
	SWin_X11_ButtonCallback buttonRelease;
} SWin_X11_Button;

extern Display* __sWin_X11_display;
extern int __sWin_X11_screen;
extern unsigned long __sWin_X11_black;
extern unsigned long __sWin_X11_white;
extern XEvent __sWin_X11_event;
extern Atom __sWin_X11_wmDeleteMessage;
extern XContext __sWin_X11_context;

extern XFontStruct* __sWin_X11_font_fixed;
extern XFontStruct* __sWin_X11_font_9x15;
extern XFontStruct* __sWin_X11_font_9x15bold;
extern XFontStruct* __sWin_X11_font_10x20;
extern XFontStruct* __sWin_X11_font_10x20bold;
extern XFontStruct* __sWin_X11_font_button;

extern unsigned long __sWin_X11_hover;
extern unsigned long __sWin_X11_press;
extern unsigned long __sWin_X11_lightBlue;
extern unsigned long __sWin_X11_darkBlue;

extern unsigned long __sWin_X11_lightGray;
extern unsigned long __sWin_X11_gray;
extern unsigned long __sWin_X11_DimGray;

#endif
