#include "Scanner.h"
#include <pm.h>

#pragma region Scanner_EMML_Tag_Handlers

//  Function used for the top level scanner tags
BOOL CScanner::ScannerTag_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	BOOL bRetVal = TRUE;
	if (EnabledValue(szPropertyName) || 
		cmp(szPropertyName, L"init") ||
		cmp(szPropertyName, L"initialise"))
	{
		if (!m_bScannerOpen)
		{
			//  Scanner is not open, open the specified Scanner
			if (szPropertyValue == NULL || (wcslen(szPropertyValue) < 1))
			{
				if (!OpenScanner(m_szUserSpecifiedScanner))
				{
					LOG(WARNING) + "OpenScanner Failed!";
					return FALSE;
				}
			}
			else
			{
				if (!OpenScanner(szPropertyValue))
				{
					LOG(WARNING) + "OpenScanner Failed!";
					return FALSE;
				}
			}
		}
		else
		{
			//  The Scanner is open, check it is the correct scanner
			//  which is open and reopen the correct scanner if 
			//  appropriate
			if (szPropertyValue != NULL && 
				(wcslen(szPropertyValue) > 0) && 
				m_szCurrentlyOpenScanner &&
				!(cmp(m_szCurrentlyOpenScanner, szPropertyValue)))
			{
				CloseScanner();
				if (!OpenScanner(szPropertyValue))
				{
					LOG(WARNING) + "OpenScanner Failed!";
					return FALSE;
				}
			}
		}
		if (!m_bScannerEnabled && !m_bScannerBeingEnabled)
		{
			if (!EnableScanner())
			{
				LOG(WARNING) + "EnableScanner failed!";
			}
		}
		else
		{
			LOG(INFO) + L"Attempted to Enable the Scanner but it is already enabled";
		}
	}
	else if (DisabledValue(szPropertyName))
	{
		//  Empty the list of tags on the current page since disabling the scanner resets
		//  the hardware to a default state.
		ActionPendingMetaTags(m_hTagsOnCurrentPageQRead, false);
		bRetVal = CloseScanner();
	}
	else if (cmp(szPropertyName, L"GetDeviceCapabilities"))
	{
		//  New functionality for RhoElements
		CloseHandle(CreateThread(NULL, 0, &DeviceCapabilitiesToApplication, (void*)this, NULL, 0));
	}
	else if (cmp(szPropertyName, L"deinit") ||
		cmp(szPropertyName, L"deinitialise"))
	{
		//  Not publicly documented
		bRetVal = CloseScanner();
	}
	else if (cmp(szPropertyName, L"Start"))
	{
		BOOL bSetTrigger = TRUE;
		bRetVal = (lpfn_SCAN_SetSoftTrigger(m_hScanner, &bSetTrigger) == E_SCN_SUCCESS);
	}
	else if (cmp(szPropertyName, L"Stop"))
	{
		BOOL bSetTrigger = FALSE;
		bRetVal = (lpfn_SCAN_SetSoftTrigger(m_hScanner, &bSetTrigger) == E_SCN_SUCCESS);
	}
	else if (cmp(szPropertyName, L"triggerConnected"))
	{
		if (EnabledValue(szPropertyValue))
		{
			//  Connect the Trigger
			if (m_bTriggerConnected)
				LOG(INFO) + "Call to connect Trigger is ineffective, the trigger is already connected";
			else
			{
				m_bTriggerConnected = true;
				//  Re-submit the Scan read if the scanner is enabled
				if (m_bScannerEnabled)
					SubmitRead();
			}
		}
		else
		{
			//  Disconnect the Trigger
			if (!m_bTriggerConnected)
				LOG(INFO) + "Call to disconnect Trigger is ineffective, the trigger is already disconnected";
			else
			{
				LOG(INFO) + "Trigger disconnected.  To scan via the hardware trigger you must re-connect the trigger";
				m_bTriggerConnected = false;
				//  Flush the pending scanner read to disconnect the trigger.  The read will not be resubmitted because 
				//  m_bTriggerConnected is false
				lpfn_SCAN_Flush(m_hScanner);
			}
		}
	}
	else if (cmp(szPropertyName, L"AutoEnter"))
	{
		if (EnabledValue(szPropertyValue))
			m_bAutoEnter = true;
		else if (DisabledValue(szPropertyValue))
			m_bAutoEnter = false;
		else
			bRetVal = FALSE;
	}
	else if (cmp(szPropertyName, L"AutoTab"))
	{
		if (EnabledValue(szPropertyValue))
			m_bAutoTab = true;
		else if (DisabledValue(szPropertyValue))
			m_bAutoTab = false;
		else
			bRetVal = FALSE;
	}
	else if (cmp(szPropertyName, L"DisableScannerDuringNavigate"))
	{
		if (EnabledValue(szPropertyValue))
			m_bLegacyNavBehaviour = false;
		else
			m_bLegacyNavBehaviour = true;
	}
	else if (cmp(szPropertyName, L"lowBatteryScan"))
	{
		if (EnabledValue(szPropertyValue))
		{
			//Modified by Abhineet Agarwal
			m_bLowBatteryScan = true;
			LOG(INFO) + L"Setting LowBatteryScan property to True.";
			//Low Battery Scan - True, No effect to scanner, if battery is low or critical...
			//Terminate thread if running.....
			StartOrDisableCheckingLowBattery();
		}
		else
		{
			//Modified by Abhineet Agarwal
			m_bLowBatteryScan = false;
			LOG(INFO) + L"Setting LowBatteryScan property to False.";
			//Low Battery Scan - False, Disable scanner, if battery is low or critical...
			//Start low battery thread if not running and disable the scanner if the battery is low or critical....
			StartOrDisableCheckingLowBattery();
		}
	}

	return bRetVal;
}

