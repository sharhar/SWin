#include <swin/SWin.h>
#include <swin/SWin_Win32.h>

#define NO_VULKAN
#ifndef NO_VULKAN

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

HMODULE __sWin_Win32_libVK;

PFN_vkGetInstanceProcAddr __sWin_Win32_vkGetInstanceProcAddr;

char** __sWin_Win32_extensions = NULL;

void swInitVK() {
	__sWin_Win32_libVK = LoadLibraryW(L"vulkan-1.dll.dll");

	__sWin_Win32_vkGetInstanceProcAddr = GetProcAddress(__sWin_Win32_libVK, "vkGetInstanceProcAddr");

	__sWin_Win32_extensions = malloc(sizeof(char*) * 2);
	__sWin_Win32_extensions[0] = "VK_KHR_surface";
	__sWin_Win32_extensions[1] = "VK_KHR_win32_surface";
}

VkResult swCreateWindowSurface(VkInstance instance, SView* view, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) {
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = GetModuleHandle(NULL);
	surfaceCreateInfo.hwnd = (HWND)view;

	return vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, allocator, surface);
}

char** swGetRequiredExtensionsVK(uint32_t* count) {
	*count = 2;
	return __sWin_Win32_extensions;
}

void* swGetProcAddressVK(VkInstance instance, const char* name) {
	void* result = __sWin_Win32_vkGetInstanceProcAddr(instance, name);

	return result == NULL ? GetProcAddress(__sWin_Win32_libVK, name) : result;
}

#endif