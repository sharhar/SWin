#ifndef SWin_Cocoa_h
#define SWin_Cocoa_h

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <AppKit/NSApplication.h>

typedef struct SWin_Cocoa_Window {
	NSWindow* window;
	NSView* root;
	pfnSKeyDownCallback keyDownCallback;
	pfnSKeyUpCallback keyUpCallback;
} SWin_Cocoa_Window;

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>{
    BOOL running;
    SMouseState* mouseState;
}
@end

@interface ButtonData : NSObject  {
    void* data;
    void (*pressCallback) (void* data);
}
@end

@interface SWinContentView : NSView
{
	SWin_Cocoa_Window* window;
	NSTrackingArea* trackingArea;
}

- (instancetype)initWithSWindow:(SWin_Cocoa_Window *)initWindow;

@end

extern short int __sWin_Cocoa_keycodes[256];

#endif
