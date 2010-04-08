//-----------------------------------------------------------------------------
// Include/spirit_system.h
//
// Common definitions for SPIRIT System Library
//
// Copyright (C) 2008, SPIRIT
//-----------------------------------------------------------------------------

#ifndef __SPIRIT_INCLUDE_SYSTEM_H
#define __SPIRIT_INCLUDE_SYSTEM_H

#include "common/spirit_comtypes.h"

#ifdef SPIRIT_OS_WIN32
  #include <windows.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// Export/Import defines

#define SPIRIT_STATIC

#if defined(SPIRIT_STATIC) || !defined(SPIRIT_SYSTEM_DYNAMIC)
#define SPIRIT_SYSTEM_SYMBOL
#else // SPIRIT_STATIC

#if defined(SPIRIT_SYSTEM_EXPORTS)
#define SPIRIT_SYSTEM_SYMBOL SPIRIT_EXPORT_SYMBOL
#else
#define SPIRIT_SYSTEM_SYMBOL SPIRIT_IMPORT_SYMBOL
#endif

#endif // SPIRIT_STATIC

///////////////////////////////////////////////////////////////////////////////
// Spirit common types

#ifndef NULL
#define NULL 0
#endif // NULL

//#ifndef BOOL
//#define BOOL int
//#endif // BOOL

#ifndef TRUE
#define TRUE 1
#endif // TRUE

#ifndef FALSE
#define FALSE 0
#endif // FALSE

#ifdef SPIRIT_OS_WIN32
#ifndef snprintf
#define snprintf _snprintf
#endif // snprintf
#ifndef vsnprintf
#define vsnprintf _vsnprintf
#endif // vsnprintf
#else
#ifndef stricmp
#define stricmp strcasecmp
#endif // stricmp
#ifndef strnicmp
#define strnicmp strncasecmp
#endif // strnicmp
#endif // SPIRIT_COMPILER_MSVC

///////////////////////////////////////////////////////////////////////////////
// Spirit System defines

#ifdef  SPIRIT_OS_WIN32

#define SPIRIT_INFINITE INFINITE

#define SPIRIT_MAXIMUM_WAIT_OBJECTS MAXIMUM_WAIT_OBJECTS

#define SPIRIT_WAIT_FAILED    WAIT_FAILED
#define SPIRIT_WAIT_TIMEOUT   WAIT_TIMEOUT
#define SPIRIT_WAIT_OBJECT_0  WAIT_OBJECT_0
#define SPIRIT_WAIT_ABANDON_0 WAIT_ABANDONED_0

#define SPIRIT_WAIT_OBJECT    SPIRIT_WAIT_OBJECT_0
#define SPIRIT_WAIT_ABANDON   SPIRIT_WAIT_ABANDON_0

#define SPIRIT_THREAD_PRIORITY_IDLE          THREAD_PRIORITY_IDLE
#define SPIRIT_THREAD_PRIORITY_LOWEST        THREAD_PRIORITY_LOWEST
#define SPIRIT_THREAD_PRIORITY_BELOW_NORMAL  THREAD_PRIORITY_BELOW_NORMAL
#define SPIRIT_THREAD_PRIORITY_NORMAL        THREAD_PRIORITY_NORMAL
#define SPIRIT_THREAD_PRIORITY_ABOVE_NORMAL  THREAD_PRIORITY_ABOVE_NORMAL
#define SPIRIT_THREAD_PRIORITY_HIGHEST       THREAD_PRIORITY_HIGHEST
#define SPIRIT_THREAD_PRIORITY_TIME_CRITICAL THREAD_PRIORITY_TIME_CRITICAL

#define SPIRIT_THREAD_PRIORITY_MIN SPIRIT_THREAD_PRIORITY_IDLE
#define SPIRIT_THREAD_PRIORITY_MAX SPIRIT_THREAD_PRIORITY_TIME_CRITICAL

#elif defined(SPIRIT_OS_DSP_BIOS)
#include <sys.h>
#define SPIRIT_INFINITE SYS_FOREVER

// Maximum events count in one wait operation
#define SPIRIT_MAXIMUM_WAIT_OBJECTS 64

