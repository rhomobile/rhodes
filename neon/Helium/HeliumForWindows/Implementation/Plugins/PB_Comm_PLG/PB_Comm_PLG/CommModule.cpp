//////////////////////////////////////////////////////////////////////////////
// Authors:		Phil May (MAY)
//
// Change History:
//		July 2009 - MAY First Created
//		Sept 2009 - MAY Multi-instance support
//		Nov  2009 - MAY Added 'open' tag and 'autoenter' & 'autotab'
//////////////////////////////////////////////////////////////////////////////

#include "CommModule.h"
#include "Comm.h"

#define LOG_MESSAGE_LENGTH	200
TCHAR tcModuleErrorMessage[LOG_MESSAGE_LENGTH];

/*****************************************************************************/
BOOL CCommModule::onInit(PBStruct *pPBStructure)
{
	_tcscpy(m_szModName, L"Comm");
	return RegisterForEvent(PB_BROWSER_BEFORE_NAV_EVENT);
}

/*****************************************************************************/
BOOL CCommModule::MetaProc(PBMetaStruct *pPBMetaStructure, PBStruct *pPBStructure, VOID *pParam)
{
	CComm *pComm = (CComm *)pParam;

	LPCTSTR lpParam = pPBMetaStructure->lpParameter;
	LPCTSTR lpValue = pPBMetaStructure->lpValue;

	// test first for NULL lpParam and return FALSE if NULL
	if (lpParam == NULL)
	{
		Log(PB_LOG_WARNING, 
			L"Error occurred -- NULL lpParameter string passed to Comm module", 
			L"CCommModule::MetaProc(PBMetaStruct *, PBStruct *)", 
			__LINE__);		
		return FALSE;
	}

	// function/meta-tags to set basic parameters of serial port
	// calls functions in the current instance of the CComm class
	if      (cmp(lpParam, L"port"))
		if (lpValue)
			return pComm->SetPort(lpValue);
		else
			goto _logNullValue;

	else if (cmp(lpParam, L"baudrate"))
		if (lpValue)
			return pComm->SetBaudRate(lpValue);
		else
			goto _logNullValue;

	else if (cmp(lpParam, L"databits"))
		if (lpValue)
			return pComm->SetDataBits(lpValue);
		else
			goto _logNullValue;

	else if (cmp(lpParam, L"stopbits"))
		if (lpValue)
			return pComm->SetStopBits(lpValue);
		else
			goto _logNullValue;

	else if (cmp(lpParam, L"parity"))
		if (lpValue)
			return pComm->SetParity(lpValue);
		else
			goto _logNullValue;

	else if (cmp(lpParam, L"handshake"))
		if (lpValue)
			return pComm->SetHandshake(lpValue);
		else
			goto _logNullValue;

	else if (cmp(lpParam, L"chars"))
	{
		if (lpValue)
		{
			pComm->ResetTempCommInputOn();
			return pComm->SetCharCount(_tcstol(lpValue, NULL, 10));
		}
		else
			goto _logNullValue;
	}

	else if (cmp(lpParam, L"endchar"))
		if (lpValue)
		{
			pComm->ResetTempCommInputOn();

			// if the value is "crlf", then set CRLF trigger, else pass the
			// decimal value on to the SetEndChar method.
			if (cmp(lpValue, L"crlf"))
				return pComm->SetCRLF(TRUE);
			else
				return pComm->SetEndChar((char)_tcstol(lpValue, NULL, 10));
		}
		else
			goto _logNullValue;

	else if (cmp(lpParam, L"time"))
		if (lpValue)
		{
			pComm->ResetTempCommInputOn();
			return pComm->SetTimeout(_tcstol(lpValue, NULL, 10));
		}
		else
			goto _logNullValue;

	// specify JavaScript callback or URL to fetch when the condition
	//    specified above is triggered
	else if (cmp(lpParam, L"commevent"))
		if (lpValue)
			return pComm->SetNavigateUrl(lpValue);
		else
			goto _logNullValue;

	else if (cmp(lpParam, L"open"))
	{
		if (lpValue)
		{
			// kept the old "close" as the value of comminputon for 
			//   compatibility with 2.2
			if (cmp(lpValue, L"close"))
			{
				pComm->Close();
				return TRUE;
			}
			// if the value of this tag is not "close", check for "" and then set the
			//   navigate URL before opening the port.
			else if (lpValue[0] != NULL)
				pComm->SetNavigateUrl(lpValue);
		}
		// If the port is already
		//   open, the Open() method should return w/o doing anything			
		return pComm->Open();
	}

	// write an ASCII string.  The lpValue parameter is the string to write
	else if (cmp(lpParam, L"writestring"))
		if (lpValue)
			return pComm->PortWriteString(lpValue);
		else
			goto _logNullValue;

	// write a list of %hh formated bytes sent in the lpValue string
	else if (cmp(lpParam, L"writebytes"))
		if (lpValue)
			return pComm->PortWriteBytes(lpValue);
		else
			goto _logNullValue;

	// write the contents of a file, the path of which is specified in the
	//    lpValue parameter
	else if (cmp(lpParam, L"writefile"))
		if (lpValue)
			return pComm->PortWriteFile(lpValue);
		else
			goto _logNullValue;

	// provide an alternate to including the value="close" in the commevent tag
	else if (cmp(lpParam, L"close"))
	{
		pComm->Close();
		return TRUE;
	}

	//  Consider whether or not to append a CR or tab
	else if (cmp(lpParam, L"AutoEnter"))
	{
		if (lpValue)
		{
			if (cmp(lpValue, L"Enabled") || cmp(lpValue, L"Enable"))	//  Regex changes for 'Enabled'
				return pComm->SetAutoEnter(TRUE);
			else if (cmp(lpValue, L"Disabled") || cmp(lpValue, L"Disable"))	//  Regex changes for 'Disabled'
				return pComm->SetAutoEnter(FALSE);
		}
		else
			goto _logNullValue;
	}
	else if (cmp(lpParam, L"AutoTab"))
	{
		if (lpValue)
		{
			if (cmp(lpValue, L"Enabled") || cmp(lpValue, L"Enable"))	//  Regex changes for 'Enabled'
				return pComm->SetAutoTab(TRUE);
			else if (cmp(lpValue, L"Disabled") || cmp(lpValue, L"Disable"))	//  Regex changes for 'Disabled'
				return pComm->SetAutoTab(FALSE);
		}
		else
			goto _logNullValue;
	}

	else
	{
		wsprintf(tcModuleErrorMessage, 
		         L"Error occurred -- Unrecognized parameter name (%s) passed to Comm module",
		         lpParam);
		Log(PB_LOG_WARNING, 
			tcModuleErrorMessage, 
			L"CCommModule::MetaProc(PBMetaStruct *, PBStruct *)", 
			__LINE__);		
		return FALSE;	// unrecognized HTTP-Equiv string
	}
	return FALSE;

// log the fact that a NULL value was passed, but only if it is being
// used (control will only pass here when there is an attempt to use
// lpValue and it's NULL
_logNullValue:
	{
		Log(PB_LOG_WARNING, 
			L"Error occurred -- NULL lpValue string passed to Comm module", 
			L"CCommModule::MetaProc(PBMetaStruct *, PBStruct *)", 
			__LINE__);		
		return FALSE;
	}
}

