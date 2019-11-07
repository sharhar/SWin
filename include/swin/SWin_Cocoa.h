#ifndef SWin_Cocoa_h
#define SWin_Cocoa_h

#include <swin/SWin.h>

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <AppKit/NSApplication.h>

typedef struct SWin_Cocoa_View {
	NSRect bounds;
	NSWindow* window;
	NSView* view;
	pfnSKeyDownCallback keyDownCallback;
	pfnSKeyUpCallback keyUpCallback;
	pfnSMouseMovedCallback mouseMovedCallback;
	pfnSMouseDownCallback mouseDownCallback;
	pfnSMouseUpCallback mouseUpCallback;
	pfnSMouseScrollCallback mouseScrollCallback;
} SWin_Cocoa_View;

typedef struct SWin_Cocoa_Window {
	NSWindow* window;
	BOOL* running;
	SWin_Cocoa_View* root;
} SWin_Cocoa_Window;

@interface SWin_Cocoa_AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>{
    BOOL running;
}
@end

@interface SWin_Cocoa_ButtonData : NSObject  {
    void* data;
    void (*pressCallback) (void* data);
}
@end

@interface SWin_Cocoa_SWinContentView : NSView
{
	SWin_Cocoa_View* view;
	NSTrackingArea* trackingArea;
}

- (instancetype)initWithSView:(SWin_Cocoa_View *)initView;

@end

extern short int __sWin_Cocoa_keycodes[256];
extern double __sWin_POSIX_startTime;

double _swGetRawTime();

#endif
