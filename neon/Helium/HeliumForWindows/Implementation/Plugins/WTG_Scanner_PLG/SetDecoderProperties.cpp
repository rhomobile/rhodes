#include "Scanner.h"

BOOL CScanner::Set_all_decoders_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	BOOL bReturnValue = FALSE;
	DECODER_LIST decoderList;
	memset(&decoderList, 0, sizeof(DECODER_LIST));
	SI_INIT(&decoderList);
	DWORD dwResult;
	if (cmp(szPropertyValue, L"Enabled") || 
		cmp(szPropertyValue, L"Enable"))
	{
		//	Enable all decoders supported by the device
		dwResult = lpfn_SCAN_GetSupportedDecoders(m_hScanner, &decoderList);
		if (dwResult == E_SCN_SUCCESS) 
		{
			for (unsigned int i = 0; i < decoderList.Decoders.dwDecoderCount; i++)
			{
				//	Remove decoders unsupported by RhoElements.  Note imaging barcodes must be 
				//  received in a buffer allocated to hold binary data:
				//  scan_allocateBuffer(false, size)
				if (decoderList.Decoders.byList[i] == (BYTE)*DECODER_POINTER ||
					decoderList.Decoders.byList[i] == (BYTE)*DECODER_IMAGE ||
					decoderList.Decoders.byList[i] == (BYTE)*DECODER_DOCCAP)
				{
					//  Shift the decoder structure
					memcpy(decoderList.Decoders.byList + i,
						decoderList.Decoders.byList + (i + 1),
					decoderList.Decoders.dwDecoderCount - i);
					decoderList.Decoders.dwDecoderCount--;
					i--;
				}
			}
			dwResult = lpfn_SCAN_SetEnabledDecoders(m_hScanner, &decoderList);
			if (dwResult == E_SCN_SUCCESS) 
				bReturnValue = TRUE;
			else
			{
				//  Unable to Enable all Decoders
				Log(SCANNER_LOG_WARNING, L"Unable to Enable all decoders", 
					_T(__FUNCTION__), __LINE__);
			}
		}
		else 
		{
			//  Call to Get Supported Devices from the EMDK has failed
			Log(SCANNER_LOG_WARNING, L"Unable to retrieve supported devices", 
				_T(__FUNCTION__), __LINE__);
		}
	}
	else if (cmp(szPropertyValue, L"Disabled") || 
		cmp(szPropertyValue, L"Disable"))
	{
		//  Disable all decoders supported by the device
		dwResult = lpfn_SCAN_GetEnabledDecoders(m_hScanner, &decoderList);
		if (dwResult == E_SCN_SUCCESS) 
		{
			decoderList.Decoders.dwDecoderCount = 0;
			dwResult = lpfn_SCAN_SetEnabledDecoders(m_hScanner, &decoderList);
			if (dwResult == E_SCN_SUCCESS) 
			{
				bReturnValue = TRUE;
			}
			else
			{
				Log(SCANNER_LOG_WARNING, L"Unable to disable all decoders", 
					_T(__FUNCTION__), __LINE__);
			}
		}
		else 
		{
			//  Unable to Get Enabled decoders from the device
			Log(SCANNER_LOG_WARNING, L"Unable to retrieve currently enabled decoders",
				_T(__FUNCTION__), __LINE__);
		}
	}
	//  Cancel any pending scanner reads
	lpfn_SCAN_Flush(m_hScanner);

	return bReturnValue;

}

BOOL CScanner::Set_codabar_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)		
{
	CODABAR_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_CODABAR;
	if (cmp(szPropertyName, L"codabar"))
	{
		//  Enable or Disable the Decoder
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"CodabarRedundancy"))
	{
		//  Codabar Redundancy
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bRedundancy = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bRedundancy = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"CodabarCLSIEditing"))
	{
		//  Codabar CLSI Editing
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bClsiEditing = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bClsiEditing = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"CodabarNotisEditing"))
	{
		//  Codabar Notis Editing
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bNotisEditing = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bNotisEditing = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"CodabarMinLength"))
	{
		//  Codabar Min Length
		int iMin = StringToInt(szPropertyValue);
		if (iMin >= MINIMUM_CODABAR_LENGTH && iMin <= MAXIMUM_CODABAR_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, iMin, -1);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Codabar Minimum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"CodabarMaxLength"))
	{
		//  Codabar Max Length
		int iMax = StringToInt(szPropertyValue);
		if (iMax >= MINIMUM_CODABAR_LENGTH && iMax <= MAXIMUM_CODABAR_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, -1, iMax);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Codabar Maximum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}
	return bRetVal;
}


