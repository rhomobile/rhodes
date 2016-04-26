#pragma once

#include "Scanner.h"

#pragma region RSM_EMML_Parser

//  Meta tag handler for RSM tags
BOOL CScanner::rsm_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	BOOL bRetVal;
	//  Consider the 'Set' methods
	if (cmp(szPropertyName, L"rsmBluetoothAuthentication") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_BT_AUTHENTICATION, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothEncryption") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_BT_ENCRYPTION, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothPinCode") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_BT_PINCODE, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothPinCodeType") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_PIN_CODE_TYPE, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothReconnectionAttempts") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_RECONNECT_ATTEMPTS, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothBeepOnReconnectAttempt") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_BEEP_ON_RECON_ATTEMPT, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothHIDAutoReconnect") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_HID_AUTO_RECON, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothFriendlyName") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_BT_FRIENDLY_NAME, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothInquiryMode") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_BT_INQUIRY_MODE, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothAutoReconnect") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_BLUETOOTH_AUTO_RECON, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmForceSavePairingBarcode") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_FORCE_PAIRING_SAVE, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmLowBatteryIndication") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_LOW_BATTERY_IND_EN, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmLowBatteryIndicationCycle") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_LOW_BATTERY_IND_CYCLE, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmScanLineWidth") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_SCANLINE_WIDTH, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmGoodScansDelay") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_GOOD_SCANS_DELAY, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmDecodeFeedback") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_DISABLE_GOOD_DECODE_LED_BEEP, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmIgnoreCode128USPS") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_EXCLUSIVE_CODE128_EN, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmScanTriggerWakeup") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_SCAN_TRIG_WAKEUP_EN, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmMems") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_MEMS_ENABLE, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmProximityEnable") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_PROXIMITY_ENABLE, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmProximityContinuous") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_PROXIMITY_CONTINUOUS_EN, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmProximityDistance") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_PROXIMITY_DISTANCE, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmPagingEnable") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_PAGING_ENABLE, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmPagingBeepSequence") && 
		szPropertyValue && wcslen(szPropertyValue) > 0)
	{
		bRetVal = SetRSMValue(ATTR_PAGING_BEEP_SEQ, szPropertyValue);
	}

	if (!bRetVal)
	{
		//  Failed to process RSM setting
		WCHAR tcLog[MAXLOG];
		if (szPropertyValue && wcslen(szPropertyValue) > 0)
		{
			wsprintf(tcLog, L"Unable to process RSM property with Parameter: %s and Value: %s", 
				szPropertyName, szPropertyValue);
		}
		else
		{
			wsprintf(tcLog, L"Unable to process RSM method with Parameter: %s", 
				szPropertyName);
		}
		LOG(WARNING) + tcLog;
	}
	return bRetVal;
}

BOOL CScanner::CommandRemoteScanner(const WCHAR* szCommand)
{
	BOOL bRetVal = FALSE;
	if (cmp(szCommand, L"StartPaging"))
	{
		bRetVal = SetRSMValue(ATTR_PAGING_ACTIVATE, L"Start");
	}
	else if (cmp(szCommand, L"StopPaging"))
	{
		bRetVal = SetRSMValue(ATTR_PAGING_ACTIVATE, L"Stop");
	}
	else if (cmp(szCommand, L"Disconnect"))
	{
		bRetVal = SetRSMValue(ATTR_BT_DISCONNECT, NULL);
	}
	else if (cmp(szCommand, L"Unpair"))
	{
		bRetVal = SetRSMValue(ATTR_BT_UNPAIR, NULL);
	}

	if (!bRetVal)
	{
		//  Failed to process RSM setting
		LOG(WARNING) + L"Unable to Command BT Scanner with instruction " + szCommand + 
			", that command is unrecognised";
	}
	return bRetVal;
}

