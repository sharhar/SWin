#include <swin/SWin.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <AppKit/NSApplication.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGL/gl.h>

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>{
    BOOL running;
}
@end

@implementation AppDelegate

- (id) init {
    self = [super init];
    
    self->running = YES;
    
    return self;
}

- (BOOL) getRunning {
    return self->running;
}

#pragma mark - Window Delegate

- (void)windowWillClose:(NSNotification *)notification {
    self->running = NO;
}

@end

@interface ButtonData : NSObject  {
    void* data;
    void (*pressCallback) (void* data);
}
@end

@implementation ButtonData

- (id) initWithCallback:(void*)callback data:(void*)userData {
    self->pressCallback = callback;
    self->data = userData;
    return self;
}

- (void) buttonPress {
    (*self->pressCallback)(self->data);
}

@end

void swInit() {
    [NSApplication sharedApplication];
    
    [NSApp setDelegate:[[AppDelegate alloc] init]];
        
    [NSApp finishLaunching];
}

SWindow* swCreateWindow(int width, int height, const char* title) {
    NSUInteger windowStyle = NSWindowStyleMaskTitled  | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable;
    
    NSRect screenRect = [[NSScreen mainScreen] frame];
    NSRect viewRect = NSMakeRect(0, 0, width, height);
    NSRect windowRect = NSMakeRect(NSMidX(screenRect) - NSMidX(viewRect),
                                   NSMidY(screenRect) - NSMidY(viewRect),
                                   viewRect.size.width,
                                   viewRect.size.height);
    
    NSWindow* window = [[NSWindow alloc] initWithContentRect:windowRect
                                                   styleMask:windowStyle
                                                     backing:NSBackingStoreBuffered
                                                       defer:NO];
    
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    
    
    [window setDelegate:(AppDelegate*)[NSApp delegate]];
    [window setAcceptsMouseMovedEvents:YES];
    
    [window setTitle:@(title)];
    
    [window setContentView:[[NSView alloc] initWithFrame:viewRect]];
    [[window contentView] setWantsLayer:YES];
    
    [window makeKeyAndOrderFront:nil];
    
    return window;
}

void swPollEvents() {
    @autoreleasepool {
        NSEvent* ev;
        do {
            ev = [NSApp nextEventMatchingMask: NSEventMaskAny
                                    untilDate: nil
                                       inMode: NSDefaultRunLoopMode
                                      dequeue: YES];
            
            if (ev) {
                
                if(ev.type ==  NSEventTypeKeyDown) {
                    
                    if(ev.keyCode == 49) {
                        NSLog(@"Key w");
                    }
                    
                } else {
                    [NSApp sendEvent: ev];
                }
            }
        } while (ev);
    }
}

uint8_t swCloseRequested(SWindow* swin) {
    NSWindow* window = (NSWindow*)swin;
    AppDelegate* delegate = (AppDelegate*)[window delegate];
    return (uint8_t)([delegate getRunning] == NO);
    
}

void swDraw(SWindow* swin) {
    NSWindow* window = (NSWindow*)swin;
    [[window contentView] setNeedsDisplay:YES];
}

void swCloseWindow(SWindow* swin) {
    NSWindow* window = (NSWindow*)swin;
    [window close];
    [window dealloc];
}

void swTerminate() {
    [((AppDelegate*)[NSApp delegate]) dealloc];
}

SView* swGetRootView(SWindow* swin) {
    NSWindow* window = (NSWindow*)swin;
    return [window contentView];
}

SView* swCreateView(SView* parent, SRect* bounds) {
    NSView* rootView = (NSView*)parent;
    NSView* view = [[NSView alloc] initWithFrame:NSMakeRect(bounds->x, bounds->y, bounds->width, bounds->height)];
    [rootView addSubview:view];
    return view;
}

SOpenGLView* swCreateOpenGLView(SView* parent, SRect* bounds) {
    NSView* rootView = (NSView*)parent;

    NSOpenGLPixelFormatAttribute attribs[] = {
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 32,
        NSOpenGLPFAAccelerated,
        0
    };
    
    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
    NSOpenGLView* view = [[NSOpenGLView alloc] initWithFrame:NSMakeRect(bounds->x, bounds->y, bounds->width, bounds->height) pixelFormat:pixelFormat];
    [rootView addSubview:view];
    return view;
}

void swMakeContextCurrent(SOpenGLView* sview) {
    NSOpenGLView* view = (NSOpenGLView*)sview;
    [[view openGLContext] makeCurrentContext];
}

void swSwapBufers(SOpenGLView* sview) {
    NSOpenGLView* view = (NSOpenGLView*)sview;
    [[view openGLContext] flushBuffer];
}

SButton* swCreateButton(SView* parent, SRect* bounds, const char* title, void* callback, void* userData) {
    NSView* rootView = (NSView*)parent;
    NSButton* button = [[NSButton alloc] initWithFrame:NSMakeRect(bounds->x, bounds->y, bounds->width, bounds->height)];
    [button setTitle: @(title)];
    [button setButtonType:NSMomentaryLightButton];
    [button setBezelStyle:NSRoundedBezelStyle];
    
    ButtonData* buttonData = [[ButtonData alloc]
                              initWithCallback:callback
                              data:userData];
    
    [button setTarget:buttonData];
    [button setAction:@selector(buttonPress)];
    
    [rootView addSubview:button];
    
    return button;
}

SLabel* swCreateLabel(SView* parent, SRect* bounds, const char* text) {
    NSView* rootView = (NSView*)parent;
    NSTextField* label = [[NSTextField alloc] initWithFrame:NSMakeRect(bounds->x, bounds->y, bounds->width, bounds->height)];
    [label setStringValue:@(text)];
    label.drawsBackground = NO;
    label.bezeled = NO;
    label.bordered = YES;
    label.selectable = NO;
    
    [rootView addSubview:label];
    return label;
}

double swGetTime() {
    return [[[NSDate alloc] init] timeIntervalSinceReferenceDate];
}