BOOL CScanner::Set_code128_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)		
{
	CODE128_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_CODE128;
	if (cmp(szPropertyName, L"code128"))
	{
		//  Enable or Disable the Decoder
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"Code128Redundancy"))
	{
		//  Code 128 Redundancy
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bRedundancy = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bRedundancy = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code128ean128"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bEAN128 = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bEAN128 = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code128isBt128"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bISBT128 = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bISBT128 = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code128Other128"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bOther128 = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bOther128 = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code128checkIsBtTable"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bCheckISBTTable = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bCheckISBTTable = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code128isBt128ConcatMode"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"never"))
			decoderParameters.dwISBT128ConcatMode = ISBT128_CONCAT_MODE_NEVER;
		else if (cmp(szPropertyValue, L"always"))
			decoderParameters.dwISBT128ConcatMode = ISBT128_CONCAT_MODE_ALWAYS;
		else if (cmp(szPropertyValue, L"auto"))
			decoderParameters.dwISBT128ConcatMode = ISBT128_CONCAT_MODE_AUTO;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code128SecurityLevel"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_CODE128_SECURITY_LEVEL && iValue <= MAXIMUM_CODE128_SECURITY_LEVEL)
		{
			decoderParameters.dwSecurityLevel = iValue;
			bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		}
		else
		{
			Log(SCANNER_LOG_WARNING, L"Code 128 Security Level is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bValueRecognised = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"Code128MinLength"))
	{
		int iMin = StringToInt(szPropertyValue);
		if (iMin >= MINIMUM_CODE128_LENGTH && iMin <= MAXIMUM_CODE128_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, iMin, -1);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Code128 Minimum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"Code128MaxLength"))
	{
		int iMax = StringToInt(szPropertyValue);
		if (iMax >= MINIMUM_CODE128_LENGTH && iMax <= MAXIMUM_CODE128_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, -1, iMax);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Code 128 Maximum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}

	return bRetVal;
}

BOOL CScanner::Set_code11_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	CODE11_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_CODE11;
	if (cmp(szPropertyName, L"code11"))
	{
		//  Enable or Disable the Decoder
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"Code11Redundancy"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bRedundancy = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bRedundancy = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code11ReportCheckDigit"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bReportCheckDigit = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bReportCheckDigit = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code11CheckDigitCount"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"none"))
			decoderParameters.dwCheckDigitCount = CODE11_NO_CHECK_DIGIT;
		else if (cmp(szPropertyValue, L"one"))
			decoderParameters.dwCheckDigitCount = CODE11_ONE_CHECK_DIGIT;
		else if (cmp(szPropertyValue, L"two"))
			decoderParameters.dwCheckDigitCount = CODE11_TWO_CHECK_DIGIT;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code11MinLength"))
	{
		int iMin = StringToInt(szPropertyValue);
		if (iMin >= MINIMUM_CODE11_LENGTH && iMin <= MAXIMUM_CODE11_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, iMin, -1);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Code11 Minimum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"Code11MaxLength"))
	{
		int iMax = StringToInt(szPropertyValue);
		if (iMax >= MINIMUM_CODE11_LENGTH && iMax <= MAXIMUM_CODE11_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, -1, iMax);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Code 11 Maximum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}

	return bRetVal;
}

