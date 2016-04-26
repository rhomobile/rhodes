#pragma once

#include "..\..\Common\Public\PB_Defines.h"
#include "..\..\Common\Public\PBplugin.h"
#include "CriticalSect.h"
//set based on the events that are presented to the core via the framework 
#define SYNCARRAYSIZE 9
#define	THREAD_QUIT				1
#define THREAD_EVENT			2



/*

 The purpose of this class is to synchronise many handlers to one event
 when an event is being waited on by several threads,this class will raise 
 another event to indicate that the primary event has been handled.
 Naturally, this requires some cooperation from the waiting threads

1. Core:Create an event table for each event- Involves submission of our primary event, which returns a new event that signals when all plugins have handled the primary event.

2. Plugin:Each plugin shall call 'RegisterForEvent', thus incrementing the internal counter.
3. Core: Call 'StartWaitEvent' 
*/


//one for each primary event
struct EventRecord{
	HANDLE		hEvTrigger;		//event created here, that gets triggered depending on dwTriggerModesetting.
	DWORD		dwTriggerMode;	//determines a method for triggering
	DWORD		dwEventType;
	//CRITICAL_SECTION	m_CriticalSection;//The critical section ensures that one thread alters the counter at a time.
	HANDLE	m_hQuitEvent;
	HANDLE		hThread;
};

//one for each registered module
struct plgCallback{
	PBModule	*pObj;			//pointer to the module object
	DWORD		dwRegister;		//DWORD event list 
	plgCallback	*pNext;			//pointer to the structure in the linked list
	LPCTSTR		pModName;		//pointer to the name of the module
};


class CSync;//forward declaration

struct syncThreadParam{
	LPARAM		lParam1;
	LPARAM		lParam2;
	int			iType;
};


class CSync:public Lockable
{
public:
	CSync(void);
	~CSync(void);
	void	InitCB(PPBCORESTRUCT pbCoreStruct);
	HANDLE	CreateSyncEvent(int iEvent);
	BOOL	Run(int iEvent,LPARAM lParam1,LPARAM lParam2);
	
	//this is a synchronous way of calling each handler
	BOOL RunSync(int iEvent,LPARAM lParam1,LPARAM lParam2);
	
	BOOL	RegisterCallback(PBModule *pObj,PB_SyncEvent dwEvent,LPCTSTR pCallingModule);
	BOOL	UnRegisterCallback(PBModule *pObj,DWORD dwEvent);
protected:
	DWORD	DispatchThreadImp();							//Dispatch Thread
	plgCallback		*deletemem(plgCallback *pCallbackRec);	//deletes the linked list of callback modules
	static DWORD WINAPI	RunThread(LPVOID lparam);
	static DWORD WINAPI DispatchThread(LPVOID lParam);

	DWORD			m_DispatchID;							//Dispatch Thread ID
	HANDLE			m_hDispatch;							//Disaptch Thread Handle
	LONG			m_lRegLock;								//Registration List Lock

	EventRecord		m_ArrEventRec[SYNCARRAYSIZE];
	HANDLE			m_hQuitEvent;
	EventRecord		*m_pHeadEventRec;
	plgCallback		*m_pHeadCallback;
	PPBCORESTRUCT	m_pbCoreStruct;
	
};
