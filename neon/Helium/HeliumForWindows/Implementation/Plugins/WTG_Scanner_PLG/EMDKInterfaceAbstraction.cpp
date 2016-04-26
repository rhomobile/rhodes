#include "Scanner.h"


#pragma region Scanner_Properties

BOOL CScanner::RetrieveScannerParameters(READER_PARAMS* pReaderParams)
{
	// Initialize the structure
	memset(pReaderParams, 0, sizeof(READER_PARAMS));
	SI_INIT(pReaderParams);

	DWORD dwResult = lpfn_SCAN_GetReaderParams(m_hScanner, pReaderParams);
	if (dwResult != E_SCN_SUCCESS)
	{
		Log(SCANNER_LOG_WARNING, L"Unable to retrieve Scanner Reader Parameters", 
			_T(__FUNCTION__), __LINE__);
	}
	return (dwResult == E_SCN_SUCCESS);
}

BOOL CScanner::SetScannerParameters(READER_PARAMS* pReaderParams, DWORD dwSize)
{
	// Get pointer to start of params
	LPVOID lpParams = (LPVOID)&(pReaderParams->ReaderSpecific);
	LPDWORD lpdwParams = (LPDWORD)lpParams;
	// Loop through all params
	for(DWORD i = 0; i < (dwSize / sizeof(DWORD)); i++)
	{
		// Set this parameter into structure if it has changed
		SI_SET_IF_CHANGED(pReaderParams,
						  ReaderSpecific.dwUntyped[i],
						  lpdwParams[i]);
	}

	//  Flush any pending reads before applying the Scanner Parameters
	lpfn_SCAN_Flush(m_hScanner);
	DWORD dwResult = lpfn_SCAN_SetReaderParams(m_hScanner, pReaderParams);
	if (dwResult != E_SCN_SUCCESS)
	{
		WCHAR szLog[128];
		wsprintf(szLog, L"Unable to Set Scanner Reader Parameters, error: %x", dwResult);
		Log(SCANNER_LOG_WARNING, szLog, 
			_T(__FUNCTION__), __LINE__);
	}
	return (dwResult == E_SCN_SUCCESS);
}

BOOL CScanner::RetrieveScannerInterfaceParameters(INTERFACE_PARAMS* pInterfaceParams)
{
	// Initialize the structure
	memset(pInterfaceParams, 0, sizeof(INTERFACE_PARAMS));
	SI_INIT(pInterfaceParams);

	DWORD dwResult = lpfn_SCAN_GetInterfaceParams(m_hScanner, pInterfaceParams);
	if (dwResult != E_SCN_SUCCESS)
	{
		Log(SCANNER_LOG_WARNING, L"Unable to retrieve Scanner Interface Parameters", 
			_T(__FUNCTION__), __LINE__);
	}
	return (dwResult == E_SCN_SUCCESS);
}

BOOL CScanner::SetScannerInterfaceParameters(INTERFACE_PARAMS* interfaceParams, DWORD dwSize)
{
	// Get pointer to start of params
	LPVOID lpParams = (LPVOID)&(interfaceParams->InterfaceSpecific);
	LPDWORD lpdwParams = (LPDWORD)lpParams;
	// Loop through all params
	for(DWORD i = 0; i < (dwSize / sizeof(DWORD)); i++)
	{
		// Set this parameter into structure if it has changed
		SI_SET_IF_CHANGED(interfaceParams,
						  InterfaceSpecific.dwUntyped[i],
						  lpdwParams[i]);
	}

	//  Flush any pending reads before applying the Scanner Parameters
	lpfn_SCAN_Flush(m_hScanner);
	DWORD dwResult = lpfn_SCAN_SetInterfaceParams(m_hScanner, interfaceParams);
	if (dwResult != E_SCN_SUCCESS)
	{
		Log(SCANNER_LOG_WARNING, L"Unable to Set Scanner Reader Parameters", 
			_T(__FUNCTION__), __LINE__);
	}
	return (dwResult == E_SCN_SUCCESS);
}

#pragma endregion

#pragma region DecoderInterface

