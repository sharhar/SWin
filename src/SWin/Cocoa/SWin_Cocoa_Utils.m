#include <swin/SWin.h>
#include <swin/SWin_Cocoa.h>

void swInit() {
    [NSApplication sharedApplication];
    
    [NSApp setDelegate:[[AppDelegate alloc] init]];
    
    [NSApp finishLaunching];
}

void swTerminate() {
    [((AppDelegate*)[NSApp delegate]) dealloc];
}

double swGetTime() {
    return [[[NSDate alloc] init] timeIntervalSinceReferenceDate];
}
