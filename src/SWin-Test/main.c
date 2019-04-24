#include <swin/SWin.h>
#include <stdio.h>

//#define SWIN_TEST_VULKAN

#define GL_VERSION 0x1F02
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_TRIANGLES 0x0004

#ifdef WIN32
#define pfnglprefunc __stdcall *
#else
#define pfnglprefunc *
#endif

typedef void (pfnglprefunc PFNGLCLEARPROC)(unsigned int mask);
PFNGLCLEARPROC __glClear;
typedef void (pfnglprefunc PFNGLCLEARCOLORPROC)(float red, float green, float blue, float alpha);
PFNGLCLEARCOLORPROC __glClearColor;
typedef void (pfnglprefunc PFNGLCOLOR3FPROC)(float red, float green, float blue);
PFNGLCOLOR3FPROC __glColor3f;
typedef void (pfnglprefunc PFNGLBEGINPROC)(unsigned int mode);
PFNGLBEGINPROC __glBegin;
typedef void (pfnglprefunc PFNGLVIEWPORTPROC)(int x, int y, int width, int height);
PFNGLVIEWPORTPROC __glViewport;
typedef void (pfnglprefunc PFNGLENDPROC)();
PFNGLENDPROC __glEnd;
typedef void (pfnglprefunc PFNGLVERTEX2FPROC)(float x, float y);
PFNGLVERTEX2FPROC __glVertex2f;
typedef const char* (pfnglprefunc PFNGLGETSTRINGPROC)(unsigned int name);
PFNGLGETSTRINGPROC __glGetString;
typedef void (pfnglprefunc PFNGLROTATEFPROC)( float angle, float x, float y, float z );
PFNGLROTATEFPROC __glRotatef;

void buttonCallback(STextField* textField) {
	printf("%s\n", swGetTextFromTextField(textField));
}

uint8_t running = 1;
uint8_t fence = 0;
SMutex* mutex = NULL;

typedef struct RenderInfo {
	SView* view;
	float rotDir;
	float r, g, b;
	int ID;
} RenderInfo;

void renderFunction(RenderInfo* info) {
	SOpenGLContextAttribs attribs;
	attribs.major = 2;
	attribs.minor = 1;
	attribs.debug = 1;
	attribs.swapInterval = 1;
	attribs.forwardCompat = 1;
	attribs.profile = SWIN_OPENGL_CONTEXT_PROFILE_COMPATIBILITY;

	swLockMutex(mutex);

	swSleep(250);

	SOpenGLContext* context = swCreateOpenGLContext(info->view, &attribs);

	swMakeContextCurrent(context);

	printf("GL Version: %s\n", __glGetString(GL_VERSION));

	swUnlockMutex(mutex);

	__glViewport(0, 0, 300, 300);

	__glClearColor(info->r, info->g, info->b, 1.0f);

	while(running) {
		__glRotatef(info->rotDir, 0, 0, 1);

		__glClear(GL_COLOR_BUFFER_BIT);

		__glBegin(GL_TRIANGLES);
		__glColor3f(1.0f, 0.0f, 0.0f);
		__glVertex2f(-0.69f, -0.4f);
		__glColor3f(0.0f, 1.0f, 0.0f);
		__glVertex2f( 0.0f,  0.8f);
		__glColor3f(0.0f, 0.0f, 1.0f);
		__glVertex2f( 0.69f, -0.4f);
		__glEnd();

		swSwapBufers(context);
	}

	swDestroyOpenGLContext(context);
}

typedef void Swin_Vk;

Swin_Vk* initVk(SView* view);
void renderVk(Swin_Vk* ctx);

