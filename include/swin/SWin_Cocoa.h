#ifndef SWin_Cocoa_h
#define SWin_Cocoa_h

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <AppKit/NSApplication.h>

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

#endif
