#include "RelativeURLs.h"


/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date	February 2010 - Initial Creation, DCC
* \date	June 2010	  - Moving to a common location for File Transfer and Engines
*                       to both use, DCC
*/
BOOL IsRelativeURL(LPCTSTR tcURL)
{
	//  The URL is relative if it starts with a '.'
	if (tcURL && wcsnicmp(tcURL, L".", 1) == 0)
		return TRUE;
	else
		return FALSE;
}

/**
* \author	Darryn Campbell (DCC, JRQ768)
* \date		February 2010 - Initial Creation, DCC
* \date		June 2010	  - Moving to a common location for File Transfer and Engines
*                           to both use, DCC
*/
BOOL DereferenceURL(LPCTSTR tcRelativeURLConst, TCHAR* tcDereferencedURL, 
					TCHAR* tcCurrentURL, PBModule* pModule)
{
	BOOL retVal = FALSE;
	TCHAR tcRelativeURL[MAX_URL];
	wcscpy(tcRelativeURL, tcRelativeURLConst);
	if (tcRelativeURL)
	{
		//  First work out how many levels we need to navigate up from the 
		//  current URL (specified as ../)
		int iLevelsUp = 0;
		wchar_t* temp = wcsstr(tcRelativeURL, L"..");
		while (temp != NULL && (wcslen(temp) >= 2))
		{
			iLevelsUp++;
			temp = wcsstr(temp + 2, L"..");
		}
			
		//  We now know how many levels up we want to go from the current URL.
		//  Starting at the end of the current URL search for '/' or '\' and 
		//  work out if we can go up that many levels.
		TCHAR* pSzCurrentURL = tcCurrentURL + wcslen(tcCurrentURL) - 1;
		//  We do not want to include the protocol slashs in our search
		TCHAR tempProtocol[10];
		memset(tempProtocol, 0, 10 * sizeof(TCHAR));
		int iLengthOfProtocol = GetProtocolFromURL(tcCurrentURL, tempProtocol) + 3;
		while (pSzCurrentURL != tcCurrentURL + iLengthOfProtocol - 1)
		{
			if (*pSzCurrentURL == L'/' || *pSzCurrentURL == L'\\')
			{
				iLevelsUp--;
				if (iLevelsUp == -1)
				{
					//  pSzCurrentURL is pointing to the end of the URL 
					//  we want to use as our base
					break;
				}
			}

			pSzCurrentURL--;
		}

		//  If we exit the while loop and 'levelsUp' is not -1 then there were 
		//  insufficient backslashes in the current URL to go up the number
		//  of levels specified in the relative URL.
		if (iLevelsUp != -1)
		{
			if (pModule)
			{
#ifndef PB_ENGINE_IE_CE
#ifndef PB_ENGINE_IE_MOBILE
				pModule->Log(PB_LOG_ERROR, L"Unable to work out relative URL", 
				_T(__FUNCTION__), __LINE__);
#endif
#endif
			}
			return FALSE;
		}

		//  The actual URL we require is the juxtaposition of m_szCurrentURL
		//  up to the pSzCurrentURL with the first non . | / | \ charcter in 
		//  the relative URL
		int iFirstNonRelativeCharacter = wcsspn(tcRelativeURL, L"./\\");
		if (iFirstNonRelativeCharacter == wcslen(tcRelativeURL))
		{
			//  User specified relative string without filename, e.g. .././../
			if (pModule)
			{
#ifndef PB_ENGINE_IE_CE
#ifndef PB_ENGINE_IE_MOBILE
				pModule->Log(PB_LOG_ERROR, L"Unable to work out relative URL Filename", 
					_T(__FUNCTION__), __LINE__);
#endif
#endif
			}
			return FALSE;
		}
		TCHAR* pSzRelativeURLFilePathAndName = tcRelativeURL;
		pSzRelativeURLFilePathAndName += iFirstNonRelativeCharacter;

		//  Test the new URL is not too long
		if ((pSzCurrentURL - tcCurrentURL + 1) + 
			wcslen(pSzRelativeURLFilePathAndName) > MAX_URL)
		{
			if (pModule)
			{
#ifndef PB_ENGINE_IE_CE
#ifndef PB_ENGINE_IE_MOBILE
				pModule->Log(PB_LOG_ERROR, L"Relative URL produces URL string longer than maximum allowed", 
					_T(__FUNCTION__), __LINE__);
#endif
#endif
			}
			return FALSE;
		}

		wcsncpy(tcDereferencedURL, tcCurrentURL, pSzCurrentURL - tcCurrentURL + 1);
		wcscat(tcDereferencedURL, pSzRelativeURLFilePathAndName);
		retVal = TRUE;
	}
	return retVal;
}


/**
* \author	James Morley-Smith	(JMS, JNP837)
* \author	Darryn Campbell		(DCC, JRQ768)
* \date		December 2009 - Initial Creation, JMS
* \date		December 2009 - Bug Fixes, DCC
* \date		June 2010	  - Moving to a common location for File Transfer and Engines
*                           to both use, DCC
*/
int GetProtocolFromURL(LPCTSTR tcURL, LPTSTR lpRetStr)
{

	//search for a ":" or "@" between the "://" and the next "/"

	//     |                               |
	//     v        v        v             v
	//ftp://username:password@hostname:port/
	TCHAR* ptcStart = wcsstr(tcURL, L"://");
	if (ptcStart == NULL)
		return 0;  //  Protocol not specified

	ptcStart += 3;

	wcsncpy(lpRetStr, tcURL, (ptcStart - tcURL));
	lpRetStr[(ptcStart - tcURL)-3] = NULL;
		
	return (ptcStart - tcURL) - 3; 
	
}