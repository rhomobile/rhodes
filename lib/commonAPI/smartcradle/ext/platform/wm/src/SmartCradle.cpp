/**
 * \file SmartCradle.cpp
 * \brief Implementation of the Cradle Module Plugin
 * CCradle is an abstraction of theCradle functionality supported by
 * Symbol Devices MC18
 */

#include "SmartCradle.h"
//#include "..\..\..\common\public\PB_Defines.h"
//#include "SmartCradleModule.h"
//extern CSmartCradleModule *g_SmartCradleModule;



CCradle::CCradle()
{
INIT();
SetDefaultValues();

}

CCradle::~CCradle()
{	
CleanUP();
SetDefaultValues();
}


void CCradle::SetDefaultValues()
{
m_CradleUnlockOnDuration=0;
m_CradleUnlockOffDuration=0;
m_CradleUnlockTimeOut=10;
}





void CCradle::INIT()
{
	m_bcradledllLoaded=false;
	m_hCradle=NULL;

	hCradleModule = LoadLibraryEx(_T("CradleAPI32.dll"), NULL, 0);
	
	if (hCradleModule != NULL) 
	{

	m_bcradledllLoaded=true;

	if (NULL == (CRADLE_OPEN = (CRADLE_OPENPROC) GetProcAddress(hCradleModule, _T("Cradle_Open"))))m_bcradledllLoaded = false;
	if (NULL == (CRADLE_CLOSE = (CRADLE_CLOSEPROC) GetProcAddress(hCradleModule, _T("Cradle_Close"))))m_bcradledllLoaded = false;
	if (NULL == (CRADLE_GETCRADLEINFO = (CRADLE_GETCRADLEINFOPROC) GetProcAddress(hCradleModule, _T("Cradle_GetCradleInfo"))))m_bcradledllLoaded = false;
	if (NULL == (CRADLE_GETCRADLECONFIG = (CRADLE_GETCRADLECONFIGPROC) GetProcAddress(hCradleModule, _T("Cradle_GetCradleConfig"))))m_bcradledllLoaded = false;
	if (NULL == (CRADLE_SETCRADLECONFIG = (CRADLE_SETCRADLECONFIGPROC) GetProcAddress(hCradleModule, _T("Cradle_SetCradleConfig"))))m_bcradledllLoaded = false;
	if (NULL == (CRADLE_GETFASTCHARGESTATE= (CRADLE_GETFASTCHARGESTATEPROC) GetProcAddress(hCradleModule, _T("Cradle_GetFastChargeState"))))m_bcradledllLoaded = false;
	if (NULL == (CRADLE_SETFASTCHARGESTATE= (CRADLE_SETFASTCHARGESTATEPROC) GetProcAddress(hCradleModule, _T("Cradle_SetFastChargeState"))))m_bcradledllLoaded = false;
	if (NULL == (CRADLE_UNLOCK= (CRADLE_UNLOCKPROC) GetProcAddress(hCradleModule, _T("Cradle_Unlock"))))m_bcradledllLoaded = false;
	if (NULL == (CRADLE_GETVERSION= (CRADLE_GETVERSIONPROC) GetProcAddress(hCradleModule, _T("Cradle_GetVersion"))))m_bcradledllLoaded = false;

	}



	if(!m_bcradledllLoaded)
	{
		LOG(ERROR) + L"CradleAPI32.dll could not be loaded.";
	}


}





DWORD CCradle::OpenCradle()
{
//	Sleep(200);
	DWORD ret=RET_ERROR;
	if( m_hCradle == NULL)
	{
		ret=CRADLE_OPEN(&m_hCradle);
		if(E_CRAD_SUCCESS!=ret)
		{
		//Log
			LOG(ERROR) + L"Cradle port could not be opened";
		}

	}
	else
	{
		ret= RET_CRADLE_ALREADY_OPENED;
		LOG(ERROR) + L"Cradle is already opened";
	}
	return ret;
}




DWORD	CCradle::CloseCradle()
{	
	DWORD ret=RET_ERROR;	
	if(m_hCradle != NULL)
	{
		ret = CRADLE_CLOSE(m_hCradle);
		m_hCradle = NULL;
		if(E_CRAD_SUCCESS!=ret)
		{
			LOG(ERROR) + L"Error while closing Cradle port.";
		}
		
	}
	return ret;
}




