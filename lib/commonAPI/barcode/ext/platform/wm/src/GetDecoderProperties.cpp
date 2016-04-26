#include "Scanner.h"

int CScanner::Get_all_decoders_PropertyValue(LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	DECODER_LIST supportedDecoderList;
	memset(&supportedDecoderList, 0, sizeof(DECODER_LIST));
	SI_INIT(&supportedDecoderList);
	DECODER_LIST enabledDecoderList;
	memset(&enabledDecoderList, 0, sizeof(DECODER_LIST));
	SI_INIT(&enabledDecoderList);
	DWORD dwResult;
	//  Retrieve the device's supported decoders
	dwResult = lpfn_SCAN_GetSupportedDecoders(m_hScanner, &supportedDecoderList);
	if (dwResult == E_SCN_SUCCESS) 
	{
		for (unsigned int i = 0; i < supportedDecoderList.Decoders.dwDecoderCount; i++)
		{
			//	Remove decoders unsupported by RhoElements.  Note imaging barcodes must be 
			//  received in a buffer allocated to hold binary data:
			//  scan_allocateBuffer(false, size)
			if (supportedDecoderList.Decoders.byList[i] == (BYTE)*DECODER_POINTER ||
				supportedDecoderList.Decoders.byList[i] == (BYTE)*DECODER_IMAGE ||
				supportedDecoderList.Decoders.byList[i] == (BYTE)*DECODER_DOCCAP)
			{
				//  Shift the decoder structure
				memcpy(supportedDecoderList.Decoders.byList + i,
					supportedDecoderList.Decoders.byList + (i + 1),
				supportedDecoderList.Decoders.dwDecoderCount - i);
				supportedDecoderList.Decoders.dwDecoderCount--;
				i--;
			}
		}
		dwResult = lpfn_SCAN_GetEnabledDecoders(m_hScanner, &enabledDecoderList);
		if (dwResult == E_SCN_SUCCESS)
		{
			if (supportedDecoderList.Decoders.dwDecoderCount == 
				enabledDecoderList.Decoders.dwDecoderCount)
			{
				//  The number of enabled and supported decoders are the same
				iRetVal = wcslen(L"true") + 1;
				if (szPropertyValue)
					if (wcscpy_s(szPropertyValue, iRetVal, L"true") == EINVAL)
						iRetVal = -1;
			}
			else
			{
				//  The number of enabled and supported decoders is different
				iRetVal = wcslen(L"false") + 1;
				if (szPropertyValue)
					if (wcscpy_s(szPropertyValue, iRetVal, L"false") == EINVAL)
						iRetVal = -1;
			}
		}
	}
	return iRetVal;
}

//  Abstract Decoders which only have Enabled / Disabled to worry about (No Parameters)
int CScanner::GetSimpleDecoder(BYTE cDecoder, LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
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
			//  Search for our decoder in the list of enabled decoders
			for(DWORD i = 0; i < decoderList.Decoders.dwDecoderCount; i++)
			{
				if (decoderList.Decoders.byList[i] == cDecoder)
				{
					//  This decoder is enabled
					iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
				}
			}
			if (iRetVal == -1)
			{
				//  The decoder was disabled
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
			}
		}
		else
		{
			//	Unable to retrieve the enabled decoders
			LOG(WARNING) + L"Unable to retrieve enabled decoders";
		}		
	}
	else
	{
		//  Attempted to enable an unsupported decoder
		WCHAR wLog[MAXLOG];
		if (decoderStrings[cDecoder])
		{
			wsprintf(wLog, L"Decoder %s is not supported on this device, unable to retrive its status", 
				decoderStrings[cDecoder]);
			LOG(WARNING) + wLog;
		}
	}
	return iRetVal;
}

int CScanner::Get_pdf417_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_PDF417, szPropertyName, szPropertyValue);
}

int CScanner::Get_Chinese_2Of5_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_CHINESE_2OF5, szPropertyName, szPropertyValue);
}

int CScanner::Get_AZTEC_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_AZTEC, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_MicroQR_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_MICROQR, szPropertyName, szPropertyValue);
}
		