BOOL CScanner::ScannerBeeper_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	BOOL bRetVal = TRUE;
	if (cmp(szPropertyName, L"decodeVolume"))
	{
		DWORD dwValue = StringToInt(szPropertyValue);
		if (dwValue <= MAXIMUM_DECODE_VOLUME)
			m_decodeNotifier->SetDecodeVolume(dwValue);
		else
		{
			LOG(WARNING) + L"Attempted to set Decode Volume to an out of range value";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"decodeDuration"))
	{
		DWORD dwValue = StringToInt(szPropertyValue);
		if (dwValue <= MAXIMUM_DECODE_DURATION)
			m_decodeNotifier->SetDecodeDuration(dwValue);
		else
		{
			LOG(WARNING) + L"Attempted to set Decode Duration to an out of range value";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"successfulDecodeFrequency") || cmp(szPropertyName, L"decodeFrequency"))
	{
		DWORD dwValue = StringToInt(szPropertyValue);
		if (dwValue <= MAXIMUM_SUCCESSFUL_DECODE_FREQUENCY)
			m_decodeNotifier->SetSuccessfulDecodeFrequency(dwValue);
		else
		{
			LOG(WARNING) + L"Attempted to set Successful Decode Frequency to an out of range value";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"invalidDecodeFrequency"))
	{
		DWORD dwValue = StringToInt(szPropertyValue);
		if (dwValue <= MAXIMUM_INVALID_DECODE_FREQUENCY)
			m_decodeNotifier->SetInvalidDecodeFrequency(dwValue);
		else
		{
			LOG(WARNING) + L"Attempted to set Invalid Decode Frequency to an out of range value";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"successfulDecodeSound") || 
		cmp(szPropertyName, L"DecodeSound"))
	{
		if (szPropertyValue && wcslen(szPropertyValue) == 0)
			m_decodeNotifier->SetSuccessfulDecodeSound(NULL);
		else if (szPropertyValue && wcslen(szPropertyValue) == wcslen(L"url('')") &&
			wcsicmp(szPropertyValue, L"url('')") == 0)
			m_decodeNotifier->SetSuccessfulDecodeSound(NULL);
		else
			m_decodeNotifier->SetSuccessfulDecodeSound(szPropertyValue);
	}
	else if (cmp(szPropertyName, L"invalidDecodeSound"))
	{
		if (szPropertyValue && wcslen(szPropertyValue) == 0)
			m_decodeNotifier->SetInvalidDecodeSound(NULL);
		else if (szPropertyValue && wcslen(szPropertyValue) == wcslen(L"url('')") &&
			wcsicmp(szPropertyValue, L"url('')") == 0)
			m_decodeNotifier->SetInvalidDecodeSound(NULL);
		else
			m_decodeNotifier->SetInvalidDecodeSound(szPropertyValue);
	}

	return bRetVal;
}

