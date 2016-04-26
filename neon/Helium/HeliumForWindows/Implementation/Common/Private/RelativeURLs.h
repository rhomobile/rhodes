#pragma once

#ifndef __relativeURLs__
#define __relativeURLs__

#include <windows.h>
#include "..\Public\PB_Defines.h"
#include "..\Public\PBPlugin.h"


/**
*  Calculates whether the passed URL is a relative URL or not.  Relative
*  URLs will look like ./ or ../../
*  \param tcURL The URL to compare
*  \return Whether or not the passed URL was relative.
*/
BOOL IsRelativeURL(LPCTSTR tcURL);

/**
*  Resolves relative URL (or File) references.
*  \param tcRelativeURL The relative URL (e.g. ../../) used to calculate 
*  the offset from the actual location of the page, stored in m_szCurrentURL
*  \param tcDereferencedURL [out] Result of applying the tcRelativeURL to 
*  the current page URL.
*  \param tcCurrentURL [in] The current URL of the loaded page.
*  \param pModule [in] Owner plugin module calling this function (for logging),
*         if there is not associated plugin then set this parameter to NULL.
*  \return Whether or not the relative URL processing was successful.
*/
BOOL DereferenceURL(LPCTSTR tcRelativeURL, TCHAR* tcDereferencedURL, 
					TCHAR* tcCurrentURL, PBModule* pModule);

/**
*  Retrieve the Protocol from the URL (File, FTP or HTTP)
*  \param tcURL URL from which to extract the protocol
*  \param lpRetStr [out] Populated by this function this will be the protocol
*  \return -1 Error parsing protocol
*  \return 0 No protocol has been specified
*  \return Length of protocol
*/
int GetProtocolFromURL(LPCTSTR tcURL, LPTSTR lpRetStr);

#endif