int CScanner::Get_Korean_3Of5_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_KOREAN_3OF5;
	if (cmp(szPropertyName, L"korean3of5") ||
		cmp(szPropertyName, L"korean_3of5"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		if (cmp(szPropertyName, L"korean_3of5Redundancy") ||
			cmp(szPropertyName, L"korean3of5Redundancy"))
		{
			KOREAN_3OF5_PARAMS decoderParameters;
			RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
			if (decoderParameters.bRedundancy)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"korean_3of5MinLength") ||
			cmp(szPropertyName, L"korean3of5MinLength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, true), szPropertyValue);
		}
		else if (cmp(szPropertyName, L"korean_3of5MaxLength") ||
			cmp(szPropertyName, L"korean3of5MaxLength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, false), szPropertyValue);
		}	
	}
	return iRetVal;
}

int CScanner::Get_Us4State_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_US4STATE, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_webcode_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_WEBCODE;
	if (cmp(szPropertyName, L"webcode"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		WEBCODE_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"webcodeDecodeGtSubtype"))
		{
			if (decoderParameters.bGTWebcode)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_uspostnet_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_USPOSTNET, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_usplanet_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_USPLANET, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_ukpostal_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_UKPOSTAL, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_tlc39_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_TLC39, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_signature_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_SIGNATURE;
	if (cmp(szPropertyName, L"signature"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		SIGNATURE_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"signatureImageQuality"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				(int)decoderParameters.dwJpegImageQuality, szPropertyValue);
		}
		else if (cmp(szPropertyName, L"signatureImageSize"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				(int)decoderParameters.dwJpegImageSize, szPropertyValue);
		}
		else if (cmp(szPropertyName, L"signatureImageWidth"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				(int)decoderParameters.dwImageWidth, szPropertyValue);
		}
		else if (cmp(szPropertyName, L"SignatureImageHeight"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				(int)decoderParameters.dwImageHeight, szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_rsslim_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_RSSLIM, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_rssexp_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_RSSEXP, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_rss14_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_RSS14, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_qrcode_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_QRCODE, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_micropdf_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_MICROPDF, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_maxicode_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_MAXICODE, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_macropdf_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_MACROPDF;
	if (cmp(szPropertyName, L"macroPdf"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		MACROPDF_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"macroPdfBufferLabels"))
		{
			if (decoderParameters.bBufferLabels)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"macroPdfExclusive"))
		{
			if (decoderParameters.bExclusive)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"macroPdfConvertToPdf417"))
		{
			if (decoderParameters.bConvertToPDF417)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_macromicropdf_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_MACROMICROPDF;
	if (cmp(szPropertyName, L"macroMicroPdf"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		MACROMICROPDF_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"macroMicroPdfReportAppendInfo"))
		{
			if (decoderParameters.bReportAppendInfo)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"macroMicroPdfBufferLabels"))
		{
			if (decoderParameters.bBufferLabels)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"macroMicroPdfExclusive"))
		{
			if (decoderParameters.bExclusive)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"macroMicroPdfConvertToMicroPdf"))
		{
			if (decoderParameters.bConvertToMicroPDF)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_jappostal_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_JAPPOSTAL, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_ean13_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_EAN13, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_dutchpostal_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_DUTCHPOSTAL, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_datamatrix_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_DATAMATRIX, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_composit_c_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_COMPOSITE_C, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_composit_ab_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_COMPOSITE_AB;
	if (cmp(szPropertyName, L"compositeAb"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		COMPOSITE_AB_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"compositeAbUseUpcPreambleCheckDigitRules"))
		{
			if (decoderParameters.bUseUPCPreambleCheckDigitRules)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"compositeAbUccLinkMode"))
		{
			if (decoderParameters.dwUCCLinkMode == UCC_NEVER)
				iRetVal = ProcessDecoderPropertyForReturn(L"never", szPropertyValue);
			else if (decoderParameters.dwUCCLinkMode == UCC_ALWAYS)
				iRetVal = ProcessDecoderPropertyForReturn(L"always", szPropertyValue);
			else if (decoderParameters.dwUCCLinkMode == UCC_AUTO)
				iRetVal = ProcessDecoderPropertyForReturn(L"auto", szPropertyValue);
			else
				iRetVal = -1;
		}
	}
	return iRetVal;
}
	
int CScanner::Get_canpostal_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_CANPOSTAL, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_auspostal_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_AUSPOSTAL, szPropertyName, szPropertyValue);
}
	
