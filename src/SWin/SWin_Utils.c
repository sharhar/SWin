#include <swin/SWin.h>

SRect* swMakeRect(float x, float y, float w, float h) {
    SRect* result = malloc(sizeof(SRect));
    result->x = x;
    result->y = y;
    result->width = w;
    result->height = h;
    return result;
}