int main(int argc, const char * argv[]) {
    swInit();
	swInitGL();

    SWindow* window = swCreateWindow(1000, 630, "UI Test");

    SColor winBG = {1,1,1,1};

    SView* rootView = swGetRootView(window);

    swSetViewBackgroundColor(rootView, winBG);
    
	SView* glView1 = swCreateView(rootView, swMakeRect(380, 10, 300, 300));
	SView* glView2 = swCreateView(rootView, swMakeRect(690, 10, 300, 300));
	SView* glView3 = swCreateView(rootView, swMakeRect(380, 320, 300, 300));
	SView* glView4 = swCreateView(rootView, swMakeRect(690, 320, 300, 300));

	STextField* textField = swCreateTextField(rootView, swMakeRect(10, 580, 275, 30), "text");

	SLabel* label = swCreateLabel(rootView, swMakeRect(10, 390, 100, 100), "Hello, world!");

	SButton* button = swCreateButton(rootView, swMakeRect(295, 580, 75, 30), "Submit", &buttonCallback, textField);

	SView* vkView = swCreateView(rootView, swMakeRect(10, 10, 360, 360));

	__glClear = swGetProcAddressGL("glClear");
	__glClearColor = swGetProcAddressGL("glClearColor");
	__glViewport = swGetProcAddressGL("glViewport");
	__glColor3f = swGetProcAddressGL("glColor3f");
	__glBegin = swGetProcAddressGL("glBegin");
	__glVertex2f = swGetProcAddressGL("glVertex2f");
	__glEnd = swGetProcAddressGL("glEnd");
	__glGetString = swGetProcAddressGL("glGetString");
	__glRotatef = swGetProcAddressGL("glRotatef");

	running = 1;

	RenderInfo renderInfos[4];
	renderInfos[0].view = glView1;
	renderInfos[0].rotDir = 1;
	renderInfos[0].r = 0;
	renderInfos[0].g = 0;
	renderInfos[0].b = 0;
	renderInfos[0].ID = 0;

	renderInfos[1].view = glView2;
	renderInfos[1].rotDir = -1;
	renderInfos[1].r = 1;
	renderInfos[1].g = 1;
	renderInfos[1].b = 1;
	renderInfos[1].ID = 1;

	renderInfos[2].view = glView3;
	renderInfos[2].rotDir = -1;
	renderInfos[2].r = 1;
	renderInfos[2].g = 1;
	renderInfos[2].b = 1;
	renderInfos[2].ID = 2;

	renderInfos[3].view = glView4;
	renderInfos[3].rotDir = 1;
	renderInfos[3].r = 0;
	renderInfos[3].g = 0;
	renderInfos[3].b = 0;
	renderInfos[3].ID = 3;

	mutex = swCreateMutex();

	SThread* thread1 = swCreateThread(renderFunction, renderInfos);
	SThread* thread2 = swCreateThread(renderFunction, renderInfos+1);
	SThread* thread3 = swCreateThread(renderFunction, renderInfos+2);
	SThread* thread4 = swCreateThread(renderFunction, renderInfos+3);

    uint32_t frames = 0;
    uint32_t fps = 0;

	Swin_Vk* vkContext = initVk(vkView);

	double startTime = swGetTime();
	double currentTime = 0;

    while (!swCloseRequested(window)) {
       currentTime = swGetTime();
        
        frames++;
        if(currentTime - startTime >= 1) {
            startTime = currentTime;
            fps = frames;
            frames = 0;
            printf("%d\n", fps);
        }

        swPollEvents();

        swSleep(10);

		renderVk(vkContext);

		swDraw(window);
    }

	running = 0;

    swWaitForThread(thread1);
	swWaitForThread(thread2);
	swWaitForThread(thread3);
	swWaitForThread(thread4);

	swDestroyThread(thread1);
	swDestroyThread(thread2);
	swDestroyThread(thread3);
	swDestroyThread(thread4);

	swDestroyMutex(mutex);

	swDestroyWindow(window);
    
    return 0;
}

#ifndef SWIN_TEST_VULKAN
Swin_Vk* initVk(SView* view) {
	return NULL;
}

void renderVk(Swin_Vk* ctx) {

}
#else

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <malloc.h>

typedef struct ContextVK {
	VkInstance instance;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceProperties physicalDeviceProperties;
	uint32_t presentQueueIdx;
	VkDevice device;
	VkSwapchainKHR swapChain;
	VkQueue queue;
	VkCommandPool commandPool;
	VkCommandBuffer cmdBuffer;
	VkImage* presentImages;
} ContextVK;

