#include "../../../include/swin/SWin.h"
#include "../../../include/swin/SWin_X11.h"

typedef void (*__sWin_X11_PFN_vkVoidFunction)(void);

typedef struct __sWin_X11_VkXlibSurfaceCreateInfoKHR {
	int32_t sType;
	const void* pNext;
	uint32_t flags;
	Display* dpy;
	Window window;
} __sWin_X11_VkXlibSurfaceCreateInfoKHR;

typedef __sWin_X11_PFN_vkVoidFunction(*__sWin_X11_PFN_vkGetInstanceProcAddr)(void* instance, const char* pName);
typedef int32_t (*__sWin_X11_PFN_vkCreateXlibSurfaceKHR)(void* instance, const __sWin_X11_VkXlibSurfaceCreateInfoKHR* pCreateInfo, void* pAllocator, void* pSurface);
//typedef int32_t (*__sWin_X11_PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)(void* physicalDevice, uint32_t queueFamilyIndex, Display* dpy, VisualID visualID);

__sWin_X11_PFN_vkGetInstanceProcAddr __sWin_X11_vkGetInstanceProcAddr;
void* __sWin_X11_libVK;

char** __sWin_X11_extensions = NULL;

void swInitVK() {
	__sWin_X11_libVK = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_GLOBAL);
	if(__sWin_X11_libVK == NULL) {
		printf("Error loading libvulkan.so.1, trying libvulkan.so\n");
		__sWin_X11_libVK = dlopen("libvulkan.so", RTLD_NOW | RTLD_GLOBAL);
		if(__sWin_X11_libVK == NULL) {
			printf("Error loading libvulkan.so too! Failed Initialization...");
			return;
		}
	}

	__sWin_X11_vkGetInstanceProcAddr = dlsym(__sWin_X11_libVK, "vkGetInstanceProcAddr");

	__sWin_X11_extensions = malloc(sizeof(char*) * 2);
	__sWin_X11_extensions[0] = "VK_KHR_surface";
	__sWin_X11_extensions[1] = "VK_KHR_xlib_surface";
}

void* swGetProcAddressVK(void* instance, const char* name) {
	void* result = __sWin_X11_vkGetInstanceProcAddr(instance, name);
	return result == NULL ? dlsym(__sWin_X11_libVK, name) : result;
}

char** swGetRequiredExtensionsVK(uint32_t* count) {
	*count = 2;
	return __sWin_X11_extensions;
}

int32_t swCreateWindowSurfaceVK(void* instance, SView* view, void* allocator, void* surface) {
	__sWin_X11_VkXlibSurfaceCreateInfoKHR surfaceCreateInfo;
	surfaceCreateInfo.sType = 1000004000; // this is the integer value of VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR
	surfaceCreateInfo.pNext = NULL;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.dpy = __sWin_X11_display;
	surfaceCreateInfo.window = ((SWin_X11_View*)view)->window;

	return ((__sWin_X11_PFN_vkCreateXlibSurfaceKHR)swGetProcAddressVK(instance, "vkCreateXlibSurfaceKHR"))(instance, &surfaceCreateInfo, allocator, surface);
}
