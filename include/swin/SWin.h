#ifndef SWin_h
#define SWin_h

#include <swin/SWin_Base.h>

#define CHECK_RECT(rect) if(rect->dispose) { DEALLOC(rect); }

#define 	SWIN_KEY_UNKNOWN   -1
#define 	SWIN_KEY_SPACE   32
#define 	SWIN_KEY_APOSTROPHE   39 /* ' */
#define 	SWIN_KEY_COMMA   44 /* , */
#define 	SWIN_KEY_MINUS   45 /* - */
#define 	SWIN_KEY_PERIOD   46 /* . */
#define 	SWIN_KEY_SLASH   47 /* / */
#define 	SWIN_KEY_0   48
#define 	SWIN_KEY_1   49
#define 	SWIN_KEY_2   50
#define 	SWIN_KEY_3   51
#define 	SWIN_KEY_4   52
#define 	SWIN_KEY_5   53
#define 	SWIN_KEY_6   54
#define 	SWIN_KEY_7   55
#define 	SWIN_KEY_8   56
#define 	SWIN_KEY_9   57
#define 	SWIN_KEY_SEMICOLON   59 /* ; */
#define 	SWIN_KEY_EQUAL   61 /* = */
#define 	SWIN_KEY_A   65
#define 	SWIN_KEY_B   66
#define 	SWIN_KEY_C   67
#define 	SWIN_KEY_D   68
#define 	SWIN_KEY_E   69
#define 	SWIN_KEY_F   70
#define 	SWIN_KEY_G   71
#define 	SWIN_KEY_H   72
#define 	SWIN_KEY_I   73
#define 	SWIN_KEY_J   74
#define 	SWIN_KEY_K   75
#define 	SWIN_KEY_L   76
#define 	SWIN_KEY_M   77
#define 	SWIN_KEY_N   78
#define 	SWIN_KEY_O   79
#define 	SWIN_KEY_P   80
#define 	SWIN_KEY_Q   81
#define 	SWIN_KEY_R   82
#define 	SWIN_KEY_S   83
#define 	SWIN_KEY_T   84
#define 	SWIN_KEY_U   85
#define 	SWIN_KEY_V   86
#define 	SWIN_KEY_W   87
#define 	SWIN_KEY_X   88
#define 	SWIN_KEY_Y   89
#define 	SWIN_KEY_Z   90
#define 	SWIN_KEY_LEFT_BRACKET   91 /* [ */
#define 	SWIN_KEY_BACKSLASH   92 /* \ */
#define 	SWIN_KEY_RIGHT_BRACKET   93 /* ] */
#define 	SWIN_KEY_GRAVE_ACCENT   96 /* ` */
#define 	SWIN_KEY_WORLD_1   161 /* non-US #1 */
#define 	SWIN_KEY_WORLD_2   162 /* non-US #2 */
#define 	SWIN_KEY_ESCAPE   256
#define 	SWIN_KEY_ENTER   257
#define 	SWIN_KEY_TAB   258
#define 	SWIN_KEY_BACKSPACE   259
#define 	SWIN_KEY_INSERT   260
#define 	SWIN_KEY_DELETE   261
#define 	SWIN_KEY_RIGHT   262
#define 	SWIN_KEY_LEFT   263
#define 	SWIN_KEY_DOWN   264
#define 	SWIN_KEY_UP   265
#define 	SWIN_KEY_PAGE_UP   266
#define 	SWIN_KEY_PAGE_DOWN   267
#define 	SWIN_KEY_HOME   268
#define 	SWIN_KEY_END   269
#define 	SWIN_KEY_CAPS_LOCK   280
#define 	SWIN_KEY_SCROLL_LOCK   281
#define 	SWIN_KEY_NUM_LOCK   282
#define 	SWIN_KEY_PRINT_SCREEN   283
#define 	SWIN_KEY_PAUSE   284
#define 	SWIN_KEY_F1   290
#define 	SWIN_KEY_F2   291
#define 	SWIN_KEY_F3   292
#define 	SWIN_KEY_F4   293
#define 	SWIN_KEY_F5   294
#define 	SWIN_KEY_F6   295
#define 	SWIN_KEY_F7   296
#define 	SWIN_KEY_F8   297
#define 	SWIN_KEY_F9   298
#define 	SWIN_KEY_F10   299
#define 	SWIN_KEY_F11   300
#define 	SWIN_KEY_F12   301
#define 	SWIN_KEY_F13   302
#define 	SWIN_KEY_F14   303
#define 	SWIN_KEY_F15   304
#define 	SWIN_KEY_F16   305
#define 	SWIN_KEY_F17   306
#define 	SWIN_KEY_F18   307
#define 	SWIN_KEY_F19   308
#define 	SWIN_KEY_F20   309
#define 	SWIN_KEY_F21   310
#define 	SWIN_KEY_F22   311
#define 	SWIN_KEY_F23   312
#define 	SWIN_KEY_F24   313
#define 	SWIN_KEY_F25   314
#define 	SWIN_KEY_KP_0   320
#define 	SWIN_KEY_KP_1   321
#define 	SWIN_KEY_KP_2   322
#define 	SWIN_KEY_KP_3   323
#define 	SWIN_KEY_KP_4   324
#define 	SWIN_KEY_KP_5   325
#define 	SWIN_KEY_KP_6   326
#define 	SWIN_KEY_KP_7   327
#define 	SWIN_KEY_KP_8   328
#define 	SWIN_KEY_KP_9   329
#define 	SWIN_KEY_KP_DECIMAL   330
#define 	SWIN_KEY_KP_DIVIDE   331
#define 	SWIN_KEY_KP_MULTIPLY   332
#define 	SWIN_KEY_KP_SUBTRACT   333
#define 	SWIN_KEY_KP_ADD   334
#define 	SWIN_KEY_KP_ENTER   335
#define 	SWIN_KEY_KP_EQUAL   336
#define 	SWIN_KEY_LEFT_SHIFT   340
#define 	SWIN_KEY_LEFT_CONTROL   341
#define 	SWIN_KEY_LEFT_ALT   342
#define 	SWIN_KEY_LEFT_SUPER   343
#define 	SWIN_KEY_RIGHT_SHIFT   344
#define 	SWIN_KEY_RIGHT_CONTROL   345
#define 	SWIN_KEY_RIGHT_ALT   346
#define 	SWIN_KEY_RIGHT_SUPER   347
#define 	SWIN_KEY_MENU   348