int CScanner::Get_upce1_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_UPCE1;
	if (cmp(szPropertyName, L"upce1"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		UPCE1_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"upce1Preamble"))
		{
			if (decoderParameters.dwPreamble == PREAMBLE_NONE)
				iRetVal = ProcessDecoderPropertyForReturn(L"none", szPropertyValue);
			else if (decoderParameters.dwPreamble == PREAMBLE_SYSTEM_CHAR)
				iRetVal = ProcessDecoderPropertyForReturn(L"systemChar", szPropertyValue);
			else if (decoderParameters.dwPreamble == PREAMBLE_COUNTRY_AND_SYSTEM_CHARS)
				iRetVal = ProcessDecoderPropertyForReturn(L"countryAndSystemChars", szPropertyValue);
			else
				iRetVal = -1;
		}
		else if (cmp(szPropertyName, L"upce1ReportCheckDigit"))
		{
			if (decoderParameters.bReportCheckDigit)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"upce1ConvertToUPCA"))
		{
			if (decoderParameters.bConvertToUPCA)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_upce0_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_UPCE0;
	if (cmp(szPropertyName, L"upce0"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		UPCE0_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"upce0Preamble"))
		{
			if (decoderParameters.dwPreamble == PREAMBLE_NONE)
				iRetVal = ProcessDecoderPropertyForReturn(L"none", szPropertyValue);
			else if (decoderParameters.dwPreamble == PREAMBLE_SYSTEM_CHAR)
				iRetVal = ProcessDecoderPropertyForReturn(L"systemChar", szPropertyValue);
			else if (decoderParameters.dwPreamble == PREAMBLE_COUNTRY_AND_SYSTEM_CHARS)
				iRetVal = ProcessDecoderPropertyForReturn(L"countryAndSystemChars", szPropertyValue);
			else
				iRetVal = -1;
		}
		else if (cmp(szPropertyName, L"upce0ReportCheckDigit"))
		{
			if (decoderParameters.bReportCheckDigit)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"upce0ConvertToUPCA"))
		{
			if (decoderParameters.bConvertToUPCA)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_upca_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_UPCA;
	if (cmp(szPropertyName, L"upca"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		UPCA_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"upcaPreamble"))
		{
			if (decoderParameters.dwPreamble == PREAMBLE_NONE)
				iRetVal = ProcessDecoderPropertyForReturn(L"none", szPropertyValue);
			else if (decoderParameters.dwPreamble == PREAMBLE_SYSTEM_CHAR)
				iRetVal = ProcessDecoderPropertyForReturn(L"systemChar", szPropertyValue);
			else if (decoderParameters.dwPreamble == PREAMBLE_COUNTRY_AND_SYSTEM_CHARS)
				iRetVal = ProcessDecoderPropertyForReturn(L"countryAndSystemChars", szPropertyValue);
			else
				iRetVal = -1;
		}
		else if (cmp(szPropertyName, L"upcaReportCheckDigit"))
		{
			if (decoderParameters.bReportCheckDigit)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_upc_ean_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	UPC_EAN_PARAMS upceanParams;
	RetrieveUPCEANParams(&upceanParams);
		if (cmp(szPropertyName, L"upc_eanbookland") ||
		cmp(szPropertyName, L"upceanbookland"))
	{
		if (upceanParams.bBookland)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"upc_eanCoupon") ||
		cmp(szPropertyName, L"upceanCoupon"))
	{
		if (upceanParams.bCoupon)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"upc_eanLinearDecode") ||
		cmp(szPropertyName, L"upceanLinearDecode"))
	{
		if (upceanParams.bLinearDecode)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"upc_eanRandomWeightCheckDigit") ||
		cmp(szPropertyName, L"upceanRandomWeightCheckDigit"))
	{
		if (upceanParams.bRandomWeightCheckDigit)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"upc_eanSupplemental2") ||
		cmp(szPropertyName, L"upceanSupplemental2"))
	{
		if (upceanParams.bSupplemental2)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"upc_eanSupplemental5") ||
		cmp(szPropertyName, L"upceanSupplemental5"))
	{
		if (upceanParams.bSupplemental5)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"upc_eanConvertRSSToUpcean") ||
		cmp(szPropertyName, L"upceanConvertRSSToUpcean") ||
		cmp(szPropertyName, L"upcEanConvertGs1dataBarToUpcEan"))
	{
		if (upceanParams.bConvertRSSToUPCEAN)
			iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
		else
			iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
	}
	else if (cmp(szPropertyName, L"upc_eanRetryCount") ||
		cmp(szPropertyName, L"upceanRetryCount"))
	{
		iRetVal = ProcessDecoderPropertyForReturn(
			(int)upceanParams.dwRetryCount, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"upc_eanSecurityLevel") ||
		cmp(szPropertyName, L"upceanSecurityLevel"))
	{
		//  None, Ambiguous and All have been deprecated in the EMDK and replaced by numbers
		iRetVal = ProcessDecoderPropertyForReturn(
			(int)upceanParams.dwSecurityLevel, szPropertyValue);
	}
	else if (cmp(szPropertyName, L"upc_eanSupplementalMode") ||
		cmp(szPropertyName, L"upceanSupplementalMode"))
	{
		if (upceanParams.dwSupplementalMode == SUPPLEMENTALS_NONE)
			iRetVal = ProcessDecoderPropertyForReturn(L"none", szPropertyValue);
		else if (upceanParams.dwSupplementalMode == SUPPLEMENTALS_ALWAYS)
			iRetVal = ProcessDecoderPropertyForReturn(L"always", szPropertyValue);
		else if (upceanParams.dwSupplementalMode == SUPPLEMENTALS_AUTO)
			iRetVal = ProcessDecoderPropertyForReturn(L"auto", szPropertyValue);
		else if (upceanParams.dwSupplementalMode == SUPPLEMENTALS_SMART)
			iRetVal = ProcessDecoderPropertyForReturn(L"smart", szPropertyValue);
		else if (upceanParams.dwSupplementalMode == SUPPLEMENTALS_378_379)
			iRetVal = ProcessDecoderPropertyForReturn(L"378or379", szPropertyValue);
		//  Supplementals 978 has been deprecated in EMDK and also shares the same value as it's
		//  replacement
//		else if (upceanParams.dwSupplementalMode == SUPPLEMENTALS_978)
//			iRetVal = ProcessDecoderPropertyForReturn(L"978", szPropertyValue);
		else if (upceanParams.dwSupplementalMode == SUPPLEMENTALS_978_979)
			iRetVal = ProcessDecoderPropertyForReturn(L"978or979", szPropertyValue);
		else if (upceanParams.dwSupplementalMode == SUPPLEMENTALS_977)
			iRetVal = ProcessDecoderPropertyForReturn(L"977", szPropertyValue);
		else if (upceanParams.dwSupplementalMode == SUPPLEMENTALS_414_419_434_439)
			iRetVal = ProcessDecoderPropertyForReturn(L"414or419or434or439", szPropertyValue);
		else
			iRetVal = -1;
	}
	else if (cmp(szPropertyName, L"upc_eanBooklandFormat") ||
		cmp(szPropertyName, L"upceanBooklandFormat"))
	{
		if (upceanParams.dwBooklandFormat == BOOKLAND_FORMAT_ISBN_10)
			iRetVal = ProcessDecoderPropertyForReturn(L"isbn10", szPropertyValue);
		else if (upceanParams.dwBooklandFormat == BOOKLAND_FORMAT_ISBN_13)
			iRetVal = ProcessDecoderPropertyForReturn(L"isbn13", szPropertyValue);
		else
			iRetVal = -1;
	}

	return iRetVal;
}
	
