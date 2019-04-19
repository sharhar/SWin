#include <swin/SWin.h>
#include <swin/SWin_Win32.h>

#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_FLAGS_ARB             0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126

typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int interval);
//typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef HGLRC(WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef HGLRC(WINAPI * PFNWGLCREATECONTEXT)(HDC hdc);
typedef BOOL(WINAPI * PFNWGLMAKECURRENT)(HDC hdc, HGLRC hglrc);
typedef PROC(WINAPI * PFNWGLGETPROCADDRESS)(LPCSTR lpszProc);

HMODULE __sWin_Win32_libGL;

PFNWGLCREATECONTEXT __sWin_Win32_wglCreateContext;
PFNWGLMAKECURRENT __sWin_Win32_wglMakecurrent;
PFNWGLGETPROCADDRESS __sWin_Win32_wglGetProcAddress;
PFNWGLCREATECONTEXTATTRIBSARBPROC __sWin_Win32_wglCreateContextAttribsARB;
PFNWGLSWAPINTERVALEXTPROC __sWin_Win32_wglSwapIntervalEXT;

void swInitGL() {
	__sWin_Win32_libGL = LoadLibraryW(L"opengl32.dll");

	__sWin_Win32_wglCreateContext = GetProcAddress(__sWin_Win32_libGL, "wglCreateContext");
	__sWin_Win32_wglMakecurrent = GetProcAddress(__sWin_Win32_libGL, "wglMakeCurrent");
	__sWin_Win32_wglGetProcAddress = GetProcAddress(__sWin_Win32_libGL, "wglGetProcAddress");
}

SOpenGLContext* swCreateOpenGLContext(SView* view, SOpenGLContextAttribs* attribs) {
	HWND hWnd = (HWND)view;

	SWin_Win32_OpenGLContext* result = (SWin_Win32_OpenGLContext*)malloc(sizeof(SWin_Win32_OpenGLContext));
	
	result->hDc = GetDC(hWnd);

	PIXELFORMATDESCRIPTOR pfd;

	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;

	int pf = ChoosePixelFormat(result->hDc, &pfd);
	if (pf == 0) {
		MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return NULL;
	}

	if (SetPixelFormat(result->hDc, pf, &pfd) == FALSE) {
		MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
		return NULL;
	}

	result->hRc = __sWin_Win32_wglCreateContext(result->hDc);

	__sWin_Win32_wglMakecurrent(result->hDc, result->hRc);

	__sWin_Win32_wglCreateContextAttribsARB = swGetProcAddressGL("wglCreateContextAttribsARB");
	__sWin_Win32_wglSwapIntervalEXT = swGetProcAddressGL("wglSwapIntervalEXT");

	__sWin_Win32_wglMakecurrent(result->hDc, NULL);

	int iContextAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, attribs->major,
		WGL_CONTEXT_MINOR_VERSION_ARB, attribs->minor,
		WGL_CONTEXT_PROFILE_MASK_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, attribs->debug ? 1 : 0,
		0
	};

	result->hRc = __sWin_Win32_wglCreateContextAttribsARB(result->hDc, 0, iContextAttribs);

	__sWin_Win32_wglMakecurrent(result->hDc, result->hRc);

	__sWin_Win32_wglSwapIntervalEXT(attribs->swapInterval);

	return result;
}

void swMakeContextCurrent(SOpenGLContext* view) {
	SWin_Win32_OpenGLContext* context = (SWin_Win32_OpenGLContext*)view;
	__sWin_Win32_wglMakecurrent(context->hDc, context->hRc);
}

void swSwapBufers(SOpenGLContext* view) {
	SWin_Win32_OpenGLContext* context = (SWin_Win32_OpenGLContext*)view;
	SwapBuffers(context->hDc);
}

void* swGetProcAddressGL(const char* name) {
	void* result = __sWin_Win32_wglGetProcAddress(name);

	return result == NULL ? GetProcAddress(__sWin_Win32_libGL, name) : result;
}