#define 	SWIN_MOUSE_BUTTON_LEFT   0
#define 	SWIN_MOUSE_BUTTON_RIGHT   1
#define 	SWIN_MOUSE_BUTTON_MIDDLE   2

typedef void SWindow;
typedef void SView;
typedef void SOpenGLContext;
typedef void SButton;
typedef void SLabel;
typedef void STextField;
typedef void SThread;
typedef void SMutex;
typedef short int SKeyCode;

typedef struct SRect {
    float x, y, width, height;
	SBool dispose;
} SRect;

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
typedef void (*pfnSButtonCallback)(void*);

typedef void (*pfnSKeyDownCallback)(SKeyCode);
typedef void (*pfnSKeyUpCallback)(SKeyCode);

typedef void (*pfnSMouseMovedCallback)(float xpos, float ypos);
typedef void (*pfnSMouseDownCallback)(float xpos, float ypos, int button);
typedef void (*pfnSMouseUpCallback)(float xpos, float ypos, int button);
typedef void (*pfnSMouseScrollCallback)(float xpos, float ypos, float scroll);

SResult swInit();
SResult swInitGL();
SResult swInitVK();

SWindow* swCreateWindow(int width, int height, const char* title);

void swPollEvents();
SResult swDraw(SWindow* window);
uint8_t swCloseRequested(SWindow* window);

SResult swSetKeyDownCallback(SView* view, pfnSKeyDownCallback callback);
SResult swSetKeyUpCallback(SView* view, pfnSKeyUpCallback callback);

SResult swSetMouseMovedCallback(SView* view, pfnSMouseMovedCallback callback);
SResult swSetMouseDownCallback(SView* view, pfnSMouseDownCallback callback);
SResult swSetMouseUpCallback(SView* view, pfnSMouseUpCallback callback);
SResult swSetMouseScrollCallback(SView* view, pfnSMouseScrollCallback callback);

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

SButton* swCreateButton(SView* parent, SRect* bounds, const char* title, pfnSButtonCallback callback, void* userData);

SLabel* swCreateLabel(SView* parent, SRect* bounds, const char* text);

STextField* swCreateTextField(SView* parent, SRect* bounds, const char* text);

char* swGetTextFromTextField(STextField* textField);

void swPopup(SWindow* window, char* title, char* text);

double swGetTime();
void swSleep(uint32_t milliSeconds);

SRect* swMakeRect(float x, float y, float w, float h);
SRect* swMakeDisposableRect(float x, float y, float w, float h);

void swTerminate();

SThread* swCreateThread(pfnSThreadCallback callback, void* data);
SResult swWaitForThread(SThread* thread);
SResult swDestroyThread(SThread* thread);

SMutex* swCreateMutex();
SResult swLockMutex(SMutex* mutex);
SResult swUnlockMutex(SMutex* mutex);
SResult swDestroyMutex(SMutex* mutex);

#endif
