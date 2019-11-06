#include <swin/SWin.h>
#include <swin/SWin_Cocoa.h>

@implementation AppDelegate

- (id) init {
    self = [super init];
    
    self->running = YES;
    
    return self;
}

- (BOOL) getRunning {
    return self->running;
}

#pragma mark - Window Delegate

- (void)windowWillClose:(NSNotification *)notification {
    self->running = NO;
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

@implementation SWinContentView

- (instancetype)initWithSView:(SWin_Cocoa_View *)initView
{
	self = [super initWithFrame:initView->bounds];
	if (self != nil)
	{
		view = initView;
		trackingArea = nil;
		
		[self updateTrackingAreas];
	}
	
	return self;
}

- (void)dealloc
{
	[trackingArea release];
	[super dealloc];
}

- (BOOL)isOpaque { return YES; }

- (BOOL)canBecomeKeyView { return YES; }

- (BOOL)acceptsFirstResponder { return YES; }

- (BOOL)wantsUpdateLayer { return YES; }

- (void)updateLayer {}

- (void)cursorUpdate:(NSEvent *)event {}

- (BOOL)acceptsFirstMouse:(NSEvent *)event { return YES; }

- (void)mouseDown:(NSEvent *)event {
	if(view->mouseDownCallback != NULL) {
		NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
		point.y = [self bounds].size.height - point.y;
		
		view->mouseDownCallback(point.x, point.y, SWIN_MOUSE_BUTTON_LEFT);
	}
}

- (void)mouseDragged:(NSEvent *)event {
	[self mouseMoved:event];
}

- (void)mouseUp:(NSEvent *)event {
	if(view->mouseUpCallback != NULL) {
		NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
		point.y = [self bounds].size.height - point.y;
		
		view->mouseUpCallback(point.x, point.y, SWIN_MOUSE_BUTTON_LEFT);
	}
}

- (void)mouseMoved:(NSEvent *)event {
	if(view->mouseMovedCallback != NULL) {
		NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
		point.y = [self bounds].size.height - point.y;
		
		view->mouseMovedCallback(point.x, point.y);
	}
}

- (void)rightMouseDown:(NSEvent *)event {
	if(view->mouseDownCallback != NULL) {
		NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
		point.y = [self bounds].size.height - point.y;
		
		view->mouseDownCallback(point.x, point.y, SWIN_MOUSE_BUTTON_RIGHT);
	}
}

- (void)rightMouseDragged:(NSEvent *)event {
	[self mouseMoved:event];
}

- (void)rightMouseUp:(NSEvent *)event {
	if(view->mouseUpCallback != NULL) {
		NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
		point.y = [self bounds].size.height - point.y;
		
		view->mouseUpCallback(point.x, point.y, SWIN_MOUSE_BUTTON_RIGHT);
	}
}

- (void)otherMouseDown:(NSEvent *)event {
	if(view->mouseDownCallback != NULL) {
		NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
		point.y = [self bounds].size.height - point.y;
		
		view->mouseDownCallback(point.x, point.y, [event buttonNumber]);
	}
}

- (void)otherMouseDragged:(NSEvent *)event {
	[self mouseMoved:event];
}

- (void)otherMouseUp:(NSEvent *)event {
	if(view->mouseUpCallback != NULL) {
		NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
		point.y = [self bounds].size.height - point.y;
		
		view->mouseUpCallback(point.x, point.y, [event buttonNumber]);
	}
}

- (void)mouseExited:(NSEvent *)event {
	
}

- (void)mouseEntered:(NSEvent *)event {
	
}

- (void)viewDidChangeBackingProperties {
	
}

- (void)drawRect:(NSRect)rect {}

- (void)updateTrackingAreas {
	if (trackingArea != nil) {
		[self removeTrackingArea:trackingArea];
		[trackingArea release];
	}
	
	const NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited |
	NSTrackingActiveInKeyWindow |
	NSTrackingEnabledDuringMouseDrag |
	NSTrackingCursorUpdate |
	NSTrackingInVisibleRect |
	NSTrackingAssumeInside;
	
	trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
												options:options
												  owner:self
											   userInfo:nil];
	
	[self addTrackingArea:trackingArea];
	[super updateTrackingAreas];
}

