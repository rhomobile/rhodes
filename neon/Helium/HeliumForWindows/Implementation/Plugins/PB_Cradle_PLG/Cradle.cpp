#include "Cradle.h"
#include "CradleModule.h"
#include "RcmCAPI.h"

static TCHAR* tcCradleEventNames[] = { L"cradleId" };
typedef DWORD (WINAPI* LPFN_GET_CRADLEID_T)(LPDWORD);	

/**
* \author	John Heywood (JHMH, VBC863)
* \date		June 2012 Initial Creation, JHMH
*/
CCradle::CCradle(int iInstanceID, CCradleModule *pModule)
{
	m_pModule = pModule;
	m_iInstanceID = iInstanceID;
	m_pCradleURI = NULL;
}


/**
* \author	John Heywood (JHMH, VBC863)
* \date		June 2012 Initial Creation, JHMH
*/
CCradle::~CCradle()
{
	ClearCradleURI();
}


/**
* \author	John Heywood (JHMH, VBC863)
* \date		June 2012 Initial Creation, JHMH
*/
BOOL CCradle::GetCradleID()
{
	m_pModule->Log(PB_LOG_INFO, L"GetCradleId is no longer supported.",_T(__FUNCTION__),__LINE__);	
	return FALSE;
}

/**
* \author	John Heywood (JHMH, VBC863)
* \date		June 2012 Initial Creation, JHMH
*/
BOOL CCradle::SetCradleURI(LPCTSTR pszURI)
{
	delete [] m_pCradleURI;
	m_pCradleURI = NULL;
	int iBufLen = _tcslen(pszURI)+1;

	if (pszURI && (iBufLen <= MAX_URL))
	{
		m_pCradleURI = new TCHAR[iBufLen];
		if (m_pCradleURI)
		{
			_tcscpy(m_pCradleURI, pszURI);
			return TRUE;
		}
	}
	return FALSE;
}

/**
* \author	John Heywood (JHMH, VBC863)
* \date		June 2012 Initial Creation, JHMH
*/
BOOL CCradle::ClearCradleURI()
{
	if (m_pCradleURI)
	{
		delete [] m_pCradleURI;
		m_pCradleURI = NULL;
		return TRUE;
	}
	return FALSE;
}
