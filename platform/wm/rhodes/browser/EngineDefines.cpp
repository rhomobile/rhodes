#include <stdafx.h>

#include "common/RhodesApp.h"
#include "EngineDefines.h"

//////////////////////////////////////////////////////////////////////////

#define HTMLVIEWDLL L"HTMLVIEW.DLL"

//////////////////////////////////////////////////////////////////////////

typedef BOOL (*lpfnInitHTMLControl)(HINSTANCE hinst);

//////////////////////////////////////////////////////////////////////////

namespace rho
{
namespace browser
{

BOOL IsRelativeURL(LPCTSTR tcURL)
{
    //  The URL is relative if it starts with a '.'
    if (tcURL && wcsnicmp(tcURL, L".", 1) == 0)
        return TRUE;
    else
        return FALSE;
}

int GetProtocolFromURL(LPCTSTR tcURL, LPTSTR lpRetStr)
{
    TCHAR* ptcStart = wcsstr(tcURL, L"://");
    if (ptcStart == NULL)
        return 0;  //  Protocol not specified

    ptcStart += 3;

    wcsncpy(lpRetStr, tcURL, (ptcStart - tcURL));
    lpRetStr[(ptcStart - tcURL)-3] = NULL;

    return (ptcStart - tcURL) - 3; 
}

BOOL DereferenceURL(LPCTSTR tcRelativeURLConst, TCHAR* tcDereferencedURL, TCHAR* tcCurrentURL)
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

        if (iLevelsUp != -1)
        {
            return FALSE;
        }

        int iFirstNonRelativeCharacter = wcsspn(tcRelativeURL, L"./\\");
        if (iFirstNonRelativeCharacter == wcslen(tcRelativeURL))
        {
            return FALSE;
        }

        TCHAR* pSzRelativeURLFilePathAndName = tcRelativeURL;
        pSzRelativeURLFilePathAndName += iFirstNonRelativeCharacter;

        //  Test the new URL is not too long
        if ((pSzCurrentURL - tcCurrentURL + 1) + 
            wcslen(pSzRelativeURLFilePathAndName) > MAX_URL)
        {
            return FALSE;
        }

        wcsncpy(tcDereferencedURL, tcCurrentURL, pSzCurrentURL - tcCurrentURL + 1);
        wcscat(tcDereferencedURL, pSzRelativeURLFilePathAndName);
        retVal = TRUE;
    }
    return retVal;
}

HRESULT InitHTMLControl(HINSTANCE hInstance)
{
    HMODULE htmlModule = LoadLibrary(HTMLVIEWDLL);

    if (htmlModule == NULL)
        return S_FALSE;

    lpfnInitHTMLControl InitHTMLControl = (lpfnInitHTMLControl)GetProcAddressA(htmlModule, "InitHTMLControl");

    //initialize the HTML Control
    if(!InitHTMLControl(hInstance))
        return S_FALSE;

    return S_OK;
}

void MetaHandler(int tabIndex, EngineMETATag* metaTag)
{
    RHODESAPP().getExtManager().onSetPropertiesData((LPCWSTR)tabIndex, (LPCWSTR)metaTag);
    
    if (wcscmp(metaTag->tcHTTPEquiv, L"initialiseRhoElementsExtension") != 0)
    {
        free(metaTag->tcHTTPEquiv);
        free(metaTag->tcContents);
        delete metaTag;
    }
}

} //end of browser
} //end of rho