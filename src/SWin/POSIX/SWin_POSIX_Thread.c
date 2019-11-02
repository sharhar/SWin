#include <swin/SWin.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

typedef struct SWin_POSIX_Thread {
	pthread_t thread;
} SWin_POSIX_Thread;

typedef struct SWin_POSIX_Mutex {
	pthread_mutex_t mutex;
	pthread_mutexattr_t attr;
} SWin_POSIX_Mutex;

void* SWin_POSIX_Thread_ThreadFunction(void* data) {
	void** datas = (void**)data;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	pfnSThreadCallback callback = (pfnSThreadCallback)datas[0];

	int32_t result = callback(datas[1]);

	free(datas);

	return result;
}

SThread* swCreateThread(pfnSThreadCallback callback, void* data) {
	SWin_POSIX_Thread* result = (SWin_POSIX_Thread*)malloc(sizeof(SWin_POSIX_Thread));

	void** datas = malloc(sizeof(void*) * 2);
	datas[0] = callback;
	datas[1] = data;

	pthread_create(&result->thread, NULL, SWin_POSIX_Thread_ThreadFunction, datas);

	return result;
}

SResult swWaitForThread(SThread* thread) {
	pthread_join(((SWin_POSIX_Thread*)thread)->thread, NULL);
    
    return SWIN_OK;
}

SResult swDestroyThread(SThread* thread) {
	pthread_cancel(((SWin_POSIX_Thread*)thread)->thread);
    
    return SWIN_OK;
}

SMutex* swCreateMutex() {
	SWin_POSIX_Mutex* result = (SWin_POSIX_Mutex*)malloc(sizeof(SWin_POSIX_Mutex));
	memset(result, 0, sizeof(SWin_POSIX_Mutex));

	pthread_mutexattr_init(&result->attr);
	pthread_mutexattr_settype(&result->attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&result->mutex, &result->attr);

	return result;
}

SResult swLockMutex(SMutex* mutex) {
	pthread_mutex_lock(&((SWin_POSIX_Mutex*)mutex)->mutex);
    
    return SWIN_OK;
}

SResult swUnlockMutex(SMutex* mutex) {
	pthread_mutex_unlock(&((SWin_POSIX_Mutex*)mutex)->mutex);
    
    return SWIN_OK;
}

SResult swDestroyMutex(SMutex* mutex) {
	pthread_mutex_destroy(&((SWin_POSIX_Mutex*)mutex)->mutex);
    
    return SWIN_OK;
}

void swSleep(uint32_t milliSeconds) {
    usleep(milliSeconds * 1000);
}
