#include "Scanner.h"

int CScanner::ScannerTag_GetPropertyValue	(LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = TRUE;
	if (cmp(szPropertyName, L"enabled"))
	{
		if (m_bScannerOpen && m_bScannerEnabled && m_szCurrentlyOpenScanner)
			iRetVal = ProcessDecoderPropertyForReturn(m_szCurrentlyOpenScanner, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"AutoEnter"))
	{
		if (m_bAutoEnter)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"AutoTab"))
	{
		if (m_bAutoTab)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"FriendlyName"))
	{
		WCHAR szScannerDeviceID[5];
		wsprintf(szScannerDeviceID, L"SCN%i:", m_iInstanceID);
		WCHAR szFriendlyName[30];
		memset(szFriendlyName, 0, 30 * sizeof(WCHAR));
		FindScanner(szScannerDeviceID, &m_bUsingBluetoothScanner, szFriendlyName);
		iRetVal = wcslen(szFriendlyName);
		if (szPropertyValue)
			wcscpy(szPropertyValue, szFriendlyName);
	}
	else if (cmp(szPropertyName, L"lowBatteryScan"))
	{
		if (m_bLowBatteryScan)
		{
			iRetVal = wcslen(L"true");
			if (szPropertyValue)
				wcscpy(szPropertyValue, L"true");
		}
		else
		{
			iRetVal = wcslen(L"false");
			if (szPropertyValue)
				wcscpy(szPropertyValue, L"false");
		}

	}
	else if (cmp(szPropertyName, L"disableScannerDuringNavigate"))
	{
		if (!m_bLegacyNavBehaviour)
		{
			iRetVal = wcslen(L"true");
			if (szPropertyValue)
				wcscpy(szPropertyValue, L"true");
		}
		else
		{
			iRetVal = wcslen(L"false");
			if (szPropertyValue)
				wcscpy(szPropertyValue, L"false");
		}
	}
	else if (cmp(szPropertyName, L"triggerConnected"))
	{
		if (m_bTriggerConnected)
		{
			iRetVal = wcslen(L"true");
			if (szPropertyValue)
				wcscpy(szPropertyValue, L"true");
		}
		else
		{
			iRetVal = wcslen(L"false");
			if (szPropertyValue)
				wcscpy(szPropertyValue, L"false");
		}
	}
	return iRetVal;
}

