#include <swin/SWin.h>
#include <swin/SWin_Cocoa.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGL/gl.h>

CFBundleRef __sWin_Cocoa_libGL;

void swInitGL() {
    __sWin_Cocoa_libGL = CFBundleGetBundleWithIdentifier(CFSTR("com.apple.opengl"));
}

SOpenGLContext* swCreateOpenGLContext(SView* parent, SOpenGLContextAttribs* attribs) {
    NSView* rootView = (NSView*)parent;
    
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
    [context setView:rootView];
    
    [context makeCurrentContext];
    
    GLint sync = attribs->swapInterval;
    [context setValues:&sync forParameter:NSOpenGLCPSwapInterval];
    
    return context;
}

void swMakeContextCurrent(SOpenGLContext* sview) {
    NSOpenGLContext* context = (NSOpenGLContext*)sview;
    [context makeCurrentContext];
}

void swSwapBufers(SOpenGLContext* sview) {
    NSOpenGLContext* context = (NSOpenGLContext*)sview;
    [context flushBuffer];
}

void* swGetProcAddressGL(const char* name) {
    CFStringRef symbolName = CFStringCreateWithCString(kCFAllocatorDefault, name, kCFStringEncodingASCII);
    
    void* result = CFBundleGetFunctionPointerForName(__sWin_Cocoa_libGL, symbolName);
    
    CFRelease(symbolName);
    
    return result;
}

void swDestroyOpenGLContext(SOpenGLContext* context) {
    
}