int CScanner::Get_trioptic39_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_TRIOPTIC39;
	if (cmp(szPropertyName, L"trioptic39"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		TRIOPTIC39_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"trioptic39Redundancy"))
		{
			if (decoderParameters.bRedundancy)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_msi_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_MSI;
	if (cmp(szPropertyName, L"msi"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		MSI_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"msiCheckDigits"))
		{
			if (decoderParameters.dwCheckDigits == MSI_ONE_CHECK_DIGIT)
				iRetVal = ProcessDecoderPropertyForReturn(L"one", szPropertyValue);
			else if (decoderParameters.dwCheckDigits == MSI_TWO_CHECK_DIGIT)
				iRetVal = ProcessDecoderPropertyForReturn(L"two", szPropertyValue);
			else
				iRetVal = -1;
		}
		else if (cmp(szPropertyName, L"msiCheckDigitScheme"))
		{
			if (decoderParameters.dwCheckDigitScheme == MSI_CHKDGT_MOD_11_10)
				iRetVal = ProcessDecoderPropertyForReturn(L"mod11", szPropertyValue);
			else if (decoderParameters.dwCheckDigitScheme == MSI_CHKDGT_MOD_10_10)
				iRetVal = ProcessDecoderPropertyForReturn(L"mod10", szPropertyValue);
			else
				iRetVal = -1;
		}
		else if (cmp(szPropertyName, L"msiRedundancy"))
		{
			if (decoderParameters.bRedundancy)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"msiReportCheckDigit"))
		{
			if (decoderParameters.bReportCheckDigit)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"msiMinLength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, true), szPropertyValue);
		}
		else if (cmp(szPropertyName, L"msiMaxLength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, false), szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_i2of5_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_I2OF5;
	if (cmp(szPropertyName, L"i2of5"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		I2OF5_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"I2of5Redundancy"))
		{
			if (decoderParameters.bRedundancy)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"I2of5ConvertToEAN13"))
		{
			if (decoderParameters.bConvertToEAN13)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"I2of5ReportCheckDigit"))
		{
			if (decoderParameters.bReportCheckDigit)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"I2of5VerifyCheckDigit"))
		{
			if (decoderParameters.dwVerifyCheckDigit == I2OF5_NO_CHECK_DIGIT)
				iRetVal = ProcessDecoderPropertyForReturn(L"none", szPropertyValue);
			else if (decoderParameters.dwVerifyCheckDigit == I2OF5_USS_CHECK_DIGIT)
				iRetVal = ProcessDecoderPropertyForReturn(L"uss", szPropertyValue);
			else if (decoderParameters.dwVerifyCheckDigit == I2OF5_OPCC_CHECK_DIGIT)
				iRetVal = ProcessDecoderPropertyForReturn(L"opcc", szPropertyValue);
			else
				iRetVal = -1;
		}
		else if (cmp(szPropertyName, L"I2of5Minlength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, true), szPropertyValue);
		}
		else if (cmp(szPropertyName, L"I2of5Maxlength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, false), szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_ean8_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_EAN8;
	if (cmp(szPropertyName, L"ean8"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		EAN8_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"ean8ConvertToEan13"))
		{
			if (decoderParameters.bConvertToEAN13)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_d2of5_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_D2OF5;
	if (cmp(szPropertyName, L"d2of5"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		D2OF5_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"D2of5Redundancy"))
		{
			if (decoderParameters.bRedundancy)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"D2of5Minlength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, true), szPropertyValue);
		}
		else if (cmp(szPropertyName, L"D2of5Maxlength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, false), szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_code93_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_CODE93;
	if (cmp(szPropertyName, L"code93"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		CODE93_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"Code93Redundancy"))
		{
			if (decoderParameters.bRedundancy)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code93MinLength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, true), szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code93MaxLength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, false), szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_code39_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_CODE39;
	if (cmp(szPropertyName, L"code39"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		CODE39_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"Code39Redundancy"))
		{
			if (decoderParameters.bRedundancy)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code39ReportCheckDigit"))
		{
			if (decoderParameters.bReportCheckDigit)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code39VerifyCheckDigit"))
		{
			if (decoderParameters.bVerifyCheckDigit)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code39Code32Prefix"))
		{
			if (decoderParameters.bCode32Prefix)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code39Concatenation"))
		{
			if (decoderParameters.bConcatenation)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code39ConvertToCode32"))
		{
			if (decoderParameters.bConvertToCode32)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code39FullAscii"))
		{
			if (decoderParameters.bFullAscii)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code39SecurityLevel"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				(int)decoderParameters.dwSecurityLevel, szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code39MinLength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, true), szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code39MaxLength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, false), szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_code11_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_CODE11;
	if (cmp(szPropertyName, L"code11"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		CODE11_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"Code11Redundancy"))
		{
			if (decoderParameters.bRedundancy)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code11ReportCheckDigit"))
		{
			if (decoderParameters.bReportCheckDigit)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code11CheckDigitCount"))
		{
			if (decoderParameters.dwCheckDigitCount == CODE11_NO_CHECK_DIGIT)
				iRetVal = ProcessDecoderPropertyForReturn(L"none", szPropertyValue);
			else if (decoderParameters.dwCheckDigitCount == CODE11_ONE_CHECK_DIGIT)
				iRetVal = ProcessDecoderPropertyForReturn(L"one", szPropertyValue);
			else if (decoderParameters.dwCheckDigitCount == CODE11_TWO_CHECK_DIGIT)
				iRetVal = ProcessDecoderPropertyForReturn(L"two", szPropertyValue);
			else
				iRetVal = -1;
		}
		else if (cmp(szPropertyName, L"Code11MinLength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, true), szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code11MaxLength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, false), szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_code128_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_CODE128;
	if (cmp(szPropertyName, L"code128"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		CODE128_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"Code128Redundancy"))
		{
			if (decoderParameters.bRedundancy)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code128ean128"))
		{
			if (decoderParameters.bEAN128)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code128isBt128"))
		{
			if (decoderParameters.bISBT128)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code128Other128"))
		{
			if (decoderParameters.bOther128)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code128checkIsBtTable"))
		{
			if (decoderParameters.bCheckISBTTable)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code128isBt128ConcatMode"))
		{
			if (decoderParameters.dwISBT128ConcatMode == ISBT128_CONCAT_MODE_NEVER)
				iRetVal = ProcessDecoderPropertyForReturn(L"never", szPropertyValue);
			else if (decoderParameters.dwISBT128ConcatMode == ISBT128_CONCAT_MODE_ALWAYS)
				iRetVal = ProcessDecoderPropertyForReturn(L"always", szPropertyValue);
			else if (decoderParameters.dwISBT128ConcatMode == ISBT128_CONCAT_MODE_AUTO)
				iRetVal = ProcessDecoderPropertyForReturn(L"auto", szPropertyValue);
			else
				iRetVal = -1;
		}
		else if (cmp(szPropertyName, L"Code128SecurityLevel"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				(int)decoderParameters.dwSecurityLevel, szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code128MinLength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, true), szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Code128MaxLength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, false), szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_codabar_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_CODABAR;
	if (cmp(szPropertyName, L"codabar"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		CODABAR_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"CodabarRedundancy"))
		{
			//  Codabar Redundancy
			if (decoderParameters.bRedundancy)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"CodabarCLSIEditing"))
		{
			//  Codabar CLSI Editing
			if (decoderParameters.bClsiEditing)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"CodabarNotisEditing"))
		{
			//  Codabar Notis Editing
			if (decoderParameters.bNotisEditing)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"CodabarMinLength"))
		{
			//  Codabar Min Length
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, true), szPropertyValue);
		}
		else if (cmp(szPropertyName, L"CodabarMaxLength"))
		{
			//  Codabar Max Length
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, false), szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_matrix2of5_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	int iRetVal = -1;
	BYTE cCurrentDecoder = (BYTE)*DECODER_MATRIX_2OF5;
	if (cmp(szPropertyName, L"matrix2of5"))
	{
		iRetVal = GetSimpleDecoder(cCurrentDecoder, szPropertyName, szPropertyValue);
	}
	else
	{
		MATRIX_2OF5_PARAMS decoderParameters;
		RetrieveDecoderParams(cCurrentDecoder, &decoderParameters, sizeof(decoderParameters));
		if (cmp(szPropertyName, L"Matrix2of5ReportCheckDigit"))
		{
			if (decoderParameters.bReportCheckDigit)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Matrix2of5VerifyCheckDigit"))
		{
			if (decoderParameters.bVerifyCheckDigit)
				iRetVal = ProcessDecoderPropertyForReturn(L"true", szPropertyValue);
			else
				iRetVal = ProcessDecoderPropertyForReturn(L"false", szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Matrix2of5Minlength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, true), szPropertyValue);
		}
		else if (cmp(szPropertyName, L"Matrix2of5Maxlength"))
		{
			iRetVal = ProcessDecoderPropertyForReturn(
				GetDecoderLength(cCurrentDecoder, false), szPropertyValue);
		}
	}
	return iRetVal;
}
	
int CScanner::Get_Us4StateFics_PropertyValue (LPCTSTR szPropertyName, WCHAR* szPropertyValue)
{
	return GetSimpleDecoder((BYTE)*DECODER_US4STATE_FICS, szPropertyName, szPropertyValue);
}
	
