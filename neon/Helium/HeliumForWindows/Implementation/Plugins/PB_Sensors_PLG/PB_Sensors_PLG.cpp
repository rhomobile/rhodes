#include "SensorsModule.h"
#include "LightSensorModule.h"

/******************************************************************************/
CRawSensorModule *pSensorModule;
CLightSensorModule *pLightSensorModule;

/******************************************************************************/
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

    return TRUE;
}

/******************************************************************************/
BOOL RawSensors_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!pSensorModule)
	{
		// Take copy of core struct so we have pointers to the browser functions
		// OK to do now as PBCoreStruct is static member
		//memcpy (&(pSensorModule->PBCoreStruct), pPBCoreStructure, sizeof (PBCORESTRUCT));

		pSensorModule = new CRawSensorModule ();

		if (!pSensorModule)
			return false;

		return pSensorModule->Preload (pPBStructure, pPBCoreStructure);
	}

	return true;
}

/******************************************************************************/
BOOL RawSensors_Dispose (PPBSTRUCT pPBStructure)
{
	if (pSensorModule)
	{
		if (pSensorModule->Dispose(pPBStructure) == DEINITIALISED)
		{
			delete pSensorModule;
			pSensorModule = NULL;
		}
	}

	return true;
}

/******************************************************************************/
BOOL RawSensors_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure)
{
	if (pSensorModule)
	{		
		return pSensorModule->Process (pbMetaStructure, pPBStructure);
	}
	else
		return false;
}


/******************************************************************************/
BOOL LightSensor_Preload (PPBSTRUCT pPBStructure, PPBCORESTRUCT pPBCoreStructure)
{
	if (!pLightSensorModule)
	{
		// Take copy of core struct so we have pointers to the browser functions
		// OK to do now as PBCoreStruct is static member
		//memcpy (&(pSensorModule->PBCoreStruct), pPBCoreStructure, sizeof (PBCORESTRUCT));

		pLightSensorModule = new CLightSensorModule ();

		if (!pLightSensorModule)
			return false;

		return pLightSensorModule->Preload (pPBStructure, pPBCoreStructure);
	}

	return true;
}

/******************************************************************************/
BOOL LightSensor_Dispose (PPBSTRUCT pPBStructure)
{
	if (pLightSensorModule)
	{
		if (pLightSensorModule->Dispose(pPBStructure) == DEINITIALISED)
		{
			delete pLightSensorModule;
			pLightSensorModule = NULL;
		}
	}

	return true;
}

/******************************************************************************/
BOOL LightSensor_MetaProc (PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure)
{
	if (pLightSensorModule)
	{		
		return pLightSensorModule->Process (pbMetaStructure, pPBStructure);
	}
	else
		return false;
}

