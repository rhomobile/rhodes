#pragma once
//Sabir VT
namespace engineWatchDog
{
	enum eTimeOutType
	{
		eNavtimeOut=0,
		eDocTimeOut,
		eUnknown
	};
	class IEngineTimeOutHandler
	{
		virtual bool OnEngineTimeOut(eTimeOutType eType)=0;

	};

	class CEngineWatchDog
	{
	public:
		CEngineWatchDog(DWORD nWaitTimet,IEngineTimeOutHandler* pIf );
		~CEngineWatchDog();
	private:
		DWORD m_nWaitTime;
		DWORD m_ThreadID;
		HANDLE m_ThreadHandle;
		HANDLE m_StopEvent;
		IEngineTimeOutHandler* m_pTimeOutHandlerIf;
		static DWORD WINAPI ThereadProc( LPVOID lpParameter );
	};
}