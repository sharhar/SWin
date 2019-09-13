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

void swPopup(SWindow* window, char* title, char* text) {
    NSWindow* _window = (NSWindow*)window;
    
    NSPopUpButton* popup = [[NSPopUpButton alloc] initWithFrame:NSMakeRect(100, 100, 100, 100) pullsDown:FALSE];
    
    [[_window contentView] addSubview:popup];
    [popup setNeedsDisplay:YES];
}

double swGetTime() {
    return [[[NSDate alloc] init] timeIntervalSinceReferenceDate];
}