//  Gets the decoders supported by the Scanner
BOOL CScanner::RetrieveSupportedDecoders(WCHAR* szSupportedDecodersArray)
{
	BOOL bReturnValue = TRUE;
	DECODER_LIST decoderList;
	// Mark entire buffer allocated and not used
	memset(&decoderList, 0, sizeof(DECODER_LIST));
	SI_INIT(&decoderList);
	wcscpy(szSupportedDecodersArray, L"new Array(");

	// Get supported decoder list
	if (lpfn_SCAN_GetSupportedDecoders(m_hScanner, &decoderList) == E_SCN_SUCCESS)
	{
		for(DWORD i=0; i < decoderList.Decoders.dwDecoderCount; i++)
		{
			if (decoderStrings[decoderList.Decoders.byList[i]])
			{	
				//  Only report the Decoder if we support it
				wcscat(szSupportedDecodersArray, L"'");
				wcscat(szSupportedDecodersArray, 
					decoderStrings[decoderList.Decoders.byList[i]]);
				wcscat(szSupportedDecodersArray, L"',");
			}
		}
	}
	else
	{
		//  Call to GetSupportedDecoders has failed
		Log(SCANNER_LOG_WARNING, L"Unable to retrieve supported decoders", 
			_T(__FUNCTION__), __LINE__);
		bReturnValue = FALSE;
	}
	//  Remove the final comma in the array
	if (szSupportedDecodersArray[wcslen(szSupportedDecodersArray) - 1] == L',')
		szSupportedDecodersArray[wcslen(szSupportedDecodersArray) - 1] = L')';
	else
		wcscat(szSupportedDecodersArray, L")");

	return bReturnValue;
}

