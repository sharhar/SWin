#include <swin/SWin.h>
#include <swin/SWin_Win32.h>

#define WGL_CONTEXT_MAJOR_VERSION_ARB               0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB               0x2092
#define WGL_CONTEXT_FLAGS_ARB                       0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB                0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB            0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB   0x00000002
#define WGL_CONTEXT_DEBUG_BIT_ARB                   0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB      0x0002

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXT)(HDC hdc);
typedef BOOL(WINAPI* PFNWGLMAKECURRENT)(HDC hdc, HGLRC hglrc);
typedef PROC(WINAPI* PFNWGLGETPROCADDRESS)(LPCSTR lpszProc);
typedef BOOL(WINAPI* PFNWGLDELETECONTEXT)(HGLRC hglrc);

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);

HMODULE __sWin_Win32_libGL;

PFNWGLCREATECONTEXT __sWin_Win32_wglCreateContext;
PFNWGLMAKECURRENT __sWin_Win32_wglMakecurrent;
PFNWGLGETPROCADDRESS __sWin_Win32_wglGetProcAddress;
PFNWGLDELETECONTEXT  __sWin_Win32_wglDeleteContext;

PFNWGLCREATECONTEXTATTRIBSARBPROC __sWin_Win32_wglCreateContextAttribsARB;
PFNWGLSWAPINTERVALEXTPROC __sWin_Win32_wglSwapIntervalEXT;

SResult swInitGL() {
	__sWin_Win32_libGL = LoadLibraryW(L"opengl32.dll");
	CHECK(__sWin_Win32_libGL, "Failed to load opengl32.dll", SWIN_FAILED);

	__sWin_Win32_wglCreateContext = GetProcAddress(__sWin_Win32_libGL, "wglCreateContext");
	CHECK(__sWin_Win32_wglCreateContext, "Failed to load wglCreateContext", SWIN_FAILED);

	__sWin_Win32_wglMakecurrent = GetProcAddress(__sWin_Win32_libGL, "wglMakeCurrent");
	CHECK(__sWin_Win32_wglMakecurrent, "Failed to load wglMakeCurrent", SWIN_FAILED);

	__sWin_Win32_wglGetProcAddress = GetProcAddress(__sWin_Win32_libGL, "wglGetProcAddress");
	CHECK(__sWin_Win32_wglGetProcAddress, "Failed to load wglGetProcAddress", SWIN_FAILED);

	__sWin_Win32_wglDeleteContext = GetProcAddress(__sWin_Win32_libGL, "wglDeleteContext");
	CHECK(__sWin_Win32_wglDeleteContext, "Failed to load wglDeleteContext", SWIN_FAILED);

	return SWIN_OK;
}

SOpenGLContext* swCreateOpenGLContext(SView* view, SOpenGLContextAttribs* attribs) {
	CHECK(view, "view was NULL", NULL);
	CHECK(attribs, "attribs was NULL", NULL);

	SWin_Win32_View* _view = (SWin_Win32_View*)view;

	SWin_Win32_OpenGLContext* result = ALLOC_S(SWin_Win32_OpenGLContext);
	CHECK(result, "Failed to allocate SWin_Win32_OpenGLContext", NULL);

	result->hDc = GetDC(_view->hWnd);

	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;

	int pf = ChoosePixelFormat(result->hDc, &pfd);

	CHECK(pf, "ChoosePixelFormat() failed", NULL);

	CHECK_F(SetPixelFormat(result->hDc, pf, &pfd), "SetPixelFormat() failed", NULL);

	result->hRc = __sWin_Win32_wglCreateContext(result->hDc);
	CHECK(result->hRc, "wglCreateContext failed", NULL);

	__sWin_Win32_wglMakecurrent(result->hDc, result->hRc);

	__sWin_Win32_wglCreateContextAttribsARB = swGetProcAddressGL("wglCreateContextAttribsARB");
	CHECK(__sWin_Win32_wglCreateContextAttribsARB, "Failed to load wglCreateContextAttribsARB", NULL);

	__sWin_Win32_wglSwapIntervalEXT = swGetProcAddressGL("wglSwapIntervalEXT");
	CHECK(__sWin_Win32_wglSwapIntervalEXT, "Failed to load wglSwapIntervalEXT", NULL);

	__sWin_Win32_wglMakecurrent(result->hDc, NULL);

	__sWin_Win32_wglDeleteContext(result->hRc);

	int iContextAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, attribs->major,
		WGL_CONTEXT_MINOR_VERSION_ARB, attribs->minor,
		WGL_CONTEXT_PROFILE_MASK_ARB, attribs->profile == SWIN_OPENGL_CONTEXT_PROFILE_CORE ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		WGL_CONTEXT_FLAGS_ARB, (attribs->debug ? WGL_CONTEXT_DEBUG_BIT_ARB : 0) & (attribs->forwardCompat ? WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB : 0),
		0
	};

	result->hRc = __sWin_Win32_wglCreateContextAttribsARB(result->hDc, 0, iContextAttribs);
	CHECK(result->hRc, "wglCreateContextAttribsARB failed", NULL);

	__sWin_Win32_wglMakecurrent(result->hDc, result->hRc);

	__sWin_Win32_wglSwapIntervalEXT(attribs->swapInterval);

	return result;
}

SResult swMakeContextCurrent(SOpenGLContext* context) {
	CHECK(context, "context was NULL", SWIN_FAILED);

	SWin_Win32_OpenGLContext* _context = (SWin_Win32_OpenGLContext*)context;

	__sWin_Win32_wglMakecurrent(_context->hDc, _context->hRc);

	return SWIN_OK;
}

SResult swSwapBufers(SOpenGLContext* context) {
	CHECK(context, "context was NULL", SWIN_FAILED);

	SWin_Win32_OpenGLContext* _context = (SWin_Win32_OpenGLContext*)context;

	SwapBuffers(_context->hDc);

	return SWIN_OK;
}

void* swGetProcAddressGL(const char* name) {
	CHECK(name, "name was NULL", SWIN_FAILED);

	void* result = __sWin_Win32_wglGetProcAddress(name);

	return result == NULL ? GetProcAddress(__sWin_Win32_libGL, name) : result;
}

SResult swDestroyOpenGLContext(SOpenGLContext* context) {
	CHECK(context, "context was NULL", SWIN_FAILED);

	SWin_Win32_OpenGLContext* _context = (SWin_Win32_OpenGLContext*)context;

	__sWin_Win32_wglDeleteContext(_context->hRc);

	DEALLOC(_context);
}