/*
DWORD CCradle::GetCradleMatrixData()
{
	DWORD ret=RET_ERROR;
	TCHAR szRowIDString[5];
	TCHAR szColumnIDString[5];
	TCHAR szWallIDString[5];

	CRADLE_MATRIX_CONFIG CradleMatrixData;
	SI_INIT(&CradleMatrixData);
		if(NULL!= m_hCradle)
		{
			ret=CRADLE_GETCRADLECONFIG(	m_hCradle,&CradleMatrixData);

			if(E_CRAD_SUCCESS!=ret)
			{
				LOG(ERROR) + L"Error while calling CRADLE_GETCRADLECONFIG.";
			}
			else
			{
			wsprintf(szRowIDString,_T("%d"),CradleMatrixData.dwRowID); 
			wsprintf(szColumnIDString,_T("%d"),CradleMatrixData.dwColumnID); 
			wsprintf(szWallIDString,_T("%d"),CradleMatrixData.dwWallID); 


			if (m_CradleMatrixEvent[0])
			{
//				g_SmartCradleModule->
//					SendPBNavigate(
//						g_szCradleMatrixEventNames,
//						this->PBStructure.iTabID,
//						m_CradleMatrixEvent,
//						szRowIDString,
//						szColumnIDString,
//						szWallIDString,
//						NULL);	
			}

			}

		}

	return ret;
}
*/
DWORD CCradle::SetCradleFastChargeStateData(BOOL CradleFastChargeState)
{
	DWORD ret=RET_ERROR;
	
		if(NULL!= m_hCradle)
		{
			ret=CRADLE_SETFASTCHARGESTATE(m_hCradle,CradleFastChargeState);
			if(E_CRAD_SUCCESS!=ret)
			{
			//Log
			}
			else
			{
			DEBUGMSG(true, (L"\r\nCradle charge State Set"));
			}
		}
	return ret;
}






DWORD CCradle::SetCradleMatrixROWData(WORD RowID)
{
	DWORD ret=	RET_ERROR;
	DWORD ret1=	RET_ERROR;

	CRADLE_MATRIX_CONFIG CradleMatrixData;
	SI_INIT(&CradleMatrixData);
		if(NULL!= m_hCradle)
		{
			ret1=CRADLE_GETCRADLECONFIG(m_hCradle,&CradleMatrixData);
			if(E_CRAD_SUCCESS!=ret1)
			{
				LOG(ERROR) + L"Error while calling CRADLE_GETCRADLECONFIG.";
			}

		CradleMatrixData.dwRowID=RowID;
		ret=CRADLE_SETCRADLECONFIG(m_hCradle,&CradleMatrixData);
			if(E_CRAD_SUCCESS!=ret)
			{
				LOG(ERROR) + L"Error while calling CRADLE_SETCRADLECONFIG.";
			}

		}

	return ret;
}



DWORD CCradle::SetCradleMatrixColumnData(WORD ColumnID)
{
	DWORD ret=	RET_ERROR;
	DWORD ret1=	RET_ERROR;

	CRADLE_MATRIX_CONFIG CradleMatrixData;
	SI_INIT(&CradleMatrixData);
		if(NULL!= m_hCradle)
		{
			ret1=CRADLE_GETCRADLECONFIG(m_hCradle,&CradleMatrixData);
			if(E_CRAD_SUCCESS!=ret1)
			{
			LOG(ERROR) + L"Error while calling CRADLE_GETCRADLECONFIG.";
			}
		CradleMatrixData.dwColumnID=ColumnID;
		ret=CRADLE_SETCRADLECONFIG(m_hCradle,&CradleMatrixData);
			if(E_CRAD_SUCCESS!=ret)
			{
			LOG(ERROR) + L"Error while calling CRADLE_SETCRADLECONFIG.";
			}
		
		}

	return ret;
}

DWORD CCradle::SetCradleMatrixWallData(WORD WallID)
{
	DWORD ret=	RET_ERROR;
	DWORD ret1=	RET_ERROR;

	CRADLE_MATRIX_CONFIG CradleMatrixData;
	SI_INIT(&CradleMatrixData);
		if(NULL!= m_hCradle)
		{
		ret1=CRADLE_GETCRADLECONFIG(m_hCradle,&CradleMatrixData);
			if(E_CRAD_SUCCESS!=ret1)
			{
			LOG(ERROR) + L"Error while calling CRADLE_GETCRADLECONFIG.";
			}

		CradleMatrixData.dwWallID=WallID;
		ret=CRADLE_SETCRADLECONFIG(m_hCradle,&CradleMatrixData);
			if(E_CRAD_SUCCESS!=ret)
			{
			LOG(ERROR) + "Error while calling CRADLE_SETCRADLECONFIG.";
			}
		
		}

	return ret;
}


