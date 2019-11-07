#include <swin/SWin.h>
#include <swin/SWin_Cocoa.h>

#include <dlfcn.h>
#import <QuartzCore/CAMetalLayer.h>

typedef struct __sWin_Cocoa_VkMacOSSurfaceCreateInfoMVK {
	int32_t            		        sType;
	const void*                     pNext;
	uint32_t                        flags;
	const void*                     pView;
} __sWin_Cocoa_VkMacOSSurfaceCreateInfoMVK;

typedef void (*__sWin_Cocoa_PFN_vkVoidFunction)(void);
typedef __sWin_Cocoa_PFN_vkVoidFunction(*__sWin_Cocoa_PFN_vkGetInstanceProcAddr)(void* instance, const char* pName);
typedef int32_t (*__sWin_Cocoa_PFN_vkCreateMacOSSurfaceMVK)(void* instance, const __sWin_Cocoa_VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const void* pAllocator, void* pSurface);

__sWin_Cocoa_PFN_vkGetInstanceProcAddr __sWin_Cocoa_vkGetInstanceProcAddr;

void* __sWin_Cocoa_libVK;
char** __sWin_Cocoa_extensions = NULL;

SResult swInitVK() {
	__sWin_Cocoa_libVK = dlopen("libvulkan.1.dylib", RTLD_LAZY | RTLD_LOCAL);
	
	CHECK(__sWin_Cocoa_libVK, "could not load vulkan", SWIN_FAILED);
	
	__sWin_Cocoa_vkGetInstanceProcAddr = dlsym(__sWin_Cocoa_libVK, "vkGetInstanceProcAddr");
	
	__sWin_Cocoa_extensions = malloc(sizeof(char*) * 2);
	__sWin_Cocoa_extensions[0] = "VK_KHR_surface";
	__sWin_Cocoa_extensions[1] = "VK_MVK_macos_surface";
	
	return SWIN_OK;
}

void* swGetProcAddressVK(void* instance, const char* name) {
	void* result = __sWin_Cocoa_vkGetInstanceProcAddr(instance, name);
	return result == NULL ? dlsym(__sWin_Cocoa_libVK, name) : result;
}

char** swGetRequiredExtensionsVK(uint32_t* count) {
	*count = 2;
	return __sWin_Cocoa_extensions;
}

int32_t swCreateWindowSurfaceVK(void* instance, SView* view, void* allocator, void* surface) {
	SWin_Cocoa_View* _view = (SWin_Cocoa_View*)view;
	
	[_view->view setWantsLayer:YES];
	[_view->view setLayer:[CAMetalLayer layer]];
	
	__sWin_Cocoa_VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo;
	surfaceCreateInfo.sType = 1000123000; // this is the integer value of VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK
	surfaceCreateInfo.pNext = NULL;
	surfaceCreateInfo.flags = 0;
	surfaceCreateInfo.pView = _view->view;
	
	return ((__sWin_Cocoa_PFN_vkCreateMacOSSurfaceMVK)swGetProcAddressVK(instance, "vkCreateMacOSSurfaceMVK"))(instance, &surfaceCreateInfo, allocator, surface);
}
