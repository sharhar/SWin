#include "../../../include/swin/SWin.h"
#include "../../../include/swin/SWin_X11.h"

XEvent __sWin_X11_event;

SWindow* swCreateWindow(int width, int height, const char* title) {
    SWin_X11_Window* result = (SWin_X11_Window*)malloc(sizeof(SWin_X11_Window));

    result->visual = DefaultVisual(__sWin_X11_display, __sWin_X11_screen);

    result->view.window = XCreateWindow(__sWin_X11_display, DefaultRootWindow(__sWin_X11_display), 0, 0, width, height, 5, CopyFromParent, InputOutput, result->visual, 0, NULL);

    XSetStandardProperties(__sWin_X11_display, result->view.window,"My Window","HI!",None,NULL,0,NULL);

    XSetWMProtocols(__sWin_X11_display, result->view.window, &__sWin_X11_wmDeleteMessage, 1);
    result->closeRequested = 0;

    XSelectInput(__sWin_X11_display, result->view.window, ExposureMask|ButtonPressMask|KeyPressMask);

    result->view.gc = XCreateGC(__sWin_X11_display, result->view.window, 0,0);

    result->view.children = NULL;
    result->view.childCount = 0;
    result->view.width = width;
    result->view.height = height;
    result->view.bg.r = 1;
    result->view.bg.g = 1;
    result->view.bg.b = 1;
    result->view.bg.a = 1;
    result->view.win = result;

    XSetBackground(__sWin_X11_display,result->view.gc,__sWin_X11_black);
    XSetForeground(__sWin_X11_display,result->view.gc,__sWin_X11_white);

    XSaveContext(__sWin_X11_display, result->view.window, __sWin_X11_context, (XPointer)result);

    XClearWindow(__sWin_X11_display, result->view.window);
    XMapRaised(__sWin_X11_display, result->view.window);

	result->colormap = XCreateColormap(__sWin_X11_display, result->view.window, result->visual, AllocNone);

    return (SWindow*)result;
}

void swSetViewBackgroundColor(SView* view, SColor color) {
    ((SWin_X11_View*)view)->bg.r = color.r;
    ((SWin_X11_View*)view)->bg.g = color.g;
    ((SWin_X11_View*)view)->bg.b = color.b;
    ((SWin_X11_View*)view)->bg.a = color.a;
}

void __sWin_X11_swDrawViewBackground(SWin_X11_View* view) {
	if(view->bg.a > 0.5f) {
		XColor xcolour;

		xcolour.red = (unsigned short)(view->bg.r * (256*256-1));
		xcolour.green = (unsigned short)(view->bg.g * (256*256-1));
		xcolour.blue = (unsigned short)(view->bg.b * (256*256-1));
		xcolour.flags = DoRed | DoGreen | DoBlue;
		XAllocColor(__sWin_X11_display, view->win->colormap, &xcolour);

		XSetForeground(__sWin_X11_display, view->gc, xcolour.pixel);
		XFillRectangle(__sWin_X11_display, view->window, view->gc, 0, 0, view->width, view->height);

		for(int i = 0; i < view->childCount;i++) {
			__sWin_X11_swDrawViewBackground(view->children[i]);
		}
	}
}

void swPollEvents() {
	while(XPending(__sWin_X11_display)) {
		XNextEvent(__sWin_X11_display, &__sWin_X11_event);

		if (__sWin_X11_event.type == ClientMessage) {
			if (__sWin_X11_event.xclient.data.l[0] == __sWin_X11_wmDeleteMessage) {
				SWin_X11_Window *window = NULL;
				XFindContext(__sWin_X11_display, __sWin_X11_event.xany.window, __sWin_X11_context,
				             (XPointer *) &window);
				window->closeRequested = 1;
			}
		}

		if (__sWin_X11_event.type == Expose && __sWin_X11_event.xexpose.count == 0) {
			SWin_X11_View *view = NULL;
			XFindContext(__sWin_X11_display, __sWin_X11_event.xany.window, __sWin_X11_context, (XPointer *) &view);
			__sWin_X11_swDrawViewBackground(view);
		}

		//KeySym key;
		//char text[255];

		//if (__sWin_X11_event.type == KeyPress && XLookupString(&__sWin_X11_event.xkey, text, 255, &key, 0) == 1) {
		//	printf("You pressed the %c key!\n", text[0]);
		//}

		//if (__sWin_X11_event.type == KeyRelease && XLookupString(&__sWin_X11_event.xkey, text, 255, &key, 0) == 1) {
		//	printf("You released the %c key!\n", text[0]);
		//}

		//if (__sWin_X11_event.type == ButtonPress) {
		//	printf("You pressed a button at (%i,%i) in window %p\n", __sWin_X11_event.xbutton.x, __sWin_X11_event.xbutton.y, __sWin_X11_event.xany.window);
		//}
	}
}

void swDraw(SWindow* window) {
    XFlush(__sWin_X11_display);
}

uint8_t swCloseRequested(SWindow* window) {
    return ((SWin_X11_Window*)window)->closeRequested;
}

void swDestroyWindow(SWindow* window) {
    XFreeGC(__sWin_X11_display, ((SWin_X11_Window*)window)->view.gc);
    XDestroyWindow(__sWin_X11_display, ((SWin_X11_Window*)window)->view.window);
}

SView* swGetRootView(SWindow* window) {
    return window;
}

SView* swCreateView(SView* parent, SRect* bounds) {
    SWin_X11_View* view = (SWin_X11_View*)parent;

    SWin_X11_View* result = (SWin_X11_View*)malloc(sizeof(SWin_X11_View));

    result->window = XCreateWindow(__sWin_X11_display, view->window, bounds->x-1, bounds->y-1, bounds->width+2, bounds->height+2, 1, CopyFromParent, InputOutput, CopyFromParent, 0, NULL);

    result->gc = XCreateGC(__sWin_X11_display, result->window, 0,0);

    result->children = NULL;
    result->childCount = 0;
    result->width = (int)bounds->width;
    result->height = (int)bounds->height;
    result->bg.r = 1;
    result->bg.g = 1;
    result->bg.b = 1;
    result->bg.a = 1;

    XSetBackground(__sWin_X11_display,result->gc,__sWin_X11_white);
    XSetForeground(__sWin_X11_display,result->gc,__sWin_X11_white);

    XSaveContext(__sWin_X11_display, result->window, __sWin_X11_context, (XPointer)result);

    XClearWindow(__sWin_X11_display, result->window);
    XMapRaised(__sWin_X11_display, result->window);

    SWin_X11_View** views = (SWin_X11_View**)malloc(sizeof(SWin_X11_View**) * (view->childCount + 1));

    if(view->children != NULL) {
        memcpy(views, view->children, sizeof(SWin_X11_View**) * view->childCount);
        free(view->children);
    }

    views[view->childCount] = result;
    view->children = views;
    view->childCount++;

    result->win = view->win;

    return result;
}