BOOL CScanner::Set_code39_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)		
{
	CODE39_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_CODE39;
	if (cmp(szPropertyName, L"code39"))
	{
		//  Enable or Disable the Decoder
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"Code39Redundancy"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bRedundancy = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bRedundancy = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code39ReportCheckDigit"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bReportCheckDigit = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bReportCheckDigit = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code39VerifyCheckDigit"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bVerifyCheckDigit = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bVerifyCheckDigit = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code39Code32Prefix"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bCode32Prefix = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bCode32Prefix = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code39Concatenation"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bConcatenation = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bConcatenation = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code39ConvertToCode32"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bConvertToCode32 = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bConvertToCode32 = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code39FullAscii"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bFullAscii = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bFullAscii = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"code39securityLevel"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_CODE39_SECURITY_LEVEL && iValue <= MAXIMUM_CODE39_SECURITY_LEVEL)
		{
			decoderParameters.dwSecurityLevel = iValue;
			bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		}
		else
		{
			Log(SCANNER_LOG_WARNING, L"Code 39 Security Level is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bValueRecognised = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"Code39MinLength"))
	{
		int iMin = StringToInt(szPropertyValue);
		if (iMin >= MINIMUM_CODE39_LENGTH && iMin <= MAXIMUM_CODE39_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, iMin, -1);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Code39 Minimum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"Code39MaxLength"))
	{
		int iMax = StringToInt(szPropertyValue);
		if (iMax >= MINIMUM_CODE39_LENGTH && iMax <= MAXIMUM_CODE39_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, -1, iMax);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Code39 Maximum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}

	return bRetVal;
}


BOOL CScanner::Set_code93_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)		
{
	CODE93_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_CODE93;
	if (cmp(szPropertyName, L"code93"))
	{
		//  Enable or Disable the Decoder
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"Code93Redundancy"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bRedundancy = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bRedundancy = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Code93MinLength"))
	{
		int iMin = StringToInt(szPropertyValue);
		if (iMin >= MINIMUM_CODE93_LENGTH && iMin <= MAXIMUM_CODE93_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, iMin, -1);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Code93 Minimum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"Code93MaxLength"))
	{
		int iMax = StringToInt(szPropertyValue);
		if (iMax >= MINIMUM_CODE93_LENGTH && iMax <= MAXIMUM_CODE93_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, -1, iMax);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Code93 Maximum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}

	return bRetVal;
}


