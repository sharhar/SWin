#include <swin/SWin.h>
#include <swin/SWin_Cocoa.h>

@implementation AppDelegate

- (id) init {
    self = [super init];
    
    self->running = YES;
    self->mouseState = malloc(sizeof(SMouseState));
    self->mouseState->x = 0;
    self->mouseState->y = 0;
    self->mouseState->ldown = 0;
    
    return self;
}

- (BOOL) getRunning {
    return self->running;
}

- (SMouseState*) getMouseState {
    return self->mouseState;
}

#pragma mark - Window Delegate

- (void)windowWillClose:(NSNotification *)notification {
    self->running = NO;
}

- (void)mouseMoved:(NSEvent *)event {
    printf("hello\n");
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

SResult swDestroyWindow(SWindow* window) {
    CHECK(window, "window was NULL", SWIN_FAILED);
    
    return SWIN_OK;
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
                
                if((ev.type == NSEventTypeMouseMoved || ev.type == NSEventTypeLeftMouseDragged) && ev.window != NULL) {
                    NSPoint point = [[ev.window contentView] convertPoint:[ev locationInWindow] fromView:nil];
                    point.y = [[ev.window contentView] bounds].size.height - point.y;
                    
                    AppDelegate* delegate = (AppDelegate*)[ev.window delegate];
                    SMouseState* mouseState = [delegate getMouseState];
                    
                    mouseState->x = point.x;
                    mouseState->y = point.y;
                }
                
                if(ev.type == NSEventTypeLeftMouseDown) {
                    AppDelegate* delegate = (AppDelegate*)[ev.window delegate];
                    SMouseState* mouseState = [delegate getMouseState];
                    mouseState->ldown = 1;
                }
                
                if(ev.type == NSEventTypeLeftMouseUp) {
                    AppDelegate* delegate = (AppDelegate*)[ev.window delegate];
                    SMouseState* mouseState = [delegate getMouseState];
                    mouseState->ldown = 0;
                }
                
                if(ev.type == NSEventTypeScrollWheel) {
                    AppDelegate* delegate = (AppDelegate*)[ev.window delegate];
                    SMouseState* mouseState = [delegate getMouseState];
                    mouseState->scroll += ev.scrollingDeltaY/10.0f;
                }
                
                /*
                if(ev.type ==  NSEventTypeKeyDown) {
                    
                    if(ev.keyCode == 49) {
                        NSLog(@"Key w");
                    }
                    
                } else {
                
                }
                 
                 */
                
                [NSApp sendEvent: ev];
            }
        } while (ev);
    }
}

uint8_t swCloseRequested(SWindow* swin) {
    NSWindow* window = (NSWindow*)swin;
    AppDelegate* delegate = (AppDelegate*)[window delegate];
    return (uint8_t)([delegate getRunning] == NO);
    
}

SResult swDraw(SWindow* window) {
    CHECK(window, "window was NULL", SWIN_FAILED);
    
    NSWindow* _window = (NSWindow*)window;
    [[_window contentView] setNeedsDisplay:YES];
    
    return SWIN_OK;
}

void swCloseWindow(SWindow* swin) {
    NSWindow* window = (NSWindow*)swin;
    [window close];
    [window dealloc];
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

STextField* swCreateTextField(SView* parent, SRect* bounds, const char* text) {
    NSView* rootView = (NSView*)parent;
    NSTextField* textField = [[NSTextField alloc] initWithFrame:NSMakeRect(bounds->x, bounds->y, bounds->width, bounds->height)];
    
    [textField setStringValue:@(text)];
    textField.drawsBackground = YES;
    textField.bezeled = YES;
    textField.bordered = YES;
    textField.selectable = YES;
    textField.editable = YES;
    
    [rootView addSubview:textField];
    
	return textField;
}

char* swGetTextFromTextField(STextField* textField) {
    NSTextField* field = (NSTextField*)textField;
    
	return (char*)[[field stringValue] UTF8String];
}

SMouseState* swGetMouseState(SWindow* swin) {
    NSWindow* window = (NSWindow*)swin;
    AppDelegate* delegate = (AppDelegate*)[window delegate];
    return [delegate getMouseState];
}

void swSetViewBackgroundColor(SView* view, SColor color) {
    
}
