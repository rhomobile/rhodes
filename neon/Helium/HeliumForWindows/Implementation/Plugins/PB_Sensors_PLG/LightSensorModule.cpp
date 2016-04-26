#include "LightSensorModule.h"

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
BOOL CLightSensorModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy(m_szModName,L"LightSensor");	
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	return TRUE;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
void CLightSensorModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);	
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
BOOL CLightSensorModule::onAttachInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Attaching Light Sensors Instance",_T(__FUNCTION__),__LINE__);	
		
	pInstStruct->pWrappedObj = (CLightSensor*) new CLightSensor(pInstStruct->instID,this);
	
	if(pInstStruct->pWrappedObj)
		return TRUE;
	else
		return FALSE;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
BOOL CLightSensorModule::onReleaseInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Deleting Light Sensors Instance",_T(__FUNCTION__),__LINE__);
	delete (CLightSensor*)pInstStruct->pWrappedObj;
	pInstStruct->pWrappedObj = NULL;
	return TRUE;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
BOOL CLightSensorModule::onBeforeNavigate(int iInstID)
{
	//  If we are navigating to a new page hide the Raw Sensor box and 
	//  set the TransferEvent navigation URIs to blank
	CLightSensor *pLightSensor = (CLightSensor *)GetObjFromID(iInstID);
	if(pLightSensor)
	{
		pLightSensor->SetLightSensorURI(L"");	
		pLightSensor->UnregisterSensor();
	}
	return TRUE;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
BOOL CLightSensorModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, VOID* pParam)
{	
	Log(PB_LOG_WARNING, L"The Light Sensor API from the RhoElements Extension has been deprecated in 4.0, please transition your applications to use the common Sensors API for future releases", _T(__FUNCTION__), __LINE__);

	CLightSensor *pLightSensor = (CLightSensor *)pParam;
	if (!pLightSensor)
	{
		Log(PB_LOG_WARNING, L"Device Does not Support Sensor API", 
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	Log(PB_LOG_INFO, pbMetaStructure->lpParameter, _T(__FUNCTION__), __LINE__);

	//  Process the received Meta Tag
	if(cmp(pbMetaStructure->lpParameter, L"status"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pLightSensor->RegisterSensor() != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"Ambient Light Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pLightSensor->UnregisterSensor() != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"Ambient Light Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}	
	else if (cmp(pbMetaStructure->lpParameter, L"MinimumInterval"))
	{		
		if (pbMetaStructure->lpValue)
		{
			int iMinimumInterval = _wtoi(pbMetaStructure->lpValue);	
			if (iMinimumInterval < 200)
				iMinimumInterval = 200;
			pLightSensor->SetInterval(iMinimumInterval);
		}		
	}
	else if(cmp(pbMetaStructure->lpParameter, L"lightSensorEvent"))
	{
		if (pbMetaStructure->lpValue)
			return pLightSensor->SetLightSensorURI(pbMetaStructure->lpValue);
	}	
	else if(cmp(pbMetaStructure->lpParameter, L"getSensorData"))
	{
		if (pLightSensor->IsSensorAPIAvailable())
			pLightSensor->SendEventData();
	}
	else
	{
		//  Unrecognized tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Light Sensors Module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}
