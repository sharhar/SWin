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

void swInitVK() {
	__sWin_Win32_libVK = LoadLibraryW(L"vulkan-1.dll");

	__sWin_Win32_vkGetInstanceProcAddr = GetProcAddress(__sWin_Win32_libVK, "vkGetInstanceProcAddr");

	__sWin_Win32_extensions = malloc(sizeof(char*) * 2);
	__sWin_Win32_extensions[0] = "VK_KHR_surface";
	__sWin_Win32_extensions[1] = "VK_KHR_win32_surface";
}

int32_t swCreateWindowSurfaceVK(void* instance, SView* view, void* allocator, void* surface) {
	__sWin_Win32_VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = 1000009000; // this is the integer value of VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR
	surfaceCreateInfo.pNext = NULL;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.hinstance = GetModuleHandle(NULL);
	surfaceCreateInfo.hwnd = (HWND)view;

	return ((__sWin_Win32_PFN_vkCreateWin32SurfaceKHR)swGetProcAddressVK(instance, "vkCreateWin32SurfaceKHR"))(instance, &surfaceCreateInfo, allocator, surface);
}

char** swGetRequiredExtensionsVK(uint32_t* count) {
	*count = 2;
	return __sWin_Win32_extensions;
}

void* swGetProcAddressVK(void* instance, const char* name) {
	void* result = __sWin_Win32_vkGetInstanceProcAddr(instance, name);

	return result == NULL ? GetProcAddress(__sWin_Win32_libVK, name) : result;
}