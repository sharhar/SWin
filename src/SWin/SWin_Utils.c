#include <swin/SWin.h>

SRect* _swMakeRect(float x, float y, float w, float h, SBool dispose) {
	SRect* result = ALLOC_S(SRect);
	CHECK(result, "Failed to Allocate Rect", NULL);

	result->x = x;
	result->y = y;
	result->width = w;
	result->height = h;
	result->dispose = dispose;

	return result;
}

SRect* swMakeRect(float x, float y, float w, float h) {
	return _swMakeRect(x, y, w, h, SWIN_FALSE);
}

SRect* swMakeDisposableRect(float x, float y, float w, float h) {
	return _swMakeRect(x, y, w, h, SWIN_TRUE);
}