// Wait result constants
#define SPIRIT_WAIT_FAILED    -1
#define SPIRIT_WAIT_TIMEOUT   0
#define SPIRIT_WAIT_OBJECT_0  1
#define SPIRIT_WAIT_ABANDON_0 (SPIRIT_WAIT_OBJECT_0 + SPIRIT_MAXIMUM_WAIT_OBJECTS)

#define SPIRIT_WAIT_OBJECT    SPIRIT_WAIT_OBJECT_0
#define SPIRIT_WAIT_ABANDON   SPIRIT_WAIT_ABANDON_0


//TSK priority cannot be set to 0
//because it is reserved priorioty value for IDL threads
#define SPIRIT_THREAD_PRIORITY_IDLE          1
#define SPIRIT_THREAD_PRIORITY_LOWEST        3
#define SPIRIT_THREAD_PRIORITY_BELOW_NORMAL  6
#define SPIRIT_THREAD_PRIORITY_NORMAL        8
#define SPIRIT_THREAD_PRIORITY_ABOVE_NORMAL  10
#define SPIRIT_THREAD_PRIORITY_HIGHEST       12
#define SPIRIT_THREAD_PRIORITY_TIME_CRITICAL 15

//the following value is defined as 1
#define SPIRIT_THREAD_PRIORITY_MIN TSK_MINPRI
//the following value is defined as 15
#define SPIRIT_THREAD_PRIORITY_MAX TSK_MAXPRI

#else

// Infinite timeout constant
#define SPIRIT_INFINITE 0xffffffff

// Maximum events count in one wait operation
#define SPIRIT_MAXIMUM_WAIT_OBJECTS 64

// Wait result constants
#define SPIRIT_WAIT_FAILED    -1
#define SPIRIT_WAIT_TIMEOUT   0
#define SPIRIT_WAIT_OBJECT_0  1
#define SPIRIT_WAIT_ABANDON_0 (SPIRIT_WAIT_OBJECT_0 + SPIRIT_MAXIMUM_WAIT_OBJECTS)

#define SPIRIT_WAIT_OBJECT    SPIRIT_WAIT_OBJECT_0
#define SPIRIT_WAIT_ABANDON   SPIRIT_WAIT_ABANDON_0

// Thread priority constants
#define SPIRIT_THREAD_PRIORITY_IDLE          -3
#define SPIRIT_THREAD_PRIORITY_LOWEST        -2
#define SPIRIT_THREAD_PRIORITY_BELOW_NORMAL  -1
#define SPIRIT_THREAD_PRIORITY_NORMAL        0
#define SPIRIT_THREAD_PRIORITY_ABOVE_NORMAL  1
#define SPIRIT_THREAD_PRIORITY_HIGHEST       2
#define SPIRIT_THREAD_PRIORITY_TIME_CRITICAL 3

#define SPIRIT_THREAD_PRIORITY_MIN SPIRIT_THREAD_PRIORITY_IDLE
#define SPIRIT_THREAD_PRIORITY_MAX SPIRIT_THREAD_PRIORITY_TIME_CRITICAL

#endif
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// BEGIN: SPIRIT System declaration
//-----------------------------------------------------------------------------
#ifndef tSystemHandle_DEFINED
#define tSystemHandle_DEFINED
typedef void* tSystemHandle;
#endif // tSystemHandle_DEFINED

SPIRIT_NAMESPACE_BEGIN(SpiritSystem)

//-----------------------------------------------------------------------------
// Interlocked
#ifndef SPIRIT_OS_DSP_BIOS
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  int32 SPIRIT_METHOD_CALL_TYPE Interlocked_ExchangeAdd(volatile int32* pResult, int32 value);
#endif
//-----------------------------------------------------------------------------
// Critical Section

SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tSystemHandle SPIRIT_METHOD_CALL_TYPE CriticalSection_Create(void);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE CriticalSection_Destroy(tSystemHandle handle);

SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE CriticalSection_Lock(tSystemHandle handle);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE CriticalSection_TryLock(tSystemHandle handle);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE CriticalSection_Unlock(tSystemHandle handle);