PFN_vkCreateInstance __vkCreateInstance;
PFN_vkDestroyInstance __vkDestroyInstance;
PFN_vkEnumeratePhysicalDevices __vkEnumeratePhysicalDevices;
PFN_vkGetPhysicalDeviceProperties __vkGetPhysicalDeviceProperties;
PFN_vkGetPhysicalDeviceQueueFamilyProperties __vkGetPhysicalDeviceQueueFamilyProperties;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR __vkGetPhysicalDeviceSurfaceSupportKHR;
PFN_vkCreateDevice __vkCreateDevice;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR __vkGetPhysicalDeviceSurfaceFormatsKHR;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR __vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR __vkGetPhysicalDeviceSurfacePresentModesKHR;
PFN_vkCreateSwapchainKHR __vkCreateSwapchainKHR;
PFN_vkGetDeviceQueue __vkGetDeviceQueue;
PFN_vkCreateCommandPool __vkCreateCommandPool;
PFN_vkAllocateCommandBuffers __vkAllocateCommandBuffers;
PFN_vkBeginCommandBuffer __vkBeginCommandBuffer;
PFN_vkEndCommandBuffer __vkEndCommandBuffer;
PFN_vkQueueSubmit __vkQueueSubmit;
PFN_vkGetSwapchainImagesKHR __vkGetSwapchainImagesKHR;
PFN_vkCreateFence __vkCreateFence;
PFN_vkBeginCommandBuffer __vkBeginCommandBuffer;
PFN_vkCmdPipelineBarrier __vkCmdPipelineBarrier;
PFN_vkEndCommandBuffer __vkEndCommandBuffer;
PFN_vkQueueSubmit __vkQueueSubmit;
PFN_vkWaitForFences __vkWaitForFences;
PFN_vkResetFences __vkResetFences;
PFN_vkResetCommandBuffer __vkResetCommandBuffer;
PFN_vkCreateImageView __vkCreateImageView;
PFN_vkAcquireNextImageKHR __vkAcquireNextImageKHR;
PFN_vkQueuePresentKHR __vkQueuePresentKHR;