BOOL CScanner::ScannerReaderTag_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	BOOL bRetVal = TRUE;
	READER_PARAMS readerParams;
	RetrieveScannerParameters(&readerParams);
	bool bLaser = readerParams.dwReaderType == READER_TYPE_LASER;
	if (cmp(szPropertyName, L"RasterMode"))
	{
		if (bLaser)
		{
			//  Note - Raster Mode Cyclone is no longer mentioned in the EMDK help file
			if (cmp(szPropertyValue, L"none"))
				readerParams.ReaderSpecific.laser_specific.dwRasterMode = RASTER_MODE_NONE;
			else if (cmp(szPropertyValue, L"Open_Always") || 
				cmp(szPropertyValue, L"OpenAlways"))
				readerParams.ReaderSpecific.laser_specific.dwRasterMode = RASTER_MODE_OPEN_ALWAYS;
			else if (cmp(szPropertyValue, L"Smart"))
				readerParams.ReaderSpecific.laser_specific.dwRasterMode = RASTER_MODE_SMART;
			else if (cmp(szPropertyValue, L"Cyclone"))
				readerParams.ReaderSpecific.laser_specific.dwRasterMode = RASTER_MODE_CYCLONE;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set laser specific Raster Mode on an Imager Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"AimType"))
	{
		if (bLaser)
		{
			if (cmp(szPropertyValue, L"Trigger"))
				readerParams.ReaderSpecific.laser_specific.dwAimType = AIM_TYPE_TRIGGER;
			else if (cmp(szPropertyValue, L"Timed_Hold") ||
					cmp(szPropertyValue, L"TimedHold"))
				readerParams.ReaderSpecific.laser_specific.dwAimType = AIM_TYPE_TIMED_HOLD;
			else if (cmp(szPropertyValue, L"Timed_Release") ||
					cmp(szPropertyValue, L"TimedRelease"))
				readerParams.ReaderSpecific.laser_specific.dwAimType = AIM_TYPE_TIMED_RELEASE;
			else if (cmp(szPropertyValue, L"Presentation"))
				readerParams.ReaderSpecific.laser_specific.dwAimType = AIM_TYPE_PRESENTATION;
			else if (cmp(szPropertyValue, L"Press_And_Release") ||
					cmp(szPropertyValue, L"PressAndRelease"))
				readerParams.ReaderSpecific.laser_specific.dwAimType = AIM_TYPE_PRESS_AND_RELEASE;
			else if (cmp(szPropertyValue, L"Continuous_Read") ||
					cmp(szPropertyValue, L"ContinuousRead"))
				readerParams.ReaderSpecific.laser_specific.dwAimType = AIM_TYPE_CONTINUOUS_READ;
			else
				bRetVal = FALSE;
		}
		else
		{
			//  Setting AimType to Presentation on MK31xx can cause it not to start (ane possibly other side effects)
			WCHAR buf[255];
			if (SystemParametersInfo(SPI_GETOEMINFO, 255, buf, 0))
			{
				if (wcsstr(buf, L"MK31") != NULL)
				{
					LOG(WARNING) + L"Setting Aim Type is not supported on MK31xx devices, the default value is 'Presentation'";
					return FALSE;
				}
			}

			if (cmp(szPropertyValue, L"Trigger"))
				readerParams.ReaderSpecific.imager_specific.dwAimType = AIM_TYPE_TRIGGER;
			else if (cmp(szPropertyValue, L"Timed_Hold") ||
					cmp(szPropertyValue, L"TimedHold"))
				readerParams.ReaderSpecific.imager_specific.dwAimType = AIM_TYPE_TIMED_HOLD;
			else if (cmp(szPropertyValue, L"Timed_Release") ||
					cmp(szPropertyValue, L"TimedRelease"))
				readerParams.ReaderSpecific.imager_specific.dwAimType = AIM_TYPE_TIMED_RELEASE;
			else if (cmp(szPropertyValue, L"Presentation"))
				readerParams.ReaderSpecific.imager_specific.dwAimType = AIM_TYPE_PRESENTATION;
			else if (cmp(szPropertyValue, L"Press_And_Release") ||
					cmp(szPropertyValue, L"PressAndRelease"))
				readerParams.ReaderSpecific.imager_specific.dwAimType = AIM_TYPE_PRESS_AND_RELEASE;
			else if (cmp(szPropertyValue, L"Continuous_Read") ||
					cmp(szPropertyValue, L"ContinuousRead"))
				readerParams.ReaderSpecific.imager_specific.dwAimType = AIM_TYPE_CONTINUOUS_READ;
			else
				bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"AimMode"))
	{
		if (bLaser)
		{
			if (cmp(szPropertyValue, L"None"))
				readerParams.ReaderSpecific.laser_specific.dwAimMode = AIM_MODE_NONE;
			else if (cmp(szPropertyValue, L"Dot"))
				readerParams.ReaderSpecific.laser_specific.dwAimMode = AIM_MODE_DOT;
			else if (cmp(szPropertyValue, L"Slab"))
				readerParams.ReaderSpecific.laser_specific.dwAimMode = AIM_MODE_SLAB;
			else if (cmp(szPropertyValue, L"Reticle"))
				readerParams.ReaderSpecific.laser_specific.dwAimMode = AIM_MODE_RETICLE;
			else
				bRetVal = FALSE;
		}
		else
		{
			if (cmp(szPropertyValue, L"None"))
				readerParams.ReaderSpecific.imager_specific.dwAimMode = AIM_MODE_NONE;
			else if (cmp(szPropertyValue, L"Dot"))
				readerParams.ReaderSpecific.imager_specific.dwAimMode = AIM_MODE_DOT;
			else if (cmp(szPropertyValue, L"Slab"))
				readerParams.ReaderSpecific.imager_specific.dwAimMode = AIM_MODE_SLAB;
			else if (cmp(szPropertyValue, L"Reticle"))
				readerParams.ReaderSpecific.imager_specific.dwAimMode = AIM_MODE_RETICLE;
			else
				bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"LinearSecurityLevel"))
	{
		if (bLaser)
		{
			if (cmp(szPropertyValue, L"redundancyAndLength"))
				readerParams.ReaderSpecific.laser_specific.dwLinearSecurityLevel = SECURITY_REDUNDANCY_AND_LENGTH;
			else if (cmp(szPropertyValue, L"shortOrCodabar"))
				readerParams.ReaderSpecific.laser_specific.dwLinearSecurityLevel = SECURITY_SHORT_OR_CODABAR;
			else if (cmp(szPropertyValue, L"longAndShort"))
				readerParams.ReaderSpecific.laser_specific.dwLinearSecurityLevel = SECURITY_LONG_AND_SHORT;
			else if (cmp(szPropertyValue, L"allTwice"))
				readerParams.ReaderSpecific.laser_specific.dwLinearSecurityLevel = SECURITY_ALL_TWICE;
			else if (cmp(szPropertyValue, L"allThrice"))
				readerParams.ReaderSpecific.laser_specific.dwLinearSecurityLevel = SECURITY_ALL_THRICE;
			else
				bRetVal = FALSE;
		}
		else
		{
			//  Imager
			if (cmp(szPropertyValue, L"redundancyAndLength"))
				readerParams.ReaderSpecific.imager_specific.dwLinearSecurityLevel = SECURITY_REDUNDANCY_AND_LENGTH;
			else if (cmp(szPropertyValue, L"shortOrCodabar"))
				readerParams.ReaderSpecific.imager_specific.dwLinearSecurityLevel = SECURITY_SHORT_OR_CODABAR;
			else if (cmp(szPropertyValue, L"longAndShort"))
				readerParams.ReaderSpecific.imager_specific.dwLinearSecurityLevel = SECURITY_LONG_AND_SHORT;
			else if (cmp(szPropertyValue, L"allTwice"))
				readerParams.ReaderSpecific.imager_specific.dwLinearSecurityLevel = SECURITY_ALL_TWICE;
			else if (cmp(szPropertyValue, L"allThrice"))
				readerParams.ReaderSpecific.imager_specific.dwLinearSecurityLevel = SECURITY_ALL_THRICE;
			else
				bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"DBPMode"))
	{
		if (bLaser)
		{
			if (cmp(szPropertyValue, L"Normal"))
				readerParams.ReaderSpecific.laser_specific.dwDBPMode = DBP_NORMAL;
			else if (cmp(szPropertyValue, L"Composite"))
				readerParams.ReaderSpecific.laser_specific.dwDBPMode = DBP_COMPOSITE;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Laser setting DBPMode on an Imager Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"FocusMode"))
	{
		if (!bLaser)
		{
			if (cmp(szPropertyValue, L"Fixed"))
				readerParams.ReaderSpecific.imager_specific.dwFocusMode = FOCUS_MODE_FIXED;
			else if (cmp(szPropertyValue, L"Auto"))
				readerParams.ReaderSpecific.imager_specific.dwFocusMode = FOCUS_MODE_AUTO;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property FocusMode on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"PicklistMode"))
	{
		if (!bLaser)
		{
			if (cmp(szPropertyValue, L"Disabled"))
				readerParams.ReaderSpecific.imager_specific.dwPicklistModeEx = PICKLIST_DISABLED;
			else if (cmp(szPropertyValue, L"HardwareReticle"))
				readerParams.ReaderSpecific.imager_specific.dwPicklistModeEx = PICKLIST_HARDWARE_RETICLE;
			else if (cmp(szPropertyValue, L"SoftwareReticle"))
				readerParams.ReaderSpecific.imager_specific.dwPicklistModeEx = PICKLIST_SOFTWARE_RETICLE;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property PicklistMode on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"IlluminationMode"))
	{
		if (!bLaser)
		{
			if (cmp(szPropertyValue, L"Auto"))
				readerParams.ReaderSpecific.imager_specific.dwIlluminationMode = ILLUMINATION_AUTO;
			else if (cmp(szPropertyValue, L"AlwaysOn"))
				readerParams.ReaderSpecific.imager_specific.dwIlluminationMode = ILLUMINATION_ALWAYS_ON;
			else if (cmp(szPropertyValue, L"AlwaysOff"))
				readerParams.ReaderSpecific.imager_specific.dwIlluminationMode = ILLUMINATION_ALWAYS_OFF;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property IlluminationMode on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"ViewfinderMode"))
	{
		if (!bLaser)
		{
			if (EnabledValue(szPropertyValue))
				readerParams.ReaderSpecific.imager_specific.dwVFMode = VIEWFINDER_MODE_ENABLED;
			else if (DisabledValue(szPropertyValue))
				readerParams.ReaderSpecific.imager_specific.dwVFMode = VIEWFINDER_MODE_DISABLED;
			else if (cmp(szPropertyValue, L"StaticReticle"))
				readerParams.ReaderSpecific.imager_specific.dwVFMode = VIEWFINDER_MODE_STATIC_RETICLE;
			else if (cmp(szPropertyValue, L"DynamicReticle"))
				readerParams.ReaderSpecific.imager_specific.dwVFMode = VIEWFINDER_MODE_DYNAMIC_RETICLE;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property ViewfinderMode on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"ViewfinderFeedback"))
	{
		if (!bLaser)
		{
			if (EnabledValue(szPropertyValue))
				readerParams.ReaderSpecific.imager_specific.dwVFFeedback = VIEWFINDER_FEEDBACK_ENABLED;
			else if (DisabledValue(szPropertyValue))
				readerParams.ReaderSpecific.imager_specific.dwVFFeedback = VIEWFINDER_FEEDBACK_DISABLED;
			else if (cmp(szPropertyValue, L"Reticle"))
				readerParams.ReaderSpecific.imager_specific.dwVFFeedback = VIEWFINDER_FEEDBACK_RETICLE;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property ViewfinderFeedback on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"Inverse1DMode"))
	{
		if (!bLaser)
		{
			if (EnabledValue(szPropertyValue))
				readerParams.ReaderSpecific.imager_specific.dwInverse1DMode = INVERSE_ENABLE;
			else if (DisabledValue(szPropertyValue))
				readerParams.ReaderSpecific.imager_specific.dwInverse1DMode = INVERSE_DISABLE;
			else if (cmp(szPropertyValue, L"Auto"))
				readerParams.ReaderSpecific.imager_specific.dwInverse1DMode = INVERSE_AUTO;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property Inverse1DMode on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"TimedAimDuration"))
	{
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_TIMED_AIM_DURATION)
		{
			if (bLaser)
				readerParams.ReaderSpecific.laser_specific.dwAimDuration = iValue;
			else
				readerParams.ReaderSpecific.imager_specific.dwAimDuration = iValue;
		}
		else
		{
			LOG(WARNING) + L"TimedAimDuration is out of Range";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"NarrowBeam"))
	{
		LOG(INFO) + L"NarrowBeam has been deprecated, please use BeamWidth in preference";
		if (bLaser)
		{
			if (cmp(szPropertyValue, L"TRUE"))
				readerParams.ReaderSpecific.laser_specific.bNarrowBeam = TRUE;
			else if (cmp(szPropertyValue, L"FALSE"))
				readerParams.ReaderSpecific.laser_specific.bNarrowBeam = FALSE;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Laser Scanner only property NarrowBeam on an Imager Scanner";
			bRetVal = FALSE;
		}

	}
	else if (cmp(szPropertyName, L"BeamWidth"))
	{
		if (bLaser)
		{
			if (cmp(szPropertyValue, L"Normal"))
				readerParams.ReaderSpecific.laser_specific.dwBeamWidth = BEAM_WIDTH_NORMAL;
			else if (cmp(szPropertyValue, L"Narrow"))
				readerParams.ReaderSpecific.laser_specific.dwBeamWidth = BEAM_WIDTH_NARROW;
			else if (cmp(szPropertyValue, L"Wide"))
				readerParams.ReaderSpecific.laser_specific.dwBeamWidth = BEAM_WIDTH_WIDE;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Laser Scanner only property BeamWidth on an Imager Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"ScanTimeout"))
	{
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_SCAN_TIMEOUT_DURATION)
		{
			if (bLaser)
				readerParams.ReaderSpecific.laser_specific.dwBeamTimer = iValue;
			else	
				readerParams.ReaderSpecific.imager_specific.dwBeamTimer = iValue;
		}
		else
		{
			LOG(WARNING) + L"ScanTimeout is out of Range";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"KlasseEins"))
	{
		if (bLaser)
		{
			if (cmp(szPropertyValue, L"TRUE"))
				readerParams.ReaderSpecific.laser_specific.bKlasseEinsEnable = TRUE;
			else if (cmp(szPropertyValue, L"FALSE"))
				readerParams.ReaderSpecific.laser_specific.bKlasseEinsEnable = FALSE;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Laser Scanner only property KlasseEins on an Imager Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"BidirectionalRedundancy"))
	{
		if (bLaser)
		{
			if (cmp(szPropertyValue, L"TRUE"))
				readerParams.ReaderSpecific.laser_specific.bBidirRedundancy = TRUE;
			else if (cmp(szPropertyValue, L"FALSE"))
				readerParams.ReaderSpecific.laser_specific.bBidirRedundancy = FALSE;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Laser Scanner only property BidirectionalRedundancy on an Imager Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"RasterHeight"))
	{
		if (bLaser)
		{
			int iValue = StringToInt(szPropertyValue);
			if (iValue >= MINIMUM_RASTER_HEIGHT && iValue <= MAXIMUM_RASTER_HEIGHT)
			{
				readerParams.ReaderSpecific.laser_specific.dwRasterHeight = iValue;
			}
			else
			{
				LOG(WARNING) + L"RasterHeight is out of Range";
				bRetVal = FALSE;
			}
		}
		else
		{
			LOG(WARNING) + "Attempted to set Laser Scanner only property RasterHeight on an Imager Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"SameSymbolTimeout"))
	{
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_SAME_SYMBOL_TIMEOUT)
		{
			if (bLaser)
				readerParams.ReaderSpecific.laser_specific.dwSameSymbolTimeout = iValue;
			else
				readerParams.ReaderSpecific.imager_specific.dwSameSymbolTimeout = iValue;
		}
		else
		{
			LOG(WARNING) + L"SameSymbolTimeout is out of Range";
			bRetVal = FALSE;
		}

	}
	else if (cmp(szPropertyName, L"DifferentSymbolTimeout"))
	{
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_DIFFERENT_SYMBOL_TIMEOUT)
		{
			if (bLaser)
				readerParams.ReaderSpecific.laser_specific.dwDifferentSymbolTimeout = iValue;
			else
				readerParams.ReaderSpecific.imager_specific.dwDifferentSymbolTimeout = iValue;
		}
		else
		{
			LOG(WARNING) + L"DifferentSymbolTimeout is out of Range";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"PoorQuality1DMode"))
	{
		if (!bLaser)
		{
			if (cmp(szPropertyValue, L"TRUE"))
				readerParams.ReaderSpecific.imager_specific.bPoorQuality1DMode = TRUE;
			else if (cmp(szPropertyValue, L"FALSE"))
				readerParams.ReaderSpecific.imager_specific.bPoorQuality1DMode = FALSE;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property PoorQuality1DMode on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"DPMMode"))
	{
		if (bLaser)
		{
			if (EnabledValue(szPropertyValue) ||
				cmp(szPropertyValue, L"TRUE"))
				readerParams.ReaderSpecific.imager_specific.dwDPMMode = DPM_ENABLED;
			else if (DisabledValue(szPropertyValue) ||
				cmp(szPropertyValue, L"FALSE"))
				readerParams.ReaderSpecific.imager_specific.dwDPMMode = DPM_DISABLED;
			else
				bRetVal = FALSE;
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property DPMMode on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"ViewfinderX"))
	{
		if (!bLaser)
		{
			//  There is lots of logic around setting the viewfinder position as the EMDK
			//  complains if you try and set the values beyond the screen coordinates.
			//  We therefore keep track of the requested coordinates but only set the values
			//  at the EMDK that do not exceed the screen dimensions (in which case we
			//  set to the screen dimensions)
			//  Unfortunately the EMDK locks it's maximum size to the screen orientation
			//  when you first launch so you can't properly handle orientation changes.
			int iScreenWidth = GetSystemMetrics(SM_CXSCREEN) - 1;
			int iScreenHeight = GetSystemMetrics(SM_CYSCREEN) - 1;
			int iValue = StringToInt(szPropertyValue);
			if (iValue >= MINIMUM_VIEWFINDER_X && iValue <= iScreenWidth)
			{
				int iRight = m_iViewfinderRequestedRight;
				if (iRight == -1)
					iRight = readerParams.ReaderSpecific.imager_specific.rVFPosition.right;
				int iLeft = m_iViewfinderRequestedLeft;
				if (iLeft == -1)
					iLeft = readerParams.ReaderSpecific.imager_specific.rVFPosition.left;
				int iWidth = iRight - iLeft;
				//  Move the whole window
				m_iViewfinderRequestedLeft = iValue;
				m_iViewfinderRequestedRight = iValue + iWidth;
				readerParams.ReaderSpecific.imager_specific.rVFPosition.left = iValue;
				if (m_iViewfinderRequestedRight > iScreenWidth)
					readerParams.ReaderSpecific.imager_specific.rVFPosition.right = iScreenWidth;
				else
					readerParams.ReaderSpecific.imager_specific.rVFPosition.right = m_iViewfinderRequestedRight;
				if (readerParams.ReaderSpecific.imager_specific.rVFPosition.bottom > iScreenHeight)
				{
					m_iViewfinderRequestedBottom = readerParams.ReaderSpecific.imager_specific.rVFPosition.bottom;
					readerParams.ReaderSpecific.imager_specific.rVFPosition.bottom = iScreenHeight;
				}
			}
			else
			{
				LOG(WARNING) + L"ViewfinderX is out of Range";
				bRetVal = FALSE;
			}
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property ViewfinderX on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"ViewfinderY"))
	{
		if (!bLaser)
		{
			int iScreenWidth = GetSystemMetrics(SM_CXSCREEN) - 1;
			int iScreenHeight = GetSystemMetrics(SM_CYSCREEN) - 1;
			int iValue = StringToInt(szPropertyValue);
			if (iValue >= MINIMUM_VIEWFINDER_Y && iValue <= iScreenHeight)
			{
				int iBottom = m_iViewfinderRequestedBottom;
				if (iBottom == -1)
					iBottom = readerParams.ReaderSpecific.imager_specific.rVFPosition.bottom;
				int iTop = m_iViewfinderRequestedTop;
				if (iTop == -1)
					iTop = readerParams.ReaderSpecific.imager_specific.rVFPosition.top;
				int iHeight = iBottom - iTop;
				//  Move the whole window
				m_iViewfinderRequestedTop = iValue;
				m_iViewfinderRequestedBottom = iValue + iHeight;
				readerParams.ReaderSpecific.imager_specific.rVFPosition.top = m_iViewfinderRequestedTop;
				if (m_iViewfinderRequestedBottom > iScreenHeight)
					readerParams.ReaderSpecific.imager_specific.rVFPosition.bottom = iScreenHeight;
				else
					readerParams.ReaderSpecific.imager_specific.rVFPosition.bottom = m_iViewfinderRequestedBottom;
				if (readerParams.ReaderSpecific.imager_specific.rVFPosition.right > iScreenWidth)
				{
					m_iViewfinderRequestedRight = readerParams.ReaderSpecific.imager_specific.rVFPosition.right;
					readerParams.ReaderSpecific.imager_specific.rVFPosition.right = iScreenWidth;
				}
			}
			else
			{
				LOG(WARNING) + L"ViewfinderY is out of Range";
				bRetVal = FALSE;
			}
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property ViewfinderY on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"ViewfinderWidth"))
	{
		if (!bLaser)
		{
			int iScreenWidth = GetSystemMetrics(SM_CXSCREEN) - 1;
			int iScreenHeight = GetSystemMetrics(SM_CYSCREEN) - 1;
			int iValue = StringToInt(szPropertyValue);
			if ((iValue) >= MINIMUM_VIEWFINDER_WIDTH)
			{
				int iLeft = m_iViewfinderRequestedLeft;
				if (iLeft == -1)
					iLeft = readerParams.ReaderSpecific.imager_specific.rVFPosition.left;
				m_iViewfinderRequestedRight = iLeft + iValue;
				if (m_iViewfinderRequestedRight > iScreenWidth)
					readerParams.ReaderSpecific.imager_specific.rVFPosition.right = iScreenWidth;
				else
					readerParams.ReaderSpecific.imager_specific.rVFPosition.right = m_iViewfinderRequestedRight;
				if (readerParams.ReaderSpecific.imager_specific.rVFPosition.bottom > iScreenHeight)
				{
					m_iViewfinderRequestedBottom = readerParams.ReaderSpecific.imager_specific.rVFPosition.bottom;
					readerParams.ReaderSpecific.imager_specific.rVFPosition.bottom = iScreenHeight;
				}
			}
			else
			{
				LOG(WARNING) + L"Viewfinder Width is out of Range";
				bRetVal = FALSE;
			}
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property ViewfinderWidth on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"ViewfinderHeight"))
	{
		if (!bLaser)
		{
			int iScreenWidth = GetSystemMetrics(SM_CXSCREEN) - 1;
			int iScreenHeight = GetSystemMetrics(SM_CYSCREEN) - 1;
			int iValue = StringToInt(szPropertyValue);
			if ((iValue) >= MINIMUM_VIEWFINDER_HEIGHT)
			{
				int iTop = m_iViewfinderRequestedTop;
				if (iTop == -1)
					iTop = readerParams.ReaderSpecific.imager_specific.rVFPosition.top;
				m_iViewfinderRequestedBottom = iTop + iValue;
				if (m_iViewfinderRequestedBottom > iScreenHeight)
					readerParams.ReaderSpecific.imager_specific.rVFPosition.bottom = iScreenHeight;
				else
					readerParams.ReaderSpecific.imager_specific.rVFPosition.bottom = m_iViewfinderRequestedBottom;
				if (readerParams.ReaderSpecific.imager_specific.rVFPosition.right > iScreenWidth)
				{
					m_iViewfinderRequestedRight = readerParams.ReaderSpecific.imager_specific.rVFPosition.right;
					readerParams.ReaderSpecific.imager_specific.rVFPosition.right = iScreenWidth;
				}
			}
			else
			{
				LOG(WARNING) + L"Viewfinder Height is out of Range";
				bRetVal = FALSE;
			}
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property ViewfinderHeight on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"ViewfinderFeedbackTime"))
	{
		if (!bLaser)
		{
			int iValue = StringToInt(szPropertyValue);
			if (iValue >= MINIMUM_VIEWFINDER_FEEDBACK_TIME)
			{
				readerParams.ReaderSpecific.imager_specific.dwVFFeedbackTime = iValue;
			}
			else
			{
				LOG(WARNING) + L"Viewfinder Feedback Time is out of Range";
				bRetVal = FALSE;
			}
		}
		else
		{
			LOG(WARNING) + "Attempted to set Imager Scanner only property ViewfinderFeedbackTime on a Laser Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"AdaptiveScanning"))
	{
		if (bLaser)
		{	
			if (cmp(szPropertyValue, L"TRUE"))
				readerParams.ReaderSpecific.laser_specific.dwAdaptiveScanning = ADAPTIVE_SCANNING_ENABLE;
			else if (cmp(szPropertyValue, L"FALSE"))
				readerParams.ReaderSpecific.laser_specific.dwAdaptiveScanning = ADAPTIVE_SCANNING_DISABLE;
			else
				bRetVal = FALSE;		
		}
		else
		{
			LOG(WARNING) + "Attempted to set Laser Scanner only property AdaptiveScanning on an Imager Scanner";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"ScanType"))
	{
		if (cmp(szPropertyValue, L"Foreground"))
			bRetVal = ChangeReadType(SCAN_TYPE_FOREGROUND);
		else if (cmp(szPropertyValue, L"Background"))
			bRetVal = ChangeReadType(SCAN_TYPE_BACKGROUND);
		else if (cmp(szPropertyValue, L"Monitor"))
			bRetVal = ChangeReadType(SCAN_TYPE_MONITOR);
		else
			bRetVal = FALSE;
	}
	else if (cmp(szPropertyName, L"dataBufferSize"))
	{
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_DATA_BUFFER_SIZE && 
			iValue <= MAXIMUM_DATA_BUFFER_SIZE)
		{
			m_iDataBufferSize = iValue;
			//  Free the memory into which the scanned data was stored and reallocate it
			delete[] m_lastScannedData.szData;
			m_lastScannedData.szData = new WCHAR[m_iDataBufferSize];
			if (!m_lastScannedData.szData)
			{
				LOG(ERROR) + L"Unable to allocate memory to return data buffer";
				bRetVal = FALSE;
			}

			//  In order to apply these settings re-establish the Scan Buffer
			m_bResetScanBuffer = true;		
		}
		else
		{
			LOG(WARNING) + L"Data Buffer Size is out of Range";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"BarcodeDataFormat"))
	{
		if (cmp(szPropertyValue, L"Text"))
			m_bufferFormat = BUFFER_TEXT;
		else if (cmp(szPropertyValue, L"Binary"))
			m_bufferFormat = BUFFER_RAW_DATA;
		else
			bRetVal = FALSE;

		//  In order to apply these settings re-establish the Scan Buffer
		if (bRetVal)
			m_bResetScanBuffer = true;
	}

	//  Apply the settings
	if (bRetVal)
		bRetVal = SetScannerParameters(&readerParams, sizeof(readerParams));		

	return bRetVal;
}