BOOL CCradle::GetCradleMatrixData(CradleMatrixType matrixType, DWORD* pId)
{
	DWORD ret=	RET_ERROR;

	CRADLE_MATRIX_CONFIG CradleMatrixData;
	SI_INIT(&CradleMatrixData);
	if(NULL != m_hCradle)
	{
		ret = CRADLE_GETCRADLECONFIG(m_hCradle,&CradleMatrixData);
		if(E_CRAD_SUCCESS != ret)
			LOG(ERROR) + L"Error while calling CRADLE_GETCRADLECONFIG.";
		else
		{
		switch(matrixType)
		{
		case CRADLE_MATRIX_ROW:
			*pId = CradleMatrixData.dwRowID;
			break;
		case CRADLE_MATRIX_COLUMN:
			*pId = CradleMatrixData.dwColumnID;
			break;
		case CRADLE_MATRIX_WALL:
			*pId = CradleMatrixData.dwWallID;
			break;
		}
		}
	}
	return ret == E_CRAD_SUCCESS;
}


/*
DWORD CCradle::SetCradleMatrixColumnData(WORD ColumnID)
{
	DWORD ret=	RET_ERROR;
	DWORD ret1=	RET_ERROR;

	CRADLE_MATRIX_CONFIG CradleMatrixData;
	SI_INIT(&CradleMatrixData);
		if(NULL!= m_hCradle)
		{
			ret1=CRADLE_GETCRADLECONFIG(m_hCradle,&CradleMatrixData);
			if(E_CRAD_SUCCESS!=ret1)
			{
			LOG(ERROR) + L"Error while calling CRADLE_GETCRADLECONFIG.";
			}
		CradleMatrixData.dwColumnID=ColumnID;
		ret=CRADLE_SETCRADLECONFIG(m_hCradle,&CradleMatrixData);
			if(E_CRAD_SUCCESS!=ret)
			{
			LOG(ERROR) + L"Error while calling CRADLE_SETCRADLECONFIG.";
			}
		
		}

	return ret;
}

DWORD CCradle::SetCradleMatrixWallData(WORD WallID)
{
	DWORD ret=	RET_ERROR;
	DWORD ret1=	RET_ERROR;

	CRADLE_MATRIX_CONFIG CradleMatrixData;
	SI_INIT(&CradleMatrixData);
		if(NULL!= m_hCradle)
		{
		ret1=CRADLE_GETCRADLECONFIG(m_hCradle,&CradleMatrixData);
			if(E_CRAD_SUCCESS!=ret1)
			{
			LOG(ERROR) + L"Error while calling CRADLE_GETCRADLECONFIG.";
			}

		CradleMatrixData.dwWallID=WallID;
		ret=CRADLE_SETCRADLECONFIG(m_hCradle,&CradleMatrixData);
			if(E_CRAD_SUCCESS!=ret)
			{
			LOG(ERROR) + "Error while calling CRADLE_SETCRADLECONFIG.";
			}
		
		}

	return ret;
}
*/


bool CCradle::UnloadLibrary()
{
	BOOL bret=FALSE;
	if(hCradleModule)
	{
	bret = FreeLibrary(hCradleModule);
	hCradleModule=NULL;

	CRADLE_OPEN=NULL;
	CRADLE_CLOSE=NULL;
	CRADLE_GETCRADLEINFO=NULL;
	CRADLE_GETCRADLECONFIG=NULL;
	CRADLE_GETFASTCHARGESTATE=NULL;
	CRADLE_SETCRADLECONFIG=NULL;
	CRADLE_SETFASTCHARGESTATE=NULL;
	CRADLE_UNLOCK=NULL;
	CRADLE_GETVERSION=NULL;
	}
	return bret == TRUE;
}


void CCradle::CleanUP()
{
UnloadLibrary();
}

