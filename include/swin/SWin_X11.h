#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <malloc.h>
#include <dlfcn.h>

struct SWin_X11_Window;

typedef struct SWin_X11_View {
    Window window;
    GC gc;
    int width, height;
    struct SWin_X11_View** children;
    int childCount;
    SColor bg;
    struct SWin_X11_Window* win;
} SWin_X11_View;

typedef struct SWin_X11_Window {
    SWin_X11_View view;
    uint8_t closeRequested;
    Colormap colormap;
    Visual* visual;
} SWin_X11_Window;

extern Display* __sWin_X11_display;
extern int __sWin_X11_screen;
extern unsigned long __sWin_X11_black;
extern unsigned long __sWin_X11_white;
extern XEvent __sWin_X11_event;
extern Atom __sWin_X11_wmDeleteMessage;
extern XContext __sWin_X11_context;