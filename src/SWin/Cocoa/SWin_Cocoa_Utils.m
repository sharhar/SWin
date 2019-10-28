#include <swin/SWin.h>
#include <swin/SWin_Cocoa.h>

short int __sWin_Cocoa_keycodes[256];

void __swCreateKeyTables() {
	memset(__sWin_Cocoa_keycodes, -1, sizeof(__sWin_Cocoa_keycodes));
	
	__sWin_Cocoa_keycodes[0x1D] = SWIN_KEY_0;
	__sWin_Cocoa_keycodes[0x12] = SWIN_KEY_1;
	__sWin_Cocoa_keycodes[0x13] = SWIN_KEY_2;
	__sWin_Cocoa_keycodes[0x14] = SWIN_KEY_3;
	__sWin_Cocoa_keycodes[0x15] = SWIN_KEY_4;
	__sWin_Cocoa_keycodes[0x17] = SWIN_KEY_5;
	__sWin_Cocoa_keycodes[0x16] = SWIN_KEY_6;
	__sWin_Cocoa_keycodes[0x1A] = SWIN_KEY_7;
	__sWin_Cocoa_keycodes[0x1C] = SWIN_KEY_8;
	__sWin_Cocoa_keycodes[0x19] = SWIN_KEY_9;
	__sWin_Cocoa_keycodes[0x00] = SWIN_KEY_A;
	__sWin_Cocoa_keycodes[0x0B] = SWIN_KEY_B;
	__sWin_Cocoa_keycodes[0x08] = SWIN_KEY_C;
	__sWin_Cocoa_keycodes[0x02] = SWIN_KEY_D;
	__sWin_Cocoa_keycodes[0x0E] = SWIN_KEY_E;
	__sWin_Cocoa_keycodes[0x03] = SWIN_KEY_F;
	__sWin_Cocoa_keycodes[0x05] = SWIN_KEY_G;
	__sWin_Cocoa_keycodes[0x04] = SWIN_KEY_H;
	__sWin_Cocoa_keycodes[0x22] = SWIN_KEY_I;
	__sWin_Cocoa_keycodes[0x26] = SWIN_KEY_J;
	__sWin_Cocoa_keycodes[0x28] = SWIN_KEY_K;
	__sWin_Cocoa_keycodes[0x25] = SWIN_KEY_L;
	__sWin_Cocoa_keycodes[0x2E] = SWIN_KEY_M;
	__sWin_Cocoa_keycodes[0x2D] = SWIN_KEY_N;
	__sWin_Cocoa_keycodes[0x1F] = SWIN_KEY_O;
	__sWin_Cocoa_keycodes[0x23] = SWIN_KEY_P;
	__sWin_Cocoa_keycodes[0x0C] = SWIN_KEY_Q;
	__sWin_Cocoa_keycodes[0x0F] = SWIN_KEY_R;
	__sWin_Cocoa_keycodes[0x01] = SWIN_KEY_S;
	__sWin_Cocoa_keycodes[0x11] = SWIN_KEY_T;
	__sWin_Cocoa_keycodes[0x20] = SWIN_KEY_U;
	__sWin_Cocoa_keycodes[0x09] = SWIN_KEY_V;
	__sWin_Cocoa_keycodes[0x0D] = SWIN_KEY_W;
	__sWin_Cocoa_keycodes[0x07] = SWIN_KEY_X;
	__sWin_Cocoa_keycodes[0x10] = SWIN_KEY_Y;
	__sWin_Cocoa_keycodes[0x06] = SWIN_KEY_Z;
	
	__sWin_Cocoa_keycodes[0x27] = SWIN_KEY_APOSTROPHE;
	__sWin_Cocoa_keycodes[0x2A] = SWIN_KEY_BACKSLASH;
	__sWin_Cocoa_keycodes[0x2B] = SWIN_KEY_COMMA;
	__sWin_Cocoa_keycodes[0x18] = SWIN_KEY_EQUAL;
	__sWin_Cocoa_keycodes[0x32] = SWIN_KEY_GRAVE_ACCENT;
	__sWin_Cocoa_keycodes[0x21] = SWIN_KEY_LEFT_BRACKET;
	__sWin_Cocoa_keycodes[0x1B] = SWIN_KEY_MINUS;
	__sWin_Cocoa_keycodes[0x2F] = SWIN_KEY_PERIOD;
	__sWin_Cocoa_keycodes[0x1E] = SWIN_KEY_RIGHT_BRACKET;
	__sWin_Cocoa_keycodes[0x29] = SWIN_KEY_SEMICOLON;
	__sWin_Cocoa_keycodes[0x2C] = SWIN_KEY_SLASH;
	__sWin_Cocoa_keycodes[0x0A] = SWIN_KEY_WORLD_1;
	
	__sWin_Cocoa_keycodes[0x33] = SWIN_KEY_BACKSPACE;
	__sWin_Cocoa_keycodes[0x39] = SWIN_KEY_CAPS_LOCK;
	__sWin_Cocoa_keycodes[0x75] = SWIN_KEY_DELETE;
	__sWin_Cocoa_keycodes[0x7D] = SWIN_KEY_DOWN;
	__sWin_Cocoa_keycodes[0x77] = SWIN_KEY_END;
	__sWin_Cocoa_keycodes[0x24] = SWIN_KEY_ENTER;
	__sWin_Cocoa_keycodes[0x35] = SWIN_KEY_ESCAPE;
	__sWin_Cocoa_keycodes[0x7A] = SWIN_KEY_F1;
	__sWin_Cocoa_keycodes[0x78] = SWIN_KEY_F2;
	__sWin_Cocoa_keycodes[0x63] = SWIN_KEY_F3;
	__sWin_Cocoa_keycodes[0x76] = SWIN_KEY_F4;
	__sWin_Cocoa_keycodes[0x60] = SWIN_KEY_F5;
	__sWin_Cocoa_keycodes[0x61] = SWIN_KEY_F6;
	__sWin_Cocoa_keycodes[0x62] = SWIN_KEY_F7;
	__sWin_Cocoa_keycodes[0x64] = SWIN_KEY_F8;
	__sWin_Cocoa_keycodes[0x65] = SWIN_KEY_F9;
	__sWin_Cocoa_keycodes[0x6D] = SWIN_KEY_F10;
	__sWin_Cocoa_keycodes[0x67] = SWIN_KEY_F11;
	__sWin_Cocoa_keycodes[0x6F] = SWIN_KEY_F12;
	__sWin_Cocoa_keycodes[0x69] = SWIN_KEY_F13;
	__sWin_Cocoa_keycodes[0x6B] = SWIN_KEY_F14;
	__sWin_Cocoa_keycodes[0x71] = SWIN_KEY_F15;
	__sWin_Cocoa_keycodes[0x6A] = SWIN_KEY_F16;
	__sWin_Cocoa_keycodes[0x40] = SWIN_KEY_F17;
	__sWin_Cocoa_keycodes[0x4F] = SWIN_KEY_F18;
	__sWin_Cocoa_keycodes[0x50] = SWIN_KEY_F19;
	__sWin_Cocoa_keycodes[0x5A] = SWIN_KEY_F20;
	__sWin_Cocoa_keycodes[0x73] = SWIN_KEY_HOME;
	__sWin_Cocoa_keycodes[0x72] = SWIN_KEY_INSERT;
	__sWin_Cocoa_keycodes[0x7B] = SWIN_KEY_LEFT;
	__sWin_Cocoa_keycodes[0x3A] = SWIN_KEY_LEFT_ALT;
	__sWin_Cocoa_keycodes[0x3B] = SWIN_KEY_LEFT_CONTROL;
	__sWin_Cocoa_keycodes[0x38] = SWIN_KEY_LEFT_SHIFT;
	__sWin_Cocoa_keycodes[0x37] = SWIN_KEY_LEFT_SUPER;
	__sWin_Cocoa_keycodes[0x6E] = SWIN_KEY_MENU;
	__sWin_Cocoa_keycodes[0x47] = SWIN_KEY_NUM_LOCK;
	__sWin_Cocoa_keycodes[0x79] = SWIN_KEY_PAGE_DOWN;
	__sWin_Cocoa_keycodes[0x74] = SWIN_KEY_PAGE_UP;
	__sWin_Cocoa_keycodes[0x7C] = SWIN_KEY_RIGHT;
	__sWin_Cocoa_keycodes[0x3D] = SWIN_KEY_RIGHT_ALT;
	__sWin_Cocoa_keycodes[0x3E] = SWIN_KEY_RIGHT_CONTROL;
	__sWin_Cocoa_keycodes[0x3C] = SWIN_KEY_RIGHT_SHIFT;
	__sWin_Cocoa_keycodes[0x36] = SWIN_KEY_RIGHT_SUPER;
	__sWin_Cocoa_keycodes[0x31] = SWIN_KEY_SPACE;
	__sWin_Cocoa_keycodes[0x30] = SWIN_KEY_TAB;
	__sWin_Cocoa_keycodes[0x7E] = SWIN_KEY_UP;
	
	__sWin_Cocoa_keycodes[0x52] = SWIN_KEY_KP_0;
	__sWin_Cocoa_keycodes[0x53] = SWIN_KEY_KP_1;
	__sWin_Cocoa_keycodes[0x54] = SWIN_KEY_KP_2;
	__sWin_Cocoa_keycodes[0x55] = SWIN_KEY_KP_3;
	__sWin_Cocoa_keycodes[0x56] = SWIN_KEY_KP_4;
	__sWin_Cocoa_keycodes[0x57] = SWIN_KEY_KP_5;
	__sWin_Cocoa_keycodes[0x58] = SWIN_KEY_KP_6;
	__sWin_Cocoa_keycodes[0x59] = SWIN_KEY_KP_7;
	__sWin_Cocoa_keycodes[0x5B] = SWIN_KEY_KP_8;
	__sWin_Cocoa_keycodes[0x5C] = SWIN_KEY_KP_9;
	__sWin_Cocoa_keycodes[0x45] = SWIN_KEY_KP_ADD;
	__sWin_Cocoa_keycodes[0x41] = SWIN_KEY_KP_DECIMAL;
	__sWin_Cocoa_keycodes[0x4B] = SWIN_KEY_KP_DIVIDE;
	__sWin_Cocoa_keycodes[0x4C] = SWIN_KEY_KP_ENTER;
	__sWin_Cocoa_keycodes[0x51] = SWIN_KEY_KP_EQUAL;
	__sWin_Cocoa_keycodes[0x43] = SWIN_KEY_KP_MULTIPLY;
	__sWin_Cocoa_keycodes[0x4E] = SWIN_KEY_KP_SUBTRACT;
}

SResult swInit() {
    NSString *infoPlistPath = [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:@"Contents/Info.plist"];
    NSMutableDictionary *infoPlist = [NSMutableDictionary dictionaryWithContentsOfFile:infoPlistPath];
    [infoPlist setObject:[NSNumber numberWithBool:YES] forKey:@"NSHighResolutionCapable"];
    [infoPlist writeToFile:infoPlistPath atomically:YES];
    
    [NSApplication sharedApplication];
    
    [NSApp setDelegate:[[AppDelegate alloc] init]];
    
    [NSApp finishLaunching];
	
	__swCreateKeyTables();
    
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
