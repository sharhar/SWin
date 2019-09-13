#include <swin/SWin_GTK.h>
#include <unistd.h>
#include <time.h>

Display* __sWin_X11_display;
int __sWin_X11_screen;

SThread* __sWin_GTK_gtk_main_thread;

void* __sWin_GTK_start(void* data) {
	gtk_main();
}

void swInit() {
	XInitThreads();
	int argc = 0;
	gtk_init(&argc, NULL);

	__sWin_GTK_gtk_main_thread = swCreateThread(__sWin_GTK_start, NULL);

	__sWin_X11_display = gdk_x11_get_default_xdisplay();
	__sWin_X11_screen = gdk_x11_get_default_screen();

	__sWin_X11_startTime = _swGetRawTime();
}

void swPopup(char* title, char* text) {

}

void swTerminate() {
	gtk_main_quit();
	swWaitForThread(__sWin_GTK_gtk_main_thread);
	swDestroyThread(__sWin_GTK_gtk_main_thread);
}