BOOL CScanner::Set_d2of5_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)		
{
	D2OF5_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_D2OF5;
	if (cmp(szPropertyName, L"d2of5"))
	{
		//  Enable or Disable the Decoder
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"D2of5Redundancy"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bRedundancy = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bRedundancy = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"D2of5Minlength"))
	{
		int iMin = StringToInt(szPropertyValue);
		if (iMin >= MINIMUM_D2OF5_LENGTH && iMin <= MAXIMUM_D2OF5_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, iMin, -1);
		else
		{
			Log(SCANNER_LOG_WARNING, L"D2of5 Minimum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bValueRecognised = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"D2of5Maxlength"))
	{
		int iMax = StringToInt(szPropertyValue);
		if (iMax >= MINIMUM_D2OF5_LENGTH && iMax <= MAXIMUM_D2OF5_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, -1, iMax);
		else
		{
			Log(SCANNER_LOG_WARNING, L"D2of5 Maximum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bValueRecognised = FALSE;
		}
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}

	return bRetVal;
}


BOOL CScanner::Set_ean8_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)			
{
	EAN8_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_EAN8;
	if (cmp(szPropertyName, L"ean8"))
	{
		//  Enable or Disable the Decoder
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"ean8ConvertToEan13"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bConvertToEAN13 = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bConvertToEAN13 = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}

	return bRetVal;
}


BOOL CScanner::Set_i2of5_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)		
{
	I2OF5_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_I2OF5;
	if (cmp(szPropertyName, L"i2of5"))
	{
		//  Enable or Disable the Decoder
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"I2of5Redundancy"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bRedundancy = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bRedundancy = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"I2of5ConvertToEAN13"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bConvertToEAN13 = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bConvertToEAN13 = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"I2of5ReportCheckDigit"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bReportCheckDigit = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bReportCheckDigit = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"I2of5VerifyCheckDigit"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"none"))
			decoderParameters.dwVerifyCheckDigit = I2OF5_NO_CHECK_DIGIT;
		else if (cmp(szPropertyValue, L"uss"))
			decoderParameters.dwVerifyCheckDigit = I2OF5_USS_CHECK_DIGIT;
		else if (cmp(szPropertyValue, L"opcc"))
			decoderParameters.dwVerifyCheckDigit = I2OF5_OPCC_CHECK_DIGIT;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"I2of5Minlength"))
	{
		int iMin = StringToInt(szPropertyValue);
		if (iMin >= MINIMUM_I2OF5_LENGTH && iMin <= MAXIMUM_I2OF5_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, iMin, -1);
		else
		{
			Log(SCANNER_LOG_WARNING, L"I2of5 Minimum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"I2of5Maxlength"))
	{
		int iMax = StringToInt(szPropertyValue);
		if (iMax >= MINIMUM_I2OF5_LENGTH && iMax <= MAXIMUM_I2OF5_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, -1, iMax);
		else
		{
			Log(SCANNER_LOG_WARNING, L"I2of5 Maximum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}

	return bRetVal;
}


BOOL CScanner::Set_msi_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)			
{
	MSI_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_MSI;
	if (cmp(szPropertyName, L"msi"))
	{
		//  Enable or Disable the Decoder
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"msiCheckDigits"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"one"))
			decoderParameters.dwCheckDigits = MSI_ONE_CHECK_DIGIT;
		else if (cmp(szPropertyValue, L"two"))
			decoderParameters.dwCheckDigits = MSI_TWO_CHECK_DIGIT;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"msiCheckDigitScheme"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"mod_11_10") || cmp(szPropertyValue, L"mod11"))
			decoderParameters.dwCheckDigitScheme = MSI_CHKDGT_MOD_11_10;
		else if (cmp(szPropertyValue, L"mod_10_10") || cmp(szPropertyValue, L"mod10"))
			decoderParameters.dwCheckDigitScheme = MSI_CHKDGT_MOD_10_10;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"msiRedundancy"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bRedundancy = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bRedundancy = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"msiReportCheckDigit"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bReportCheckDigit = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bReportCheckDigit = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"msiMinLength"))
	{
		int iMin = StringToInt(szPropertyValue);
		if (iMin >= MINIMUM_MSI_LENGTH && iMin <= MAXIMUM_MSI_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, iMin, -1);
		else
		{
			Log(SCANNER_LOG_WARNING, L"msi Minimum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"msiMaxLength"))
	{
		int iMax = StringToInt(szPropertyValue);
		if (iMax >= MINIMUM_MSI_LENGTH && iMax <= MAXIMUM_MSI_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, -1, iMax);
		else
		{
			Log(SCANNER_LOG_WARNING, L"msi Maximum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}

	return bRetVal;
}

BOOL CScanner::Set_trioptic39_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)	
{
	TRIOPTIC39_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_TRIOPTIC39;
	if (cmp(szPropertyName, L"Trioptic39"))
	{
		//  Enable or Disable the Decoder
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"trioptic39Redundancy"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bRedundancy = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bRedundancy = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}

	return bRetVal;
}

BOOL CScanner::Set_upc_ean_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)		
{
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	UPC_EAN_PARAMS upceanParams;
	//  Since there is not enable / disable of upc_ean we can retrieve the 
	//  current parameters at the top level
	RetrieveUPCEANParams(&upceanParams);

	if (cmp(szPropertyName, L"upc_eanbookland") ||
		cmp(szPropertyName, L"upceanbookland"))
	{
		if (cmp(szPropertyValue, L"True"))
			upceanParams.bBookland = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			upceanParams.bBookland = FALSE;
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"upc_eanCoupon") ||
		cmp(szPropertyName, L"upceanCoupon"))
	{
		if (cmp(szPropertyValue, L"True"))
			upceanParams.bCoupon = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			upceanParams.bCoupon = FALSE;		
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"upc_eanLinearDecode") ||
		cmp(szPropertyName, L"upceanLinearDecode"))
	{
		if (cmp(szPropertyValue, L"True"))
			upceanParams.bLinearDecode = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			upceanParams.bLinearDecode = FALSE;		
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"upc_eanRandomWeightCheckDigit") ||
		cmp(szPropertyName, L"upceanRandomWeightCheckDigit"))
	{
		if (cmp(szPropertyValue, L"True"))
			upceanParams.bRandomWeightCheckDigit = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			upceanParams.bRandomWeightCheckDigit = FALSE;		
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"upc_eanSupplemental2") ||
		cmp(szPropertyName, L"upceanSupplemental2"))
	{
		if (cmp(szPropertyValue, L"True"))
			upceanParams.bSupplemental2 = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			upceanParams.bSupplemental2 = FALSE;		
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"upc_eanSupplemental5") ||
		cmp(szPropertyName, L"upceanSupplemental5"))
	{
		if (cmp(szPropertyValue, L"True"))
			upceanParams.bSupplemental5 = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			upceanParams.bSupplemental5 = FALSE;		
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"upc_eanConvertRSSToUpcean") ||
		cmp(szPropertyName, L"upceanConvertRSSToUpcean") ||
		cmp(szPropertyName, L"upcEanConvertGs1dataBarToUpcEan"))
	{
		if (cmp(szPropertyValue, L"True"))
			upceanParams.bConvertRSSToUPCEAN = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			upceanParams.bConvertRSSToUPCEAN = FALSE;		
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"upc_eanRetryCount") ||
		cmp(szPropertyName, L"upceanRetryCount"))
	{
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_UPCEAN_RETRY_COUNT && iValue <= MAXIMUM_UPCEAN_RETRY_COUNT)
			upceanParams.dwRetryCount = iValue;
		else
		{
			Log(SCANNER_LOG_WARNING, L"UPC EAN Retry Count is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bValueRecognised = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"upc_eanSecurityLevel") ||
		cmp(szPropertyName, L"upceanSecurityLevel"))
	{
		//  None, Ambiguous and All have been deprecated in the EMDK and replaced by numbers
		if (cmp(szPropertyValue, L"none"))
			upceanParams.dwSecurityLevel = UPCEAN_SECURITY_NONE;
		else if (cmp(szPropertyValue, L"ambiguous"))
			upceanParams.dwSecurityLevel = UPCEAN_SECURITY_AMBIGUOUS;		
		else if (cmp(szPropertyValue, L"all"))
			upceanParams.dwSecurityLevel = UPCEAN_SECURITY_ALL;
		else 
		{
			int iValue = StringToInt(szPropertyValue);
			if (iValue >= MINIMUM_UPC_EAN_SECURITY_LEVEL && iValue <= MAXIMUM_UPC_EAN_SECURITY_LEVEL)
				upceanParams.dwSecurityLevel = iValue;
			else
			{
				Log(SCANNER_LOG_WARNING, L"UPC EAN Security Level is out of Range", 
					_T(__FUNCTION__), __LINE__);
				bValueRecognised = FALSE;
			}
		}
	}
	else if (cmp(szPropertyName, L"upc_eanSupplementalMode") ||
		cmp(szPropertyName, L"upceanSupplementalMode"))
	{
		if (cmp(szPropertyValue, L"none"))
			upceanParams.dwSupplementalMode = SUPPLEMENTALS_NONE;
		else if (cmp(szPropertyValue, L"always"))
			upceanParams.dwSupplementalMode = SUPPLEMENTALS_ALWAYS;		
		else if (cmp(szPropertyValue, L"auto"))
			upceanParams.dwSupplementalMode = SUPPLEMENTALS_AUTO;		
		else if (cmp(szPropertyValue, L"smart"))
			upceanParams.dwSupplementalMode = SUPPLEMENTALS_SMART;		
		else if (cmp(szPropertyValue, L"378or379"))
			upceanParams.dwSupplementalMode = SUPPLEMENTALS_378_379;		
		else if (cmp(szPropertyValue, L"978"))
			upceanParams.dwSupplementalMode = SUPPLEMENTALS_978;		
		else if (cmp(szPropertyValue, L"978or979"))
			upceanParams.dwSupplementalMode = SUPPLEMENTALS_978_979;		
		else if (cmp(szPropertyValue, L"977"))
			upceanParams.dwSupplementalMode = SUPPLEMENTALS_977;		
		else if (cmp(szPropertyValue, L"414or419or434or439"))
			upceanParams.dwSupplementalMode = SUPPLEMENTALS_414_419_434_439;		
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"upc_eanBooklandFormat") ||
		cmp(szPropertyName, L"upceanBooklandFormat"))
	{
		if (cmp(szPropertyValue, L"Isbn10"))
			upceanParams.dwBooklandFormat = BOOKLAND_FORMAT_ISBN_10;
		else if (cmp(szPropertyValue, L"Isbn13"))
			upceanParams.dwBooklandFormat = BOOKLAND_FORMAT_ISBN_13;		
		else
			bValueRecognised = FALSE;
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		SetUPCEANParams(&upceanParams);
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}

	return bRetVal;
}


