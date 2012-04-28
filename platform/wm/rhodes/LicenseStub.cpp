#include "stdafx.h"
#include "common/app_build_capabilities.h"

#if defined(APP_BUILD_CAPABILITY_WEBKIT_BROWSER) && defined(APP_BUILD_CAPABILITY_NON_MOTOROLA_DEVICE)
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
#endif