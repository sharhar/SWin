#include "../../../include/swin/SWin.h"
#include "../../../include/swin/SWin_X11.h"
#include <pthread.h>

typedef struct SWin_X11_Thread {
	pthread_t thread;
} SWin_X11_Thread;

typedef struct SWin_X11_Mutex {
	pthread_mutex_t mutex;
	pthread_mutexattr_t attr;
} SWin_X11_Mutex;

void* SWin_X11_Thread_ThreadFunction(void* data) {
	void** datas = (void**)data;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

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

SMutex* swCreateMutex() {
	SWin_X11_Mutex* result = (SWin_X11_Mutex*)malloc(sizeof(SWin_X11_Mutex));
	memset(result, 0, sizeof(SWin_X11_Mutex));

	pthread_mutexattr_init(&result->attr);
	pthread_mutexattr_settype(&result->attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&result->mutex, &result->attr);

	return result;
}

void swLockMutex(SMutex* mutex) {
	pthread_mutex_lock(&((SWin_X11_Mutex*)mutex)->mutex);
}

void swUnlockMutex(SMutex* mutex) {
	pthread_mutex_unlock(&((SWin_X11_Mutex*)mutex)->mutex);
}

void swDestroyMutex(SMutex* mutex) {
	pthread_mutex_destroy(&((SWin_X11_Mutex*)mutex)->mutex);
}
