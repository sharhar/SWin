#include "../../../include/swin/SWin.h"
#include "../../../include/swin/SWin_X11.h"

XEvent __sWin_X11_event;

SWindow* swCreateWindow(int width, int height, const char* title) {
    SWin_X11_Window* result = (SWin_X11_Window*)malloc(sizeof(SWin_X11_Window));

    result->visual = DefaultVisual(__sWin_X11_display, __sWin_X11_screen);

    result->view.window = XCreateWindow(__sWin_X11_display, DefaultRootWindow(__sWin_X11_display), 0, 0, width, height, 5, CopyFromParent, InputOutput, result->visual, 0, NULL);

    XSetStandardProperties(__sWin_X11_display, result->view.window, title, "SWin_X11", None, NULL, 0, NULL);

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
	result->view.type = SWIN_X11_VIEW_TYPE_VIEW;

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

void _swFillView(SWin_X11_View* pView, SView* parent, SRect* bounds) {
	SWin_X11_View* view = (SWin_X11_View*)parent;

	pView->window = XCreateWindow(__sWin_X11_display, view->window, bounds->x, view->height - bounds->y - bounds->height, bounds->width, bounds->height, 1, CopyFromParent, InputOutput, CopyFromParent, 0, NULL);

	pView->gc = XCreateGC(__sWin_X11_display, pView->window, 0,0);

	pView->children = NULL;
	pView->childCount = 0;
	pView->width = (int)bounds->width;
	pView->height = (int)bounds->height;
	pView->bg.r = 1;
	pView->bg.g = 1;
	pView->bg.b = 1;
	pView->bg.a = 1;
	pView->type = SWIN_X11_VIEW_TYPE_VIEW;

	XSetBackground(__sWin_X11_display, pView->gc, __sWin_X11_black);
	XSetForeground(__sWin_X11_display, pView->gc, __sWin_X11_black);

	XSaveContext(__sWin_X11_display, pView->window, __sWin_X11_context, (XPointer)pView);

	XClearWindow(__sWin_X11_display, pView->window);
	XMapRaised(__sWin_X11_display, pView->window);

	SWin_X11_View** views = (SWin_X11_View**)malloc(sizeof(SWin_X11_View**) * (view->childCount + 1));

	if(view->children != NULL) {
		memcpy(views, view->children, sizeof(SWin_X11_View**) * view->childCount);
		free(view->children);
	}

	views[view->childCount] = pView;
	view->children = views;
	view->childCount++;

	pView->win = view->win;
}

SView* swCreateView(SView* parent, SRect* bounds) {
	SWin_X11_View* result = (SWin_X11_View*)malloc(sizeof(SWin_X11_View));

	_swFillView(result, parent, bounds);

    return result;
}

SButton* swCreateButton(SView* parent, SRect* bounds, const char* title, void* callback, void* userData) {
	SWin_X11_Button* result = (SWin_X11_Button*)malloc(sizeof(SWin_X11_Button));

	int strlength = strlen(title);

	_swFillView((SWin_X11_View*)result, parent, bounds);

	result->view.type = SWIN_X11_VIEW_TYPE_BUTTON;

	result->font_ascent = __sWin_X11_font_button->ascent;

	result->text = malloc(sizeof(XChar2b) * (strlength+1));

	for(int i = 0 ; i < strlength; i++) {
		result->text[i].byte1 = 0;
		result->text[i].byte2 = title[i];
	}

	result->text[strlength].byte1 = 0;
	result->text[strlength].byte2 = 0;

	result->text_width = XTextWidth16(__sWin_X11_font_button, result->text, strlength);
	result->buttonRelease = callback;
	result->cbdata = userData;
	result->width = bounds->width-4;
	result->height = bounds->height-4;
	result->titleLength = strlength;
	result->currentColor = __sWin_X11_gray;
	result->upColor = __sWin_X11_lightGray;
	result->downColor = __sWin_X11_DimGray;

	printf("Setting up button with callback %p\n", callback);

	XSelectInput(__sWin_X11_display, result->view.window,
			ButtonPressMask|ButtonReleaseMask|StructureNotifyMask|ExposureMask|LeaveWindowMask|EnterWindowMask);

	XGCValues values;
	values.line_width = 1;
	values.line_style = LineSolid;
	values.font = __sWin_X11_font_button->fid;
	values.foreground = __sWin_X11_black;

	XChangeGC(__sWin_X11_display, result->view.gc, GCForeground|GCLineWidth|GCLineStyle|GCFont, &values);

	return result;
}

SLabel* swCreateLabel(SView* parent, SRect* bounds, const char* text) {
	SWin_X11_View* result = (SWin_X11_View*)malloc(sizeof(SWin_X11_View));

	_swFillView(result, parent, bounds);

	return result;
}

STextField* swCreateTextField(SView* parent, SRect* bounds, const char* text) {
	SWin_X11_View* result = (SWin_X11_View*)malloc(sizeof(SWin_X11_View));

	_swFillView(result, parent, bounds);

	return result;
}

char* swGetTextFromTextField(STextField* textField) {
	return "[placeholder]";
}

void _swClearButton(SWin_X11_Button* button) {
	XSetForeground(__sWin_X11_display, button->view.gc, button->downColor);
	XFillRectangle(__sWin_X11_display, button->view.window, button->view.gc, 0, 0, button->width+4, button->height+4);

	XSetForeground(__sWin_X11_display, button->view.gc, button->upColor);
	XDrawLine(__sWin_X11_display, button->view.window, button->view.gc, 0, 0, button->width+4, 0);
	XDrawLine(__sWin_X11_display, button->view.window, button->view.gc, 0, 0, 0, button->height+4);
	XDrawLine(__sWin_X11_display, button->view.window, button->view.gc, 1, 1, button->width+3, 1);
	XDrawLine(__sWin_X11_display, button->view.window, button->view.gc, 1, 1, 1, button->height+3);

	XSetWindowAttributes attrs2;
	attrs2.background_pixel = button->currentColor;
	attrs2.border_pixel = __sWin_X11_black;
	XChangeWindowAttributes(__sWin_X11_display, button->view.window, CWBackPixel|CWBorderPixel, &attrs2);
	XClearArea(__sWin_X11_display, button->view.window, 2, 2, button->width, button->height, True);
}

void __sWin_X11_swExposeView(SWin_X11_View* view, XEvent* ev) {
	if(view->type == SWIN_X11_VIEW_TYPE_VIEW) {
		XColor xcolour;

		xcolour.red = (unsigned short)(view->bg.r * (256*256-1));
		xcolour.green = (unsigned short)(view->bg.g * (256*256-1));
		xcolour.blue = (unsigned short)(view->bg.b * (256*256-1));
		xcolour.flags = DoRed | DoGreen | DoBlue;
		XAllocColor(__sWin_X11_display, view->win->colormap, &xcolour);

		XSetForeground(__sWin_X11_display, view->gc, xcolour.pixel);
		XFillRectangle(__sWin_X11_display, view->window, view->gc, 0, 0, view->width, view->height);
	} else if (view->type == SWIN_X11_VIEW_TYPE_BUTTON) {
		SWin_X11_Button* button = (SWin_X11_Button*)view;

		_swClearButton(button);

		XSetForeground(__sWin_X11_display, button->view.gc, __sWin_X11_black);

		int textx, texty, len;
		if (button->text){
			//len = XChar2bLen(button->text);
			textx = (button->width - button->text_width)/2;
			texty = (button->height + button->font_ascent)/2;
			XDrawString16(__sWin_X11_display, button->view.window, button->view.gc, textx, texty, button->text, button->titleLength);
		} else {  /* if there's no text draw the big X */
			XDrawLine(__sWin_X11_display, button->view.window, button->view.gc, 2, 2, button->width+2, button->height+2);
			XDrawLine(__sWin_X11_display, button->view.window, button->view.gc, button->width+2, 2, 2, button->height+2);
		}

	}

	for(int i = 0; i < view->childCount;i++) {
		__sWin_X11_swExposeView(view->children[i], ev);
	}
}

void swPollEvents() {
	while(XPending(__sWin_X11_display)) {
		XNextEvent(__sWin_X11_display, &__sWin_X11_event);

		SWin_X11_View *view = NULL;
		XFindContext(__sWin_X11_display, __sWin_X11_event.xany.window, __sWin_X11_context, (XPointer *) &view);

		if (__sWin_X11_event.type == ClientMessage) {
			if (__sWin_X11_event.xclient.data.l[0] == __sWin_X11_wmDeleteMessage) {
				SWin_X11_Window* window = (SWin_X11_Window*)view;
				window->closeRequested = 1;
			}
		}

		if (__sWin_X11_event.type == Expose && __sWin_X11_event.xexpose.count == 0) {
			__sWin_X11_swExposeView(view, &__sWin_X11_event);
		}

		if (__sWin_X11_event.type == ConfigureNotify) {
			if(view->type == SWIN_X11_VIEW_TYPE_BUTTON) {
				SWin_X11_Button* button = (SWin_X11_Button*)view;
				if (button->width != __sWin_X11_event.xconfigure.width
				    || button->height != __sWin_X11_event.xconfigure.height) {
					button->width = __sWin_X11_event.xconfigure.width;
					button->height = __sWin_X11_event.xconfigure.height;
					XClearWindow(__sWin_X11_display, button->view.window);
				}
			}
		}

		if (__sWin_X11_event.type == EnterNotify) {
			if(view->type == SWIN_X11_VIEW_TYPE_BUTTON) {
				SWin_X11_Button* button = (SWin_X11_Button*)view;
				button->currentColor = __sWin_X11_hover;
				button->upColor = __sWin_X11_lightBlue;
				button->downColor = __sWin_X11_darkBlue;
				//button->secondColor = __sWin_X11_press;
				_swClearButton(button);
			}
		}

		if (__sWin_X11_event.type == LeaveNotify) {
			if(view->type == SWIN_X11_VIEW_TYPE_BUTTON) {
				SWin_X11_Button* button = (SWin_X11_Button*)view;
				button->currentColor = __sWin_X11_gray;
				button->upColor = __sWin_X11_lightGray;
				button->downColor = __sWin_X11_DimGray;
				_swClearButton(button);
			}
		}

		if (__sWin_X11_event.type == ButtonPress) {
			if(view->type == SWIN_X11_VIEW_TYPE_BUTTON) {
				SWin_X11_Button* button = (SWin_X11_Button*)view;
				button->currentColor = __sWin_X11_press;
				button->upColor = __sWin_X11_darkBlue;
				button->downColor = __sWin_X11_lightBlue;
				//button->secondColor = __sWin_X11_hover;
				_swClearButton(button);
				if(button->buttonRelease) {
					button->buttonRelease(button->cbdata);
				}
			}
		}

		if (__sWin_X11_event.type == ButtonRelease) {
			if(view->type == SWIN_X11_VIEW_TYPE_BUTTON) {
				SWin_X11_Button* button = (SWin_X11_Button*)view;
				button->currentColor = __sWin_X11_hover;
				button->upColor = __sWin_X11_lightBlue;
				button->downColor = __sWin_X11_darkBlue;
				//button->secondColor = __sWin_X11_press;
				_swClearButton(button);
			}
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