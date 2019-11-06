#include <swin/SWin_X11_Base.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <dlfcn.h>
#include <malloc.h>

void* __sWin_GLX_libGL;

typedef void (*PFNGLXSWAPBUFFERSPROC)(Display *dpy, GLXDrawable drawable);
typedef void (*PFNGLXDESTROYCONTEXTPROC)(Display *dpy, GLXContext ctx);

PFNGLXGETPROCADDRESSPROC __sWin_GLX_glxGetProcAddressARB;
PFNGLXCHOOSEFBCONFIGPROC __sWin_GLX_glXChooseFBConfig;
PFNGLXCREATENEWCONTEXTPROC __sWin_GLX_glXCreateNewContext;
PFNGLXCREATEWINDOWPROC __sWin_GLX_glXCreateWindow;
PFNGLXDESTROYWINDOWPROC __sWin_GLX_glXDestroyWindow;
PFNGLXMAKECONTEXTCURRENTPROC __sWin_GLX_glXMakeContextCurrent;
PFNGLXCREATECONTEXTATTRIBSARBPROC __sWin_GLX_glXCreateContextAttribsARB;

PFNGLXSWAPBUFFERSPROC __sWin_GLX_glXSwapBuffers;
PFNGLXDESTROYCONTEXTPROC __sWin_GLX_glXDestroyContext;

typedef struct SWin_GLX_OpenGLContext {
	GLXContext context;
	GLXWindow window;
	GLXDrawable drawable;
} SWin_GLX_OpenGLContext;

void swInitGL() {
	__sWin_GLX_libGL = dlopen("libGL.so.1", RTLD_NOW | RTLD_GLOBAL);
    if(__sWin_GLX_libGL == NULL) {
        printf("Error loading libGL.so.1, trying libGL.so\n");
	    __sWin_GLX_libGL = dlopen("libGL.so", RTLD_NOW | RTLD_GLOBAL);
        if(__sWin_GLX_libGL == NULL) {
            printf("Error loading libGL.so too! Failed Initialization...");
            return;
        }
    }

	__sWin_GLX_glxGetProcAddressARB = (PFNGLXGETPROCADDRESSPROC)dlsym(__sWin_GLX_libGL, "glXGetProcAddressARB");
	__sWin_GLX_glXChooseFBConfig = __sWin_GLX_glxGetProcAddressARB("glXChooseFBConfig");
	__sWin_GLX_glXCreateNewContext = __sWin_GLX_glxGetProcAddressARB("glXCreateNewContext");
	__sWin_GLX_glXCreateWindow = __sWin_GLX_glxGetProcAddressARB("glXCreateWindow");
	__sWin_GLX_glXDestroyWindow = __sWin_GLX_glxGetProcAddressARB("glXDestroyWindow");
	__sWin_GLX_glXMakeContextCurrent = __sWin_GLX_glxGetProcAddressARB("glXMakeContextCurrent");
	__sWin_GLX_glXCreateContextAttribsARB = __sWin_GLX_glxGetProcAddressARB("glXCreateContextAttribsARB");
	__sWin_GLX_glXSwapBuffers = __sWin_GLX_glxGetProcAddressARB("glXSwapBuffers");
	__sWin_GLX_glXDestroyContext = __sWin_GLX_glxGetProcAddressARB("glXDestroyContext");
}

static Bool __sWin_X11_ctxErrorOccurred = False;
static int __sWin_X11_ctxErrorHandler( Display *dpy, XErrorEvent *ev ) {
	__sWin_X11_ctxErrorOccurred = True;
	return 0;
}

