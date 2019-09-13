#include <swin/SWin.h>
#include <swin/SWin_X11_Base.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>
#include <string.h>

long __sWin_X11_milliSeconds;
time_t __sWin_X11_seconds;
struct timespec __sWin_X11_timespec;
double __sWin_X11_startTime = 0;

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

void swWaitForThread(SThread* thread) {
	pthread_join(((SWin_POSIX_Thread*)thread)->thread, NULL);
}

void swDestroyThread(SThread* thread) {
	pthread_cancel(((SWin_POSIX_Thread*)thread)->thread);

}

SMutex* swCreateMutex() {
	SWin_POSIX_Mutex* result = (SWin_POSIX_Mutex*)malloc(sizeof(SWin_POSIX_Mutex));
	memset(result, 0, sizeof(SWin_POSIX_Mutex));

	pthread_mutexattr_init(&result->attr);
	pthread_mutexattr_settype(&result->attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&result->mutex, &result->attr);

	return result;
}

void swLockMutex(SMutex* mutex) {
	pthread_mutex_lock(&((SWin_POSIX_Mutex*)mutex)->mutex);
}

void swUnlockMutex(SMutex* mutex) {
	pthread_mutex_unlock(&((SWin_POSIX_Mutex*)mutex)->mutex);
}

void swDestroyMutex(SMutex* mutex) {
	pthread_mutex_destroy(&((SWin_POSIX_Mutex*)mutex)->mutex);
}

void swSleep(uint32_t milliSeconds) {
    usleep(milliSeconds * 1000);
}

double _swGetRawTime() {
	clock_gettime(CLOCK_REALTIME, &__sWin_X11_timespec);

	__sWin_X11_seconds  = __sWin_X11_timespec.tv_sec;
	__sWin_X11_milliSeconds = __sWin_X11_timespec.tv_nsec;
	if (__sWin_X11_milliSeconds > 999999999) {
		__sWin_X11_seconds++;
		__sWin_X11_milliSeconds = 0;
	}

	return (double)(__sWin_X11_seconds + (((double)__sWin_X11_milliSeconds)/1000000000.0));
}

double swGetTime() {
	return _swGetRawTime() - __sWin_X11_startTime;
}