BOOL CScanner::Set_upca_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)			
{
	UPCA_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_UPCA;
	if (cmp(szPropertyName, L"upca"))
	{
		//  Parameter is UPCA, Enable or Disable the Decoder
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"upcaPreamble"))
	{
		//  UPCA Preamble
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"None"))
			decoderParameters.dwPreamble = PREAMBLE_NONE;
		else if (cmp(szPropertyValue, L"System_Char") || cmp(szPropertyValue, L"systemChar"))
			decoderParameters.dwPreamble = PREAMBLE_SYSTEM_CHAR;
		else if (cmp(szPropertyValue, L"Country_And_System_Chars") || cmp(szPropertyValue, L"countryAndSystemChars"))
			decoderParameters.dwPreamble = PREAMBLE_COUNTRY_AND_SYSTEM_CHARS;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"upcaReportCheckDigit"))
	{
		//  UPCA Report Check Digit
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bReportCheckDigit = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bReportCheckDigit = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}
	return bRetVal;
}


BOOL CScanner::Set_upce0_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)		
{
	UPCE0_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_UPCE0;
	if (cmp(szPropertyName, L"upce0"))
	{
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"upce0Preamble"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"None"))
			decoderParameters.dwPreamble = PREAMBLE_NONE;
		else if (cmp(szPropertyValue, L"System_Char") || cmp(szPropertyValue, L"systemChar"))
			decoderParameters.dwPreamble = PREAMBLE_SYSTEM_CHAR;
		else if (cmp(szPropertyValue, L"Country_And_System_Chars") || cmp(szPropertyValue, L"countryAndSystemChars"))
			decoderParameters.dwPreamble = PREAMBLE_COUNTRY_AND_SYSTEM_CHARS;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"upce0ReportCheckDigit"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bReportCheckDigit = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bReportCheckDigit = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"upce0ConvertToUPCA"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bConvertToUPCA = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bConvertToUPCA = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}
	return bRetVal;
}