- (void)keyDown:(NSEvent *)event {
	if(view->keyDownCallback != NULL) {
		view->keyDownCallback(__sWin_Cocoa_keycodes[event.keyCode]);
	}
}

- (void)keyUp:(NSEvent *)event {
	if(view->keyUpCallback != NULL) {
		view->keyUpCallback(__sWin_Cocoa_keycodes[event.keyCode]);
	}
}

- (void)scrollWheel:(NSEvent *)event {
	float deltaY = [event scrollingDeltaY];
	
	if ([event hasPreciseScrollingDeltas]) {
		deltaY *= 0.1;
	}
	
	if(view->mouseScrollCallback != NULL) {
		NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
		point.y = [self bounds].size.height - point.y;
		
		view->mouseScrollCallback(point.x, point.y, deltaY);
	}
}

- (void)doCommandBySelector:(SEL)selector {}

@end


SWindow* swCreateWindow(int width, int height, const char* title) {
	SWin_Cocoa_Window* result = ALLOC_S(SWin_Cocoa_Window);
	
    NSUInteger windowStyle = NSWindowStyleMaskTitled  | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable;
    
    NSRect screenRect = [[NSScreen mainScreen] frame];
    NSRect viewRect = NSMakeRect(0, 0, width, height);
    NSRect windowRect = NSMakeRect(NSMidX(screenRect) - NSMidX(viewRect),
                                   NSMidY(screenRect) - NSMidY(viewRect),
                                   viewRect.size.width,
                                   viewRect.size.height);
    
    result->window = [[NSWindow alloc] initWithContentRect:windowRect
                                                   styleMask:windowStyle
                                                     backing:NSBackingStoreBuffered
                                                       defer:NO];
	
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	
	result->root = ALLOC_S(SWin_Cocoa_View);
	
	result->root->bounds = viewRect;
	
	result->root->view = [[SWinContentView alloc] initWithSView:result->root];
	
	[result->window setContentView:result->root->view];
	
    [result->window setDelegate:(AppDelegate*)[NSApp delegate]];
    [result->window setAcceptsMouseMovedEvents:YES];
    
    [result->window setTitle:@(title)];
    
    [result->window makeKeyAndOrderFront:nil];
	
	result->root->window = result->window;
	
	result->root->keyDownCallback = NULL;
	result->root->keyUpCallback = NULL;
	
	result->root->mouseMovedCallback = NULL;
	result->root->mouseDownCallback = NULL;
	result->root->mouseUpCallback = NULL;
	result->root->mouseScrollCallback = NULL;
	
    return result;
}

SResult swDestroyWindow(SWindow* window) {
    CHECK(window, "window was NULL", SWIN_FAILED);
	
    return SWIN_OK;
}

void swPollEvents() {
    @autoreleasepool {
		NSEvent* ev = [NSApp nextEventMatchingMask: NSEventMaskAny
										 untilDate: nil
											inMode: NSDefaultRunLoopMode
										   dequeue: YES];
        while (ev) {
			[NSApp sendEvent: ev];
			
			ev = [NSApp nextEventMatchingMask: NSEventMaskAny
									untilDate: nil
									   inMode: NSDefaultRunLoopMode
									  dequeue: YES];
        }
    }
}

uint8_t swCloseRequested(SWindow* window) {
    SWin_Cocoa_Window* _window = (SWin_Cocoa_Window*)window;
    AppDelegate* delegate = (AppDelegate*)[_window->window delegate];
    return (uint8_t)([delegate getRunning] == NO);
}


SResult swSetKeyDownCallback(SView* view, pfnSKeyDownCallback callback) {
	SWin_Cocoa_View* _view = (SWin_Cocoa_View*)view;
	_view->keyDownCallback = callback;
	return SWIN_OK;
}

SResult swSetKeyUpCallback(SView* view, pfnSKeyUpCallback callback) {
	SWin_Cocoa_View* _view = (SWin_Cocoa_View*)view;
	_view->keyUpCallback = callback;
	return SWIN_OK;
}

SResult swSetMouseMovedCallback(SView* view, pfnSMouseMovedCallback callback) {
	SWin_Cocoa_View* _view = (SWin_Cocoa_View*)view;
	_view->mouseMovedCallback = callback;
	return SWIN_OK;
}

SResult swSetMouseDownCallback(SView* view, pfnSMouseDownCallback callback) {
	SWin_Cocoa_View* _view = (SWin_Cocoa_View*)view;
	_view->mouseDownCallback = callback;
	return SWIN_OK;
}

