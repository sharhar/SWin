#include <swin/SWin.h>
#include <stdio.h>
#include <math.h>

#include "vulkan_test.h"

#define GL_VERSION 0x1F02
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_TRIANGLES 0x0004

#define TWO_PI_OVER_THREE 2.09439510239

#ifdef WIN32
#define pfnglprefunc __stdcall *
#else
#define pfnglprefunc *
#endif

typedef void (pfnglprefunc PFNGLCLEARPROC)(unsigned int mask);
PFNGLCLEARPROC __glClear;
typedef void (pfnglprefunc PFNGLCLEARCOLORPROC)(float red, float green, float blue, float alpha);
PFNGLCLEARCOLORPROC __glClearColor;
typedef void (pfnglprefunc PFNGLCOLOR3FPROC)(float red, float green, float blue);
PFNGLCOLOR3FPROC __glColor3f;
typedef void (pfnglprefunc PFNGLBEGINPROC)(unsigned int mode);
PFNGLBEGINPROC __glBegin;
typedef void (pfnglprefunc PFNGLVIEWPORTPROC)(int x, int y, int width, int height);
PFNGLVIEWPORTPROC __glViewport;
typedef void (pfnglprefunc PFNGLENDPROC)();
PFNGLENDPROC __glEnd;
typedef void (pfnglprefunc PFNGLVERTEX2FPROC)(float x, float y);
PFNGLVERTEX2FPROC __glVertex2f;
typedef const char* (pfnglprefunc PFNGLGETSTRINGPROC)(unsigned int name);
PFNGLGETSTRINGPROC __glGetString;
typedef void (pfnglprefunc PFNGLROTATEFPROC)( float angle, float x, float y, float z );
PFNGLROTATEFPROC __glRotatef;

void buttonCallback(STextField* textField) {
	printf("%s\n", swGetTextFromTextField(textField));
}

uint8_t running = 1;
uint8_t fence = 0;
SMutex* mutex = NULL;

typedef struct RenderInfo {
	SView* view;
	float rotDir;
	float r, g, b;
	int ID;
} RenderInfo;

void renderFunction(RenderInfo* info) {
	SOpenGLContextAttribs attribs;
	attribs.major = 2;
	attribs.minor = 1;
	attribs.debug = 1;
	attribs.swapInterval = 0;
	attribs.forwardCompat = 1;
	attribs.profile = SWIN_OPENGL_CONTEXT_PROFILE_COMPATIBILITY;

	swLockMutex(mutex);

	swSleep(250);

	SOpenGLContext* context = swCreateOpenGLContext(info->view, &attribs);

	swMakeContextCurrent(context);

	printf("GL Version: %s\n", __glGetString(GL_VERSION));

	swUnlockMutex(mutex);

	__glViewport(0, 0, 300, 300);

	__glClearColor(info->r, info->g, info->b, 1.0f);

	while(running) {
		__glClear(GL_COLOR_BUFFER_BIT);

		__glRotatef(info->rotDir, 0, 0, 1);

		__glBegin(GL_TRIANGLES);
		__glColor3f(1.0f, 0.0f, 0.0f);
		__glVertex2f(-0.69f, -0.4f);
		__glColor3f(0.0f, 1.0f, 0.0f);
		__glVertex2f( 0.0f,  0.8f);
		__glColor3f(0.0f, 0.0f, 1.0f);
		__glVertex2f( 0.69f, -0.4f);
		__glEnd();
        
        swSleep(5);

		swSwapBufers(context);
	}

	swDestroyOpenGLContext(context);
}

