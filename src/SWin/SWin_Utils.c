#include <swin/SWin.h>

#ifdef __APPLE__
void* malloc(size_t size);
#endif

SRect* swMakeRect(float x, float y, float w, float h) {
    SRect* result = malloc(sizeof(SRect));
    result->x = x;
    result->y = y;
    result->width = w;
    result->height = h;
    return result;
}