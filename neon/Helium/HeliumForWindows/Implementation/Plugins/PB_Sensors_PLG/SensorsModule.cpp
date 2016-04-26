#include "SensorsModule.h"

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
BOOL CRawSensorModule::onInit(PPBSTRUCT pPBStructure)
{
	wcscpy(m_szModName,L"RawSensors");	
	RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
	return TRUE;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
void CRawSensorModule::onDeInit(PPBSTRUCT pPBStructure)
{
	UnRegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
BOOL CRawSensorModule::onAttachInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Attaching Raw Sensors Instance",_T(__FUNCTION__),__LINE__);	
		
	pInstStruct->pWrappedObj = (CRawSensor*) new CRawSensor(pInstStruct->instID,this);
	
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
BOOL CRawSensorModule::onReleaseInstance(PPBSTRUCT pPBStructure,PPBINSTSTRUCT pInstStruct)
{
	Log(PB_LOG_INFO,L"Deleting raw Sensors Instance",_T(__FUNCTION__),__LINE__);
	delete (CRawSensor*)pInstStruct->pWrappedObj;
	pInstStruct->pWrappedObj = NULL;
	return TRUE;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
BOOL CRawSensorModule::onBeforeNavigate(int iInstID)
{
	//  If we are navigating to a new page hide the Raw Sensor box and 
	//  set the TransferEvent navigation URIs to blank
	CRawSensor *pRawSensor = (CRawSensor *)GetObjFromID(iInstID);
	if(pRawSensor)
	{
		pRawSensor->SetRawSensorsURI(L"");		
		pRawSensor->UnregisterSensor(SENSOR_TYPE_ACCELEROMETER);
		pRawSensor->UnregisterSensor(SENSOR_TYPE_ORIENTATION);
		pRawSensor->UnregisterSensor(SENSOR_TYPE_TILT_ANGLE);
		pRawSensor->UnregisterSensor(SENSOR_TYPE_MOTION);
		pRawSensor->UnregisterSensor(SENSOR_TYPE_ECOMPASS);
		pRawSensor->UnregisterSensor(SENSOR_TYPE_MAGNETOMETER);
		pRawSensor->UnregisterSensor(SENSOR_TYPE_GYROSCOPE);
		pRawSensor->UnregisterSensor(SENSOR_TYPE_AMBIENT_LIGHT);
		pRawSensor->UnregisterSensor(SENSOR_TYPE_PROXIMITY);
		pRawSensor->UnregisterSensor(SENSOR_TYPE_PROXIMITY_LONG_RANGE);
		pRawSensor->UnregisterSensor(SENSOR_TYPE_PRESSURE);
		pRawSensor->UnregisterSensor(SENSOR_TYPE_TEMPERATURE);
		pRawSensor->UnregisterSensor(SENSOR_TYPE_HUMIDITY);
	}
	return TRUE;
}

/******************************************************************************/
/**
* \author	Harish Pulimi (HP, PXB743)
* \date		July 2012 Initial Creation, HP
*/
/******************************************************************************/
BOOL CRawSensorModule::MetaProc(PBMetaStruct *pbMetaStructure, PPBSTRUCT pPBStructure, VOID* pParam)
{	
	Log(PB_LOG_WARNING, L"The RawSensors API has been deprecated in 4.0, please transition your applications to use the common Sensors API for future releases", _T(__FUNCTION__), __LINE__);

	CRawSensor *pRawSensors = (CRawSensor *)pParam;
	if (!pRawSensors)
	{
		Log(PB_LOG_WARNING, L"Device Does not Support Sensor API", 
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}
	Log(PB_LOG_INFO, pbMetaStructure->lpParameter, _T(__FUNCTION__), __LINE__);

	//  Process the received Meta Tag
	if(cmp(pbMetaStructure->lpParameter, L"accelerometer"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_ACCELEROMETER) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"Acceleromneter Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_ACCELEROMETER) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"Acceleromneter Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"deviceOrientation"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_ORIENTATION) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"deviceOrientation Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_ORIENTATION) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"deviceOrientation Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"tiltangle"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_TILT_ANGLE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"tiltangle Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_TILT_ANGLE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"tiltangle Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"motion"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_MOTION) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"motion Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_MOTION) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"motion Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"ecompass"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_ECOMPASS) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"ecompass Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_ECOMPASS) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"ecompass Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"magnetometer"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_MAGNETOMETER) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"magnetometer Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_MAGNETOMETER) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"magnetometer Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"gyroscope"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_GYROSCOPE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"gyroscope Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_GYROSCOPE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"gyroscope Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"ambientlight"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_AMBIENT_LIGHT) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"ambientlight Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_AMBIENT_LIGHT) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"ambientlight Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"proximity"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_PROXIMITY) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"proximity Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_PROXIMITY) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"proximity Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"proximitylongrange"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_PROXIMITY_LONG_RANGE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"proximitylongrange Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_PROXIMITY_LONG_RANGE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"proximitylongrange Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"pressure"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_PRESSURE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"pressure Sensor Open Failed", _T(__FUNCTION__), __LINE__);	
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_PRESSURE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"pressure Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"temperature"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_TEMPERATURE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"temperature Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_TEMPERATURE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"temperature Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"humidity"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_HUMIDITY) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"humidity Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_HUMIDITY) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"humidity Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"all"))
	{
		if(cmp(pbMetaStructure->lpValue, L"Enabled")) 
		{
			//Start the thread		
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_ACCELEROMETER) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"Acceleromneter Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_ORIENTATION) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"deviceOrientation Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_TILT_ANGLE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"tiltangle Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_MOTION) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"motion Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_ECOMPASS) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"ecompass Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_MAGNETOMETER) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"magnetometer Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_GYROSCOPE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"gyroscope Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_AMBIENT_LIGHT) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"ambientlight Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_PROXIMITY) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"proximity Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_PROXIMITY_LONG_RANGE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"proximitylongrange Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_PRESSURE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"pressure Sensor Open Failed", _T(__FUNCTION__), __LINE__);	
			}
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_TEMPERATURE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"temperature Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->RegisterSensor(SENSOR_TYPE_HUMIDITY) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"humidity Sensor Open Failed", _T(__FUNCTION__), __LINE__);
			}
			Log(PB_LOG_WARNING, L"gravity, linearAcceleration, rotation, orientation Sensors not supported on Windows platform",
				_T(__FUNCTION__), __LINE__);
		}
		else if(cmp(pbMetaStructure->lpValue, L"Disabled")) 
		{
			//Stop the thread	
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_ACCELEROMETER) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"Acceleromneter Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_ORIENTATION) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"deviceOrientation Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_TILT_ANGLE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"tiltangle Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_MOTION) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"motion Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_ECOMPASS) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"ecompass Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_MAGNETOMETER) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"magnetometer Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_GYROSCOPE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"gyroscope Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_AMBIENT_LIGHT) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"ambientlight Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_PROXIMITY) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"proximity Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_PROXIMITY_LONG_RANGE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"proximitylongrange Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_PRESSURE) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"pressure Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_TEMPERATURE))
			{
				Log(PB_LOG_WARNING, L"temperature Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			if (pRawSensors->UnregisterSensor(SENSOR_TYPE_HUMIDITY) != E_SENSOR_SUCCESS)
			{
				Log(PB_LOG_WARNING, L"humidity Sensor Close Failed", _T(__FUNCTION__), __LINE__);
			}
			Log(PB_LOG_WARNING, L"gravity, linearAcceleration, rotation, orientation Sensors not supported on Windows platform",
				_T(__FUNCTION__), __LINE__);
		}	
	}
	else if(cmp(pbMetaStructure->lpParameter, L"gravity"))
	{
		Log(PB_LOG_WARNING, L"Gravity Sensor is not supported on Windows platform",
			_T(__FUNCTION__), __LINE__);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"linearAcceleration"))
	{
		Log(PB_LOG_WARNING, L"linearAcceleration Sensor is not supported on Windows platform",
			_T(__FUNCTION__), __LINE__);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"rotation"))
	{
		Log(PB_LOG_WARNING, L"rotation Sensor is not supported on Windows platform",
			_T(__FUNCTION__), __LINE__);
	}
	else if(cmp(pbMetaStructure->lpParameter, L"orientation"))
	{
		Log(PB_LOG_WARNING, L"orientation Sensor is not supported on Windows platform",
			_T(__FUNCTION__), __LINE__);
	}
	else if (cmp(pbMetaStructure->lpParameter, L"MinimumInterval"))
	{		
		if (pbMetaStructure->lpValue)
		{
			int iMinimumInterval = _wtoi(pbMetaStructure->lpValue);	
			if (iMinimumInterval < 200)
				iMinimumInterval = 200;
			pRawSensors->SetInterval(iMinimumInterval);
		}		
	}
	else if(cmp(pbMetaStructure->lpParameter, L"sensorevent"))
	{
		if (pbMetaStructure->lpValue)
			return pRawSensors->SetRawSensorsURI(pbMetaStructure->lpValue);
	}	
	else if(cmp(pbMetaStructure->lpParameter, L"getSensorData"))
	{
		if (pRawSensors->IsSensorAPIAvailable())
			pRawSensors->SendEventData();
	}
	else
	{
		//  Unrecognized tag
		Log(PB_LOG_WARNING, L"Unrecognised Meta Tag Provided to Raw Sensors Module",
			_T(__FUNCTION__), __LINE__);
		return FALSE;
	}

	return TRUE;
}
