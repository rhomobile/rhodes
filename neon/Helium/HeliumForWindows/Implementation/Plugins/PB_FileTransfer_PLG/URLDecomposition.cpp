#include "URLDecomposition.h"

/**
* \author	James Morley-Smith	(JMS, JNP837)
* \author	Darryn Campbell		(DCC, JRQ768)
* \date		December 2009 - Initial Creation, JMS
* \date		December 2009 - Bug Fixes, DCC
*/
int GetPortFromURL(LPCTSTR tcURL, LPTSTR lpRetStr)
{
	//search for a ":" before the last "/" and after the "://" if there is a "@" 
	//we must stop there as there was no port
	//and the next ":" will be the end of the username

	//ps, we are searching backwards

	//                      |              
	//                      v             
	//ftp://username:password@hostname:port/

	TCHAR* ptcStart = wcsstr(tcURL, L"://");
	if(ptcStart == NULL)
		return -1; //invalid URL
		
	ptcStart += 3;

	//now find the next "/"
	TCHAR* ptcEnd = wcsstr(ptcStart, L"/");
	if (!ptcEnd)
		return -1;

	ptcStart = ptcEnd-1;

	if(!ptcStart)
		return -1;

	while(*ptcStart != L'/' && *ptcStart != L'@')
	{
		if(*ptcStart == L':')
		{
			//copy the password to the return string
			wcsncpy(lpRetStr, ++ptcStart, (ptcEnd - ptcStart));
			lpRetStr[(ptcEnd - ptcStart)] = NULL;

			//return past the password
			return ((ptcEnd - ptcStart)); //return length of password
		}

		ptcStart--;

	}

	return 0; //no port
	
}

/**
* \author	James Morley-Smith	(JMS, JNP837)
* \author	Darryn Campbell		(DCC, JRQ768)
* \date		December 2009 - Initial Creation, JMS
* \date		December 2009 - Bug Fixes, DCC
*/
int GetServerFromURL(LPCTSTR tcURL, LPTSTR lpRetStr)
{
	//search for a ":" before the last "/" and after the "://" if there is a "@" 
	//we must stop there as there was no port
	//and the next ":" will be the end of the username

	//ps, we are searching backwards

	//                      |              
	//                      v             
	//ftp://username:password@hostname:port/some/path/here/page.html

	TCHAR* ptcStart = wcsstr(tcURL, L"://");
	if(ptcStart == NULL)
		return -1; //invalid URL
		
	ptcStart += 3;

	//now find the next "/" if there is one
	TCHAR* ptcEnd = wcsstr(ptcStart, L"/");
	if (!ptcEnd)
	{
		//  There is no terminating '/', return the whole thing
		wcsncpy(lpRetStr, ptcStart, wcslen(ptcStart));
		lpRetStr[wcslen(ptcStart)] = NULL;
		return wcslen(ptcStart);
	}

	ptcEnd = ptcEnd - 1;
	ptcStart = ptcEnd - 1;

	if(!ptcStart)
		return -1;

	while(*ptcStart != L'/' && *ptcStart != L'@')
	{
		if(*ptcStart == L':')
		{
			ptcEnd = ptcStart-1;
		}

		ptcStart--;

	}

	wcsncpy(lpRetStr, ptcStart + 1, ptcEnd - ptcStart);
	lpRetStr[ptcEnd - ptcStart] = NULL;

	return ptcEnd - ptcStart;
	
}

/**
* \author	James Morley-Smith	(JMS, JNP837)
* \author	Darryn Campbell		(DCC, JRQ768)
* \date		December 2009 - Initial Creation, JMS
* \date		December 2009 - Bug Fixes, DCC
*/
int GetFileAndPathFromURL(LPCTSTR tcURL, LPTSTR lpRetStr)
{
	//search for the next "/" and after the "://" 

	//     |                               |              
	//     v                               v
	//ftp://username:password@hostname:port/some/path/here/page.html

	TCHAR* ptcStart = wcsstr(tcURL, L"://");
	if(ptcStart == NULL)
		return -1; //invalid URL
		
	ptcStart += 3;

	//now find the next "/" if there is one
	ptcStart = wcsstr(ptcStart, L"/");
	if (!ptcStart)
		return 0;

	ptcStart += 1;

	if(!ptcStart)
		return -1;
	
	wcscpy(lpRetStr, ptcStart);
	return wcslen(ptcStart);
	
}

/**
* \author	James Morley-Smith	(JMS, JNP837)
* \author	Darryn Campbell		(DCC, JRQ768)
* \date		December 2009 - Initial Creation, JMS
* \date		December 2009 - Bug Fixes, DCC
*/
int GetUsernameFromURL(LPCTSTR tcURL, LPTSTR lpRetStr)
{

	//search for a ":" or "@" between the "://" and the next "/"

	//     |                               |
	//     v        v        v             v
	//ftp://username:password@hostname:port/

	TCHAR* ptcStart = wcsstr(tcURL, L"://");
	if(ptcStart == NULL)
		return -1; //invalid URL
		
	ptcStart += 3;

	TCHAR* ptcEnd = wcsstr(ptcStart, L"/");
	TCHAR* ptcAt = wcsstr(ptcStart, L"@");
	
	//check that there is an "@" before the path
	//or it's not a username and probably a port
	if(!ptcAt || ptcAt > ptcEnd)
		return -1;

	if(!ptcStart)
		return -1;

	ptcEnd = ptcStart;

	while(*ptcEnd != L'/')
	{
		if(*ptcEnd == L':' || *ptcEnd == L'@')
		{

			//copy the username to the return string
			wcsncpy(lpRetStr, ptcStart, (ptcEnd - ptcStart));
			lpRetStr[(ptcEnd - ptcStart)] = NULL;

			//return past the username
			return ((ptcEnd - ptcStart)); //return length of username
		}

		ptcEnd++;

	}
	
	return 0; //no username
	
}

/**
* \author	James Morley-Smith	(JMS, JNP837)
* \author	Darryn Campbell		(DCC, JRQ768)
* \date		December 2009 - Initial Creation, JMS
* \date		December 2009 - Bug Fixes, DCC
*/
int GetPasswordFromURL(LPCTSTR tcURL, LPTSTR lpRetStr)
{
	//search for an "@" between the ":" after the "://" if there is no "@" before the "/" there is no password

	//              |        |              
	//              v        v             
	//ftp://username:password@hostname:port/

	TCHAR* ptcStart = wcsstr(tcURL, L"://");
	if(ptcStart == NULL)
		return -1; //invalid URL
		
	ptcStart += 3;

	while(*ptcStart != L'/')
	{
		if(*ptcStart == L':')
		{

			break;
		}

		ptcStart++;

	}

	if((ptcStart > (tcURL + wcslen(tcURL))) || *ptcStart == L'/')
		return 0; //no password

	TCHAR* ptcEnd = ++ptcStart;
	
	while(*ptcEnd != L'/')
	{
		if(*ptcEnd == L'@')
		{

			//copy the password to the return string
			wcsncpy(lpRetStr, ptcStart, (ptcEnd - ptcStart));
			lpRetStr[(ptcEnd - ptcStart)] = NULL;

			//return past the password
			return ((ptcEnd - ptcStart)); //return length of password
		}

		//  No @ symbol found, the ':' we found was the port delimiter, 
		//  hence no password
		if (ptcEnd > (tcURL + wcslen(tcURL)))
			return 0;

		ptcEnd++;

	}
	
	return 0; //no password
	
}