//-----------------------------------------------------------------------------
// Wait

SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  int32 SPIRIT_METHOD_CALL_TYPE Event_Wait(tSystemHandle event, uint32 timeOut);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  int32 SPIRIT_METHOD_CALL_TYPE Event_WaitMultiple(uint32 count, const tSystemHandle* events, uint32 timeOut);

//-----------------------------------------------------------------------------
// Event

SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tSystemHandle SPIRIT_METHOD_CALL_TYPE Event_Create(tBool manualReset, tBool initState);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Event_Destroy(tSystemHandle event);

SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Event_Set(tSystemHandle event);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Event_Reset(tSystemHandle event);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Event_Pulse(tSystemHandle event);

//-----------------------------------------------------------------------------
// Semaphore

SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tSystemHandle SPIRIT_METHOD_CALL_TYPE Semaphore_Create(uint32 initialCount, uint32 maxCount);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Semaphore_Destroy(tSystemHandle semaphore);

SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Semaphore_Release(tSystemHandle semaphore, uint32 count, uint32* oldCount);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Semaphore_Reset(tSystemHandle semaphore);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Semaphore_Wait(tSystemHandle semaphore, uint32 timeOut);

//-----------------------------------------------------------------------------
// Waitable Timer

SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tSystemHandle SPIRIT_METHOD_CALL_TYPE WaitableTimer_Create(tBool manualReset);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE WaitableTimer_Destroy(tSystemHandle timer);

#ifndef SPIRIT_OS_DSP_BIOS
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE WaitableTimer_Set(tSystemHandle timer, int64 startTime, uint32 period);
#endif
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE WaitableTimer_Cancel(tSystemHandle timer);

/////////////////////////////////////////////////////////////////////////////////////////
// LightThread

#define SPIRIT_LIGTH_THREAD_RETURN_TYPE void*
typedef SPIRIT_LIGTH_THREAD_RETURN_TYPE (SPIRIT_CDECL *tLightThreadProc)(void* pParam);
#define SPIRIT_LIGTH_THREAD_RETURN return 0;

// Start light thread
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE LightThread_Create(tLightThreadProc Proc, void* pParam);

//-----------------------------------------------------------------------------
// Thread

typedef void (SPIRIT_METHOD_CALL_TYPE *tThreadProc)(tSystemHandle handle, void*);

// Yield execution to another thread
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  void SPIRIT_METHOD_CALL_TYPE Thread_Yield(void);
// Suspends the execution of the current thread
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  void SPIRIT_METHOD_CALL_TYPE Thread_Sleep(uint32 milliseconds);
// Get current thread Id
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  uint32 SPIRIT_METHOD_CALL_TYPE Thread_CurrentId(void);
// Get current thread priority
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  int32 SPIRIT_METHOD_CALL_TYPE Thread_GetCurrentPriority(void);
// Set thread priority
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Thread_SetCurrentPriority(int32 priority);
// Set thread priority boost
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Thread_SetCurrentPriorityBoost(tBool boost);
// Get thread priority
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  int32 SPIRIT_METHOD_CALL_TYPE Thread_GetPriority(tSystemHandle thread);
// Set thread priority
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Thread_SetPriority(tSystemHandle thread, int32 priority);

SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Thread_IsRunning(uint32 ThreadId);


#ifdef SPIRIT_OS_DSP_BIOS
//the following value specifiesthe default TASK stack size
//the valid value should be determined
//warning: you should rebuild lib to cause value take effect
#define SPIRIT_DSPBIOS_THREAD_DEFAULT_STACK_SIZE (1024*8*8)
#endif
// Start thread
//Warning: first argument of the thread proc would be allways set to NULL
//for the DSP/BIOS tasks
//you can use Thread_GetID to obtain thread handle (this is the same thing for the DSP/BIOS)
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Thread_Create(tThreadProc proc, void* param, tSystemHandle *pResult);
// Close thread descriptor
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Thread_Close(tSystemHandle thread);
#ifndef SPIRIT_OS_DSP_BIOS
//there are no reasonable way to kill the thread in the DSP/BIOS
// Kill thread and close descriptor
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Thread_Kill(tSystemHandle thread);
#endif
// Get thread identifier
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  uint32 SPIRIT_METHOD_CALL_TYPE Thread_GetId(tSystemHandle thread);