BOOL CScanner::Set_upce1_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	UPCE1_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_UPCE1;
	if (cmp(szPropertyName, L"upce1"))
	{
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"upce1Preamble"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"None"))
			decoderParameters.dwPreamble = PREAMBLE_NONE;
		else if (cmp(szPropertyValue, L"System_Char") || cmp(szPropertyValue, L"systemChar"))
			decoderParameters.dwPreamble = PREAMBLE_SYSTEM_CHAR;
		else if (cmp(szPropertyValue, L"Country_And_System_Chars") || cmp(szPropertyValue, L"countryAndSystemChars"))
			decoderParameters.dwPreamble = PREAMBLE_COUNTRY_AND_SYSTEM_CHARS;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"upce1ReportCheckDigit"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bReportCheckDigit = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bReportCheckDigit = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"upce1ConvertToUPCA"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bConvertToUPCA = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bConvertToUPCA = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}
	return bRetVal;
}

//  Abstract Decoders which only have Enabled / Disabled to worry about (No Parameters)
BOOL CScanner::SetSimpleDecoder(BYTE cCurrentDecoder, LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	BOOL bRetVal = FALSE;
	if (cmp(szPropertyValue, L"Enabled") ||
		cmp(szPropertyValue, L"Enable"))
	{
		bRetVal = EnableDecoder(cCurrentDecoder);
	}
	else if (cmp(szPropertyValue, L"Disabled") ||
		cmp(szPropertyValue, L"Disable"))
	{
		bRetVal = DisableDecoder(cCurrentDecoder);
	}

	if (bRetVal)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}
	return bRetVal;
}

#pragma region PostalMetaProcs

BOOL CScanner::Set_auspostal_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_AUSPOSTAL, szPropertyName, szPropertyValue);
}


BOOL CScanner::Set_canpostal_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_CANPOSTAL, szPropertyName, szPropertyValue);
}

BOOL  CScanner::Set_dutchpostal_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_DUTCHPOSTAL, szPropertyName, szPropertyValue);
}


BOOL  CScanner::Set_jappostal_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_JAPPOSTAL, szPropertyName, szPropertyValue);
}

BOOL  CScanner::Set_ukpostal_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_UKPOSTAL, szPropertyName, szPropertyValue);
}


BOOL  CScanner::Set_uspostnet_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_USPOSTNET, szPropertyName, szPropertyValue);
}

#pragma endregion


BOOL CScanner::Set_composit_ab_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	COMPOSITE_AB_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_COMPOSITE_AB;
	if (cmp(szPropertyName, L"composit_ab") ||
		cmp(szPropertyName, L"compositeAb"))
	{
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"compositeAbUseUpcPreambleCheckDigitRules"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bUseUPCPreambleCheckDigitRules = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bUseUPCPreambleCheckDigitRules = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"compositeAbUccLinkMode"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"never"))
			decoderParameters.dwUCCLinkMode = UCC_NEVER;
		else if (cmp(szPropertyValue, L"always"))
			decoderParameters.dwUCCLinkMode = UCC_ALWAYS;
		else if (cmp(szPropertyValue, L"auto"))
			decoderParameters.dwUCCLinkMode = UCC_AUTO;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}
	return bRetVal;
}

BOOL CScanner::Set_composit_c_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_COMPOSITE_C, szPropertyName, szPropertyValue);
}

BOOL  CScanner::Set_datamatrix_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_DATAMATRIX, szPropertyName, szPropertyValue);
}


BOOL  CScanner::Set_pdf417_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_PDF417, szPropertyName, szPropertyValue);
}

BOOL  CScanner::Set_ean13_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_EAN13, szPropertyName, szPropertyValue);
}


