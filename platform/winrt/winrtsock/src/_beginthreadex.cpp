
#include "..\include\winrtsock.h"

#include <Synchapi.h>
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;

HANDLE WINAPI _beginthreadex(LPSECURITY_ATTRIBUTES unusedThreadAttributes, SIZE_T unusedStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD unusedThreadId){
	// Create a handle for the new thread.

	HANDLE threadHandle = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);
	if (!threadHandle)
		return nullptr;

	try{
		auto workItemHandler = ref new WorkItemHandler([=](IAsyncAction^){
			lpStartAddress(lpParameter);
			SetEvent(threadHandle);         // Signal that the thread has completed (assuming this handle was not destroyed by the caller due to time-out).
		}, CallbackContext::Any);
		ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
		return threadHandle;                // Return the handle to the caller (he can wait for this handle until thread terminates).
	}
	catch (...){
		CloseHandle(threadHandle);  // Clean up if thread creation fails.
		return nullptr;
	}
}
