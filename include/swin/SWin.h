#ifndef SWin_h
#define SWin_h

#include <swin/SWin_Base.h>

#define CHECK_RECT(rect) if(rect->dispose) { DEALLOC(rect); }

typedef void SWindow;
typedef void SView;
typedef void SOpenGLContext;
typedef void SButton;
typedef void SLabel;
typedef void STextField;
typedef void SThread;
typedef void SMutex;

typedef struct SRect {
    float x, y, width, height;
	SBool dispose;
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
	uint8_t major;
	uint8_t minor;
	uint8_t debug;
	uint8_t forwardCompat;
	uint8_t swapInterval;
	SOpenGLContextProfile profile;
} SOpenGLContextAttribs;

typedef struct SColor {
    float r, g, b, a;
} SColor;

typedef int32_t(*pfnSThreadCallback)(void* data);

SResult swInit();
SResult swInitGL();
SResult swInitVK();

SWindow* swCreateWindow(int width, int height, const char* title);

void swPollEvents();
SResult swDraw(SWindow* window);
uint8_t swCloseRequested(SWindow* window);

SResult swDestroyWindow(SWindow* window);

SView* swGetRootView(SWindow* window);

SView* swCreateView(SView* parent, SRect* bounds);

void swSetViewBackgroundColor(SView* view, SColor color);

SOpenGLContext* swCreateOpenGLContext(SView* view, SOpenGLContextAttribs* attribs);
SResult swMakeContextCurrent(SOpenGLContext* context);
SResult swSwapBufers(SOpenGLContext* context);
void* swGetProcAddressGL(const char* name);
SResult swDestroyOpenGLContext(SOpenGLContext* context);

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
SRect* swMakeDisposableRect(float x, float y, float w, float h);

void swTerminate();

SMouseState* swGetMouseState(SWindow* window);

SThread* swCreateThread(pfnSThreadCallback callback, void* data);
SResult swWaitForThread(SThread* thread);
SResult swDestroyThread(SThread* thread);

SMutex* swCreateMutex();
SResult swLockMutex(SMutex* mutex);
SResult swUnlockMutex(SMutex* mutex);
SResult swDestroyMutex(SMutex* mutex);

#endif