/*****************************************************************************/
BOOL CCommModule::onAttachInstance(PBStruct *pPBStructure, PBInstStruct *pPBInstStructure)
{
	pPBInstStructure->pWrappedObj = new CComm(pPBStructure, this);
	if(pPBInstStructure->pWrappedObj){
		return TRUE;
	}
	return FALSE;
}

/*****************************************************************************/
BOOL CCommModule::onReleaseInstance(PBStruct *pPBStructure, PBInstStruct *pPBInstStructure)
{
	CComm *pComm = (CComm *)(pPBInstStructure->pWrappedObj);
	delete pComm;
	pPBInstStructure = NULL;
	return TRUE;
}

/*****************************************************************************/
BOOL CCommModule::onBeforeNavigate(int iInstID)
{
	CComm *pComm = (CComm *)(GetObjFromID(iInstID));

	// call Close if the InstID passed matches one in the list
	if (pComm != NULL)
	{
		pComm->SetNavigateUrl(L"");
		pComm->Close();
	}
	return TRUE;
}

/*****************************************************************************/
BOOL CCommModule::LogInst(LogTypeInterface logSeverity, LPCTSTR pLogComment, 
						  LPCTSTR pFunctionName, int iTabID, DWORD dwErrorCode, 
						  DWORD dwLineNumber)
{
	// storage for log message strings
	TCHAR tcLogMessage[LOG_MESSAGE_LENGTH];

	if (dwErrorCode)
		wsprintf(tcLogMessage, 
				 L"%s -- Error code: %d -- Instance ID: %d",
				 pLogComment,
				 dwErrorCode,
				 iTabID);
	else
		wsprintf(tcLogMessage, 
			     L"%s -- Instance ID: %d",
				 pLogComment, 
				 iTabID);

	return Log(logSeverity, tcLogMessage, pFunctionName, dwLineNumber);
}

/*****************************************************************************/
BOOL CCommModule::SendPBNavigateInst(PBStruct *pPBStructure, TCHAR *tcNavigateUrl, TCHAR *tcNavigateData)
{
	VARSTRUCT	navigateData;

	navigateData.pNextVar = NULL;
	navigateData.pStr = tcNavigateData;

	return m_pCoreStructure->pNavigateFunc(tcCommEventNames, pPBStructure->iTabID, tcNavigateUrl, &navigateData, L"Comm");
}

/*****************************************************************************/
PBCoreStruct * CCommModule::GetCoreStructure()
{
	return m_pCoreStructure;
}

/*****************************************************************************/
CCommModule::CCommModule(void)
{
}

/*****************************************************************************/
CCommModule::~CCommModule(void)
{
}
