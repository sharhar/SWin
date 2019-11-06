#include <swin/SWin_GTK.h>

void __sWin_GTK_destroyWindowCallback(GtkWidget *widget, gpointer data) {
	SWin_GTK_Window* window = (SWin_GTK_Window*)data;
	window->closeRequested = 1;
}

SWindow* swCreateWindow(int width, int height, const char* title) {
    SWin_GTK_Window* result = (SWin_GTK_Window*)malloc(sizeof(SWin_GTK_Window));

	result->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(result->window), title);
	g_signal_connect(result->window, "destroy", G_CALLBACK(__sWin_GTK_destroyWindowCallback), (gpointer)result);
	gtk_widget_set_size_request(GTK_WIDGET(result->window), width, height);
	gtk_widget_show(GTK_WIDGET(result->window));

	result->view.win = result;

	result->view.container = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(result->window), result->view.container);
	gtk_widget_show(GTK_WIDGET(result->view.container));

	result->view.baseView.xWindow = gdk_x11_window_get_xid(gtk_widget_get_window(result->window));

	result->closeRequested = 0;
	result->view.height = height;

    return (SWindow*)result;
}

void swSetViewBackgroundColor(SView* view, SColor color) {
    //((SWin_GTK_View*)view)->bg.r = color.r;
    //((SWin_GTK_View*)view)->bg.g = color.g;
    //((SWin_GTK_View*)view)->bg.b = color.b;
    //((SWin_GTK_View*)view)->bg.a = color.a;
}

void swDraw(SWindow* window) {

}

uint8_t swCloseRequested(SWindow* window) {
    return ((SWin_GTK_Window*)window)->closeRequested;
}

void swDestroyWindow(SWindow* window) {

}

SView* swGetRootView(SWindow* window) {
    return window;
}

void _swFillView(SWin_GTK_View* pView, SView* parent, SRect* bounds) {
	SWin_GTK_View* view = (SWin_GTK_View*)parent;

	pView->win = view->win;
	pView->height = bounds->height;

	pView->frame = gtk_frame_new("");
	gtk_fixed_put (GTK_FIXED(view->container), pView->frame, bounds->x, view->height - bounds->height - bounds->y-18); //There is some weird issue with frames where they are places 18 pixels too far up (I will try to figure out why this happens later)
	gtk_widget_set_size_request(pView->frame, bounds->width, bounds->height);
	gtk_frame_set_shadow_type(pView->frame, GTK_SHADOW_NONE);
	gtk_widget_show(GTK_WIDGET(pView->frame));

	pView->container = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(pView->frame), pView->container);
	gtk_widget_show(GTK_WIDGET(pView->container));

	pView->area = gtk_drawing_area_new();
	gtk_fixed_put (GTK_FIXED(pView->container), pView->area, 0, 0);
	gtk_widget_set_size_request(pView->area, bounds->width, bounds->height);
	gtk_widget_show(GTK_WIDGET(pView->area));

	pView->baseView.xWindow = gdk_x11_window_get_xid(gtk_widget_get_window(pView->area));
}

SView* swCreateView(SView* parent, SRect* bounds) {
	SWin_GTK_View* result = (SWin_GTK_View*)malloc(sizeof(SWin_GTK_View));

	_swFillView(result, parent, bounds);

    return result;
}

void __sWin_GTK_clickedButtonCallback(GtkWidget *widget, gpointer data) {
	void** datas = (void**)data;
	pfnSButtonCallback callback = (pfnSButtonCallback)datas[0];
	callback(datas[1]);
}

SButton* swCreateButton(SView* parent, SRect* bounds, const char* title, pfnSButtonCallback callback, void* userData) {
	SWin_GTK_View* view = (SWin_GTK_View*)parent;

	SWin_GTK_Button* result = (SWin_GTK_Button*)malloc(sizeof(SWin_GTK_Button));

	void** datas = malloc(sizeof(void*) * 2);
	datas[0] = callback;
	datas[1] = userData;

	result->view.area = gtk_button_new_with_label(title);
	gtk_fixed_put(GTK_FIXED(view->container), result->view.area, bounds->x, view->height - bounds->height - bounds->y);
	gtk_widget_set_size_request(result->view.area, bounds->width, bounds->height);
	g_signal_connect(result->view.area, "clicked", G_CALLBACK(__sWin_GTK_clickedButtonCallback), (gpointer)datas);
	gtk_widget_show(GTK_WIDGET(result->view.area));

	return result;
}

SLabel* swCreateLabel(SView* parent, SRect* bounds, const char* text) {
	SWin_GTK_View* result = (SWin_GTK_View*)malloc(sizeof(SWin_GTK_View));

	_swFillView(result, parent, bounds);

	return result;
}

STextField* swCreateTextField(SView* parent, SRect* bounds, const char* text) {
	SWin_GTK_View* result = (SWin_GTK_View*)malloc(sizeof(SWin_GTK_View));

	_swFillView(result, parent, bounds);

	return result;
}

char* swGetTextFromTextField(STextField* textField) {
	return "[placeholder]";
}

void swPollEvents() {

}