SResult swSetMouseUpCallback(SView* view, pfnSMouseUpCallback callback) {
	SWin_Cocoa_View* _view = (SWin_Cocoa_View*)view;
	_view->mouseUpCallback = callback;
	return SWIN_OK;
}

SResult swSetMouseScrollCallback(SView* view, pfnSMouseScrollCallback callback) {
	SWin_Cocoa_View* _view = (SWin_Cocoa_View*)view;
	_view->mouseScrollCallback = callback;
	return SWIN_OK;
}

SResult swDraw(SWindow* window) {
    CHECK(window, "window was NULL", SWIN_FAILED);
    
    SWin_Cocoa_Window* _window = (SWin_Cocoa_Window*)window;
    [[_window->window contentView] setNeedsDisplay:YES];
    
    return SWIN_OK;
}

void swCloseWindow(SWindow* window) {
    SWin_Cocoa_Window* _window = (SWin_Cocoa_Window*)window;
    [_window->window close];
    [_window->window dealloc];
}

SView* swGetRootView(SWindow* window) {
    SWin_Cocoa_Window* _window = (SWin_Cocoa_Window*)window;
    return _window->root;
}

SView* swCreateView(SView* parent, SRect* bounds) {
	SWin_Cocoa_View* _parent = (SWin_Cocoa_View*)parent;
	
	SWin_Cocoa_View* result = ALLOC_S(SWin_Cocoa_View);
	
	result->bounds = NSMakeRect(bounds->x, bounds->y, bounds->width, bounds->height);
	
    NSView* view = [[SWinContentView alloc] initWithSView:result];
    [_parent->view addSubview:view];
	
	result->view = view;
	
	result->keyDownCallback = NULL;
	result->keyUpCallback = NULL;
	
	result->mouseMovedCallback = NULL;
	result->mouseDownCallback = NULL;
	result->mouseUpCallback = NULL;
	result->mouseScrollCallback = NULL;
	
	result->window = _parent->window;
	
    return result;
}

SButton* swCreateButton(SView* parent, SRect* bounds, const char* title, pfnSButtonCallback callback, void* userData) {
    SWin_Cocoa_View* _parent = (SWin_Cocoa_View*)parent;
	
    NSButton* button = [[NSButton alloc] initWithFrame:NSMakeRect(bounds->x, bounds->y, bounds->width, bounds->height)];
    [button setTitle: @(title)];
    [button setButtonType:NSButtonTypeMomentaryLight];
    [button setBezelStyle:NSBezelStyleRounded];
    
    ButtonData* buttonData = [[ButtonData alloc]
                              initWithCallback:callback
                              data:userData];
    
    [button setTarget:buttonData];
    [button setAction:@selector(buttonPress)];
    
    [_parent->view addSubview:button];
    
    return button;
}

SLabel* swCreateLabel(SView* parent, SRect* bounds, const char* text) {
    SWin_Cocoa_View* _parent = (SWin_Cocoa_View*)parent;
	
    NSTextField* label = [[NSTextField alloc] initWithFrame:NSMakeRect(bounds->x, bounds->y, bounds->width, bounds->height)];
    [label setStringValue:@(text)];
    label.drawsBackground = NO;
    label.bezeled = NO;
    label.bordered = YES;
    label.selectable = NO;
    
    [_parent->view addSubview:label];
    return label;
}

STextField* swCreateTextField(SView* parent, SRect* bounds, const char* text) {
    SWin_Cocoa_View* _parent = (SWin_Cocoa_View*)parent;
	
    NSTextField* textField = [[NSTextField alloc] initWithFrame:NSMakeRect(bounds->x, bounds->y, bounds->width, bounds->height)];
    
    [textField setStringValue:@(text)];
    textField.drawsBackground = YES;
    textField.bezeled = YES;
    textField.bordered = YES;
    textField.selectable = YES;
    textField.editable = YES;
    
    [_parent->view addSubview:textField];
    
	return textField;
}

char* swGetTextFromTextField(STextField* textField) {
    NSTextField* field = (NSTextField*)textField;
    
	return (char*)[[field stringValue] UTF8String];
}

void swSetViewBackgroundColor(SView* view, SColor color) {
    
}