Swin_Vk* initVk(SView* view) {
	swInitVK();

	ContextVK* context = (ContextVK*)malloc(sizeof(ContextVK));

	__vkCreateInstance = swGetProcAddressVK(NULL, "vkCreateInstance");

	VkApplicationInfo applicationInfo;
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pNext = NULL;
	applicationInfo.pApplicationName = "SWin Vulkan Test";
	applicationInfo.pEngineName = NULL;
	applicationInfo.engineVersion = 1;
	applicationInfo.apiVersion = VK_MAKE_VERSION(1, 1, 0);

	uint32_t countVK;
	char** extensionsVK = swGetRequiredExtensionsVK(&countVK);

	VkInstanceCreateInfo instanceInfo;
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = NULL;
	instanceInfo.pApplicationInfo = &applicationInfo;
	instanceInfo.enabledLayerCount = 0;
	instanceInfo.ppEnabledLayerNames = NULL;
	instanceInfo.enabledExtensionCount = countVK;
	instanceInfo.ppEnabledExtensionNames = extensionsVK;

	__vkCreateInstance(&instanceInfo, NULL, &context->instance);

	swCreateWindowSurfaceVK(context->instance, view, NULL, &context->surface);

	__vkEnumeratePhysicalDevices = swGetProcAddressVK(context->instance, "vkEnumeratePhysicalDevices");
	__vkGetPhysicalDeviceProperties = swGetProcAddressVK(context->instance, "vkGetPhysicalDeviceProperties");
	__vkGetPhysicalDeviceQueueFamilyProperties = swGetProcAddressVK(context->instance, "vkGetPhysicalDeviceQueueFamilyProperties");
	__vkGetPhysicalDeviceSurfaceSupportKHR = swGetProcAddressVK(context->instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
	__vkCreateDevice = swGetProcAddressVK(context->instance, "vkCreateDevice");
	__vkGetPhysicalDeviceSurfaceFormatsKHR = swGetProcAddressVK(context->instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
	__vkGetPhysicalDeviceSurfaceCapabilitiesKHR = swGetProcAddressVK(context->instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
	__vkGetPhysicalDeviceSurfacePresentModesKHR = swGetProcAddressVK(context->instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
	__vkCreateSwapchainKHR = swGetProcAddressVK(context->instance, "vkCreateSwapchainKHR");
	__vkGetDeviceQueue = swGetProcAddressVK(context->instance, "vkGetDeviceQueue");
	__vkCreateCommandPool = swGetProcAddressVK(context->instance, "vkCreateCommandPool");
	__vkAllocateCommandBuffers = swGetProcAddressVK(context->instance, "vkAllocateCommandBuffers");
	__vkBeginCommandBuffer = swGetProcAddressVK(context->instance, "vkBeginCommandBuffer");
	__vkEndCommandBuffer = swGetProcAddressVK(context->instance, "vkEndCommandBuffer");
	__vkQueueSubmit = swGetProcAddressVK(context->instance, "vkQueueSubmit");
	__vkGetSwapchainImagesKHR = swGetProcAddressVK(context->instance, "vkGetSwapchainImagesKHR");
	__vkCreateFence = swGetProcAddressVK(context->instance, "vkCreateFence");
	__vkBeginCommandBuffer = swGetProcAddressVK(context->instance, "vkBeginCommandBuffer");
	__vkCmdPipelineBarrier = swGetProcAddressVK(context->instance, "vkCmdPipelineBarrier");
	__vkEndCommandBuffer = swGetProcAddressVK(context->instance, "vkEndCommandBuffer");
	__vkQueueSubmit = swGetProcAddressVK(context->instance, "vkQueueSubmit");
	__vkWaitForFences = swGetProcAddressVK(context->instance, "vkWaitForFences");
	__vkResetFences = swGetProcAddressVK(context->instance, "vkResetFences");
	__vkResetCommandBuffer = swGetProcAddressVK(context->instance, "vkResetCommandBuffer");
	__vkCreateImageView = swGetProcAddressVK(context->instance, "vkCreateImageView");
	__vkAcquireNextImageKHR = swGetProcAddressVK(context->instance, "vkAcquireNextImageKHR");
	__vkQueuePresentKHR = swGetProcAddressVK(context->instance, "vkQueuePresentKHR");

	uint32_t physicalDeviceCount = 0;
	__vkEnumeratePhysicalDevices(context->instance, &physicalDeviceCount, NULL);
	VkPhysicalDevice* physicalDevices = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * physicalDeviceCount);
	__vkEnumeratePhysicalDevices(context->instance, &physicalDeviceCount, physicalDevices);

	for (uint32_t i = 0; i < physicalDeviceCount; ++i) {

		VkPhysicalDeviceProperties deviceProperties;
		__vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);

		uint32_t queueFamilyCount = 0;
		__vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyCount, NULL);
		VkQueueFamilyProperties* queueFamilyProperties = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
		__vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i],
			&queueFamilyCount,
			queueFamilyProperties);

		for (uint32_t j = 0; j < queueFamilyCount; ++j) {

			VkBool32 supportsPresent;
			__vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[i], j, context->surface, &supportsPresent);

			if (supportsPresent && (queueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
				context->physicalDevice = physicalDevices[i];
				context->physicalDeviceProperties = deviceProperties;
				context->presentQueueIdx = j;
				break;
			}
		}
		free(queueFamilyProperties);

		if (context->physicalDevice) {
			break;
		}
	}
	free(physicalDevices);

	if (context->physicalDevice == NULL) {
		printf("No physical device detected that can render and present!");
	}

	VkDeviceQueueCreateInfo queueCreateInfo;
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.pNext = NULL;
	queueCreateInfo.flags = 0;
	queueCreateInfo.queueFamilyIndex = context->presentQueueIdx;
	queueCreateInfo.queueCount = 1;
	float queuePriorities[] = { 1.0f };
	queueCreateInfo.pQueuePriorities = queuePriorities;

	VkDeviceCreateInfo deviceInfo;
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext = NULL;
	deviceInfo.flags = 0;
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = NULL;

	const char *deviceExtensions[] = { "VK_KHR_swapchain" };
	deviceInfo.enabledExtensionCount = 1;
	deviceInfo.ppEnabledExtensionNames = deviceExtensions;
	deviceInfo.pEnabledFeatures = NULL;

	__vkCreateDevice(context->physicalDevice, &deviceInfo, NULL, &context->device);

	uint32_t formatCount = 0;
	__vkGetPhysicalDeviceSurfaceFormatsKHR(context->physicalDevice, context->surface, &formatCount, NULL);
	VkSurfaceFormatKHR* surfaceFormats = (VkSurfaceFormatKHR*)malloc(sizeof(VkSurfaceFormatKHR) * formatCount);
	__vkGetPhysicalDeviceSurfaceFormatsKHR(context->physicalDevice, context->surface, &formatCount, surfaceFormats);

	VkFormat colorFormat;
	if (formatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
		colorFormat = VK_FORMAT_B8G8R8_UNORM;
	}
	else {
		colorFormat = surfaceFormats[0].format;
	}
	VkColorSpaceKHR colorSpace;
	colorSpace = surfaceFormats[0].colorSpace;
	free(surfaceFormats);

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	__vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->physicalDevice, context->surface, &surfaceCapabilities);

	uint32_t width = 370, height = 370;

	uint32_t desiredImageCount = 2;
	if (desiredImageCount < surfaceCapabilities.minImageCount) {
		desiredImageCount = surfaceCapabilities.minImageCount;
	}
	else if (surfaceCapabilities.maxImageCount != 0 &&
		desiredImageCount > surfaceCapabilities.maxImageCount) {
		desiredImageCount = surfaceCapabilities.maxImageCount;
	}

	VkExtent2D surfaceResolution = surfaceCapabilities.currentExtent;
	if (surfaceResolution.width == -1) {
		surfaceResolution.width = width;
		surfaceResolution.height = height;
	}
	else {
		width = surfaceResolution.width;
		height = surfaceResolution.height;
	}

	VkSurfaceTransformFlagBitsKHR preTransform = surfaceCapabilities.currentTransform;
	if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}

	uint32_t presentModeCount = 0;
	__vkGetPhysicalDeviceSurfacePresentModesKHR(context->physicalDevice, context->surface, &presentModeCount, NULL);
	VkPresentModeKHR* presentModes = (VkPresentModeKHR*)malloc(sizeof(VkPresentModeKHR) * presentModeCount);
	__vkGetPhysicalDeviceSurfacePresentModesKHR(context->physicalDevice, context->surface, &presentModeCount, presentModes);

	VkPresentModeKHR presentationMode = VK_PRESENT_MODE_FIFO_KHR;
	for (uint32_t i = 0; i < presentModeCount; ++i) {
		if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentationMode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
	}
	free(presentModes);

	VkSwapchainCreateInfoKHR swapChainCreateInfo;
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.pNext = NULL;
	swapChainCreateInfo.flags = 0;
	swapChainCreateInfo.surface = context->surface;
	swapChainCreateInfo.minImageCount = desiredImageCount;
	swapChainCreateInfo.imageFormat = colorFormat;
	swapChainCreateInfo.imageColorSpace = colorSpace;
	swapChainCreateInfo.imageExtent = surfaceResolution;
	swapChainCreateInfo.imageArrayLayers = 1;
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;   // <--
	swapChainCreateInfo.queueFamilyIndexCount = 0;
	swapChainCreateInfo.pQueueFamilyIndices = NULL;
	swapChainCreateInfo.preTransform = preTransform;
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.presentMode = presentationMode;
	swapChainCreateInfo.clipped = VK_TRUE;
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

	__vkCreateSwapchainKHR(context->device, &swapChainCreateInfo, NULL, &context->swapChain);

	__vkGetDeviceQueue(context->device, context->presentQueueIdx, 0, &context->queue);

	VkCommandPoolCreateInfo commandPoolCreateInfo;
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.pNext = NULL;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = context->presentQueueIdx;

	__vkCreateCommandPool(context->device, &commandPoolCreateInfo, NULL, &context->commandPool);

	VkCommandBufferAllocateInfo commandBufferAllocationInfo;
	commandBufferAllocationInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocationInfo.pNext = NULL;
	commandBufferAllocationInfo.commandPool = context->commandPool;
	commandBufferAllocationInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocationInfo.commandBufferCount = 1;

	__vkAllocateCommandBuffers(context->device, &commandBufferAllocationInfo, &context->cmdBuffer);

	uint32_t imageCount = 0;
	__vkGetSwapchainImagesKHR(context->device, context->swapChain, &imageCount, NULL);
	context->presentImages = (VkImage*)malloc(sizeof(VkImage) * imageCount);
	__vkGetSwapchainImagesKHR(context->device, context->swapChain, &imageCount, context->presentImages);

	VkImageViewCreateInfo presentImagesViewCreateInfo;
	presentImagesViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	presentImagesViewCreateInfo.pNext = NULL;
	presentImagesViewCreateInfo.flags = 0;
	presentImagesViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	presentImagesViewCreateInfo.format = colorFormat;
	presentImagesViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
	presentImagesViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
	presentImagesViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
	presentImagesViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
	presentImagesViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	presentImagesViewCreateInfo.subresourceRange.baseMipLevel = 0;
	presentImagesViewCreateInfo.subresourceRange.levelCount = 1;
	presentImagesViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	presentImagesViewCreateInfo.subresourceRange.layerCount = 1;

	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = NULL;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	beginInfo.pInheritanceInfo = NULL;

	VkFenceCreateInfo fenceCreateInfo;
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.pNext = NULL;
	fenceCreateInfo.flags = 0;
	VkFence submitFence;
	__vkCreateFence(context->device, &fenceCreateInfo, NULL, &submitFence);

	VkImageView* presentImageViews = (VkImageView*)malloc(sizeof(VkImageView) * imageCount);
	for (uint32_t i = 0; i < imageCount; ++i) {
		presentImagesViewCreateInfo.image = context->presentImages[i];

		__vkBeginCommandBuffer(context->cmdBuffer, &beginInfo);

		VkImageMemoryBarrier layoutTransitionBarrier;
		layoutTransitionBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		layoutTransitionBarrier.pNext = NULL;
		layoutTransitionBarrier.srcAccessMask = 0;
		layoutTransitionBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		layoutTransitionBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		layoutTransitionBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		layoutTransitionBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		layoutTransitionBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		layoutTransitionBarrier.image = context->presentImages[i];
		layoutTransitionBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		layoutTransitionBarrier.subresourceRange.baseMipLevel = 0;
		layoutTransitionBarrier.subresourceRange.levelCount = 1;
		layoutTransitionBarrier.subresourceRange.baseArrayLayer = 0;
		layoutTransitionBarrier.subresourceRange.layerCount = 1;

		__vkCmdPipelineBarrier(context->cmdBuffer,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			0,
			0, NULL,
			0, NULL,
			1, &layoutTransitionBarrier);

		__vkEndCommandBuffer(context->cmdBuffer);

		VkPipelineStageFlags waitStageMask[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSubmitInfo submitInfo;
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pNext = NULL;
		submitInfo.waitSemaphoreCount = 0;
		submitInfo.pWaitSemaphores = NULL;
		submitInfo.pWaitDstStageMask = waitStageMask;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &context->cmdBuffer;
		submitInfo.signalSemaphoreCount = 0;
		submitInfo.pSignalSemaphores = NULL;
		__vkQueueSubmit(context->queue, 1, &submitInfo, submitFence);

		__vkWaitForFences(context->device, 1, &submitFence, VK_TRUE, UINT64_MAX);
		__vkResetFences(context->device, 1, &submitFence);

		__vkResetCommandBuffer(context->cmdBuffer, 0);

		__vkCreateImageView(context->device, &presentImagesViewCreateInfo, NULL, &presentImageViews[i]);
	}

	return context;
}

void renderVk(Swin_Vk* ctx) {
	ContextVK* context = (ContextVK*)ctx;

	uint32_t nextImageIdx;
	__vkAcquireNextImageKHR(context->device, context->swapChain, UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE, &nextImageIdx);

	VkPresentInfoKHR presentInfo;
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.pNext = NULL;
	presentInfo.waitSemaphoreCount = 0;
	presentInfo.pWaitSemaphores = NULL;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &context->swapChain;
	presentInfo.pImageIndices = &nextImageIdx;
	presentInfo.pResults = NULL;
	__vkQueuePresentKHR(context->queue, &presentInfo);
}

#endif