SOpenGLContext* swCreateOpenGLContext(SView* view, SOpenGLContextAttribs* attribs) {
	SWin_GLX_OpenGLContext* result = (SWin_GLX_OpenGLContext*)malloc(sizeof(SWin_GLX_OpenGLContext));

	int visual_attribs[] = {
			GLX_X_RENDERABLE    , True,
			GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
			GLX_RENDER_TYPE     , GLX_RGBA_BIT,
			//GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
			GLX_RED_SIZE        , 8,
			GLX_GREEN_SIZE      , 8,
			GLX_BLUE_SIZE       , 8,
			//GLX_ALPHA_SIZE    , 8,
			//GLX_DEPTH_SIZE      , 24,
			//GLX_STENCIL_SIZE    , 8,
			GLX_DOUBLEBUFFER    , True,
			//GLX_SAMPLE_BUFFERS  , 1,
			//GLX_SAMPLES         , 4,
			None
	};



	int fbcCount = 0;
	GLXFBConfig* nativeConfigs = __sWin_GLX_glXChooseFBConfig(__sWin_X11_display, __sWin_X11_screen, visual_attribs, &fbcCount);

	XLockDisplay(__sWin_X11_display);

	__sWin_X11_ctxErrorOccurred = False;
	int (*oldHandler)(Display*, XErrorEvent*) =
	XSetErrorHandler(&__sWin_X11_ctxErrorHandler);

	for(int i = 0; i < fbcCount; ++i) {
		__sWin_X11_ctxErrorOccurred = False;
		XSync(__sWin_X11_display, False);

		if (__sWin_GLX_glXCreateContextAttribsARB) {
			int mask = 0, flags = 0;

			if (attribs->forwardCompat && attribs->major >= 3) {
				flags |= GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
			}

			if(attribs->profile == SWIN_OPENGL_CONTEXT_PROFILE_CORE) {
				mask |= GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
			} else if(attribs->profile == SWIN_OPENGL_CONTEXT_PROFILE_COMPATIBILITY) {
				mask |= GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
			}

			if (attribs->debug) {
				flags |= GLX_CONTEXT_DEBUG_BIT_ARB;
			}

			int attribs_list[] = {
					GLX_CONTEXT_MAJOR_VERSION_ARB, attribs->major,
					GLX_CONTEXT_MINOR_VERSION_ARB, attribs->minor,
					GLX_CONTEXT_PROFILE_MASK_ARB, mask,
					GLX_CONTEXT_FLAGS_ARB, flags,
					None
			};

			result->context = __sWin_GLX_glXCreateContextAttribsARB(__sWin_X11_display, nativeConfigs[i], NULL, True, attribs_list);

			if (!result->context) {
				printf("glXCreateContextAttribsARB failed, using glXCreateNewContext\n");
				result->context = __sWin_GLX_glXCreateNewContext(__sWin_X11_display, nativeConfigs[i], GLX_RGBA_TYPE, NULL, True);
			}
		}
		else
		{
			result->context = __sWin_GLX_glXCreateNewContext(__sWin_X11_display, nativeConfigs[i], GLX_RGBA_TYPE, NULL, True);
		}

		result->window = __sWin_GLX_glXCreateWindow(__sWin_X11_display, nativeConfigs[i], ((SWin_X11_Base_View*)view)->xWindow, NULL);

		XSync(__sWin_X11_display, False);

		if(!__sWin_X11_ctxErrorOccurred) {
			break;
		}

		__sWin_GLX_glXDestroyContext(__sWin_X11_display, result->context);
	}

	XSetErrorHandler(oldHandler);

	if ( __sWin_X11_ctxErrorOccurred || !result->context ) {
		printf( "Failed to create an OpenGL context\n" );
		return NULL;
	}

	XUnlockDisplay(__sWin_X11_display);

	result->drawable = result->window;
	((SWin_X11_Base_View*)view)->type = SWIN_X11_VIEW_TYPE_OPENGL;

	return result;
}

void swMakeContextCurrent(SOpenGLContext* context) {
	if(context == NULL) {
		__sWin_GLX_glXMakeContextCurrent(__sWin_X11_display, NULL, NULL, NULL);
		return;
	}

	SWin_GLX_OpenGLContext* ctx = (SWin_GLX_OpenGLContext*)context;
	__sWin_GLX_glXMakeContextCurrent(__sWin_X11_display, ctx->drawable, ctx->drawable, ctx->context);
}

void swSwapBufers(SOpenGLContext* context) {
	__sWin_GLX_glXSwapBuffers(__sWin_X11_display, ((SWin_GLX_OpenGLContext*)context)->drawable);
}

void* swGetProcAddressGL(const char* name) {
    void* result = __sWin_GLX_glxGetProcAddressARB(name);
    return result == NULL ? dlsym(__sWin_GLX_libGL, name) : result;
}

void swDestroyOpenGLContext(SOpenGLContext* context) {
	__sWin_GLX_glXDestroyWindow(__sWin_X11_display, ((SWin_GLX_OpenGLContext*)context)->window);
	__sWin_GLX_glXDestroyContext(__sWin_X11_display, ((SWin_GLX_OpenGLContext*)context)->context);
}
