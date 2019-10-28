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

@implementation SWinContentView

- (instancetype)initWithSWindow:(SWin_Cocoa_Window *)initWindow
{
	self = [super init];
	if (self != nil)
	{
		window = initWindow;
		trackingArea = nil;
		//markedText = [[NSMutableAttributedString alloc] init];
		
		[self updateTrackingAreas];
		// NOTE: kUTTypeURL corresponds to NSPasteboardTypeURL but is available
		//       on 10.7 without having been deprecated yet
		//[self registerForDraggedTypes:@[(__bridge NSString*) kUTTypeURL]];
	}
	
	return self;
}

- (void)dealloc
{
	[trackingArea release];
	[super dealloc];
}

- (BOOL)isOpaque
{
	return YES;//[window->ns.object isOpaque];
}

- (BOOL)canBecomeKeyView
{
	return YES;
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (BOOL)wantsUpdateLayer
{
	return YES;
}

- (void)updateLayer
{
	
}

- (void)cursorUpdate:(NSEvent *)event
{
	
}

- (BOOL)acceptsFirstMouse:(NSEvent *)event
{
	return YES;
}

- (void)mouseDown:(NSEvent *)event
{
	//_glfwInputMouseClick(window,
	//					 GLFW_MOUSE_BUTTON_LEFT,
	//					 GLFW_PRESS,
	//					 translateFlags([event modifierFlags]));
}

- (void)mouseDragged:(NSEvent *)event
{
	
	[self mouseMoved:event];
}

- (void)mouseUp:(NSEvent *)event
{
	//_glfwInputMouseClick(window,
	//					 GLFW_MOUSE_BUTTON_LEFT,
	//					 GLFW_RELEASE,
	//					 translateFlags([event modifierFlags]));
}

- (void)mouseMoved:(NSEvent *)event
{
	//if (window->cursorMode == GLFW_CURSOR_DISABLED)
	//{
	//	const double dx = [event deltaX] - window->ns.cursorWarpDeltaX;
	//	const double dy = [event deltaY] - window->ns.cursorWarpDeltaY;
		
	//	_glfwInputCursorPos(window,
	//						window->virtualCursorPosX + dx,
	//						window->virtualCursorPosY + dy);
	//}
	//else
	//{
	//	const NSRect contentRect = [window->ns.view frame];
	//	// NOTE: The returned location uses base 0,1 not 0,0
	//	const NSPoint pos = [event locationInWindow];
		
	//	_glfwInputCursorPos(window, pos.x, contentRect.size.height - pos.y);
	//}
	
	//window->ns.cursorWarpDeltaX = 0;
	//window->ns.cursorWarpDeltaY = 0;
}

- (void)rightMouseDown:(NSEvent *)event
{
	//_glfwInputMouseClick(window,
	//					 GLFW_MOUSE_BUTTON_RIGHT,
	//					 GLFW_PRESS,
	//					 translateFlags([event modifierFlags]));
}

- (void)rightMouseDragged:(NSEvent *)event
{
	[self mouseMoved:event];
}

- (void)rightMouseUp:(NSEvent *)event
{
	//_glfwInputMouseClick(window,
	//					 GLFW_MOUSE_BUTTON_RIGHT,
	//					 GLFW_RELEASE,
	//					 translateFlags([event modifierFlags]));
}

- (void)otherMouseDown:(NSEvent *)event
{
	//_glfwInputMouseClick(window,
	//					 (int) [event buttonNumber],
	//					 GLFW_PRESS,
	//					 translateFlags([event modifierFlags]));
}

- (void)otherMouseDragged:(NSEvent *)event
{
	[self mouseMoved:event];
}

- (void)otherMouseUp:(NSEvent *)event
{
	//_glfwInputMouseClick(window,
	//					 (int) [event buttonNumber],
	//					 GLFW_RELEASE,
	//					 translateFlags([event modifierFlags]));
}

- (void)mouseExited:(NSEvent *)event
{
	//if (window->cursorMode == GLFW_CURSOR_HIDDEN)
	//	showCursor(window);
	
	//_glfwInputCursorEnter(window, GLFW_FALSE);
}

- (void)mouseEntered:(NSEvent *)event
{
	//if (window->cursorMode == GLFW_CURSOR_HIDDEN)
	//	hideCursor(window);
	
	//_glfwInputCursorEnter(window, GLFW_TRUE);
}

- (void)viewDidChangeBackingProperties
{
	//const NSRect contentRect = [window->ns.view frame];
	//const NSRect fbRect = [window->ns.view convertRectToBacking:contentRect];
	
	//if (fbRect.size.width != window->ns.fbWidth ||
	//	fbRect.size.height != window->ns.fbHeight)
	//{
	//	window->ns.fbWidth  = fbRect.size.width;
	//	window->ns.fbHeight = fbRect.size.height;
	//	_glfwInputFramebufferSize(window, fbRect.size.width, fbRect.size.height);
	//}
	
	//const float xscale = fbRect.size.width / contentRect.size.width;
	//const float yscale = fbRect.size.height / contentRect.size.height;
	
	//if (xscale != window->ns.xscale || yscale != window->ns.yscale)
	//{
	//	window->ns.xscale = xscale;
	//	window->ns.yscale = yscale;
	//	_glfwInputWindowContentScale(window, xscale, yscale);
		
	//	if (window->ns.retina && window->ns.layer)
	//		[window->ns.layer setContentsScale:[window->ns.object backingScaleFactor]];
	//}
}

- (void)drawRect:(NSRect)rect
{
	//_glfwInputWindowDamage(window);
}

- (void)updateTrackingAreas
{
	if (trackingArea != nil)
	{
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

- (void)keyDown:(NSEvent *)event
{
	//const int key = translateKey([event keyCode]);
	//const int mods = translateFlags([event modifierFlags]);
	
	//_glfwInputKey(window, key, [event keyCode], GLFW_PRESS, mods);
	
	if(window->keyDownCallback != NULL) {
		window->keyDownCallback(__sWin_Cocoa_keycodes[event.keyCode]);
	}
	
	//[self interpretKeyEvents:@[event]];
}

- (void)keyUp:(NSEvent *)event
{
	if(window->keyUpCallback != NULL) {
		window->keyUpCallback(__sWin_Cocoa_keycodes[event.keyCode]);
	}
	
	//const int key = translateKey([event keyCode]);
	//const int mods = translateFlags([event modifierFlags]);
	//_glfwInputKey(window, key, [event keyCode], GLFW_RELEASE, mods);
}

- (void)scrollWheel:(NSEvent *)event
{
	//double deltaX = [event scrollingDeltaX];
	//double deltaY = [event scrollingDeltaY];
	
	//if ([event hasPreciseScrollingDeltas])
	//{
	//	deltaX *= 0.1;
	//	deltaY *= 0.1;
	//}
	
	//if (fabs(deltaX) > 0.0 || fabs(deltaY) > 0.0)
	//	_glfwInputScroll(window, deltaX, deltaY);
}

- (void)doCommandBySelector:(SEL)selector
{
}

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
	
	result->root = [[SWinContentView alloc] initWithSWindow:result];
	[result->window setContentView:result->root];
	[result->window makeFirstResponder:result->root];
	
    [result->window setDelegate:(AppDelegate*)[NSApp delegate]];
    [result->window setAcceptsMouseMovedEvents:YES];
    
    [result->window setTitle:@(title)];
    
    [result->window makeKeyAndOrderFront:nil];
	
	result->keyDownCallback = NULL;
	result->keyUpCallback = NULL;
	
    return result;
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
				
                [NSApp sendEvent: ev];
            }
        } while (ev);
    }
}