BOOL CScanner::ScannerBluetoothTag_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	BOOL bRetVal = TRUE;
	BOOL bApplySettings = TRUE;
	INTERFACE_PARAMS interfaceParams;
	RetrieveScannerInterfaceParameters(&interfaceParams);
	if (cmp(szPropertyName, L"ConnectionIdleTimeout"))
	{
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_CONNECTION_IDLE_TIMEOUT)
		{
			interfaceParams.InterfaceSpecific.SsiSpecific.dwConnectionIdleTimeout = iValue;
		}
		else
		{
			LOG(WARNING) + L"ConnectionIdleTimeout is out of Range";
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"DisconnectBTOnDisable"))
	{
		if (cmp(szPropertyValue, L"TRUE"))
			interfaceParams.InterfaceSpecific.SsiSpecific.bBTDisconnectOnExit = TRUE;
		else if (cmp(szPropertyValue, L"FALSE"))
			interfaceParams.InterfaceSpecific.SsiSpecific.bBTDisconnectOnExit = FALSE;
		else
			bRetVal = FALSE;

	}
	else if (cmp(szPropertyName, L"displayBTAddressBarcodeOnEnable"))
	{
		bApplySettings = FALSE;
		if (cmp(szPropertyValue, L"TRUE"))
			m_iDisplayBTPairBarcode = BT_PAIRING_BARCODE_DISPLAY;
		else if (cmp(szPropertyValue, L"FALSE"))
			m_iDisplayBTPairBarcode = BT_PAIRING_BARCODE_HIDDEN;
		else
			bRetVal = FALSE;
	}
	else if (cmp(szPropertyName, L"enableTimeout"))
	{
		bApplySettings = FALSE;
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_ENABLE_TIMEOUT && iValue <= MAXIMUM_ENABLE_TIMEOUT)
			m_dwEnableTimeout = iValue;
		else
		{
			LOG(WARNING) + L"EnableTimeout is out of Range";
			bRetVal = FALSE;
		}
	}

	//  Apply the settings
	if (bRetVal && bApplySettings)
		bRetVal = SetScannerInterfaceParameters(&interfaceParams, sizeof(interfaceParams));

	return bRetVal;
}

#pragma endregion




