BOOL  CScanner::Set_macromicropdf_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	MACROMICROPDF_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_MACROMICROPDF;
	if (cmp(szPropertyName, L"macroMicroPdf"))
	{
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"macroMicroPdfReportAppendInfo"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bReportAppendInfo = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bReportAppendInfo = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"macroMicroPdfBufferLabels"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bBufferLabels = TRUE;
		else if (cmp(szPropertyValue, L"false"))
			decoderParameters.bBufferLabels = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"macroMicroPdfExclusive"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bExclusive = TRUE;
		else if (cmp(szPropertyValue, L"false"))
			decoderParameters.bExclusive = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"macroMicroPdfConvertToMicroPdf"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bConvertToMicroPDF = TRUE;
		else if (cmp(szPropertyValue, L"false"))
			decoderParameters.bConvertToMicroPDF = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}
	return bRetVal;
}


BOOL  CScanner::Set_macropdf_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	MACROPDF_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_MACROPDF;
	if (cmp(szPropertyName, L"macroPdf"))
	{
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"macroPdfBufferLabels"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bBufferLabels = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bBufferLabels = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"macroPdfExclusive"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bExclusive = TRUE;
		else if (cmp(szPropertyValue, L"false"))
			decoderParameters.bExclusive = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"macroPdfConvertToPdf417"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bConvertToPDF417 = TRUE;
		else if (cmp(szPropertyValue, L"false"))
			decoderParameters.bConvertToPDF417 = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}
	return bRetVal;
}


BOOL  CScanner::Set_maxicode_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_MAXICODE, szPropertyName, szPropertyValue);
}


BOOL  CScanner::Set_micropdf_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_MICROPDF, szPropertyName, szPropertyValue);
}

BOOL  CScanner::Set_qrcode_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_QRCODE, szPropertyName, szPropertyValue);
}

BOOL  CScanner::Set_rss14_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_RSS14, szPropertyName, szPropertyValue);
}

BOOL  CScanner::Set_rssexp_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_RSSEXP, szPropertyName, szPropertyValue);
}

BOOL  CScanner::Set_rsslim_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_RSSLIM, szPropertyName, szPropertyValue);
}

BOOL  CScanner::Set_signature_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	SIGNATURE_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_SIGNATURE;
	if (cmp(szPropertyName, L"signature"))
	{
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"signatureImageQuality"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_SIGNATURE_IMAGE_QUALITY && iValue <= MAXIMUM_SIGNATURE_IMAGE_QUALITY)
		{
			decoderParameters.dwJpegImageQuality = iValue;
			decoderParameters.dwJpegImageSize = 0;
			SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		}
		else
		{
			Log(SCANNER_LOG_WARNING, L"Signature Image Quality is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bValueRecognised = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"signatureImageSize"))
	{
		//  NOTE: This is not currently supported as it did not function as expected during my testing
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_SIGNATURE_IMAGE_SIZE)
		{
			decoderParameters.dwJpegImageSize = iValue;
			decoderParameters.dwJpegImageQuality = 0;
			SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		}
		else
		{
			Log(SCANNER_LOG_WARNING, L"Signature Image Size is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bValueRecognised = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"signatureImageWidth"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_SIGNATURE_IMAGE_WIDTH)
		{
			decoderParameters.dwImageWidth = iValue;
			SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		}
		else
		{
			Log(SCANNER_LOG_WARNING, L"Signature Image Width is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bValueRecognised = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"SignatureImageHeight"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		int iValue = StringToInt(szPropertyValue);
		if (iValue >= MINIMUM_SIGNATURE_IMAGE_HEIGHT)
		{
			decoderParameters.dwImageHeight = iValue;
			SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		}
		else
		{
			Log(SCANNER_LOG_WARNING, L"Signature Image Height is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bValueRecognised = FALSE;
		}
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}
	return bRetVal;
}

BOOL  CScanner::Set_tlc39_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_TLC39, szPropertyName, szPropertyValue);
}


BOOL  CScanner::Set_usplanet_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_USPLANET, szPropertyName, szPropertyValue);
}


BOOL  CScanner::Set_webcode_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	WEBCODE_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_WEBCODE;
	if (cmp(szPropertyName, L"webcode"))
	{
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"webcodeDecodeGtSubtype"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bGTWebcode = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bGTWebcode = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}
	return bRetVal;
}

BOOL  CScanner::Set_Chinese_2Of5_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_CHINESE_2OF5, szPropertyName, szPropertyValue);
}

BOOL  CScanner::Set_AZTEC_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_AZTEC, szPropertyName, szPropertyValue);
}

BOOL  CScanner::Set_MicroQR_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_MICROQR, szPropertyName, szPropertyValue);
}

