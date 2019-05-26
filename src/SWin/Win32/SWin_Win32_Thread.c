#include <swin/SWin.h>
#include <swin/SWin_Win32.h>

DWORD WINAPI SWin_Win32_Thread_ThreadFunction(LPVOID lpParam) {
	void** datas = (void**)lpParam;

	pfnSThreadCallback callback = (pfnSThreadCallback)datas[0];

	int32_t result = callback(datas[1]);

	DEALLOC(datas);

	return result;
}

SThread* swCreateThread(pfnSThreadCallback callback, void* data) {
	CHECK(callback, "callback was NULL", NULL);

	SWin_Win32_Thread* result = ALLOC_S(SWin_Win32_Thread);
	CHECK(result, "Failed to allocate SWin_Win32_Thread", NULL);

	void** datas = ALLOC(void*, 2);
	CHECK(datas, "Failed to allocate void**", NULL);

	datas[0] = callback;
	datas[1] = data;

	result->handle = CreateThread(NULL, 0, SWin_Win32_Thread_ThreadFunction, datas, 0, &result->threadId);
	CHECK(result->handle, "CreateThread() failed", NULL);

	return result;
}

SResult swWaitForThread(SThread* thread) {
	CHECK(thread, "thread was NULL", SWIN_FAILED);

	SWin_Win32_Thread* _thread = (SWin_Win32_Thread*)thread;
	
	WaitForSingleObject(_thread->handle, INFINITE);

	return SWIN_OK;
}

SResult swDestroyThread(SThread* thread) {
	CHECK(thread, "thread was NULL", SWIN_FAILED);
	
	SWin_Win32_Thread* _thread = (SWin_Win32_Thread*)thread;
	
	CloseHandle(_thread->handle);
	DEALLOC(_thread);

	return SWIN_OK;
}

SMutex* swCreateMutex() {
	SWin_Win32_Mutex* result = ALLOC_S(SWin_Win32_Mutex);
	CHECK(result, "Failed to allocate SWin_Win32_Mutex", NULL);

	result->handle = CreateMutex(NULL, FALSE, NULL);
	CHECK(result->handle, "CreateMutex() failed", NULL);

	return result;
}

SResult swLockMutex(SMutex* mutex) {
	CHECK(mutex, "mutex was NULL", SWIN_FAILED);

	SWin_Win32_Mutex* _mutex = (SWin_Win32_Mutex*)mutex;

	WaitForSingleObject(_mutex->handle, INFINITE);

	return SWIN_OK;
}

SResult swUnlockMutex(SMutex* mutex) {
	CHECK(mutex, "mutex was NULL", SWIN_FAILED);

	SWin_Win32_Mutex* _mutex = (SWin_Win32_Mutex*)mutex;

	ReleaseMutex(_mutex->handle);

	return SWIN_OK;
}

SResult swDestroyMutex(SMutex* mutex) {
	CHECK(mutex, "mutex was NULL", SWIN_FAILED);

	SWin_Win32_Mutex* _mutex = (SWin_Win32_Mutex*)mutex;

	CloseHandle(_mutex->handle);
	DEALLOC(_mutex);

	return SWIN_OK;
}