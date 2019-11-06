#ifndef vulkan_test_h
#define vulkan_test_h

#include <stdio.h>
#include <swin/SWin.h>

//#define SWIN_TEST_VULKAN

typedef void Swin_Vk;

Swin_Vk* initVk(SView* view);
void renderVk(Swin_Vk* ctx);

#endif /* vulkan_test_h */