DWORD CCradle::Unlock()
{
	DWORD ret=	RET_ERROR;
	//bool bOnAndOffDurationNotSet=false;
	
	if(NULL != m_hCradle){
		
		if(m_CradleUnlockOnDuration || m_CradleUnlockOffDuration){
			CRADLE_UNLOCK_LED_INFO CradleUnlockData;
			SI_INIT(&CradleUnlockData);
			CradleUnlockData.dwOnDuration=m_CradleUnlockOnDuration;
			CradleUnlockData.dwOffDuration=m_CradleUnlockOffDuration;
			ret=CRADLE_UNLOCK(m_hCradle,m_CradleUnlockTimeOut,&CradleUnlockData);
		}
		else{
			ret=CRADLE_UNLOCK(m_hCradle,m_CradleUnlockTimeOut,NULL);

		}
		
		if(E_CRAD_SUCCESS!=ret){
			LOG(ERROR) + L"Error while calling CRADLE_UNLOCK.";
		}
	}
	

	return ret;

}


















//////////////Public Methods

void CCradle::GetCradleInfo()
{
	
	DWORD ret_cradleInfo=RET_ERROR;
	DWORD ret_cradleVersion=RET_ERROR;
	DWORD ret_cradleChargestate=RET_ERROR;

	OpenCradle();
	//if(E_CRAD_SUCCESS==OpenCradle())
	//{

		TCHAR szPartNoString[MAX_PATH];
		TCHAR szSerialNOString[MAX_PATH];
		TCHAR szHardWareIDString[5];
		TCHAR szSMFGDateString[16];

		CRADLE_DEVICE_INFO CradleDeviceInfoData;
		SI_INIT(&CradleDeviceInfoData);
		if(NULL!= m_hCradle)
		{
			ret_cradleInfo=CRADLE_GETCRADLEINFO(m_hCradle,&CradleDeviceInfoData);
			if(E_CRAD_SUCCESS!=ret_cradleInfo)
			{
			LOG(ERROR) + L"Error while calling CRADLE_GETCRADLEINFO.";
			}
			else
			{
			
			wsprintf(szPartNoString,_T("%.19s"),CradleDeviceInfoData.szPartNumber); 
			wsprintf(szSerialNOString,_T("%.15s"),CradleDeviceInfoData.szSerialNumber); 
			wsprintf(szHardWareIDString,_T("%d"),CradleDeviceInfoData.dwHardwareID);
			wsprintf(szSMFGDateString,_T("%d-%d-%d"),CradleDeviceInfoData.cradleManufactureInfo.dwDate,CradleDeviceInfoData.cradleManufactureInfo.dwMonth,CradleDeviceInfoData.cradleManufactureInfo.dwYear); 
			
			}


		}
	
		
		CradleDeviceInfoData.szSerialNumber[0]=NULL;
		CradleDeviceInfoData.szPartNumber[0]=NULL;

	

		//Version

		TCHAR szCAPIVersionString[5];
		TCHAR szDriverVersionString[5];
		TCHAR szFirmwareVersionString[5];

		CRADLE_VERSION_INFO CradleVersionInfo;
		SI_INIT(&CradleVersionInfo);

		if(NULL!= m_hCradle)
		{
			ret_cradleVersion=CRADLE_GETVERSION(m_hCradle,&CradleVersionInfo);
			if(E_CRAD_SUCCESS!=ret_cradleVersion)
			{
			LOG(ERROR) + L"Error while calling CRADLE_GETVERSION.";
			}
			else
			{
			
			wsprintf(szCAPIVersionString,_T("%d.%d"),HIWORD(CradleVersionInfo.dwCAPIVersion),LOWORD(CradleVersionInfo.dwCAPIVersion)); 
			wsprintf(szDriverVersionString,_T("%d.%d"),HIWORD(CradleVersionInfo.dwDriverVersion),LOWORD(CradleVersionInfo.dwDriverVersion)); 
			wsprintf(szFirmwareVersionString,_T("%d.%d"),HIWORD(CradleVersionInfo.dwFirmwareVersion),LOWORD(CradleVersionInfo.dwFirmwareVersion)); 
			}
		}

		//FastChargeState
		TCHAR szCradleChargeStateString[5];
		BOOL CradleFastChargeState;

	
		if(NULL!= m_hCradle)
		{
			 ret_cradleChargestate=CRADLE_GETFASTCHARGESTATE(m_hCradle,&CradleFastChargeState);

			if(E_CRAD_SUCCESS!=ret_cradleChargestate)
			{
			LOG(ERROR) + L"Error while calling CRADLE_GETFASTCHARGESTATE.";
			}
			else
			{
				if(CradleFastChargeState)
				{
				wsprintf(szCradleChargeStateString,_T("%s"),L"fast"); 
				}
				else
				{
				wsprintf(szCradleChargeStateString,_T("%s"),L"slow");
				}
			}
		}


		if((E_CRAD_SUCCESS==ret_cradleChargestate)||((E_CRAD_SUCCESS==ret_cradleInfo))||(E_CRAD_SUCCESS==ret_cradleVersion))//If any of the methhods returns success we need to send the navigate event
		{
//			if (m_CradleInfoEvent[0])
//			{
//				g_SmartCradleModule->
//					SendPBNavigate(
//						g_szCradleInfoEventNames,
//						this->PBStructure.iTabID,
//						m_CradleInfoEvent,
//						szHardWareIDString,
//						szPartNoString,
//						szSerialNOString,
//						szSMFGDateString,
//						szDriverVersionString,
//						szCAPIVersionString,
//						szFirmwareVersionString,
//						szCradleChargeStateString,
//						NULL);	
//			}
		}





	//}


CloseCradle();
}
/*
void CCradle::GetCradleMatrix()
{
	OpenCradle();
	//if(E_CRAD_SUCCESS== OpenCradle())
	//{
	GetCradleMatrixData();
	//}
	CloseCradle();
}
*/
void CCradle::SetCradleUnlockOnDuration(DWORD OnDuration)
{
	if((OnDuration>65535)||(OnDuration<0))
	{
		LOG(ERROR) + L"Invalid CradleUnlockOnDuration passed.Ignore it.";
	}
	else
	{
		m_CradleUnlockOnDuration= OnDuration;
	}
}