BOOL  CScanner::Set_Korean_3Of5_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	KOREAN_3OF5_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_KOREAN_3OF5;
	if (cmp(szPropertyName, L"korean3of5") ||
		cmp(szPropertyName, L"korean_3of5"))
	{
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"korean_3of5Redundancy") ||
		cmp(szPropertyName, L"korean3of5Redundancy"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bRedundancy = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bRedundancy = FALSE;
		else
			bValueRecognised = FALSE;
		SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"korean_3of5MinLength") ||
		cmp(szPropertyName, L"korean3of5MinLength"))
	{
		int iMin = StringToInt(szPropertyValue);
		if (iMin >= MINIMUM_KOREAN2OF5_LENGTH && iMin <= MAXIMUM_KOREAN2OF5_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, iMin, -1);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Korean 2of5 Minimum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"korean_3of5MaxLength") ||
		cmp(szPropertyName, L"korean3of5MaxLength"))
	{
		int iMax = StringToInt(szPropertyValue);
		if (iMax >= MINIMUM_KOREAN2OF5_LENGTH && iMax <= MAXIMUM_KOREAN2OF5_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, -1, iMax);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Korean 2of5 Maximum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}
	return bRetVal;
}

BOOL  CScanner::Set_Us4State_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_US4STATE, szPropertyName, szPropertyValue);
}

BOOL CScanner::Set_matrix2of5_MetaProc (LPCTSTR szPropertyName, LPCTSTR szPropertyValue)		
{
	MATRIX_2OF5_PARAMS decoderParameters;
	BOOL bRetVal = TRUE;
	BOOL bValueRecognised = TRUE;
	BYTE cCurrentDecoder = (BYTE)*DECODER_MATRIX_2OF5;
	if (cmp(szPropertyName, L"matrix2of5"))
	{
		//  Enable or Disable the Decoder
		if (cmp(szPropertyValue, L"Enabled") ||
			cmp(szPropertyValue, L"Enable"))
		{
			bRetVal = EnableDecoder(cCurrentDecoder);
		}
		else if (cmp(szPropertyValue, L"Disabled") ||
			cmp(szPropertyValue, L"Disable"))
		{
			bRetVal = DisableDecoder(cCurrentDecoder);
		}
		else
			bValueRecognised = FALSE;
	}
	else if (cmp(szPropertyName, L"Matrix2of5ReportCheckDigit"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bReportCheckDigit = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bReportCheckDigit = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Matrix2of5VerifyCheckDigit"))
	{
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyValue, L"True"))
			decoderParameters.bVerifyCheckDigit = TRUE;
		else if (cmp(szPropertyValue, L"False"))
			decoderParameters.bVerifyCheckDigit = FALSE;
		else
			bValueRecognised = FALSE;
		bRetVal = SetDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
	}
	else if (cmp(szPropertyName, L"Matrix2of5Minlength"))
	{
		int iMin = StringToInt(szPropertyValue);
		if (iMin >= MINIMUM_MATRIX2OF5_LENGTH && iMin <= MAXIMUM_MATRIX2OF5_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, iMin, -1);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Matrix2of5 Minimum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else if (cmp(szPropertyName, L"Matrix2of5Maxlength"))
	{
		int iMax = StringToInt(szPropertyValue);
		if (iMax >= MINIMUM_MATRIX2OF5_LENGTH && iMax <= MAXIMUM_MATRIX2OF5_LENGTH)
			bRetVal = SetDecoderLengths(cCurrentDecoder, -1, iMax);
		else
		{
			Log(SCANNER_LOG_WARNING, L"Matrix2of5 Maximum Length is out of Range", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	else
		bRetVal = FALSE;

	if (bRetVal && bValueRecognised)
	{
		//  Cancel any pending scanner reads to enable these new settings
		lpfn_SCAN_Flush(m_hScanner);
	}
	else
	{
		//  Failed to apply setting
		WCHAR tcLog[MAXLOG];
		wsprintf(tcLog, L"Unable to set Parameter %s to Value %s", 
			szPropertyName, szPropertyValue);
		Log(SCANNER_LOG_WARNING, tcLog, _T(__FUNCTION__), __LINE__);
	}

	return bRetVal;
}

BOOL CScanner::Set_Us4StateFics_MetaProc(LPCTSTR szPropertyName, LPCTSTR szPropertyValue)
{
	return SetSimpleDecoder((BYTE)*DECODER_US4STATE_FICS, szPropertyName, szPropertyValue);
}
