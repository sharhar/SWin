#ifndef SWin_h
#define SWin_h

#include <stdint.h>

typedef void SWindow;
typedef void SView;
typedef void SOpenGLView;
typedef void SButton;
typedef void SLabel;
typedef void STextField;

typedef struct SRect {
    float x, y, width, height;
} SRect;

void swInit();

SWindow* swCreateWindow(int width, int height, const char* title);

void swPollEvents();
void swDraw(SWindow* window);
uint8_t swCloseRequested(SWindow* swin);

void swCloseWindow(SWindow* window);

SView* swGetRootView(SWindow* window);

SView* swCreateView(SView* parent, SRect* bounds);

SOpenGLView* swCreateOpenGLView(SView* parent, SRect* bounds);
void swMakeContextCurrent(SOpenGLView* view);
void swSwapBufers(SOpenGLView* view);

void* swGetProcAddress(const char* name);

SButton* swCreateButton(SView* parent, SRect* bounds, const char* title, void* callback, void* userData);

SLabel* swCreateLabel(SView* parent, SRect* bounds, const char* text);

double swGetTime();

SRect* swMakeRect(float x, float y, float w, float h);

void swTerminate();

#endif