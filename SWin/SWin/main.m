//
//  main.m
//  OBJCTest
//
//  Created by Shahar Sandhaus on 6/25/17.
//  Copyright © 2017 Shahar Sandhaus. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <AppKit/NSApplication.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGL/gl.h>

BOOL running = YES;

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

@implementation AppDelegate

#pragma mark - Window Delegate

- (void)windowWillClose:(NSNotification *)notification {
    running = NO;
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

void testCallback(void* userData) {
    printf("UserData: %s\n", (char*)userData);
}

int main(int argc, const char * argv[]) {
    [NSApplication sharedApplication];
    
    AppDelegate* delegate = [[AppDelegate alloc] init];
    
    [NSApp setDelegate:delegate];
    
    [NSApp finishLaunching];
    
    NSUInteger windowStyle = NSWindowStyleMaskTitled  | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable;
    
    NSRect screenRect = [[NSScreen mainScreen] frame];
    NSRect viewRect = NSMakeRect(0, 0, 800, 600);
    NSRect windowRect = NSMakeRect(NSMidX(screenRect) - NSMidX(viewRect),
                                   NSMidY(screenRect) - NSMidY(viewRect),
                                   viewRect.size.width,
                                   viewRect.size.height);
    
    NSWindow* window = [[NSWindow alloc] initWithContentRect:windowRect
                                                   styleMask:windowStyle
                                                     backing:NSBackingStoreBuffered
                                                       defer:NO];
    
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    
    NSOpenGLPixelFormatAttribute attribs[] = {
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 32,
        NSOpenGLPFAAccelerated,
        0
    };
    
    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
    NSOpenGLView* glView = [[NSOpenGLView alloc] initWithFrame:NSMakeRect(200, 0, 600, 600) pixelFormat:pixelFormat];
    
    [window setDelegate:delegate];
    [window setAcceptsMouseMovedEvents:YES];
    
    [window setTitle:@"Hello"];
    
    NSButton* button = [[NSButton alloc] initWithFrame:NSMakeRect(0, 0, 150, 50)];
    [button setTitle: @"Button"];
    [button setButtonType:NSMomentaryLightButton];
    [button setBezelStyle:NSRoundedBezelStyle];
    
    ButtonData* buttonData = [[ButtonData alloc]
                              initWithCallback:&testCallback
                              data:"Hello"];
    
    [button setTarget:buttonData];
    [button setAction:@selector(buttonPress)];
    
    [window setContentView:[[NSView alloc] initWithFrame:viewRect]];
    [[window contentView] setWantsLayer:YES];
    [[window contentView] addSubview: glView];
    [[window contentView] addSubview: button];
    
    [window makeKeyAndOrderFront:nil];
    
    [[glView openGLContext] makeCurrentContext];
    
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    
    //NSTimeInterval startTime = [[[NSDate alloc] init] timeIntervalSinceReferenceDate];
    
    while (running) {
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
        
        //NSTimeInterval currentTime = [[[NSDate alloc] init] timeIntervalSinceReferenceDate];
        
        //NSLog(@"%g", currentTime - startTime);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.0f,  0.5f);
        glVertex2f( 0.5f, -0.5f);
        glEnd();
        
        [[glView openGLContext] flushBuffer];
        [glView setNeedsDisplay:YES];
        //[[window contentView] setNeedsDisplay:YES];
    }
    
    [window close];
    
    return 0;
}