int CScanner::rsm_GetPropertyValue(LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	RSM_ATTRIB rsmAttribute;
	memset(&rsmAttribute, 0, sizeof(RSM_ATTRIB));
	SI_INIT(&rsmAttribute);
	if (cmp(szPropertyName, L"rsmModelNumber"))		
	{
		rsmAttribute.eNumber = ATTR_MODEL_NUMBER;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmSerialNumber"))		
	{
		rsmAttribute.eNumber = ATTR_SERIAL_NUMBER;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if(cmp(szPropertyName, L"rsmDateOfManufacture"))	
	{
		rsmAttribute.eNumber = ATTR_DATE_OF_MANUFACTURE;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmDateOfService"))
	{
		rsmAttribute.eNumber = ATTR_DATE_OF_SERVICE;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothAddress"))
	{
		rsmAttribute.eNumber = ATTR_BT_ADDR;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmFirmwareVersion"))
	{
		rsmAttribute.eNumber = ATTR_FIRM_VERSION;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmDeviceClass"))
	{
		rsmAttribute.eNumber = ATTR_DEVICE_CLASS;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBatteryStatus"))
	{
		rsmAttribute.eNumber = ATTR_BATTERY_STATUS;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBatteryCapacity"))
	{
		rsmAttribute.eNumber = ATTR_BATTERY_CAPACITY;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBatteryID"))
	{
		rsmAttribute.eNumber = ATTR_BATTERY_ID;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothAuthentication"))
	{
		rsmAttribute.eNumber = ATTR_BT_AUTHENTICATION;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothEncryption"))
	{
		rsmAttribute.eNumber = ATTR_BT_ENCRYPTION;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothPINCode"))
	{
		rsmAttribute.eNumber = ATTR_BT_PINCODE;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothPINCodeType"))
	{
		rsmAttribute.eNumber = ATTR_PIN_CODE_TYPE;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothReconnectionAttempts"))
	{
		rsmAttribute.eNumber = ATTR_RECONNECT_ATTEMPTS;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothBeepOnReconnectAttempt"))
	{
		rsmAttribute.eNumber = ATTR_BEEP_ON_RECON_ATTEMPT;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothHIDAutoReconnect"))
	{
		rsmAttribute.eNumber = ATTR_HID_AUTO_RECON;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothFriendlyName"))
	{
		rsmAttribute.eNumber = ATTR_BT_FRIENDLY_NAME;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothInquiryMode"))
	{
		rsmAttribute.eNumber = ATTR_BT_INQUIRY_MODE;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmBluetoothAutoReconnect"))
	{
		rsmAttribute.eNumber = ATTR_BLUETOOTH_AUTO_RECON;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmForceSavePairingBarcode"))
	{
		rsmAttribute.eNumber = ATTR_FORCE_PAIRING_SAVE;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmLowBatteryIndication"))
	{
		rsmAttribute.eNumber = ATTR_LOW_BATTERY_IND_EN;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmLowBatteryIndicationCycle"))
	{
		rsmAttribute.eNumber = ATTR_LOW_BATTERY_IND_CYCLE;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmScanLineWidth"))
	{
		rsmAttribute.eNumber = ATTR_SCANLINE_WIDTH;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmGoodScansDelay"))
	{
		rsmAttribute.eNumber = ATTR_GOOD_SCANS_DELAY;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmDecodeFeedback"))
	{
		rsmAttribute.eNumber = ATTR_DISABLE_GOOD_DECODE_LED_BEEP;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmIgnoreCode128USPS")) 
	{
		rsmAttribute.eNumber = ATTR_EXCLUSIVE_CODE128_EN;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmScanTriggerWakeup"))
	{
		rsmAttribute.eNumber = ATTR_SCAN_TRIG_WAKEUP_EN;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmMems"))
	{
		rsmAttribute.eNumber = ATTR_MEMS_ENABLE;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmProximityEnable"))
	{
		rsmAttribute.eNumber = ATTR_PROXIMITY_ENABLE;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmProximityContinuous"))
	{
		rsmAttribute.eNumber = ATTR_PROXIMITY_CONTINUOUS_EN;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmProximityDistance"))
	{
		rsmAttribute.eNumber = ATTR_PROXIMITY_DISTANCE;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmPagingEnable"))
	{
		rsmAttribute.eNumber = ATTR_PAGING_ENABLE;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"rsmPagingBeepSequence"))
	{
		rsmAttribute.eNumber = ATTR_PAGING_BEEP_SEQ;
		iRetVal = RetrieveRSMAttributeValue(&rsmAttribute, szPropertyValue);
	}
	return iRetVal;
}

#pragma endregion

#pragma region RSM_Functions

BOOL CScanner::RSMGetSupportedAttributes()
{
	BOOL bReturnValue = FALSE;
	//  Check the Scanner supports RSM functionality
	//  Don't log if we don't supported it as we would have already logged that earlier
	if(lpfn_SCAN_RSMGetSupportedAttributes && m_hScanner)
	{
		//  Populate the member variable array rsmSupportedAttributes with
		//  the attributes supported by this device
		delete[] m_rsmSupportedAttributes;
		m_rsmSupportedAttributes = NULL;
		
		//  Find out how many attributes are supported by the connected RSM device
		//  This (calling getSupportedAttributes with NULL)
		//  doesn't seem to work on CE6 devices with the
		//  release build, though it does work with the debug build and on WM devices.
		//  Time is short therefore just defaulting to a maximum of 100 supported attributes.
//		DWORD dwRSMAttributeCount = 0;
//		DWORD dwResult = lpfn_SCAN_RSMGetSupportedAttributes(m_hScanner, NULL, &dwRSMAttributeCount);
//		if (dwResult == E_SCN_SUCCESS)
//		{
//			m_iCountOfSupportedRSMAttributes = dwRSMAttributeCount;

			//  dwAttributeCount now contains the number of attributes available on
			//  the scanner
			DWORD dwRSMAttributeCount2 = 100;
			m_rsmSupportedAttributes = new ATTRIB_NUMBER[100];
			DWORD dwResult = lpfn_SCAN_RSMGetSupportedAttributes(m_hScanner, 
															m_rsmSupportedAttributes, 
															&dwRSMAttributeCount2);
			m_iCountOfSupportedRSMAttributes = dwRSMAttributeCount2;
			//}
		bReturnValue = (dwResult == E_SCN_SUCCESS);
		if (!bReturnValue)
		{	
			WCHAR szLogMessage[1024];
			wsprintf(szLogMessage, L"Unable to retrieve supported RSM attributes, this scanner does not support RSM (%u)",
				dwResult);
			m_iCountOfSupportedRSMAttributes = 0;
			//  Removed this log as it was causing confusion with Customers, thinking something had gone wrong.
			//Log(SCANNER_LOG_INFO, szLogMessage, 
			//	_T(__FUNCTION__), __LINE__);
		}
	}
	return bReturnValue;

}


/*
BOOL CScanner::MarkRSMForRetrieval(ATTRIB_NUMBER attribute)
{
	//  Check the Scanner supports RSM functionality
	if(!lpfn_SCAN_RSMGetSupportedAttributes)
	{
		LOG(WARNING) + L"Attempted to mark an RSM Attribute for retrieval but Scanner does not support RSM";
		return FALSE;
	}
	else
	{	
		return WriteMsgQueue(m_hRSMMsgQWrite, 
						&attribute, 
						sizeof(attribute), 
						INFINITE, 0);
	}
}
*/

//  Perform checks to ensure the attribute is appropriate to retrieve and populate the 
//  m_szRSMValuesToReturn array with 'unsupported' or 'failed to get' if it is not appropriate
//  to retrieve the value.  If the value is retrievable the function RetrieveRSMAttributeValueAsString
//  is called.
int CScanner::RetrieveRSMAttributeValue(LPRSM_ATTRIB rsmAttribute, WCHAR* szValueBuffer)
{
	int iRetVal = -1;
	if (m_iCountOfMarkedRSMAttributes >= MAX_RSM_READABLE_ATTRIBUTES)
	{
		//  We are being asked to retrieve more than the maximum number of RSM attributes
		LOG(WARNING) + L"Unable to mark RSM Attribute for retrieval, please retrieve attributes";
		return FALSE;
	}

	//  Check the specified attribute is supported by the Scanner
	if (!m_rsmSupportedAttributes == NULL)
	{
		BOOL bSupported = FALSE;
		for(int attribute = 0; attribute < m_iCountOfSupportedRSMAttributes; attribute++)
		{
			if (m_rsmSupportedAttributes[attribute] == rsmAttribute->eNumber)
			{
				//  We have found the attribute in the list of supported
				//  attributes
				bSupported = TRUE;
				break;
			}
		}

		//  Log if the attribute is not supported
		if (!bSupported)
		{
			LOG(INFO) + L"Unable to get RSM attribute, not supported";
			m_szRSMValuesToReturn[m_iCountOfMarkedRSMAttributes] = 
				new TCHAR[MAX_RSM_STRING_LENGTH];
			wcscpy(m_szRSMValuesToReturn[m_iCountOfMarkedRSMAttributes++], 
				L"Unsupported Attribute");
		}
		else
		{
			//  The attribute is supported by the Scanner, 
			//  We only want to
			//  read the attribute if it is not an 'Action' attribute.
			if (rsmAttribute->eType != ATTRIB_ACTION)
			{
				//  set appropriate fields and zero the rest
				SI_SET_FIELD(rsmAttribute, eType, ATTRIB_UNTYPED);
				//  Retrieve the current value from the Scanner
				DWORD dwResult = lpfn_SCAN_RSMGetAttribute(m_hScanner, 
													VALUE_TYPE_CURRENT, 
													rsmAttribute);
					
				//  If the type of the attribute is a string or an array then 
				//  E_SCN_BUFFERTOOSMALL is returned, this indicates that memory
				//  needs to be allocated for the string / array, the size specified
				//  as part of the instantiated attribute.
				if (dwResult == E_SCN_BUFFERTOOSMALL) 
				{
					WORD wSize;
					//  Allocate memory for string or array
					if (rsmAttribute->eType == ATTRIB_STRING)
					{
						//  Attribute is a String, it's size is stored in wStrLength
						wSize = rsmAttribute->Value.StringVal.wStrLength;
						rsmAttribute->Value.StringVal.pszVal = new WCHAR[wSize];
						memset(rsmAttribute->Value.StringVal.pszVal, 0, wSize * sizeof(WCHAR));
						SI_SET_FIELD(rsmAttribute, eType, ATTRIB_STRING);
						//  Retry retrieving the attribute
						dwResult = lpfn_SCAN_RSMGetAttribute(m_hScanner,
													VALUE_TYPE_CURRENT,
													rsmAttribute);
					}
					else if (rsmAttribute->eType == ATTRIB_ARRAY)
					{
						//  Attribute is an Array
						wSize = rsmAttribute->Value.ArrayVal.wArrayLength;
						rsmAttribute->Value.ArrayVal.pArrayVal = new BYTE[wSize];
						SI_SET_FIELD(rsmAttribute, eType, ATTRIB_ARRAY);
						//  Retry retrieving the attribute
						dwResult = lpfn_SCAN_RSMGetAttribute(m_hScanner,
														VALUE_TYPE_CURRENT,
														rsmAttribute);
					}
					else
					{
						//  Unexpected : Attribute is not a string or an array
						LOG(WARNING) + L"Unable to get RSM Attribute, Memory Error";
					}
				}
					
				if (dwResult == E_SCN_SUCCESS)
				{
					//  rsmAttribute now contains the retrieved value.
					iRetVal = RetrieveRSMAttributeValueAsString(rsmAttribute, szValueBuffer);
				}
				else
				{
					m_szRSMValuesToReturn[m_iCountOfMarkedRSMAttributes] = 
						new TCHAR[MAX_RSM_STRING_LENGTH];
					wcscpy(m_szRSMValuesToReturn[m_iCountOfMarkedRSMAttributes++], 
						L"Failed to get Attribute");
				}

				//  Delete associated memory with Strings and Arrays
				if (rsmAttribute->eType == ATTRIB_STRING)
					delete[] rsmAttribute->Value.StringVal.pszVal;
				else if (rsmAttribute->eType == ATTRIB_ARRAY)
					delete[] rsmAttribute->Value.ArrayVal.pArrayVal;

				//  If the code reaches here either:
				//  * The attribute value has been
				//    successfully obtained and a navigation has been
				//    performed if appropriate.
				//  * We have failed to retrieve the attribute for the reason
				//    contained in dwResult

			}  //  End If attribute is of type ATTRIB_ACTION
		}  //  End If attribute not supported by the Scanner
	}
	return iRetVal;
}

//  Add the specified attribute's value to m_szRSMValuesToReturn
int CScanner::RetrieveRSMAttributeValueAsString(LPRSM_ATTRIB rsmAttribute, WCHAR* szRSMValue)
{
	int iRetVal = -1;
	int iLength;
	//  Character array which will store the string to return to the user
	TCHAR tcParameterValue[MAX_URL];
	memset(tcParameterValue, 0, sizeof(tcParameterValue));

	//  Converting the attribute's value to a string depends on the type
	//  of the attribute:
	switch (rsmAttribute->eType)
	{
	case ATTRIB_BYTE:
		//  A number of attributes are stored as bytes.  Interpret these bytes
		//  to mean something useful to the application
		switch (rsmAttribute->eNumber)
		{
		case ATTR_SCANLINE_WIDTH:
			//  0 : Wide
			//  1 : Narrow
			if (rsmAttribute->Value.ByteVal == 0)
				wsprintf(tcParameterValue, L"wide");
			else if (rsmAttribute->Value.ByteVal == 2)
				wsprintf(tcParameterValue, L"narrow");
			break;
		case ATTR_HID_AUTO_RECON:
			//  0 : Never Reconnect
			//  1 : Reconnect On Data
			//  2 : Reconnect Immediately
			if (rsmAttribute->Value.ByteVal == 0)
				wsprintf(tcParameterValue, L"neverReconnect");
			else if (rsmAttribute->Value.ByteVal == 1)
				wsprintf(tcParameterValue, L"reconnectOnData");
			else if (rsmAttribute->Value.ByteVal == 2)
				wsprintf(tcParameterValue, L"reconnectImmediately");
			break;
		case ATTR_PROXIMITY_DISTANCE:
			//  0 : Short
			//  1 : Medium
			//  2 : Long
			if (rsmAttribute->Value.ByteVal == 0)
				wsprintf(tcParameterValue, L"short");
			else if (rsmAttribute->Value.ByteVal == 1)
				wsprintf(tcParameterValue, L"medium");
			else if (rsmAttribute->Value.ByteVal == 2)
				wsprintf(tcParameterValue, L"long");
			break;
		case ATTR_PAGING_BEEP_SEQ:
			//  0 to 15 : String representation of number 
			wsprintf(tcParameterValue, L"%u", rsmAttribute->Value.ByteVal);
			break;
		case ATTR_BLUETOOTH_AUTO_RECON:
			//  0 : None
			//  1 : On Power
			//  2 : On Out Of Range
			//  3 : On Power Out Of Range
			if (rsmAttribute->Value.ByteVal == 0)
				wsprintf(tcParameterValue, L"none");
			else if (rsmAttribute->Value.ByteVal == 1)
				wsprintf(tcParameterValue, L"onPower");
			else if (rsmAttribute->Value.ByteVal == 2)
				wsprintf(tcParameterValue, L"onOutOfRange");
			else if (rsmAttribute->Value.ByteVal == 3)
				wsprintf(tcParameterValue, L"onPowerOutOfRange");
			break;
		case ATTR_LOW_BATTERY_IND_CYCLE:
			//  0 : 15 seconds
			//  1 : 30 seconds
			//  2 : 60 seconds
			//  3 : 90 seconds
			//  4 : 120 seconds
			if (rsmAttribute->Value.ByteVal == 0)
				wsprintf(tcParameterValue, L"15");
			else if (rsmAttribute->Value.ByteVal == 1)
				wsprintf(tcParameterValue, L"30");
			else if (rsmAttribute->Value.ByteVal == 2)
				wsprintf(tcParameterValue, L"60");
			else if (rsmAttribute->Value.ByteVal == 3)
				wsprintf(tcParameterValue, L"90");
			else if (rsmAttribute->Value.ByteVal == 4)
				wsprintf(tcParameterValue, L"120");
			break;
		case ATTR_GOOD_SCANS_DELAY:
			//  0 to 150 :	String representation of number, multiply by 100 to
			//				get the value in milliseconds.
			wsprintf(tcParameterValue, L"%u", rsmAttribute->Value.ByteVal * 100);
			break;
		case ATTR_BATTERY_STATUS:
			//  0 : Unknown
			//  1 : Full
			//  2 : Medium
			//  3 : Empty
			//  4 : Charging - Full Rate
			//  5 : Charging - Half Rate
			//  6 : Charging - Trickle
			//  7 : Discharging
			if (rsmAttribute->Value.ByteVal == 0)
				wsprintf(tcParameterValue, L"unknown");
			else if (rsmAttribute->Value.ByteVal == 1)
				wsprintf(tcParameterValue, L"full");
			else if (rsmAttribute->Value.ByteVal == 2)
				wsprintf(tcParameterValue, L"medium");
			else if (rsmAttribute->Value.ByteVal == 3)
				wsprintf(tcParameterValue, L"empty");
			else if (rsmAttribute->Value.ByteVal == 4)
				wsprintf(tcParameterValue, L"chargingFullRate");
			else if (rsmAttribute->Value.ByteVal == 5)
				wsprintf(tcParameterValue, L"chargingHalfRate");
			else if (rsmAttribute->Value.ByteVal == 6)
				wsprintf(tcParameterValue, L"chargingTrickle");
			else if (rsmAttribute->Value.ByteVal == 7)
				wsprintf(tcParameterValue, L"discharging");
			break;
		case ATTR_BATTERY_ID:
			//  0 : Simple
			//  1 : Double
			//  2 : Cabled
			//  9999 : Unknown
			if (rsmAttribute->Value.ByteVal == 0)
				wsprintf(tcParameterValue, L"simple");
			else if (rsmAttribute->Value.ByteVal == 1)
				wsprintf(tcParameterValue, L"double");
			else if (rsmAttribute->Value.ByteVal == 2)
				wsprintf(tcParameterValue, L"cabled");
			else
				wsprintf(tcParameterValue, L"unknown");
			break;
		default:
			//  Unrecognised attribute with a value of type BYTE
			LOG(WARNING) + L"Unrecognised attribute of type BYTE";
		}
		break;
	case ATTRIB_FLAG:
		//  A number of attributes are stored as flags.  Interpret these flags
		//  to mean something useful to the application
		switch (rsmAttribute->eNumber)
		{
		case ATTR_BT_AUTHENTICATION:
		case ATTR_BT_ENCRYPTION:
		case ATTR_BEEP_ON_RECON_ATTEMPT:
		case ATTR_MEMS_ENABLE:
		case ATTR_PROXIMITY_ENABLE:
		case ATTR_PAGING_ENABLE:
		case ATTR_LOW_BATTERY_IND_EN:
		case ATTR_SCAN_TRIG_WAKEUP_EN:
		case ATTR_EXCLUSIVE_CODE128_EN:
		case ATTR_PROXIMITY_CONTINUOUS_EN:
		case ATTR_FORCE_PAIRING_SAVE:
			//  These attributes are either 'True' or 'False'
			if (rsmAttribute->Value.FlagVal) 
			{
				wsprintf(tcParameterValue, L"true");
			}
			else
			{
				wsprintf(tcParameterValue, L"false");
			}
			break;
		case ATTR_PIN_CODE_TYPE:
			//  This attribute is either 'PromptUser' or 'UseStored'
			if (rsmAttribute->Value.FlagVal)
			{
				wsprintf(tcParameterValue, L"useStored");
			}
			else
			{
				wsprintf(tcParameterValue, L"promptUser");
			}
			break;
		case ATTR_BT_INQUIRY_MODE:
			//  This attribute is either 'General' or 'Limited'
			if (rsmAttribute->Value.FlagVal)
			{
				wsprintf(tcParameterValue, L"limited");
			}
			else
			{
				wsprintf(tcParameterValue, L"general");
			}
			break;
		case ATTR_DISABLE_GOOD_DECODE_LED_BEEP:
			//  These attributes are either 'Enabled' or 'Disabled'
			if (!rsmAttribute->Value.FlagVal)
			{
				wsprintf(tcParameterValue, L"true");
			}
			else
			{
				wsprintf(tcParameterValue, L"false");
			}
			break;
		default:
			LOG(WARNING) + L"Unrecognised attribute of type FLAGS";
		}
		break;
	case ATTRIB_USHORT:
		//  A number of attributes are stored as ushorts.  Interpret these numbers
		//  to mean something useful to the application
		if (rsmAttribute->eNumber == ATTR_RECONNECT_ATTEMPTS ||
			rsmAttribute->eNumber == ATTR_BATTERY_CAPACITY)
		{
			if (rsmAttribute->eNumber == ATTR_BATTERY_CAPACITY)
			{
				if (rsmAttribute->Value.UShortVal <= 100)
				{
					//  The attribute is battery capacity and between 0 and 100,
					//  copy the numeric value.
					wsprintf(tcParameterValue, L"%u", rsmAttribute->Value.UShortVal);
				}
				else
				{
					//  ATTR_BATTERY_CAPICITY not between 0 and 100.
					wsprintf(tcParameterValue, L"unknown");
				}
			}
			else
			{
				//  Parameter is ATTR_RECONNECT_ATTEMPTS, return
				//  value is 5 x greater than value stored in UShortVal
				wsprintf(tcParameterValue, L"%u", rsmAttribute->Value.UShortVal * 5);
			}
		}
		else
		{
			//  Unrecognised attribute with a value of type ushort
			LOG(WARNING) + L"Unrecognised attribute of type USHORT";
		}
		break;
	case ATTRIB_ARRAY:
		//  Only expect the Bluetooth address to be received in the form of
		//  an array
		if (rsmAttribute->eNumber == ATTR_BT_ADDR && 
			rsmAttribute->Value.ArrayVal.wArrayLength > 0)
		{
			//  The Bluetooth Address array is of length 6 with each index
			//  providing 16 bits of address, these are formatted into a string
			//  in the form FF:FF:FF:FF:FF:FF.  Though the code below will cope
			//  with arrays of any reasonable length.
			TCHAR temp[MAXURL];
			wsprintf(temp, L"%.2X", rsmAttribute->Value.ArrayVal.pArrayVal[0]);
			wcscpy(tcParameterValue, temp);
			int paramCopy;
			for(paramCopy = 1; paramCopy < rsmAttribute->Value.ArrayVal.wArrayLength; paramCopy++)
			{
				wsprintf(temp, L":%.2X", rsmAttribute->Value.ArrayVal.pArrayVal[paramCopy]);
				wcscat(tcParameterValue, temp);
			}
		}
		else
		{
			LOG(WARNING) + L"Unrecognised Array attribute";
		}
		break;
	case ATTRIB_STRING:
		//  Copy the string representing the value if it is not too large
		//  RS507 adds trailing spaces to the value we specified, remove
		//  these before returning to user
		iLength = rsmAttribute->Value.StringVal.wStrLength - 1;
		for (int i = rsmAttribute->Value.StringVal.wStrLength - 2; i>0; i--)
		{
			if (rsmAttribute->Value.StringVal.pszVal[i] != ' ')
				break;
			else
				iLength--;
		}
		if (rsmAttribute->Value.StringVal.wStrLength < MAX_URL)
		{
			wcsncpy(tcParameterValue, rsmAttribute->Value.StringVal.pszVal, iLength);
			tcParameterValue[iLength] = '\0';
		}
		break;
	default:
		//  Do not expect any of the attributes to be of other types
		// Log error 
		LOG(WARNING) + L"Unable to perform RSM navigate, Unrecognised attribuite format";
		break;
	}

	//  Check the value was copied OK
	if (tcParameterValue[0] != NULL)
	{
		//  Value was successfully copied, navigate.
		iRetVal = wcslen(tcParameterValue) + 1;
		if (szRSMValue)
			if (wcscpy_s(szRSMValue, iRetVal, tcParameterValue) == EINVAL)
				iRetVal = -1;
	}
	return iRetVal;
}

BOOL CScanner::SetRSMValue(ATTRIB_NUMBER rsmAttributeID, LPCTSTR szValue)
{
	BOOL bReturnValue = TRUE;
	RSM_ATTRIB rsmAttribute;
	memset(&rsmAttribute, 0, sizeof(RSM_ATTRIB));
	SI_INIT(&rsmAttribute);
	BOOL bSupportedAttribute = FALSE;

    // Prevent the crash if the m_rsmSupportedAttributes is null
    // usually cleared as a part of close scanner, whne the app looses focus
    // and subsequently when a property is attempted to be set
    if (NULL == m_rsmSupportedAttributes)
    {
        LOG(ERROR) + L"RSM Attribute table is empty!!!. The scanner could have been closed";
    }
    else
    {
	    //  Check the Scanner supports the selected Attribute    
	    for (int i = 0; i < m_iCountOfSupportedRSMAttributes; i++)
	    {
		    if (m_rsmSupportedAttributes[i] == rsmAttributeID)
		    {
			    bSupportedAttribute = TRUE;
			    break;
		    }
	    }
    }

	if (!bSupportedAttribute)
	{
		//  The attribute is not supported
		LOG(WARNING) + L"Unable to set RSM Attribute, it is not a supported attribute";
		bReturnValue = FALSE;
	}
	else
	{
		//  Get the current value of the Attribute, for Write Only attributes this is not 
		//  possible to we populate those separately.
		DWORD dwResult = E_SCN_SUCCESS;
		if (rsmAttributeID == ATTR_PAGING_ACTIVATE	||
			rsmAttributeID == ATTR_BT_DISCONNECT	||
			rsmAttributeID == ATTR_BT_UNPAIR)
		{
			//  WRITE ONLY attributes
			rsmAttribute.eType = ATTRIB_ACTION;
			rsmAttribute.eNumber = rsmAttributeID;
			rsmAttribute.Property = ATTRIB_PROPERTY_WRITABLE;
		}
		else if (rsmAttributeID == ATTR_BT_PINCODE ||
				rsmAttributeID == ATTR_BT_FRIENDLY_NAME)
		{
			//  READ / WRITE String attributes.  We don't retrieve these
			//  to avoid having to allocate a buffer to store the 
			//  retrieved data
			rsmAttribute.eType = ATTRIB_STRING;
			rsmAttribute.eNumber = rsmAttributeID;
			rsmAttribute.Property = ATTRIB_PROPERTY_WRITABLE & ATTRIB_PROPERTY_READABLE;
		}
		else
		{
			//  Attribute is readable, retrieve the current value
			//  Note: If Read / Write Array attributes are ever introduced (currently 
			//        only array attribute, is the BT Address and that's read only) an 
			//        exception will have to be made similar to the exception made for 
			//        strings above, because we don't allocate the buffer here.
			SI_SET_FIELD(&rsmAttribute, eType, ATTRIB_UNTYPED);
			SI_SET_FIELD(&rsmAttribute, eNumber, rsmAttributeID);
			dwResult = lpfn_SCAN_RSMGetAttribute(m_hScanner, 
												VALUE_TYPE_CURRENT, 
												&rsmAttribute);
		}
		if (dwResult != E_SCN_SUCCESS)
		{
			//  Unsuccessful at retrieving the current value
			WCHAR szLogMessage[1024];
			wsprintf(szLogMessage, 
				L"Unable to set RSM Attribute, error retrieving current value.  Error Code: %i", dwResult);
			LOG(WARNING) + szLogMessage;
			bReturnValue = FALSE;
		}
		else
		{
			switch (rsmAttributeID)
			{
				//  Byte Values
			case ATTR_SCANLINE_WIDTH:
				if (cmp(szValue, L"Wide"))
					rsmAttribute.Value.ByteVal = 0;
				else if (cmp(szValue, L"Narrow"))
					rsmAttribute.Value.ByteVal = 2;
				else
					bReturnValue = FALSE;
				break;
			case ATTR_HID_AUTO_RECON:
				if (cmp(szValue, L"NeverReconnect"))
					rsmAttribute.Value.ByteVal = 0;
				else if (cmp(szValue, L"ReconnectOnData"))
					rsmAttribute.Value.ByteVal = 1;
				else if (cmp(szValue, L"ReconnectImmediately"))
					rsmAttribute.Value.ByteVal = 2;
				else
					bReturnValue = FALSE;
				break;
			case ATTR_PROXIMITY_DISTANCE:
				if (cmp(szValue ,L"Short"))
					rsmAttribute.Value.ByteVal = 0;
				else if (cmp(szValue ,L"Medium"))
					rsmAttribute.Value.ByteVal = 1;
				else if (cmp(szValue , L"Long"))
					rsmAttribute.Value.ByteVal = 2;
				else
					bReturnValue = FALSE;
				break;
			case ATTR_BLUETOOTH_AUTO_RECON:
				if (cmp(szValue , L"OnPowerOutOfRange"))
					rsmAttribute.Value.ByteVal = 3;
				else if (cmp(szValue , L"None"))
					rsmAttribute.Value.ByteVal = 0;
				else if (cmp(szValue , L"OnPower"))
					rsmAttribute.Value.ByteVal = 1;
				else if (cmp(szValue , L"OnOutOfRange"))
					rsmAttribute.Value.ByteVal = 2;
				else
					bReturnValue = FALSE;
				break;
				//  Flag Values with parameters other than True / False
			case ATTR_PIN_CODE_TYPE:
				//  Flag: Either 'PromptUser' (0) or 'UseStored' (1)
				if (cmp(szValue ,L"PromptUser"))
					rsmAttribute.Value.FlagVal = 0;
				else if (cmp(szValue ,L"UseStored"))
					rsmAttribute.Value.FlagVal = 1;
				else
					bReturnValue = FALSE;
				break;
			case ATTR_BT_INQUIRY_MODE:
				if (cmp(szValue ,L"General"))
					rsmAttribute.Value.FlagVal = 0;
				else if (cmp(szValue ,L"Limited"))
					rsmAttribute.Value.FlagVal = 1;
				else
					bReturnValue = FALSE;
				break;
				//  Flag Values (Enabled / Disabled)
			case ATTR_EXCLUSIVE_CODE128_EN:
			case ATTR_MEMS_ENABLE:
			case ATTR_PROXIMITY_ENABLE:
			case ATTR_PAGING_ENABLE:
			case ATTR_LOW_BATTERY_IND_EN:
			case ATTR_SCAN_TRIG_WAKEUP_EN:
			case ATTR_PROXIMITY_CONTINUOUS_EN:
			case ATTR_FORCE_PAIRING_SAVE:
			case ATTR_BT_AUTHENTICATION:
			case ATTR_BT_ENCRYPTION:
			case ATTR_BEEP_ON_RECON_ATTEMPT:
				if (EnabledValue(szValue))
					rsmAttribute.Value.FlagVal = 1;
				else if (DisabledValue(szValue))
					rsmAttribute.Value.FlagVal = 0;
				else
					bReturnValue = FALSE;
				break;
				//  String Values
			case ATTR_BT_PINCODE:
			case ATTR_BT_FRIENDLY_NAME:
				{
				int iLen = wcslen(szValue) + 1;
				rsmAttribute.Value.StringVal.wStrLength = iLen;
				rsmAttribute.Value.StringVal.pszVal = new TCHAR[iLen];
				wcscpy(rsmAttribute.Value.StringVal.pszVal, szValue);
				}
				break;
			case ATTR_DISABLE_GOOD_DECODE_LED_BEEP:
				//  ATTR_DISABLE_GOOD_DECODE_LED_BEEP
				//  Flags : Enable / Disable (other way round to the other RSM flags, to make it easier
				//  to understand on our interface)
				if (EnabledValue(szValue))
					rsmAttribute.Value.FlagVal = 0;
				else if (DisabledValue(szValue))
					rsmAttribute.Value.FlagVal = 1;
				else
					bReturnValue = FALSE;
				break;
				//  Numberic Parameters
			case ATTR_RECONNECT_ATTEMPTS:
				{
				int iRequestedValue = _wtoi(szValue);
				//  Value associated with ATTR_RECONNECT_ATTEMPTS must be
				//  divisible by 5 as the value given to the EMDK API is in
				//  multiples of 5 seconds
				if (iRequestedValue % 5 == 0 && iRequestedValue >= MINIMUM_RSM_BT_RECONNECT_ATTEMPTS
					&& iRequestedValue <= MAXIMUM_RSM_BT_RECONNECT_ATTEMPTS)
					rsmAttribute.Value.ShortVal = iRequestedValue / 5;
				else
					bReturnValue = FALSE;
				}
				break;
			case ATTR_PAGING_BEEP_SEQ:
				{
				uint iValue = _wtoi(szValue);
				if (iValue <= MAXIMUM_RSM_PAGING_BEEP_SEQ)
					rsmAttribute.Value.ByteVal = (BYTE)iValue;
				else
					bReturnValue = FALSE;
				}
				break;
			case ATTR_LOW_BATTERY_IND_CYCLE:
				{
				//  0-10 secs, 2-20secs, 3-30, 4-40 secs, 5-50 secs
				uint iValue = _wtoi(szValue);
				//  Check the user defined value is in range
				if (iValue == 15)
					rsmAttribute.Value.ByteVal = (BYTE)0;
				else if (iValue == 30)
					rsmAttribute.Value.ByteVal = (BYTE)1;
				else if (iValue == 60)
					rsmAttribute.Value.ByteVal = (BYTE)2;
				else if (iValue == 90)
					rsmAttribute.Value.ByteVal = (BYTE)3;
				else if (iValue == 120)
					rsmAttribute.Value.ByteVal = (BYTE)4;
				else
					bReturnValue = FALSE;
				}
				break;
			case ATTR_GOOD_SCANS_DELAY:
				{
				//  Byte (Range 0 - 150)
				int iScanDelay = _wtoi(szValue);
				//  Value associated with ATTR_GOOD_SCANS_DELAY must be
				//  divisible by 100 as the value given to the EMDK API is in
				//  multiples of 100 seconds
				if (iScanDelay % 100 == 0 && 
					iScanDelay >= MINIMUM_RSM_GOOD_SCANS_DELAY &&
					iScanDelay <= MAXIMUM_RSM_GOOD_SCANS_DELAY)
					rsmAttribute.Value.ByteVal = iScanDelay / 100;
				else
					bReturnValue = FALSE;
				}
				break;
			//  Action Values
			case ATTR_PAGING_ACTIVATE:
				if (cmp(szValue, L"Stop"))
					rsmAttribute.Value.ActionVal = 0;
				else if (cmp(szValue, L"Start"))
					rsmAttribute.Value.ActionVal = 1;
				else
					bReturnValue = FALSE;
				break;
			case ATTR_BT_DISCONNECT:
			case ATTR_BT_UNPAIR:
				rsmAttribute.Value.ActionVal = 0;
				break;
			}

			//  If there have been no problems set the new attribute
			if (bReturnValue)
			{
				DWORD dwResult = lpfn_SCAN_RSMSetAttribute(m_hScanner, &rsmAttribute);

				//  Delete the memory allocated to the attribute string or array if
				//  appropriate.
				if (rsmAttribute.eType == ATTRIB_STRING)
					delete[] rsmAttribute.Value.StringVal.pszVal;
				else if (rsmAttribute.eType == ATTRIB_ARRAY)
					delete[] rsmAttribute.Value.ArrayVal.pArrayVal;

				if (dwResult != E_SCN_SUCCESS)
				{
					bReturnValue = FALSE;
					LOG(WARNING) + L"Unable to set RSM value, error occured whilst setting attribute";
				}
			}
			else
			{
				WCHAR szLogMsg[1024];
				wsprintf(szLogMsg, L"Unable to set RSM value, specified value (%s) is not appropriate to RSM parameter", szValue);
				LOG(WARNING) + szLogMsg;
			}

		}

	}
	return bReturnValue;
}

#pragma endregion

/*
DWORD CScanner::ProcessRSMGetQueue(LPVOID lParam)
{
	CScanner* pScanner = (CScanner*)lParam;
	//  Variable to hold the read attribute
	ATTRIB_NUMBER theAttribute;
	DWORD dwRead;
	DWORD dwFlags;
	BOOL bRet;
	WCHAR szAttributeValuesAsJSArray[MAX_URL];

	//  Process the queue until the m_hExitThreadEvent is set
	while (true) 
	{
		bRet = ReadMsgQueue(pScanner->m_hRSMMsgQRead, &theAttribute, 
				sizeof(ATTRIB_NUMBER), &dwRead, INFINITE, &dwFlags);
		if (!bRet || theAttribute == ATTR_END)
			break;
		else if (bRet) 
		{
			//  Successfully read an attribute from the queue, this attribute
			//  will either indicate an attribute to be added to the array
			//  of values to return, or indicate that a navigation should be 
			//  performed
			if (theAttribute == ATTR_MAX)
			{
				//  User has indicated they wish to receive a navigation of
				//  values retrieved by RSMGetParam
				//  Calculate the String to Return, in the form newArray(att1, att2)
				memset(szAttributeValuesAsJSArray, 0, MAX_URL * sizeof(WCHAR));
				wcscpy(szAttributeValuesAsJSArray, L"new Array(");
				for(int i = 0; i < pScanner->m_iCountOfMarkedRSMAttributes; i++)
				{
					if (i > 0)
						wcscat(szAttributeValuesAsJSArray, L",");
					wcscat(szAttributeValuesAsJSArray, L"'");
					//  The m_szRSMValuesToReturn array is populated with the values to return to the user.
					//  Memory is deallocated here once read.
					if (pScanner->m_szRSMValuesToReturn[i])
					{
						wcscat(szAttributeValuesAsJSArray, pScanner->m_szRSMValuesToReturn[i]);
						delete[] pScanner->m_szRSMValuesToReturn[i];
						pScanner->m_szRSMValuesToReturn[i] = NULL;
					}
					wcscat(szAttributeValuesAsJSArray, L"'");
				}
				wcscat(szAttributeValuesAsJSArray, L")");
				pScanner->m_iCountOfMarkedRSMAttributes = 0;

				if(pScanner->m_pRSMEvent)
				{
					//  DCC TODO Logic of RSM Event here
//					pScanner->m_pModule->SendPBNavigate(szRSMGetEventNames, 
//						pScanner->m_iInstanceID, pScanner->m_szEventRSMGetEvent, 
//						szAttributeValuesAsJSArray, NULL);
				}
			}
			else
			{
				//  Add the value to the list of values to return
				RSM_ATTRIB rsmAttribute;
				memset(&rsmAttribute, 0, sizeof(RSM_ATTRIB));
				SI_INIT(&rsmAttribute);
				LPRSM_ATTRIB pRSMAttribute = &rsmAttribute;
				SI_SET_FIELD(pRSMAttribute, eNumber, theAttribute);
				pScanner->m_szRSMValuesToReturn[pScanner->m_iCountOfMarkedRSMAttributes] = 
						new TCHAR[MAX_RSM_STRING_LENGTH];
				pScanner->RetrieveRSMAttributeValue(pRSMAttribute, 
					pScanner->m_szRSMValuesToReturn[pScanner->m_iCountOfMarkedRSMAttributes]);
				pScanner->m_iCountOfMarkedRSMAttributes++;
			}
		}
		
	}
	return 0;
}
*/


