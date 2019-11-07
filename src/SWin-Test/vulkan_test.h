#ifndef vulkan_test_h
#define vulkan_test_h

#include <stdio.h>
#include <swin/SWin.h>

//#define SWIN_TEST_VULKAN /* Uncomment this line to enable the vulakn test (does not work on macOS) */

typedef void Swin_Vk;

Swin_Vk* initVk(SView* view);
void renderVk(Swin_Vk* ctx, double red, double green, double blue);

#endif /* vulkan_test_h */
