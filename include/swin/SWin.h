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
	float scroll;
} SMouseState;

typedef enum SOpenGLContextProfile {
	SWIN_OPENGL_CONTEXT_PROFILE_CORE = 1,
	SWIN_OPENGL_CONTEXT_PROFILE_COMPATIBILITY = 2
}SOpenGLContextProfile;

typedef struct SOpenGLContextAttribs {
	int major;
	int minor;
	int debug;
	int forwardCompat;
	SOpenGLContextProfile profile;
	int swapInterval;
} SOpenGLContextAttribs;

typedef struct SColor {
    float r, g, b, a;
} SColor;

typedef int32_t(*pfnSThreadCallback)(void* data);

void swInit();
void swInitGL();
void swInitVK();

SWindow* swCreateWindow(int width, int height, const char* title);

void swPollEvents();
void swDraw(SWindow* window);
uint8_t swCloseRequested(SWindow* window);

void swCloseWindow(SWindow* window);

SView* swGetRootView(SWindow* window);

SView* swCreateView(SView* parent, SRect* bounds);

void swSetViewBackgroundColor(SView* view, SColor color);

SOpenGLContext* swCreateOpenGLContext(SView* view, SOpenGLContextAttribs* attribs);
void swMakeContextCurrent(SOpenGLContext* context);
void swSwapBufers(SOpenGLContext* context);
void* swGetProcAddressGL(const char* name);
void swDestroyOpenGLContext(SOpenGLContext* context);

void* swGetProcAddressVK(void* instance, const char* name);
char** swGetRequiredExtensionsVK(uint32_t* count);
int32_t swCreateWindowSurfaceVK(void* instance, SView* view, void* allocator, void* surface);

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