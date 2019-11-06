#include <swin/SWin.h>
#include <swin/SWin_Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGL/gl.h>

CFBundleRef __sWin_Cocoa_libGL;

SResult swInitGL() {
    __sWin_Cocoa_libGL = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
	
    CHECK(__sWin_Cocoa_libGL, "could not load opengl", SWIN_FAILED);
	
    return SWIN_OK;
}

SOpenGLContext* swCreateOpenGLContext(SView* parent, SOpenGLContextAttribs* attribs) {
	SWin_Cocoa_View* _parent = (SWin_Cocoa_View*)parent;
    
    NSOpenGLPixelFormatAttribute openGLversion = NSOpenGLProfileVersionLegacy;
    
    if(attribs->major == 4) {
        openGLversion = NSOpenGLProfileVersion4_1Core;
    } else if (attribs->major == 3) {
        openGLversion = NSOpenGLProfileVersion3_2Core;
    } else {
        openGLversion = NSOpenGLProfileVersionLegacy;
    }
    
    NSOpenGLPixelFormatAttribute glattribs[] = {
        NSOpenGLPFAOpenGLProfile, openGLversion,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 32,
        NSOpenGLPFAAccelerated,
        0
    };
    
    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:glattribs];
    //NSOpenGLView* view = [[NSOpenGLView alloc] initWithFrame:NSMakeRect(10, 10, 100, 100) pixelFormat:pixelFormat];
    //[rootView addSubview:view];
    
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
    [context setView:_parent->view];
    
    [context makeCurrentContext];
    
    GLint sync = attribs->swapInterval;
    [context setValues:&sync forParameter:NSOpenGLCPSwapInterval];
    
    return context;
}


SResult swMakeContextCurrent(SOpenGLContext* context) {
    CHECK(context, "context was NULL", SWIN_FAILED);
    
    NSOpenGLContext* _context = (NSOpenGLContext*)context;
    [_context makeCurrentContext];
	
    return SWIN_OK;
}

SResult swSwapBufers(SOpenGLContext* context) {
    CHECK(context, "context was NULL", SWIN_FAILED);
    
    NSOpenGLContext* _context = (NSOpenGLContext*)context;
    [_context flushBuffer];
	
    return SWIN_OK;
}

void* swGetProcAddressGL(const char* name) {
    CFStringRef symbolName = CFStringCreateWithCString(kCFAllocatorDefault, name, kCFStringEncodingASCII);
    
    void* result = CFBundleGetFunctionPointerForName(__sWin_Cocoa_libGL, symbolName);
    
    CFRelease(symbolName);
    
    return result;
}

SResult swDestroyOpenGLContext(SOpenGLContext* context) {
    CHECK(context, "context was NULL", SWIN_FAILED);
    
    return SWIN_OK;
}