int main(int argc, const char * argv[]) {
    swInit();
	swInitGL();

    SWindow* window = swCreateWindow(1000, 630, "UI Test");

    SColor winBG = {1,1,1,1};

    SView* rootView = swGetRootView(window);

    swSetViewBackgroundColor(rootView, winBG);
    
	SView* glView1 = swCreateView(rootView, swMakeDisposableRect(380, 10, 300, 300));
	SView* glView2 = swCreateView(rootView, swMakeDisposableRect(690, 10, 300, 300));
	SView* glView3 = swCreateView(rootView, swMakeDisposableRect(380, 320, 300, 300));
	SView* glView4 = swCreateView(rootView, swMakeDisposableRect(690, 320, 300, 300));

	STextField* textField = swCreateTextField(rootView, swMakeDisposableRect(10, 580, 275, 30), "text");

	SLabel* label = swCreateLabel(rootView, swMakeDisposableRect(10, 390, 100, 100), "Hello, world!");

	SButton* button = swCreateButton(rootView, swMakeDisposableRect(295, 580, 75, 30), "Submit", &buttonCallback, textField);

	SView* vkView = swCreateView(rootView, swMakeDisposableRect(10, 10, 360, 360));

	__glClear = swGetProcAddressGL("glClear");
	__glClearColor = swGetProcAddressGL("glClearColor");
	__glViewport = swGetProcAddressGL("glViewport");
	__glColor3f = swGetProcAddressGL("glColor3f");
	__glBegin = swGetProcAddressGL("glBegin");
	__glVertex2f = swGetProcAddressGL("glVertex2f");
	__glEnd = swGetProcAddressGL("glEnd");
	__glGetString = swGetProcAddressGL("glGetString");
	__glRotatef = swGetProcAddressGL("glRotatef");

	
	running = 1;

	RenderInfo renderInfos[4];
	renderInfos[0].view = glView1;
	renderInfos[0].rotDir = 1;
	renderInfos[0].r = 0;
	renderInfos[0].g = 0;
	renderInfos[0].b = 0;
	renderInfos[0].ID = 0;

	renderInfos[1].view = glView2;
	renderInfos[1].rotDir = -1;
	renderInfos[1].r = 1;
	renderInfos[1].g = 1;
	renderInfos[1].b = 1;
	renderInfos[1].ID = 1;

	renderInfos[2].view = glView3;
	renderInfos[2].rotDir = -1;
	renderInfos[2].r = 1;
	renderInfos[2].g = 1;
	renderInfos[2].b = 1;
	renderInfos[2].ID = 2;

	renderInfos[3].view = glView4;
	renderInfos[3].rotDir = 1;
	renderInfos[3].r = 0;
	renderInfos[3].g = 0;
	renderInfos[3].b = 0;
	renderInfos[3].ID = 3;

	mutex = swCreateMutex();

	SThread* thread1 = swCreateThread(renderFunction, renderInfos);
	SThread* thread2 = swCreateThread(renderFunction, renderInfos+1);
	SThread* thread3 = swCreateThread(renderFunction, renderInfos+2);
	SThread* thread4 = swCreateThread(renderFunction, renderInfos+3);

    uint32_t frames = 0;
    uint32_t fps = 0;

	Swin_Vk* vkContext = initVk(vkView);

	double startTime = swGetTime();
	double currentTime = 0;

    while (!swCloseRequested(window)) {
		currentTime = swGetTime();
		
        frames++;
        if(currentTime - startTime >= 1) {
            startTime = currentTime;
            fps = frames;
            frames = 0;
            printf("%d\n", fps);
        }

        swPollEvents();
		
		swSleep(5);

		renderVk(vkContext, (cos(swGetTime())+1.0)/2.0, (cos(swGetTime()+TWO_PI_OVER_THREE)+1.0)/2.0, (cos(swGetTime()+2*TWO_PI_OVER_THREE)+1.0)/2.0);

		swDraw(window);
    }

	running = 0;

    swWaitForThread(thread1);
	swWaitForThread(thread2);
	swWaitForThread(thread3);
	swWaitForThread(thread4);

	swDestroyThread(thread1);
	swDestroyThread(thread2);
	swDestroyThread(thread3);
	swDestroyThread(thread4);

	swDestroyMutex(mutex);

	swDestroyWindow(window);
    
    return 0;
}