//-----------------------------------------------------------------------------
// Utils

SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  uint32 SPIRIT_METHOD_CALL_TYPE Time_GetMillisecond(void);
#ifndef SPIRIT_OS_DSP_BIOS
//VVK
//there are no uint64 under TMS C64XX
//should we use something else?
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  uint32 SPIRIT_METHOD_CALL_TYPE Time_GetMicrosecond(uint64* lres);
#endif
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  uint32 SPIRIT_METHOD_CALL_TYPE Time_GetTickCount(void);

#ifndef SPIRIT_OS_DSP_BIOS
//VVK
//there are no uint64 under TMS C64XX
//should we use something else?
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  void SPIRIT_METHOD_CALL_TYPE Time_MicrosecondToNTP(uint32* pSec, uint32* pFrac, const uint64* pMicroseconds);
#endif
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  void SPIRIT_METHOD_CALL_TYPE Time_GetNTP(uint32* pSec, uint32* pFrac);
#ifndef SPIRIT_OS_DSP_BIOS
	//VVK
	//those functions are not supported for now
	//there are no uint64 under TMS C64XX
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  void SPIRIT_METHOD_CALL_TYPE Time_GetSystem(tTimeInfo* pInfo);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  void SPIRIT_METHOD_CALL_TYPE Time_SystemToMicrosecond(const tTimeInfo* pInfo, uint64* pSysTime);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  void SPIRIT_METHOD_CALL_TYPE Time_MicrosecondToSystem(const uint64* pSysTime, tTimeInfo* pInfo);
#endif
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  uint32 SPIRIT_METHOD_CALL_TYPE Info_GetCPUCount(void);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  uint32 SPIRIT_METHOD_CALL_TYPE Info_GetComputerName(tChar* name, uint32 size);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  uint32 SPIRIT_METHOD_CALL_TYPE Info_GetUserName(tChar* name, uint32 size);
#ifndef SPIRIT_OS_DSP_BIOS
	//VVK
	//those functions are not supported for now
	//there are no uint64 under TMS C64XX
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tBool SPIRIT_METHOD_CALL_TYPE Info_GetProcessTimes(uint64* pSysTime, uint64* pKernelTime, uint64* pUserTime);
#endif

SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  void SPIRIT_METHOD_CALL_TYPE Debug_OutStr(tConstString str);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  void SPIRIT_METHOD_CALL_TYPE Debug_OutStrFmt(tConstString strFmt, ...);

SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  tSystemHandle SPIRIT_METHOD_CALL_TYPE Module_Load(tConstString name);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  void SPIRIT_METHOD_CALL_TYPE Module_Unload(tSystemHandle module);
SPIRIT_EXTERN_C SPIRIT_SYSTEM_SYMBOL
  void* SPIRIT_METHOD_CALL_TYPE Module_GetPtr(tSystemHandle module, tConstString name);

///////////////////////////////////////////////////////////////////////////////
// System Wrappers

//-----------------------------------------------------------------------------
// Critical Section
#ifdef SPIRIT_CPLUSPLUS
#ifndef SPIRIT_OS_DSP_BIOS
//C++ features are disabled the DSP/BIOS
//due to limited time amount

class tCritSec
{
public:
  tCritSec() : m_Handle(CriticalSection_Create()) { }
  ~tCritSec() { CriticalSection_Destroy(m_Handle); m_Handle = 0; }

  tBool Lock() { return CriticalSection_Lock(m_Handle); }
  tBool Unlock() { return CriticalSection_Unlock(m_Handle); }

  tBool TryLock() { return false; }
private:
  tSystemHandle m_Handle;

  tCritSec(const tCritSec&) : m_Handle(0) { }
  tCritSec& operator= (const tCritSec&) { return *this; }
};

