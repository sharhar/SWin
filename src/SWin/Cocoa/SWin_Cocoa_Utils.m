#include <swin/SWin.h>
#include <swin/SWin_Cocoa.h>

SResult swInit() {
    NSString *infoPlistPath = [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:@"Contents/Info.plist"];
    NSMutableDictionary *infoPlist = [NSMutableDictionary dictionaryWithContentsOfFile:infoPlistPath];
    [infoPlist setObject:[NSNumber numberWithBool:YES] forKey:@"NSHighResolutionCapable"];
    [infoPlist writeToFile:infoPlistPath atomically:YES];
    
    [NSApplication sharedApplication];
    
    [NSApp setDelegate:[[AppDelegate alloc] init]];
    
    [NSApp finishLaunching];
    
    return SWIN_OK;
}

void swTerminate() {
    [((AppDelegate*)[NSApp delegate]) dealloc];
}

double swGetTime() {
    return [[[NSDate alloc] init] timeIntervalSinceReferenceDate];
}