uint8_t swCloseRequested(SWindow* window) {
    SWin_Cocoa_Window* _window = (SWin_Cocoa_Window*)window;
    AppDelegate* delegate = (AppDelegate*)[_window->window delegate];
    return (uint8_t)([delegate getRunning] == NO);
}

SResult swSetKeyDownCallback(SWindow* window, pfnSKeyDownCallback callback) {
	SWin_Cocoa_Window* _window = (SWin_Cocoa_Window*)window;
	_window->keyDownCallback = callback;
	return SWIN_OK;
}

SResult swSetKeyUpCallback(SWindow* window, pfnSKeyUpCallback callback) {
	SWin_Cocoa_Window* _window = (SWin_Cocoa_Window*)window;
	_window->keyDownCallback = callback;
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
    return [_window->window contentView];
}

SView* swCreateView(SView* parent, SRect* bounds) {
    NSView* rootView = (NSView*)parent;
    NSView* view = [[NSView alloc] initWithFrame:NSMakeRect(bounds->x, bounds->y, bounds->width, bounds->height)];
    [rootView addSubview:view];
    return view;
}

SButton* swCreateButton(SView* parent, SRect* bounds, const char* title, pfnSButtonCallback callback, void* userData) {
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

SMouseState* swGetMouseState(SWindow* window) {
    SWin_Cocoa_Window* _window = (SWin_Cocoa_Window*)window;
    AppDelegate* delegate = (AppDelegate*)[_window->window delegate];
    return [delegate getMouseState];
}

void swSetViewBackgroundColor(SView* view, SColor color) {
    
}