class tLock
{
public:
  tLock() : m_pSec(0) { }
  tLock(tCritSec *pSec) : m_pSec(pSec) { if (m_pSec) m_pSec->Lock(); }
  tLock(tCritSec &Sec) : m_pSec(&Sec) { m_pSec->Lock(); }
  tLock(const tLock &other) : m_pSec(other.m_pSec) { if (m_pSec) m_pSec->Lock(); }
  ~tLock() { Release(); }

  tLock& operator= (tCritSec* pSec)
    { if (m_pSec != pSec) Assign(pSec); return *this; }
  tLock& operator= (tCritSec& rSec)
    { if (m_pSec != &rSec) Assign(&rSec); return *this; }
  tLock& operator= (const tLock &other)
    { if (this != &other) Assign(other.m_pSec); return *this; }

  void Assign(tCritSec *pSec)
    { if (m_pSec) m_pSec->Unlock(); m_pSec = pSec; if (m_pSec) m_pSec->Lock(); }
  void Assign(tCritSec &rSec) { Assign(&rSec); }
  void Release() { if (!m_pSec) return; m_pSec->Unlock(); m_pSec = 0; }

  tCritSec* get() const { return m_pSec; }

private:
  tCritSec *m_pSec;
};

//-----------------------------------------------------------------------------
// Event

class tEvent
{
public:
  tEvent(tBool manualReset = false, tBool initState = false)
    : m_Handle( Event_Create(manualReset, initState) ) {}
  ~tEvent() { Event_Destroy(m_Handle); m_Handle = 0; }

  tSystemHandle get() const { return m_Handle; }

  tBool Wait(uint32 timeout = SPIRIT_INFINITE)
    { return Event_Wait(m_Handle, timeout) == SPIRIT_WAIT_OBJECT_0; }
  tBool Set() { return Event_Set(m_Handle); }
  tBool Reset() { return Event_Reset(m_Handle); }
  tBool Pulse() { return Event_Pulse(m_Handle); }

private:
  tSystemHandle m_Handle;

  tEvent(const tEvent&) : m_Handle(0) { }
  tEvent& operator= (const tEvent&) { return *this; }
};

//-----------------------------------------------------------------------------
// Semaphore

class tSemaphore
{
public:
  tSemaphore(uint32 initCount, uint32 maxCount)
    : m_Handle( Semaphore_Create(initCount, maxCount) ) {}
  ~tSemaphore() { Semaphore_Destroy(m_Handle); m_Handle = 0; }

  tSystemHandle get() const { return m_Handle; }

  tBool Wait(uint32 timeout = SPIRIT_INFINITE)
#ifdef SPIRIT_OS_WIN32
  { return Event_Wait(m_Handle, timeout) == SPIRIT_WAIT_OBJECT_0; }
#else
  { return Semaphore_Wait(m_Handle, timeout) == SPIRIT_WAIT_OBJECT_0; }
#endif
  tBool Release(uint32 count = 1, uint32* oldCount = 0)
  { return Semaphore_Release(m_Handle, count, oldCount); }
  tBool Reset() { return Semaphore_Reset(m_Handle); }
  tBool Set() { return Semaphore_Release(m_Handle, 1, 0); }

private:
  tSystemHandle m_Handle;

  tSemaphore(const tSemaphore&) : m_Handle(0) { }
  tSemaphore& operator= (const tSemaphore&) { return *this; }
};

//-----------------------------------------------------------------------------
// Waitable Timer
#ifndef SPIRIT_OS_DSP_BIOS
	//VVK
	//this is disabled for the TMS
	//there are no uint64 under TMS C64XX

class tWaitableTimer
{
public:
  tWaitableTimer(tBool manualReset = false)
    : m_Handle( WaitableTimer_Create(manualReset) ) {}
  ~tWaitableTimer() { WaitableTimer_Destroy(m_Handle); m_Handle = 0; }

  tSystemHandle get() const { return m_Handle; }

  tBool Wait(uint32 timeout = SPIRIT_INFINITE)
    { return Event_Wait(m_Handle, timeout) == SPIRIT_WAIT_OBJECT_0; }

