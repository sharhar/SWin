#include "../../../include/swin/SWin.h"
#include "../../../include/swin/SWin_X11.h"
#include <pthread.h>

typedef struct SWin_X11_Thread {
	pthread_t thread;
} SWin_X11_Thread;

void* SWin_X11_Thread_ThreadFunction(void* data) {
	void** datas = (void**)data;

	pfnSThreadCallback callback = (pfnSThreadCallback)datas[0];

	int32_t result = callback(datas[1]);

	free(datas);

	return result;
}

SThread* swCreateThread(pfnSThreadCallback callback, void* data) {
	SWin_X11_Thread* result = (SWin_X11_Thread*)malloc(sizeof(SWin_X11_Thread));

	void** datas = malloc(sizeof(void*) * 2);
	datas[0] = callback;
	datas[1] = data;

	pthread_create(&result->thread, NULL, SWin_X11_Thread_ThreadFunction, datas);

	return result;
}

void swWaitForThread(SThread* thread) {
	pthread_join(((SWin_X11_Thread*)thread)->thread, NULL);
}

void swDestroyThread(SThread* thread) {
	pthread_cancel(((SWin_X11_Thread*)thread)->thread);
}