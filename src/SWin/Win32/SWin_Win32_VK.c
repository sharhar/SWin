#include <swin/SWin.h>
#include <swin/SWin_Win32.h>

HMODULE __sWin_Win32_libVK;

typedef void (__stdcall *__sWin_Win32_PFN_vkVoidFunction)(void);

typedef struct __sWin_Win32_VkWin32SurfaceCreateInfoKHR {
	int32_t sType;
	const void* pNext;
	uint32_t flags;
	HINSTANCE hinstance;
	HWND hwnd;
} __sWin_Win32_VkWin32SurfaceCreateInfoKHR;

typedef __sWin_Win32_PFN_vkVoidFunction(__stdcall *__sWin_Win32_PFN_vkGetInstanceProcAddr)(void* instance, const char* pName);
typedef int32_t(__stdcall *__sWin_Win32_PFN_vkCreateWin32SurfaceKHR)(void* instance, const __sWin_Win32_VkWin32SurfaceCreateInfoKHR* pCreateInfo, void* pAllocator, void* pSurface);

__sWin_Win32_PFN_vkGetInstanceProcAddr __sWin_Win32_vkGetInstanceProcAddr;

char** __sWin_Win32_extensions = NULL;

SResult swInitVK() {
	__sWin_Win32_libVK = LoadLibraryW(L"vulkan-1.dll");
	CHECK(__sWin_Win32_libVK, "Failed to load vulkan-1.dll", -1);

	__sWin_Win32_vkGetInstanceProcAddr = GetProcAddress(__sWin_Win32_libVK, "vkGetInstanceProcAddr");
	CHECK(__sWin_Win32_vkGetInstanceProcAddr, "Failed to load vkGetInstanceProcAddr", -1);

	__sWin_Win32_extensions = ALLOC(char*, 2);
	CHECK(__sWin_Win32_extensions, "Failed to Allocate String Array", -1);

	__sWin_Win32_extensions[0] = "VK_KHR_surface";
	__sWin_Win32_extensions[1] = "VK_KHR_win32_surface";

	return SWIN_OK;
}

int32_t swCreateWindowSurfaceVK(void* instance, SView* view, void* allocator, void* surface) {
	CHECK(instance, "instance was NULL", -1);
	CHECK(view, "view was NULL", -1);
	CHECK(surface, "surface was NULL", -1);

	SWin_Win32_View* _view = (SWin_Win32_View*)view;

	__sWin_Win32_VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = 1000009000; // this is the integer value of VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR
	surfaceCreateInfo.pNext = NULL;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.hinstance = _view->hInstance;
	surfaceCreateInfo.hwnd = _view->hWnd;

	void* func = swGetProcAddressVK(instance, "vkCreateWin32SurfaceKHR");
	CHECK(func, "Failed to load vkCreateWin32SurfaceKHR", -1);

	return ((__sWin_Win32_PFN_vkCreateWin32SurfaceKHR)func)(instance, &surfaceCreateInfo, allocator, surface);
}

char** swGetRequiredExtensionsVK(uint32_t* count) {
	CHECK(count, "count was NULL", -1);

	*count = 2;
	return __sWin_Win32_extensions;
}

void* swGetProcAddressVK(void* instance, const char* name) {
	CHECK(name, "name was NULL", -1);

	void* result = __sWin_Win32_vkGetInstanceProcAddr(instance, name);

	return result == NULL ? GetProcAddress(__sWin_Win32_libVK, name) : result;
}