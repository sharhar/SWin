#include <swin/SWin.h>
#include <swin/SWin_Cocoa.h>

void swInit() {
    NSString *infoPlistPath = [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:@"Contents/Info.plist"];
    NSMutableDictionary *infoPlist = [NSMutableDictionary dictionaryWithContentsOfFile:infoPlistPath];
    [infoPlist setObject:[NSNumber numberWithBool:YES] forKey:@"NSHighResolutionCapable"];
    [infoPlist writeToFile:infoPlistPath atomically:YES];
    
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