int CScanner::ScannerBeeper_GetPropertyValue(LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	if (cmp(szPropertyName, L"decodeVolume"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(
			m_decodeNotifier->GetDecodeVolume(), szPropertyValue);
	}
	else if (cmp(szPropertyName, L"decodeDuration"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(
			m_decodeNotifier->GetDecodeDuration(), szPropertyValue);
	}
	else if (cmp(szPropertyName, L"decodeFrequency"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(
			m_decodeNotifier->GetSuccessfulDecodeFrequency(), szPropertyValue);
	}
	else if (cmp(szPropertyName, L"invalidDecodeFrequency"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(
			m_decodeNotifier->GetInvalidDecodeFrequency(), szPropertyValue);
	}
	else if (cmp(szPropertyName, L"decodeSound"))
	{
		if (m_decodeNotifier->GetSuccessfulDecodeSound())
			iRetVal = ProcessDecoderPropertyForReturn(
				m_decodeNotifier->GetSuccessfulDecodeSound(), szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"invalidDecodeSound"))
	{
		if (m_decodeNotifier->GetInvalidDecodeSound())
			iRetVal = ProcessDecoderPropertyForReturn(
				m_decodeNotifier->GetInvalidDecodeSound(), szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"", szPropertyValue);
	}

	return iRetVal;
}

int CScanner::ScannerReaderTag_GetPropertyValue(LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	READER_PARAMS readerParams;
	RetrieveScannerParameters(&readerParams);
	bool bLaser = (readerParams.dwReaderType == READER_TYPE_LASER);
	if (cmp(szPropertyName, L"RasterMode"))
	{
		//  Note - Raster Mode Cyclone is no longer mentioned in the EMDK help file
		if (readerParams.ReaderSpecific.laser_specific.dwRasterMode == RASTER_MODE_NONE)
			iRetVal = ProcessDecoderPropertyForReturn(L"none", szPropertyValue);
		else if (readerParams.ReaderSpecific.laser_specific.dwRasterMode == RASTER_MODE_OPEN_ALWAYS)
			iRetVal = ProcessDecoderPropertyForReturn(L"openAlways", szPropertyValue);
		else if (readerParams.ReaderSpecific.laser_specific.dwRasterMode == RASTER_MODE_SMART)
			iRetVal = ProcessDecoderPropertyForReturn(L"smart", szPropertyValue);
		else if (readerParams.ReaderSpecific.laser_specific.dwRasterMode == RASTER_MODE_CYCLONE)
			iRetVal = ProcessDecoderPropertyForReturn(L"cyclone", szPropertyValue);
		else
			iRetVal = -1;
	}
	else if (cmp(szPropertyName, L"AimType"))
	{
		if (bLaser)
		{
		if (readerParams.ReaderSpecific.laser_specific.dwAimType == AIM_TYPE_TRIGGER)
			iRetVal = ProcessDecoderPropertyForReturn(L"trigger", szPropertyValue);
		else if (readerParams.ReaderSpecific.laser_specific.dwAimType == AIM_TYPE_TIMED_HOLD)
			iRetVal = ProcessDecoderPropertyForReturn(L"timedHold", szPropertyValue);
		else if (readerParams.ReaderSpecific.laser_specific.dwAimType == AIM_TYPE_TIMED_RELEASE)
			iRetVal = ProcessDecoderPropertyForReturn(L"timedRelease", szPropertyValue);
		else if (readerParams.ReaderSpecific.laser_specific.dwAimType == AIM_TYPE_PRESENTATION)
			iRetVal = ProcessDecoderPropertyForReturn(L"presentation", szPropertyValue);
		else if (readerParams.ReaderSpecific.laser_specific.dwAimType == AIM_TYPE_PRESS_AND_RELEASE)
			iRetVal = ProcessDecoderPropertyForReturn(L"pressAndRelease", szPropertyValue);
		else if (readerParams.ReaderSpecific.laser_specific.dwAimType == AIM_TYPE_CONTINUOUS_READ)
			iRetVal = ProcessDecoderPropertyForReturn(L"continuousRead", szPropertyValue);
		else
			iRetVal = -1;
		}
		else
		{
			if (readerParams.ReaderSpecific.imager_specific.dwAimType == AIM_TYPE_TRIGGER)
				iRetVal = ProcessDecoderPropertyForReturn(L"trigger", szPropertyValue);
			else if (readerParams.ReaderSpecific.imager_specific.dwAimType == AIM_TYPE_TIMED_HOLD)
				iRetVal = ProcessDecoderPropertyForReturn(L"timedHold", szPropertyValue);
			else if (readerParams.ReaderSpecific.imager_specific.dwAimType == AIM_TYPE_TIMED_RELEASE)
				iRetVal = ProcessDecoderPropertyForReturn(L"timedRelease", szPropertyValue);
			else if (readerParams.ReaderSpecific.imager_specific.dwAimType == AIM_TYPE_PRESENTATION)
				iRetVal = ProcessDecoderPropertyForReturn(L"presentation", szPropertyValue);
			else if (readerParams.ReaderSpecific.imager_specific.dwAimType == AIM_TYPE_PRESS_AND_RELEASE)
				iRetVal = ProcessDecoderPropertyForReturn(L"pressAndRelease", szPropertyValue);
			else if (readerParams.ReaderSpecific.imager_specific.dwAimType == AIM_TYPE_CONTINUOUS_READ)
				iRetVal = ProcessDecoderPropertyForReturn(L"continuousRead", szPropertyValue);
			else
				iRetVal = -1;
		}
	}
	else if (cmp(szPropertyName, L"AimMode"))
	{
		if (bLaser)
		{
			if (readerParams.ReaderSpecific.laser_specific.dwAimMode == AIM_MODE_NONE)
				iRetVal = ProcessDecoderPropertyForReturn(L"none", szPropertyValue);
			else if (readerParams.ReaderSpecific.laser_specific.dwAimMode == AIM_MODE_DOT)
				iRetVal = ProcessDecoderPropertyForReturn(L"dot", szPropertyValue);
			else if (readerParams.ReaderSpecific.laser_specific.dwAimMode == AIM_MODE_SLAB)
				iRetVal = ProcessDecoderPropertyForReturn(L"slab", szPropertyValue);
			else if (readerParams.ReaderSpecific.laser_specific.dwAimMode == AIM_MODE_RETICLE)
				iRetVal = ProcessDecoderPropertyForReturn(L"reticle", szPropertyValue);
			else
				iRetVal = -1;
		}
		else
		{
			if (readerParams.ReaderSpecific.imager_specific.dwAimMode == AIM_MODE_NONE)
				iRetVal = ProcessDecoderPropertyForReturn(L"none", szPropertyValue);
			else if (readerParams.ReaderSpecific.imager_specific.dwAimMode == AIM_MODE_DOT)
				iRetVal = ProcessDecoderPropertyForReturn(L"dot", szPropertyValue);
			else if (readerParams.ReaderSpecific.imager_specific.dwAimMode == AIM_MODE_SLAB)
				iRetVal = ProcessDecoderPropertyForReturn(L"slab", szPropertyValue);
			else if (readerParams.ReaderSpecific.imager_specific.dwAimMode == AIM_MODE_RETICLE)
				iRetVal = ProcessDecoderPropertyForReturn(L"reticle", szPropertyValue);
			else
				iRetVal = -1;
		}
	}
	else if (cmp(szPropertyName, L"LinearSecurityLevel"))
	{
		if (bLaser)
		{
			if (readerParams.ReaderSpecific.laser_specific.dwLinearSecurityLevel == SECURITY_REDUNDANCY_AND_LENGTH)
				iRetVal = ProcessDecoderPropertyForReturn(L"redundancyAndLength", szPropertyValue);
			else if (readerParams.ReaderSpecific.laser_specific.dwLinearSecurityLevel == SECURITY_SHORT_OR_CODABAR)
				iRetVal = ProcessDecoderPropertyForReturn(L"shortOrCodabar", szPropertyValue);
			else if (readerParams.ReaderSpecific.laser_specific.dwLinearSecurityLevel == SECURITY_LONG_AND_SHORT)
				iRetVal = ProcessDecoderPropertyForReturn(L"longAndShort", szPropertyValue);
			else if (readerParams.ReaderSpecific.laser_specific.dwLinearSecurityLevel == SECURITY_ALL_TWICE)
				iRetVal = ProcessDecoderPropertyForReturn(L"allTwice", szPropertyValue);
			else if (readerParams.ReaderSpecific.laser_specific.dwLinearSecurityLevel == SECURITY_ALL_THRICE)
				iRetVal = ProcessDecoderPropertyForReturn(L"allThrice", szPropertyValue);
			else
				iRetVal = -1;
		}
		else
		{
			// Imager
			if (readerParams.ReaderSpecific.imager_specific.dwLinearSecurityLevel == SECURITY_REDUNDANCY_AND_LENGTH)
				iRetVal = ProcessDecoderPropertyForReturn(L"redundancyAndLength", szPropertyValue);
			else if (readerParams.ReaderSpecific.imager_specific.dwLinearSecurityLevel == SECURITY_SHORT_OR_CODABAR)
				iRetVal = ProcessDecoderPropertyForReturn(L"shortOrCodabar", szPropertyValue);
			else if (readerParams.ReaderSpecific.imager_specific.dwLinearSecurityLevel == SECURITY_LONG_AND_SHORT)
				iRetVal = ProcessDecoderPropertyForReturn(L"longAndShort", szPropertyValue);
			else if (readerParams.ReaderSpecific.imager_specific.dwLinearSecurityLevel == SECURITY_ALL_TWICE)
				iRetVal = ProcessDecoderPropertyForReturn(L"allTwice", szPropertyValue);
			else if (readerParams.ReaderSpecific.imager_specific.dwLinearSecurityLevel == SECURITY_ALL_THRICE)
				iRetVal = ProcessDecoderPropertyForReturn(L"allThrice", szPropertyValue);
			else
				iRetVal = -1;
		}
	}
	else if (cmp(szPropertyName, L"DBPMode"))
	{
		if (readerParams.ReaderSpecific.laser_specific.dwDBPMode == DBP_NORMAL)
			iRetVal = ProcessDecoderPropertyForReturn(L"normal", szPropertyValue);
		else if (readerParams.ReaderSpecific.laser_specific.dwDBPMode == DBP_COMPOSITE)
			iRetVal = ProcessDecoderPropertyForReturn(L"composite", szPropertyValue);
		else
			iRetVal = -1;
	}
	else if (cmp(szPropertyName, L"FocusMode"))
	{
		if (readerParams.ReaderSpecific.imager_specific.dwFocusMode == FOCUS_MODE_FIXED)
			iRetVal = ProcessDecoderPropertyForReturn(L"fixed", szPropertyValue);
		else if (readerParams.ReaderSpecific.imager_specific.dwFocusMode == FOCUS_MODE_AUTO)
			iRetVal = ProcessDecoderPropertyForReturn(L"auto", szPropertyValue);
		else
			iRetVal = -1;
	}
	else if (cmp(szPropertyName, L"PicklistMode"))
	{
		if (readerParams.ReaderSpecific.imager_specific.dwPicklistModeEx == PICKLIST_DISABLED)
			iRetVal = ProcessDecoderPropertyForReturn(L"disabled", szPropertyValue);
		else if (readerParams.ReaderSpecific.imager_specific.dwPicklistModeEx == PICKLIST_HARDWARE_RETICLE)
			iRetVal = ProcessDecoderPropertyForReturn(L"hardwareReticle", szPropertyValue);
		else if (readerParams.ReaderSpecific.imager_specific.dwPicklistModeEx == PICKLIST_SOFTWARE_RETICLE)
			iRetVal = ProcessDecoderPropertyForReturn(L"softwareReticle", szPropertyValue);
		else
			iRetVal = -1;
	}
	else if (cmp(szPropertyName, L"IlluminationMode"))
	{
		if (readerParams.ReaderSpecific.imager_specific.dwIlluminationMode == ILLUMINATION_AUTO)
			iRetVal = ProcessDecoderPropertyForReturn(L"auto", szPropertyValue);
		else if (readerParams.ReaderSpecific.imager_specific.dwIlluminationMode == ILLUMINATION_ALWAYS_ON)
			iRetVal = ProcessDecoderPropertyForReturn(L"alwaysOn", szPropertyValue);
		else if (readerParams.ReaderSpecific.imager_specific.dwIlluminationMode == ILLUMINATION_ALWAYS_OFF)
			iRetVal = ProcessDecoderPropertyForReturn(L"alwaysOff", szPropertyValue);
		else
			iRetVal = -1;
	}
	else if (cmp(szPropertyName, L"ViewfinderMode"))
	{
		if (readerParams.ReaderSpecific.imager_specific.dwVFMode == VIEWFINDER_MODE_ENABLED)
			iRetVal = ProcessDecoderPropertyForReturn(L"enabled", szPropertyValue);
		else if (readerParams.ReaderSpecific.imager_specific.dwVFMode == VIEWFINDER_MODE_DISABLED)
			iRetVal = ProcessDecoderPropertyForReturn(L"disabled", szPropertyValue);
		else if (readerParams.ReaderSpecific.imager_specific.dwVFMode == VIEWFINDER_MODE_STATIC_RETICLE)
			iRetVal = ProcessDecoderPropertyForReturn(L"staticReticle", szPropertyValue);
		else if (readerParams.ReaderSpecific.imager_specific.dwVFMode == VIEWFINDER_MODE_DYNAMIC_RETICLE)
			iRetVal = ProcessDecoderPropertyForReturn(L"dynamicReticle", szPropertyValue);
		else
			iRetVal = -1;
	}
	else if (cmp(szPropertyName, L"ViewfinderFeedback"))
	{
		if (readerParams.ReaderSpecific.imager_specific.dwVFFeedback == VIEWFINDER_FEEDBACK_ENABLED)
			iRetVal = ProcessDecoderPropertyForReturn(L"enabled", szPropertyValue);
		else if (readerParams.ReaderSpecific.imager_specific.dwVFFeedback == VIEWFINDER_FEEDBACK_DISABLED)
			iRetVal = ProcessDecoderPropertyForReturn(L"disabled", szPropertyValue);
		else if (readerParams.ReaderSpecific.imager_specific.dwVFFeedback == VIEWFINDER_FEEDBACK_RETICLE)
			iRetVal = ProcessDecoderPropertyForReturn(L"reticle", szPropertyValue);
		else
			iRetVal = -1;
	}
	else if (cmp(szPropertyName, L"Inverse1DMode"))
	{
		if (readerParams.ReaderSpecific.imager_specific.dwInverse1DMode == INVERSE_ENABLE)
			iRetVal = ProcessDecoderPropertyForReturn(L"enabled", szPropertyValue);
		else if (readerParams.ReaderSpecific.imager_specific.dwInverse1DMode == INVERSE_DISABLE)
			iRetVal = ProcessDecoderPropertyForReturn(L"disabled", szPropertyValue);
		else if (readerParams.ReaderSpecific.imager_specific.dwInverse1DMode == INVERSE_AUTO)
			iRetVal = ProcessDecoderPropertyForReturn(L"auto", szPropertyValue);
		else
			iRetVal = -1;
	}
	else if (cmp(szPropertyName, L"TimedAimDuration"))
	{
		if (bLaser)
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				readerParams.ReaderSpecific.laser_specific.dwAimDuration, szPropertyValue);
		}
		else
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				readerParams.ReaderSpecific.imager_specific.dwAimDuration, szPropertyValue);
		}
	}
	else if (cmp(szPropertyName, L"NarrowBeam"))
	{
		LOG(INFO) + L"NarrowBeam has been deprecated, please use BeamWidth in preference";
		if (readerParams.ReaderSpecific.laser_specific.bNarrowBeam)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"BeamWidth"))
	{
		if (readerParams.ReaderSpecific.laser_specific.dwBeamWidth == BEAM_WIDTH_NORMAL)
			iRetVal = ProcessDecoderPropertyForReturn(L"normal", szPropertyValue);
		else if (readerParams.ReaderSpecific.laser_specific.dwBeamWidth == BEAM_WIDTH_NARROW)
			iRetVal = ProcessDecoderPropertyForReturn(L"narrow", szPropertyValue);
		else if (readerParams.ReaderSpecific.laser_specific.dwBeamWidth == BEAM_WIDTH_WIDE)
			iRetVal = ProcessDecoderPropertyForReturn(L"wide", szPropertyValue);
		else
			iRetVal = -1;
	}
	else if (cmp(szPropertyName, L"ScanTimeout"))
	{
		DWORD dwScanTimeout;
		if (bLaser)
			dwScanTimeout = readerParams.ReaderSpecific.laser_specific.dwBeamTimer;
		else
			dwScanTimeout = readerParams.ReaderSpecific.imager_specific.dwBeamTimer;
		iRetVal = ProcessDecoderPropertyForReturn(dwScanTimeout, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"KlasseEins"))
	{
		if (readerParams.ReaderSpecific.laser_specific.bKlasseEinsEnable)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"BidirectionalRedundancy"))
	{
		if (readerParams.ReaderSpecific.laser_specific.bBidirRedundancy)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"RasterHeight"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(
			readerParams.ReaderSpecific.laser_specific.dwRasterHeight, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"SameSymbolTimeout"))
	{
		if (bLaser)
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				readerParams.ReaderSpecific.laser_specific.dwSameSymbolTimeout, szPropertyValue);
		}
		else
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				readerParams.ReaderSpecific.imager_specific.dwSameSymbolTimeout, szPropertyValue);
		}
	}
	else if (cmp(szPropertyName, L"DifferentSymbolTimeout"))
	{
		if (bLaser)
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				readerParams.ReaderSpecific.laser_specific.dwDifferentSymbolTimeout, szPropertyValue);
		}
		else
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				readerParams.ReaderSpecific.imager_specific.dwDifferentSymbolTimeout, szPropertyValue);
		}
	}
	else if (cmp(szPropertyName, L"PoorQuality1DMode"))
	{
		if (readerParams.ReaderSpecific.imager_specific.bPoorQuality1DMode)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"DPMMode"))
	{
		if (readerParams.ReaderSpecific.imager_specific.dwDPMMode == DPM_ENABLED)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"ViewfinderX"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(
			readerParams.ReaderSpecific.imager_specific.rVFPosition.left, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"ViewfinderY"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(
			readerParams.ReaderSpecific.imager_specific.rVFPosition.top, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"ViewfinderWidth"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(
			(readerParams.ReaderSpecific.imager_specific.rVFPosition.right - 
			readerParams.ReaderSpecific.imager_specific.rVFPosition.left), szPropertyValue);
	}
	else if (cmp(szPropertyName, L"ViewfinderHeight"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(
			(readerParams.ReaderSpecific.imager_specific.rVFPosition.bottom - 
			readerParams.ReaderSpecific.imager_specific.rVFPosition.top), szPropertyValue);
	}
	else if (cmp(szPropertyName, L"ViewfinderFeedbackTime"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(
			readerParams.ReaderSpecific.imager_specific.dwVFFeedbackTime, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"AdaptiveScanning"))
	{
		if (readerParams.ReaderSpecific.laser_specific.dwAdaptiveScanning == ADAPTIVE_SCANNING_ENABLE)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"ScanType"))
	{
		if (m_currentScanType == SCAN_TYPE_FOREGROUND)
			iRetVal = ProcessDecoderPropertyForReturn(L"foreground", szPropertyValue);
		else if (m_currentScanType == SCAN_TYPE_BACKGROUND)
			iRetVal = ProcessDecoderPropertyForReturn(L"background", szPropertyValue);
		else if (m_currentScanType == SCAN_TYPE_MONITOR)
			iRetVal = ProcessDecoderPropertyForReturn(L"monitor", szPropertyValue);
		else
			iRetVal = -1;
	}
	else if (cmp(szPropertyName, L"dataBufferSize"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(m_iDataBufferSize, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"BarcodeDataFormat"))
	{
		if (m_bufferFormat == BUFFER_TEXT)
			iRetVal = ProcessDecoderPropertyForReturn(L"text", szPropertyValue);
		else if (m_bufferFormat == BUFFER_RAW_DATA)
			iRetVal = ProcessDecoderPropertyForReturn(L"binary", szPropertyValue);
		else
			iRetVal = -1;
	}
	else if (cmp(szPropertyName, L"ScannerType"))
	{
		if (bLaser)
			iRetVal = ProcessDecoderPropertyForReturn(L"laser", szPropertyValue);
		else if (m_bUsingBluetoothScanner)
			iRetVal = ProcessDecoderPropertyForReturn(L"bluetooth", szPropertyValue);
		else{
			DWORD dwGetDeviceType = (SCANNER_TYPE_IMAGER|TRANSPORT_TYPE_INTERNAL);
			WCHAR szScannerDeviceID[5];
			wsprintf(szScannerDeviceID, L"SCN%i:", m_iInstanceID);
			
			SCAN_FINDINFO	scanFindInfo;
			memset(&scanFindInfo, 0, sizeof(scanFindInfo));
			scanFindInfo.StructInfo.dwAllocated = sizeof(scanFindInfo);
			HANDLE	scanFindHandle = NULL;

			DWORD dwResult = lpfn_SCAN_FindFirst(&scanFindInfo, &scanFindHandle);
			while(dwResult == E_SCN_SUCCESS)
			{
				//  Application has requested a specific scanner, check to see
				//  if this is the scanner requested
				if (!wcscmp(szScannerDeviceID, scanFindInfo.szDeviceName))
				{
					//Found the scanner requested by the user
					dwGetDeviceType = scanFindInfo.dwDeviceType;
					break;
				}
				dwResult = lpfn_SCAN_FindNext(&scanFindInfo, scanFindHandle);
			}
			lpfn_SCAN_FindClose(scanFindHandle);
			
			if(dwGetDeviceType == (SCANNER_TYPE_CAMERA|TRANSPORT_TYPE_INTERNAL)){
				iRetVal = ProcessDecoderPropertyForReturn(L"camera", szPropertyValue);
			}	
			else{
				iRetVal = ProcessDecoderPropertyForReturn(L"imager", szPropertyValue);
			}
		}
	}
	return iRetVal;
}

int CScanner::ScannerBluetoothTag_GetPropertyValue(LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	INTERFACE_PARAMS interfaceParams;
	RetrieveScannerInterfaceParameters(&interfaceParams);
	if (cmp(szPropertyName, L"ConnectionIdleTimeout"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(
				interfaceParams.InterfaceSpecific.SsiSpecific.dwConnectionIdleTimeout
				, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"DisconnectBTOnDisable"))
	{
		if (interfaceParams.InterfaceSpecific.SsiSpecific.bBTDisconnectOnExit)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"displayBTAddressBarcodeOnEnable"))
	{
		if (m_iDisplayBTPairBarcode)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"enableTimeout"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(m_dwEnableTimeout, szPropertyValue);
	}
	return iRetVal;
}
