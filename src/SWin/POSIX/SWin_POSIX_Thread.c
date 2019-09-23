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
    CHECK(callback, "callback was NULL", NULL);
    
	SWin_POSIX_Thread* result = ALLOC_S(SWin_POSIX_Thread);
    CHECK(result, "Failed to allocate SWin_POSIX_Thread", NULL);

	void** datas = ALLOC(void*, 2);
    CHECK(datas, "Failed to allocate void**", NULL);
    
	datas[0] = callback;
	datas[1] = data;

	pthread_create(&result->thread, NULL, SWin_POSIX_Thread_ThreadFunction, datas);
    CHECK(result->thread, "pthread_create() failed", NULL);

	return result;
}

SResult swWaitForThread(SThread* thread) {
    CHECK(thread, "thread was NULL", SWIN_FAILED);
    
	pthread_join(((SWin_POSIX_Thread*)thread)->thread, NULL);
    
    return SWIN_OK;
}

SResult swDestroyThread(SThread* thread) {
    CHECK(thread, "thread was NULL", SWIN_FAILED);
    
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
    CHECK(mutex, "mutex was NULL", SWIN_FAILED);
    
	pthread_mutex_lock(&((SWin_POSIX_Mutex*)mutex)->mutex);
    
    return SWIN_OK;
}

SResult swUnlockMutex(SMutex* mutex) {
    CHECK(mutex, "mutex was NULL", SWIN_FAILED);
    
	pthread_mutex_unlock(&((SWin_POSIX_Mutex*)mutex)->mutex);
    
    return SWIN_OK;
}

SResult swDestroyMutex(SMutex* mutex) {
    CHECK(mutex, "mutex was NULL", SWIN_FAILED);
    
	pthread_mutex_destroy(&((SWin_POSIX_Mutex*)mutex)->mutex);
    
    return SWIN_OK;
}

void swSleep(uint32_t milliSeconds) {
    usleep(milliSeconds * 1000);
}
