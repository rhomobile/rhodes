#include "stdafx.h"
#include "common/app_build_capabilities.h"

#if defined(APP_BUILD_CAPABILITY_WEBKIT_BROWSER) && !defined(APP_BUILD_CAPABILITY_MOTOROLA)
class CConfig
{
};

class CSync
{
public:	
	int Run(int a, long b,long c);
};

int CSync::Run(int a, long b,long c)
{
	return 1;
}

class CAppManager
{
public:	
	long SwitchAppInst(int iInstID);
};

long CAppManager::SwitchAppInst(int iInstID)
{
	return 1;
}

struct PBCoreStruct
{
};

CConfig *g_pConfig = 0;
CSync	*g_pEventSync = 0;
CAppManager	*g_pAppManager = 0;	
PBCoreStruct *g_pPBCore = 0;

typedef long IMOREF;

typedef struct VarsStruct{
	LPCTSTR		pStr;
	VarsStruct	*pNextVar;
}VARSTRUCT,*PVARSTRUCT;
typedef int (CALLBACK* IMOEVENTPROC)(PVARSTRUCT pVars,int iTABID,LPARAM lParam);

enum LogTypeInterface {
	PB_LOG_ERROR	= 0,	///< Show stopper
	PB_LOG_WARNING,	///< PB Can continue with reduced functionality
	PB_LOG_INFO,		///< General Information, e.g. Plugin X loaded
	PB_LOG_USER,
	PB_LOG_DEBUG,	///< the only message to show during debug
	PB_LOG_MEMORY
};

typedef struct PBStruct
{
	HWND			hWnd;				///<  Handle to the instance topmost window 
	HINSTANCE		hInstance;			///<  Main window instance
	BOOL			bInvoked;			///<  Whether the associated Meta Tag has been manually invoked
	int				iTabID;				///<  PB Application ID
	
}PBSTRUCT,*PPBSTRUCT;

BOOL	DeleteIMO		(IMOREF IMORef){ return TRUE;}
BOOL	CallIMOMethod	(IMOREF IMORef,LPCTSTR pMethod){ return TRUE;}
BOOL    SetIMOCallBack  (IMOREF IMORef,IMOEVENTPROC IMOEventProc,LPARAM lParam){ return TRUE;}
BOOL	SetIMOProperty	(IMOREF IMORef,LPCTSTR pParam,LPCTSTR pValue){ return TRUE;}
BOOL    Log (LogTypeInterface logSeverity,LPCTSTR pLogComment, 
             LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule){ return TRUE;}
IMOREF	CreateIMO		(PPBSTRUCT pPBStructure,LPCTSTR pTargetModuleName,LPCTSTR pCallingModName){ return 0; }
#endif