BOOL CScanner::IsDecoderSupported(BYTE cDecoder)
{
	DECODER_LIST decoderList;
	// Mark entire buffer allocated and not used
	memset(&decoderList, 0, sizeof(DECODER_LIST));
	SI_INIT(&decoderList);
	// Get supported decoder list
	if (lpfn_SCAN_GetSupportedDecoders(m_hScanner, &decoderList) == E_SCN_SUCCESS)
	{
		for(DWORD i=0; i < decoderList.Decoders.dwDecoderCount; i++)
		{
			if (decoderList.Decoders.byList[i] == cDecoder)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CScanner::EnableDecoder(BYTE cDecoder)
{
	BOOL bRetVal = FALSE;
	if (IsDecoderSupported(cDecoder))
	{
		DECODER_LIST decoderList;
		DWORD dwResult;
		// Mark entire buffer allocated, and not used
		memset(&decoderList, 0, sizeof(DECODER_LIST));
		SI_INIT(&decoderList);

		// Get enabled decoder list
		dwResult = lpfn_SCAN_GetEnabledDecoders(m_hScanner, &decoderList);
		if (dwResult == E_SCN_SUCCESS)
		{
			//  Check we haven't already enabled the decoder
			for(DWORD i = 0; i < decoderList.Decoders.dwDecoderCount; i++)
			{
				if (decoderList.Decoders.byList[i] == cDecoder)
				{
					//  The decoder is already enabled, alert the user and do nothing
					if (decoderStrings[cDecoder])
					{
						WCHAR wLog[MAXLOG];
						wsprintf(wLog, L"Attempted to enable the decoder %s but it is already enabled", 
							decoderStrings[cDecoder]);
						Log(SCANNER_LOG_INFO, wLog, _T(__FUNCTION__), __LINE__);
					}
					return bRetVal;
				}
			}
			decoderList.Decoders.byList[decoderList.Decoders.dwDecoderCount++] = cDecoder;
			dwResult = lpfn_SCAN_SetEnabledDecoders(m_hScanner,&decoderList);
			if (dwResult != E_SCN_SUCCESS)
			{
				Log(SCANNER_LOG_WARNING, L"Unable to set Enabled Decoders",
					_T(__FUNCTION__), __LINE__);
			}
			else
				bRetVal = TRUE;
		}
		else
		{
			//	Unable to retrieve the enabled decoders
			Log(SCANNER_LOG_WARNING, L"Unable to retrieve enabled decoders",
				_T(__FUNCTION__), __LINE__);
		}		
	}
	else
	{
		//  Attempted to enable an unsupported decoder
		WCHAR wLog[MAXLOG];
		if (decoderStrings[cDecoder])
		{
			wsprintf(wLog, L"Decoder %s is not supported on this device, unable to Enable it", 
				decoderStrings[cDecoder]);
			Log(SCANNER_LOG_WARNING, wLog, _T(__FUNCTION__), __LINE__);
		}
	}
	return bRetVal;
}

BOOL CScanner::DisableDecoder(BYTE cDecoder)
{
	BOOL bRetVal = FALSE;
	if (IsDecoderSupported(cDecoder))
	{
		DECODER_LIST decoderList;
		DWORD dwResult;
		// Mark entire buffer allocated, and not used
		memset(&decoderList, 0, sizeof(DECODER_LIST));
		SI_INIT(&decoderList);
		bool bFoundDecoder = false;

		// Get enabled decoder list
		dwResult = lpfn_SCAN_GetEnabledDecoders(m_hScanner, &decoderList);
		if (dwResult == E_SCN_SUCCESS)
		{
			//  Check we haven't already enabled the decoder
			for(DWORD i = 0; i < decoderList.Decoders.dwDecoderCount; i++)
			{
				if (decoderList.Decoders.byList[i] == cDecoder)
				{
					//  The decoder is already enabled
					memcpy(decoderList.Decoders.byList + i,
						decoderList.Decoders.byList + (i + 1),
						decoderList.Decoders.dwDecoderCount - (i - 1));
						decoderList.Decoders.dwDecoderCount--;
					bFoundDecoder = true;
				}
			}
			//  Check the decoder was enabled
			if (!bFoundDecoder)
			{
				if (decoderStrings[cDecoder])
				{
					WCHAR wLog[MAXLOG];
					wsprintf(wLog, L"Decoder %s is not enabled, therefore unable to disable it", 
						decoderStrings[cDecoder]);
					Log(SCANNER_LOG_INFO, wLog, _T(__FUNCTION__), __LINE__);
				}
			}
			else
			{
				//  The decoder is enabled, disable it (decoderList updated earlier)
				dwResult = lpfn_SCAN_SetEnabledDecoders(m_hScanner, &decoderList);
				if (dwResult != E_SCN_SUCCESS)
				{
					Log(SCANNER_LOG_WARNING, L"Unable to set Enabled Decoders",
						_T(__FUNCTION__), __LINE__);
				}
				else
					bRetVal = TRUE;
			}
		}
		else
		{
			//	Unable to retrieve the enabled decoders
			Log(SCANNER_LOG_WARNING, L"Unable to retrieve enabled decoders",
				_T(__FUNCTION__), __LINE__);
		}		
	}
	else
	{
		//  Attempted to disable an unsupported decoder
		if (decoderStrings[cDecoder])
		{
			WCHAR wLog[MAXLOG];
			wsprintf(wLog, L"Decoder %s is not supported on this device, unable to Disable it", 
				decoderStrings[cDecoder]);
			Log(SCANNER_LOG_WARNING, wLog, _T(__FUNCTION__), __LINE__);
		}
	}
	return bRetVal;
}

//  Retrieve the current parameters for the specified decoder.  lpvParams is an out parameter
BOOL CScanner::RetrieveDecoderParams(BYTE cDecoder, LPVOID lpvParams, DWORD dwSize)
{
	BOOL bRetVal = TRUE;
	DECODER_PARAMS decoder_params;
	memset(&decoder_params,sizeof(decoder_params),0);
	SI_INIT(&decoder_params);

	// Initialize the out parameter in pvParams
	memset(lpvParams, 0, dwSize);
	// Initialize the structure to all zeroes
	memset(&decoder_params,sizeof(decoder_params), 0);
	
	// Get current decoder parameters
	DWORD dwResult = lpfn_SCAN_GetDecoderParams(m_hScanner, (LPDECODER)&cDecoder, &decoder_params);

	// If there was an error getting the parameters
	if (dwResult != E_SCN_SUCCESS)
	{
		Log(SCANNER_LOG_WARNING, L"Unable to retrieve Scanner Decoder Parameters", 
			_T(__FUNCTION__), __LINE__);
		bRetVal = FALSE;
	}
	else
	{
		// Get pointer to start of params
		LPDWORD lpdwParams = (LPDWORD)lpvParams;

		// Loop through all params and retrieve them
		for(DWORD i = 0; i < (dwSize / sizeof(DWORD)); i++)
		{
			SI_GET_FIELD(&decoder_params, dec_specific.dwUntyped[i], lpdwParams[i]);
		}
	}

	return bRetVal;
}

//  Sets the specified decoder's parameters to those provided in lpvParams
BOOL CScanner::SetDecoderParams(BYTE cDecoder, LPVOID lpvParams, DWORD dwSize)
{
	BOOL bRetVal = TRUE;
	DECODER_PARAMS decoder_params;
	// Mark entire buffer allocated and not used
	memset(&decoder_params,sizeof(decoder_params),0);
	SI_INIT(&decoder_params);
	
	// Get current decoder parameters
	DWORD dwResult = lpfn_SCAN_GetDecoderParams(m_hScanner,
									 (LPDECODER)&cDecoder,
									 &decoder_params);

	// If there was an error getting the parameters
	if ( dwResult != E_SCN_SUCCESS )
	{
		Log(SCANNER_LOG_WARNING, L"Unable to retrieve decoder parameters", 
			_T(__FUNCTION__), __LINE__);
		bRetVal = FALSE;
	}
	else
	{
		// Get pointer to start of params
		LPDWORD lpdwParams = (LPDWORD)lpvParams;

		// Loop through all params
		for(DWORD i = 0; i < (dwSize / sizeof(DWORD)); i++)
		{
			// Set this parameter into structure if it has changed
			SI_SET_IF_CHANGED(&decoder_params,
							  dec_specific.dwUntyped[i],
							  lpdwParams[i]);
		}

		// Return the result of setting parameters
		dwResult = lpfn_SCAN_SetDecoderParams(m_hScanner,
									 (LPDECODER)&cDecoder,
									 &decoder_params);
		if (dwResult != E_SCN_SUCCESS)
		{
			Log(SCANNER_LOG_WARNING, L"Unable to set decoder parameters", 
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	return bRetVal;
}

//  Set iMinLength or iMaxLength to -1 if you do not wish to modify them
//  Sets the Maximum or Minimum length of the specified decoder
BOOL CScanner::SetDecoderLengths(BYTE cDecoder, int iMinLength, int iMaxLength)
{
	BOOL bRetVal = TRUE;
	DECODER_PARAMS decoder_params;
	memset(&decoder_params,sizeof(decoder_params), 0);
	SI_INIT(&decoder_params);

	// Get current decoder parameters
	DWORD dwResult = lpfn_SCAN_GetDecoderParams(m_hScanner,
									 (LPDECODER)&cDecoder,
									 &decoder_params);

	// If there was an error getting the parameters
	if (dwResult != E_SCN_SUCCESS)
	{
		Log(SCANNER_LOG_WARNING, L"Unable to retrieve the current Decoder Parameters",
			_T(__FUNCTION__), __LINE__);
		bRetVal = FALSE;
	}
	else
	{
		// Set all parameters into structure if changed
		if (iMinLength >= 0)
			SI_SET_IF_CHANGED(&decoder_params, dwMinLength, (DWORD)iMinLength);
		if (iMaxLength >= 0)
			SI_SET_IF_CHANGED(&decoder_params, dwMaxLength, (DWORD)iMaxLength);

		// Return the result of setting parameters
		dwResult = (lpfn_SCAN_SetDecoderParams(m_hScanner,
									 (LPDECODER)&cDecoder,
									 &decoder_params));
		if (dwResult != E_SCN_SUCCESS)
		{
			Log(SCANNER_LOG_WARNING, L"Unable to Set current Decoder Parameters",
				_T(__FUNCTION__), __LINE__);
			bRetVal = FALSE;
		}
	}
	return bRetVal;
}

//  Return the maximum or minimum length of the specified decoder
int CScanner::GetDecoderLength(BYTE cDecoder, bool bMinLength)
{
	int iRetVal = -1;
	DECODER_PARAMS decoder_params;
	memset(&decoder_params,sizeof(decoder_params), 0);
	SI_INIT(&decoder_params);

	// Get current decoder parameters
	DWORD dwResult = lpfn_SCAN_GetDecoderParams(m_hScanner,
									 (LPDECODER)&cDecoder,
									 &decoder_params);

	// If there was an error getting the parameters
	if (dwResult != E_SCN_SUCCESS)
	{
		Log(SCANNER_LOG_WARNING, L"Unable to retrieve the current Decoder Parameters",
			_T(__FUNCTION__), __LINE__);
		iRetVal = -1;
	}
	else
	{
		if (bMinLength)
			iRetVal = decoder_params.dwMinLength;
		else
			iRetVal = decoder_params.dwMaxLength;
	}
	return iRetVal;
}


//  Retrieve the UPC EAN parameters.  lpParams is an out parameter
BOOL CScanner::RetrieveUPCEANParams(LPUPC_EAN_PARAMS lpParams)
{
	BOOL bRetVal = TRUE;
	//  Initialise params structure to return
	SI_INIT(lpParams);

	//  Get current parameters
	DWORD dwResult = lpfn_SCAN_GetUPCEANParams(m_hScanner, lpParams);

	// If there was an error getting the parameters
	if (dwResult != E_SCN_SUCCESS)
	{
		Log(SCANNER_LOG_WARNING, L"Unable to retrieve UPC EAN Parameters", 
			_T(__FUNCTION__), __LINE__);
		bRetVal = FALSE;
	}

	return bRetVal;
}

BOOL CScanner::SetUPCEANParams(LPUPC_EAN_PARAMS lpParams)
{
	BOOL bRetVal = TRUE;
	SI_ALLOC_ALL(lpParams);

	//  Set parameters
	DWORD dwResult = lpfn_SCAN_SetUPCEANParams(m_hScanner, lpParams);

	// If there was an error setting the parameters
	if (dwResult != E_SCN_SUCCESS)
	{
		Log(SCANNER_LOG_WARNING, L"Unable to set UPC EAN Parameters", 
			_T(__FUNCTION__), __LINE__);
		bRetVal = FALSE;
	}

	return bRetVal;
}

#pragma endregion