#include "IEEngineWatchDog.h"
#define THREAD_TERMINATION_TIME_OUT 5000

CEngineWatchDog::CEngineWatchDog(DWORD nWaitTimet,IEngineTimeOutHandler* pIf )
{
	m_nWaitTime = nWaitTimet;
	m_pTimeOutHandlerIf = pIf;
	m_ThreadID =0;
	m_ThreadHandle=NULL;
	m_StopEvent=  CreateEvent(NULL, FALSE, FALSE, NULL);
	CreateThread(NULL, 0, &CEngineWatchDog::ThereadProc, (LPVOID)this, 0, NULL)

}
CEngineWatchDog::~CEngineWatchDog()
{
	if(m_ThreadHandle)
	{
		SetEvent(m_StopEvent);
		if( WAIT_TIMEOUT == WaitForSingleObject(m_ThreadHandle, THREAD_TERMINATION_TIME_OUT))
		{
			TerminateThread(m_ThreadHandle, 0);
		}
		CloseHandle(m_ThreadHandle);
		m_ThreadHandle = NULL;
		if(m_StopEvent)
		{
			Closehandle(m_StopEvent);
			m_StopEvent = NULL;
		}
		
	}
}
DWORD WINAPI CEngineWatchDog::ThereadProc( LPVOID lpParameter )
{
	DWORD dwResult = WaitForSingleObject(m_StopEvent, m_nWaitTime)
	switch(dwResult)
	{
	case WAIT_TIMEOUT:
		{
			if(false == m_pTimeOutHandlerIf->OnEngineTimeOut(engineWatchDog::eNavtimeOut))
			{
				//don't stop the thread
				continue;
			}
			break;
		}
	case WAIT_OBJECT_0:
		{
		}
	case default:
		{
		}
	}
}