void CCradle::SetCradleUnlockOffDuration(DWORD OffDuration)
{
	if((OffDuration>65535)||(OffDuration<0))
	{
	LOG(WARNING) + L"Invalid CradleUnlockOffDuration passed.Ignore it.";
	}
	else
	{
	m_CradleUnlockOffDuration= OffDuration;
	}
}

void CCradle::SetCradleUnlockTimeOut(int timeOut)
{
	if((timeOut>30)||(timeOut<10))
	{
	LOG(WARNING) + L"Invalid CradleUnlockTimeOut passed.Ignore it.";
	}
	else
	{
	m_CradleUnlockTimeOut= BYTE(timeOut);
}



}
void CCradle::UnlockCradle()
{
	OpenCradle();
	//if(E_CRAD_SUCCESS== OpenCradle())
	//{
	Unlock();
	//}
CloseCradle();
}








void CCradle::SetCradleMatrixColumn(WORD ColumnID)
{
	OpenCradle();
	//if(E_CRAD_SUCCESS== OpenCradle())
	//{
	SetCradleMatrixColumnData(ColumnID);
	//}
CloseCradle();
}


void CCradle::SetCradleMatrixRow(WORD RowID)
{
	OpenCradle();
	//if(E_CRAD_SUCCESS== OpenCradle())
	//{
		SetCradleMatrixROWData(RowID);
	//}
CloseCradle();
}

bool CCradle::GetCradleMatrixRow(DWORD* pRowId)
{
	OpenCradle();
	BOOL bRet = GetCradleMatrixData(CRADLE_MATRIX_ROW, pRowId);
	CloseCradle();
	return bRet == TRUE;
}

bool CCradle::GetCradleMatrixColumn(DWORD* pColumnId)
{
	OpenCradle();
	BOOL bRet = GetCradleMatrixData(CRADLE_MATRIX_COLUMN, pColumnId);
	CloseCradle();
	return bRet == TRUE;
}

bool CCradle::GetCradleMatrixWall(DWORD* pWallId)
{
	OpenCradle();
	BOOL bRet = GetCradleMatrixData(CRADLE_MATRIX_WALL, pWallId);
	CloseCradle();
	return bRet == TRUE;
}


void CCradle::SetCradleMatrixWall(WORD WallID)
{
	OpenCradle();
	//if(E_CRAD_SUCCESS== OpenCradle())
	//{
		SetCradleMatrixWallData(WallID);
	//}
CloseCradle();
}

void CCradle::SetCradleFastChargeState(BOOL CradleFastChargeState)
{
	OpenCradle();
	/*if(E_CRAD_SUCCESS== OpenCradle())
	{*/
		SetCradleFastChargeStateData(CradleFastChargeState);
	//}
CloseCradle();
}