  tBool Set(int64 startTime, uint32 period)
    { return WaitableTimer_Set(m_Handle, startTime, period); }
  tBool SetOnes(uint32 timeout)
    { return WaitableTimer_Set(m_Handle, -1000*(int64)timeout, 0); }
  tBool SetPeriodic(uint32 period)
    { return WaitableTimer_Set(m_Handle, -1000*(int64)period, period); }
  tBool Cancel() { return WaitableTimer_Cancel(m_Handle); }

private:
  tSystemHandle m_Handle;

  tWaitableTimer(const tWaitableTimer&) : m_Handle(0) { }
  tWaitableTimer& operator= (const tWaitableTimer&) { return *this; }
};
#endif

//-----------------------------------------------------------------------------
// Thread

class tThreadObj
{
public:
  tThreadObj() : m_Handle(0) { }
  virtual ~tThreadObj() { CloseDesc(); }

  // Get descriptor
  tSystemHandle get() const { return m_Handle; }
  // Get identifier
  uint32 GetId() const { return Thread_GetId(m_Handle); }

  // Wait exit of the thread
  tBool WaitExit(uint32 timeout = SPIRIT_INFINITE)
    { return (Event_Wait(m_Handle, timeout) != SPIRIT_WAIT_TIMEOUT); }
  // Close thread descriptor
  void CloseDesc()
    { if (m_Handle) { Thread_Close(m_Handle); m_Handle = 0; } }
  // Start thread
  tSystemHandle Start()
  {
    CloseDesc();
    return Thread_Create(&tThreadObj::InternalThreadProc, this, &m_Handle) ? m_Handle : 0;
  }
  void TerminateThread()
  {
    if (!m_Handle)
      return;
    Thread_Kill(m_Handle);
    m_Handle = 0;
  }

  // Get thread priority
  int32 GetThreadPriority() const
  {
    if (m_Handle)
      return Thread_GetPriority(m_Handle);
    return SPIRIT_THREAD_PRIORITY_IDLE;
  }
  // Set thread priority
  tBool SetThreadPriority(int32 priority)
  {
    if (m_Handle)
      return Thread_SetPriority(m_Handle, priority);
    return false;
  }

protected:
  // Virtual object thread procedure (this function must be overridden)
  virtual void ThreadProc() = 0;

private:
  static void SPIRIT_METHOD_CALL_TYPE InternalThreadProc(tSystemHandle, void* p)
    { if (p) ((tThreadObj*)p)->ThreadProc(); }

  tSystemHandle m_Handle;
};



class tThreadObjEx : public tThreadObj
{
public:
  tThreadObjEx() : m_TerminateEvent(true, false) {}

  tBool CheckTerminate(uint32 timeout = 0) { return m_TerminateEvent.Wait(timeout); }
  tBool SetTerminate() { return m_TerminateEvent.Set(); }
  tBool WaitExit(uint32 timeout = SPIRIT_INFINITE, tBool terminate = true)
  {
    terminate = terminate; /* unused */
    m_TerminateEvent.Set();
    return tThreadObj::WaitExit(timeout);
  }

  tSystemHandle Start()
  {
    m_TerminateEvent.Reset();
    return tThreadObj::Start();
  }

  tBool StartThread()
  {
    return Start() != NULL;
  }

  void StopThread()
  {
    WaitExit();
    CloseDesc();
  }

protected:
  tEvent m_TerminateEvent;

  tThreadObjEx& operator = (const tThreadObjEx&);

  tThreadObjEx(const tThreadObjEx&);
};

#endif //SPIRIT_OS_DSP_BIOS
#endif // SPIRIT_CPLUSPLUS
//-----------------------------------------------------------------------------

SPIRIT_NAMESPACE_END(SpiritSystem)

//-----------------------------------------------------------------------------
// END: SPIRIT System declaration
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

#define SPIRIT_SYSTEM_NAMESPACE_ID(id) SPIRIT_NAMESPACE_ID(SpiritSystem, id)

//-----------------------------------------------------------------------------

SPIRIT_NAMESPACE_USEALL(SpiritSystem)

#endif // __SPIRIT_INCLUDE_SYSTEM_H
