#include <swin/SWin.h>
#include <swin/SWin_Win32.h>

DWORD WINAPI SWin_Win32_Thread_ThreadFunction(LPVOID lpParam) {
	void** datas = (void**)lpParam;

	pfnSThreadCallback callback = (pfnSThreadCallback)datas[0];

	int32_t result = callback(datas[1]);

	free(datas);

	return result;
}

SThread* swCreateThread(pfnSThreadCallback callback, void* data) {
	SWin_Win32_Thread* result = (SWin_Win32_Thread*)malloc(sizeof(SWin_Win32_Thread));

	void** datas = malloc(sizeof(void*) * 2);
	datas[0] = callback;
	datas[1] = data;

	result->handle = CreateThread(NULL, 0, SWin_Win32_Thread_ThreadFunction, datas, 0, &result->threadId);

	return result;
}

void swWaitForThread(SThread* thread) {
	SWin_Win32_Thread* wthread = (SWin_Win32_Thread*)thread;
	WaitForSingleObject(wthread->handle, INFINITE);
}

void swDestroyThread(SThread* thread) {
	SWin_Win32_Thread* wthread = (SWin_Win32_Thread*)thread;
	CloseHandle(wthread->handle);
	free(thread);
}

SMutex* swCreateMutex() {
	return CreateMutex(NULL, FALSE, NULL);
}

void swLockMutex(SMutex* mutex) {
	WaitForSingleObject(mutex, INFINITE);
}

void swUnlockMutex(SMutex* mutex) {
	ReleaseMutex(mutex);
}

void swDestroyMutex(SMutex* mutex) {
	CloseHandle(mutex);
}