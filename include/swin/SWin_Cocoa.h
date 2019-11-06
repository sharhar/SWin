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
	SWin_Cocoa_View* root;
} SWin_Cocoa_Window;

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>{
    BOOL running;
}
@end

@interface ButtonData : NSObject  {
    void* data;
    void (*pressCallback) (void* data);
}
@end

@interface SWinContentView : NSView
{
	SWin_Cocoa_View* view;
	NSTrackingArea* trackingArea;
}

- (instancetype)initWithSView:(SWin_Cocoa_View *)initView;

@end

extern short int __sWin_Cocoa_keycodes[256];

#endif
