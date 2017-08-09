#include <swin/SWin.h>
#include <stdio.h>

#define GL_VERSION 0x1F02
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_TRIANGLES 0x0004

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

void buttonCallback(STextField* textField) {
    printf("%s\n", swGetTextFromTextField(textField));
}

int main(int argc, const char * argv[]) {
    swInit();
    
    SWindow* window = swCreateWindow(1000, 620, "UI Test");
    SView* rootView = swGetRootView(window);
    
	SOpenGLContextAttribs attribs;
	attribs.major = 2;
	attribs.minor = 1;
	attribs.debug = 0;
	attribs.swapInterval = 1;

    SOpenGLView* glView = swCreateOpenGLView(rootView, swMakeRect(390, 10, 600, 600), &attribs);
    
	STextField* textField = swCreateTextField(rootView, swMakeRect(10, 580, 285, 30), "text");

    SButton* button = swCreateButton(rootView, swMakeRect(305, 580, 75, 30), "Submit", &buttonCallback, textField);
    
    SLabel* label = swCreateLabel(rootView, swMakeRect(50, 200, 100, 100), "Hello, world!");
	
    double startTime = swGetTime();
    
    swMakeContextCurrent(glView);

	__glClear = swGetProcAddress("glClear");
	__glClearColor = swGetProcAddress("glClearColor");
	__glViewport = swGetProcAddress("glViewport");
	__glColor3f = swGetProcAddress("glColor3f");
	__glBegin = swGetProcAddress("glBegin");
	__glVertex2f = swGetProcAddress("glVertex2f");
	__glEnd = swGetProcAddress("glEnd");
	__glGetString = swGetProcAddress("glGetString");
	

	printf("Version: %s\n", __glGetString(GL_VERSION));

    uint32_t frames = 0;
    uint32_t fps = 0;

	__glViewport(0, 0, 600, 600);

    __glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    
    while (!swCloseRequested(window)) {
        swPollEvents();
        
        double currentTime = swGetTime();
        
        frames++;
        if(currentTime - startTime >= 1) {
            startTime = currentTime;
            fps = frames;
            frames = 0;
            printf("%d\n", fps);
        }
        
		__glClear(GL_COLOR_BUFFER_BIT);
        
        __glBegin(GL_TRIANGLES);
		__glColor3f(1.0f, 0.0f, 0.0f);
		__glVertex2f(-0.9f, -0.9f);
		__glColor3f(0.0f, 1.0f, 0.0f);
        __glVertex2f( 0.0f,  0.9f);
		__glColor3f(0.0f, 0.0f, 1.0f);
        __glVertex2f( 0.9f, -0.9f);
        __glEnd();
        
        swSwapBufers(glView);
		swDraw(window);
    }
    
    swCloseWindow(window);
    
    return 0;
}