bool CCradle::GetCradleFastChargeState(rho::apiGenerator::CMethodResult& oResult)
{
	DWORD ret_cradleChargestate=RET_ERROR;
	BOOL CradleFastChargeState;
	OpenCradle();
	ret_cradleChargestate=CRADLE_GETFASTCHARGESTATE(m_hCradle,&CradleFastChargeState);
	if(E_CRAD_SUCCESS != ret_cradleChargestate)
	{
		LOG(ERROR) + L"Error while calling CRADLE_GETFASTCHARGESTATE.";
	}
	else
	{
		if(CradleFastChargeState)
		{
			oResult.set("fast");
		}
		else
		{
			oResult.set("slow");
		}
	}
	CloseCradle();
	return (E_CRAD_SUCCESS == ret_cradleChargestate);
}

bool CCradle::GetCradleInfoProperty(CradleInfoProperty eProperty, rho::apiGenerator::CMethodResult& oResult)
{
	DWORD ret_cradleInfo=RET_ERROR;
	OpenCradle();
	CRADLE_DEVICE_INFO CradleDeviceInfoData;
	SI_INIT(&CradleDeviceInfoData);
	ret_cradleInfo=CRADLE_GETCRADLEINFO(m_hCradle,&CradleDeviceInfoData);
	wchar_t szSMFGDateString[16];
	if(E_CRAD_SUCCESS!=ret_cradleInfo)
	{
		LOG(ERROR) + L"Error while calling CRADLE_GETCRADLEINFO.";
	}
	else
	{
		switch(eProperty)
		{
		case CRADLE_INFO_HARDWARE:
			oResult.set(CradleDeviceInfoData.dwHardwareID);
			break;
		case CRADLE_INFO_PART:
			oResult.set(CradleDeviceInfoData.szPartNumber);
			break;
		case CRADLE_INFO_SERIAL: 
			oResult.set(CradleDeviceInfoData.szSerialNumber);
			break;
		case CRADLE_INFO_MFG:
			wsprintf(szSMFGDateString, _T("%d-%d-%d"), 
				CradleDeviceInfoData.cradleManufactureInfo.dwDate,CradleDeviceInfoData.cradleManufactureInfo.dwMonth,CradleDeviceInfoData.cradleManufactureInfo.dwYear);
			oResult.set(szSMFGDateString);
			break;
		}
	}
	CloseCradle();
	return (E_CRAD_SUCCESS == ret_cradleInfo);
}

bool CCradle::GetCradleVersionProperty(CradleVersionProperty eProperty, rho::apiGenerator::CMethodResult& oResult)
{
	DWORD ret_cradleVersion=RET_ERROR;
	OpenCradle();
	TCHAR szCAPIVersionString[5];
	TCHAR szDriverVersionString[5];
	TCHAR szFirmwareVersionString[5];
	CRADLE_VERSION_INFO CradleVersionInfo;
	SI_INIT(&CradleVersionInfo);
	ret_cradleVersion=CRADLE_GETVERSION(m_hCradle,&CradleVersionInfo);
	if(E_CRAD_SUCCESS!=ret_cradleVersion)
	{
		LOG(ERROR) + L"Error while calling CRADLE_GETCRADLEVERSION.";
	}
	else
	{
		switch(eProperty)
		{
		case CRADLE_VERSION_DRIVER:
			wsprintf(szCAPIVersionString,_T("%d.%d"),HIWORD(CradleVersionInfo.dwCAPIVersion),LOWORD(CradleVersionInfo.dwCAPIVersion)); 
			oResult.set(szCAPIVersionString);
			break;
		case CRADLE_VERSION_CAPI:
			wsprintf(szDriverVersionString,_T("%d.%d"),HIWORD(CradleVersionInfo.dwDriverVersion),LOWORD(CradleVersionInfo.dwDriverVersion)); 
			oResult.set(szDriverVersionString);
			break;
		case CRADLE_VERSION_FIRMWARE: 
			wsprintf(szFirmwareVersionString,_T("%d.%d"),HIWORD(CradleVersionInfo.dwFirmwareVersion),LOWORD(CradleVersionInfo.dwFirmwareVersion)); 
			oResult.set(szFirmwareVersionString);
			break;
		}
	}
	CloseCradle();
	return (E_CRAD_SUCCESS == ret_cradleVersion);
}