
#include "..\include\winrtsock.h"
#include "Include\MiscUtils.h"
#include "Include\InternalSocketManagement.h"

//From http://blogs.msdn.com/b/shawnhar/archive/2012/03/12/createthread-for-windows-8-metro.aspx

void __stdcall Sleep(_In_ DWORD dwMilliseconds)
{
    static HANDLE singletonEvent = nullptr;

    HANDLE sleepEvent = singletonEvent;

    // Demand create the event.
    if (!sleepEvent)
    {
        sleepEvent = CreateEventEx(nullptr, nullptr, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS);

        if (!sleepEvent)
            return;

        HANDLE previousEvent = InterlockedCompareExchangePointerRelease(&singletonEvent, sleepEvent, nullptr);

        if (previousEvent)
        {
            // Back out if multiple threads try to demand create at the same time.
            CloseHandle(sleepEvent);
            sleepEvent = previousEvent;
        }
    }

    // Emulate sleep by waiting with timeout on an event that is never signaled.
    WaitForSingleObjectEx(sleepEvent, dwMilliseconds, false);

    return;
}