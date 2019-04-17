#ifndef SWin_h
#define SWin_h

#include <stdint.h>

typedef void SWindow;
typedef void SView;
typedef void SOpenGLContext;
typedef void SButton;
typedef void SLabel;
typedef void STextField;
typedef void SThread;

typedef struct SRect {
    float x, y, width, height;
} SRect;

typedef struct SMouseState {
	float x, y;
	uint8_t ldown;
	int32_t scroll;
} SMouseState;

typedef struct SOpenGLContextAttribs {
	int major;
	int minor;
	int debug;
	int swapInterval;
} SOpenGLContextAttribs;

typedef int32_t(*pfnSThreadCallback)(void* data);

void swInit();
void swInitGL();

SWindow* swCreateWindow(int width, int height, const char* title);

void swPollEvents();
void swDraw(SWindow* window);
uint8_t swCloseRequested(SWindow* swin);

void swCloseWindow(SWindow* window);

SView* swGetRootView(SWindow* window);

SView* swCreateView(SView* parent, SRect* bounds);

//SOpenGLView* swCreateOpenGLView(SView* parent, SRect* bounds, SOpenGLContextAttribs* attribs);
SOpenGLContext* swCreateOpenGLContext(SView* view, SOpenGLContextAttribs* attribs);
void swMakeContextCurrent(SOpenGLContext* context);
void swSwapBufers(SOpenGLContext* context);

void* swGetProcAddress(const char* name);

SButton* swCreateButton(SView* parent, SRect* bounds, const char* title, void* callback, void* userData);

SLabel* swCreateLabel(SView* parent, SRect* bounds, const char* text);

STextField* swCreateTextField(SView* parent, SRect* bounds, const char* text);

char* swGetTextFromTextField(STextField* textField);

void swPopup(char* title, char* text);

double swGetTime();
void swSleep(uint32_t milliSeconds);

SRect* swMakeRect(float x, float y, float w, float h);

void swTerminate();

SMouseState* swGetMouseState(SWindow* window);

SThread* swCreateThread(pfnSThreadCallback callback, void* data);
void swWaitForThread(SThread* thread);
void swDeleteThread(SThread* thread);

#endif