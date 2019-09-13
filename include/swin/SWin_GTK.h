#ifndef SWin_GTK_h
#define SWin_GTK_h

#include <swin/SWin.h>
#include <swin/SWin_X11_Base.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <stdio.h>
#include <malloc.h>
#include <dlfcn.h>

struct SWin_GTK_Window;

typedef struct SWin_GTK_View {
	SWin_X11_Base_View baseView;
	GtkWidget* container;
	GtkWidget* area;
	GtkWidget* frame;
	struct SWin_GTK_Window* win;
	gint height;
} SWin_GTK_View;

typedef struct SWin_GTK_Window {
	SWin_GTK_View view;
	uint8_t closeRequested;
	GtkWidget* window;
} SWin_GTK_Window;

typedef struct SWin_GTK_Button {
	SWin_GTK_View view;
} SWin_GTK_Button;

extern Display* __sWin_X11_display;
extern int __sWin_X11_screen;


#endif
