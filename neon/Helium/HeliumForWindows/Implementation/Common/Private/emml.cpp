#include "emml.h"
#include "..\..\RegExParser\regularexpressions.h"

#include "..\public\PB_Defines.h"

#ifndef PB_EMMP_PLG_EXPORTS
extern BOOL		Log		(LogTypeInterface logSeverity,LPCTSTR pLogComment, 
						LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule);
#endif

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CNameValuePair::CNameValuePair(void)
	:m_pnvpNextEl(NULL)
{
	m_pbNVP.szModule = NULL;
	m_pbNVP.szName = NULL;
	m_pbNVP.szValue = NULL;
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CNameValuePair::~CNameValuePair(void)
{
	delete m_pnvpNextEl;	
	m_pnvpNextEl = NULL;

	delete [] m_pbNVP.szValue;	
	m_pbNVP.szValue = NULL;

	delete [] m_pbNVP.szName;		
	m_pbNVP.szName = NULL;

	delete [] m_pbNVP.szModule;		
	m_pbNVP.szModule = NULL;
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
void CEMML::CNameValuePair::AddModule(LPCWSTR sModule, int iLen)
{
	//just check it's not already been assigned
	delete [] m_pbNVP.szModule;
	m_pbNVP.szModule = NULL;
	
	int iStart = 0;
	//check for leading spaces
	while(SkipChar(*(sModule+iStart)))
	{
		++iStart;
		--iLen;
	}

	//remove trailing invalid chars
	while(iLen >= 0 && SkipChar(*(sModule+iStart+(iLen-1))))
		iLen--;

	if(iLen < 0)
		return;
	

	//allocate and assign
	m_pbNVP.szModule = new wchar_t[iLen+1];

	//copy data char by char as we want to force to lower case
	for(int i=0; i<iLen; i++)
	{
		if(*(sModule+iStart+i) >= L'A' && *(sModule+iStart+i) <= 'Z')
			*(m_pbNVP.szModule+i) = *(sModule+iStart+i)|L' ';
		else
			*(m_pbNVP.szModule+i) = *(sModule+iStart+i);
	}

	m_pbNVP.szModule[iLen] = L'\0';

}

////////////////////////////////////////////////////////////////////////
// Authors:			Abhineet Agarwal (AA)
// Change History:	
//		May 2015 - AA First Created
////////////////////////////////////////////////////////////////////////
void CEMML::CNameValuePair::AddDefaultMetaTagName(LPCWSTR sName, int iLen)
{
	//just check it's not already been assigned
	delete [] m_pbNVP.szName;
	m_pbNVP.szName = NULL;

	int iStart = 0;
	//check for leading spaces
	while(SkipChar(*(sName+iStart)))
	{
		++iStart;
		--iLen;
	}
	//remove trailing invalid chars
	while(iLen >= 0 && SkipChar(*(sName+iStart+(iLen-1))))
		iLen--;

	if(iLen < 0)
		return;
	

	//check to see if it's a url
	if(_memicmp(sName+iStart, L"url", 6) == 0)
	{
		//ok, it starts with "url" so look for the opening bracket and then quote

		//make a copy of the length and start in case it's not a proper url
		//move the start to the end of the "url" and reduce the length
		int iTmpStart = iStart + 3;
		int iTmpLen = iLen - 3;
		
		while(iTmpLen > 0) //looking for open bracket
		{
			if(*(sName+iTmpStart) == L'(')
				break;

			iTmpLen--; 
			iTmpStart++;
		}

		iTmpLen--; 
		iTmpStart++;

		while(iTmpLen > 0) //looking for open quote
		{
			if(*(sName+iTmpStart) == L'\'')
				break;

			iTmpLen--; 
			iTmpStart++;
		}

		iTmpLen--; 
		iTmpStart++;

		while(iTmpLen > 0) //looking for close bracket
		{
			if(*(sName+iTmpStart+iTmpLen) == L')')
				break;

			iTmpLen--; 
		}

		iTmpLen--; 
		
		while(iTmpLen > 0) //looking for close quote
		{
			if(*(sName+iTmpStart+iTmpLen) == L'\'')
				break;

			iTmpLen--; 
		}

		if(iTmpLen > 0)
		{
			iStart = iTmpStart;
			iLen = iTmpLen;

		}
	}

	//allocate and assign
	m_pbNVP.szName = new wchar_t[iLen+1];
	//copy data char by char and dont want to force to lower case in case of default meta tags.
	for(int i=0; i<iLen; i++)
	{
		*(m_pbNVP.szName+i) = *(sName+iStart+i);
	}

	
	m_pbNVP.szName[iLen] = L'\0';

}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
void CEMML::CNameValuePair::AddName(LPCWSTR sName, int iLen)
{
	//just check it's not already been assigned
	delete [] m_pbNVP.szName;
	m_pbNVP.szName = NULL;

	int iStart = 0;
	//check for leading spaces
	while(SkipChar(*(sName+iStart)))
	{
		++iStart;
		--iLen;
	}
	//remove trailing invalid chars
	while(iLen >= 0 && SkipChar(*(sName+iStart+(iLen-1))))
		iLen--;

	if(iLen < 0)
		return;
	

	//check to see if it's a url
	if(_memicmp(sName+iStart, L"url", 6) == 0)
	{
		//ok, it starts with "url" so look for the opening bracket and then quote

		//make a copy of the length and start in case it's not a proper url
		//move the start to the end of the "url" and reduce the length
		int iTmpStart = iStart + 3;
		int iTmpLen = iLen - 3;
		
		while(iTmpLen > 0) //looking for open bracket
		{
			if(*(sName+iTmpStart) == L'(')
				break;

			iTmpLen--; 
			iTmpStart++;
		}

		iTmpLen--; 
		iTmpStart++;

		while(iTmpLen > 0) //looking for open quote
		{
			if(*(sName+iTmpStart) == L'\'')
				break;

			iTmpLen--; 
			iTmpStart++;
		}

		iTmpLen--; 
		iTmpStart++;

		while(iTmpLen > 0) //looking for close bracket
		{
			if(*(sName+iTmpStart+iTmpLen) == L')')
				break;

			iTmpLen--; 
		}

		iTmpLen--; 
		
		while(iTmpLen > 0) //looking for close quote
		{
			if(*(sName+iTmpStart+iTmpLen) == L'\'')
				break;

			iTmpLen--; 
		}

		if(iTmpLen > 0)
		{
			iStart = iTmpStart;
			iLen = iTmpLen;

		}
	}

	//allocate and assign
	m_pbNVP.szName = new wchar_t[iLen+1];
	//copy data char by char as we want to force to lower case
	for(int i=0; i<iLen; i++)
	{
		if(*(sName+iStart+i) >= L'A' && *(sName+iStart+i) <= 'Z')
			*(m_pbNVP.szName+i) = *(sName+iStart+i)|L' ';
		else
			*(m_pbNVP.szName+i) = *(sName+iStart+i);
	}

	
	m_pbNVP.szName[iLen] = L'\0';

}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
void CEMML::CNameValuePair::AddValue(LPCWSTR sValue, int iLen)
{

	
	//just check it's not already been assigned
	delete [] m_pbNVP.szValue;
	m_pbNVP.szValue = NULL;

	int iStart = 0;
	//check for leading spaces
	while(SkipChar(*(sValue+iStart)))
	{
		++iStart;
		--iLen;
	}

	//remove trailing invalid chars
	while(iLen >= 0 && SkipChar(*(sValue+iStart+(iLen-1))))
		iLen--;

	if(iLen < 0)
		return;
	

	//check to see if it's a url
	if(_memicmp(sValue+iStart, L"url", 6) == 0)
	{
		//ok, it starts with "url" so look for the opening bracket and then quote

		//make a copy of the length and start in case it's not a proper url
		//move the start to the end of the "url" and reduce the length
		int iTmpStart = iStart + 3;
		int iTmpLen = iLen - 3;
		
		while(iTmpLen > 0) //looking for open bracket
		{
			if(*(sValue+iTmpStart) == L'(')
				break;

			iTmpLen--; 
			iTmpStart++;
		}

		iTmpLen--; 
		iTmpStart++;

		while(iTmpLen > 0) //looking for open quote
		{
			if(*(sValue+iTmpStart) == L'\'')
				break;

			iTmpLen--; 
			iTmpStart++;
		}

		iTmpLen--; 
		iTmpStart++;

		while(iTmpLen > 0) //looking for close bracket
		{
			if(*(sValue+iTmpStart+iTmpLen) == L')')
				break;

			iTmpLen--; 
		}

		iTmpLen--; 
		
		while(iTmpLen > 0) //looking for close quote
		{
			if(*(sValue+iTmpStart+iTmpLen) == L'\'')
				break;

			iTmpLen--; 
		}

		if(iTmpLen > 0)
		{
			iStart = iTmpStart;
			iLen = iTmpLen;

		}
	}


	//allocate and assign
	m_pbNVP.szValue = new wchar_t[iLen+1];
	
	//Value must not be forced to lowercase
	for(int i=0; i<iLen; i++)
		*(m_pbNVP.szValue+i) = *(sValue+iStart+i);

	m_pbNVP.szValue[iLen] = L'\0';

}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
LPPBNAMEVALUE CEMML::CNameValuePair::MakeMethod()
{
	if(m_pbNVP.szName == NULL || m_pbNVP.szName[0] == NULL)
	{
		//it's a method so swap round the name and value
		if(m_pbNVP.szName) //must delete the pointer if there was an empty string created of there will be a leak
			delete [] m_pbNVP.szName;

		//now swap them over
		m_pbNVP.szName = m_pbNVP.szValue;
		m_pbNVP.szValue = NULL;
	}

	if (m_pbNVP.szValue)
	{
		if(wcscmp(m_pbNVP.szValue, L"url('')") == 0)
		{
			m_pbNVP.szValue[0] = NULL;
		}
	}
	else
	{
		m_pbNVP.szValue = new wchar_t[1];
		m_pbNVP.szValue[0] = L'\0';
	}

	return &m_pbNVP;
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
LPPBNAMEVALUE CEMML::CNameValuePair::GetNVPPointer()
{
	return &m_pbNVP;
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
BOOL CEMML::CNameValuePair::SkipChar(TCHAR tcCharToCheck)
{
	//check that the char is not to be skipped
	int i=0;
	static const wchar_t RemoveChars[] = {L" \t\r\n"};

	while(RemoveChars[i] != NULL)
	{
		if(tcCharToCheck == RemoveChars[i++])
			return TRUE;
	}
	return FALSE;
}



////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CEMMLProfile::CEMMLProfile(void)
	:m_pepLeftEl(NULL)
	,m_pepRightEl(NULL)
	,m_pnvpHeadEl(NULL)
{
	m_szProfileName = NULL;
}

////////////////////////////////////////////////////////////////////////
// Authors:			James MorleySmith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CEMMLProfile::~CEMMLProfile(void)
{
	delete m_pnvpHeadEl;
	m_pnvpHeadEl = NULL;

	delete m_pepLeftEl;	
	m_pepLeftEl = NULL;
	
	delete m_pepRightEl;	
	m_pepRightEl = NULL;

	delete [] m_szProfileName;	
	m_szProfileName = NULL;
}


////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
void CEMML::CEMMLProfile::AddProfileName(LPCWSTR sName, int iLen)
{
	int iStart = 0;
	//check for leading spaces
	while(SkipChar(*(sName+iStart)))
	{
		++iStart;
		--iLen;
	}

	//remove trailing invalid chars
	while(iLen >= 0 && SkipChar(*(sName+iStart+(iLen-1))))
		iLen--;

	if(iLen < 0)
		return;
	
	m_szProfileName = new wchar_t[iLen+1];

	//copy data char by char as we want to force to lower case
	for(int i=0; i<iLen; i++)
	{
		if(*(sName+iStart+i) >= L'A' && *(sName+iStart+i) <= 'Z')
			*(m_szProfileName+i) = *(sName+iStart+i)|L' ';
		else
			*(m_szProfileName+i) = *(sName+iStart+i);
	}

	//wcsncpy(m_szProfileName, sName+iStart, iLen);
	
	m_szProfileName[iLen] = L'\0';

}

////////////////////////////////////////////////////////////////////////
// Authors:			James MorleySmith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
BOOL CEMML::CEMMLProfile::SkipChar(TCHAR tcCharToCheck)
{
	//check that the char is not to be skipped
	if(tcCharToCheck == L'-' ||
	   tcCharToCheck == L'.' ||
	   tcCharToCheck == L':' ||
	   tcCharToCheck == L'_' ||
	   (tcCharToCheck >= L'0' && tcCharToCheck <= L'9') ||
	   (tcCharToCheck >= L'A' && tcCharToCheck <= L'Z') ||
	   (tcCharToCheck >= L'a' && tcCharToCheck <= L'z'))
	{
		return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CEMMLProfile * CEMML::CEMMLProfile::CreateNewEMMLProfileNode(CEMMLProfile * pepNode, LPCWSTR sName, int iLen)
{

	UINT iStart = 0;
	//check for leading spaces
	while(SkipChar(*(sName+iStart)))
	{
		++iStart;
		--iLen;
	}

	//remove trailing invalid chars
	while(iLen >= 0 && SkipChar(*(sName+iStart+(iLen-1))))
		iLen--;

	if(iLen <=0)
		return NULL;

	while( pepNode !=NULL )
	{
		//we compare the names:
		//	if it's greater it goes to the right of the tree
		//	if it's less it goes to the left
		//  if it's the same, it's a duplicate so delete the current nvp link list
		int dwCompare = wcsnicmp(pepNode->m_szProfileName, sName+iStart
			, ((wcslen(pepNode->m_szProfileName) > (size_t)iLen) ? wcslen(pepNode->m_szProfileName) : (size_t)iLen));

		if(dwCompare > 0)
		{
			if(pepNode->m_pepRightEl == NULL)
			{
				pepNode = pepNode->m_pepRightEl = new CEMMLProfile();
				break;
			}
			else
				pepNode = pepNode->m_pepRightEl;
		}
		else if(dwCompare < 0)
		{
			if(pepNode->m_pepLeftEl == NULL)
			{
				pepNode = pepNode->m_pepLeftEl = new CEMMLProfile();
				break;
			}
			else
				pepNode = pepNode->m_pepLeftEl;
		}
		else
		{
			delete pepNode->m_pnvpHeadEl;
			pepNode->m_pnvpHeadEl = NULL;
			return pepNode;
		}
	}

	//if the node is null, it was the head of the tree
	if(pepNode == NULL)
		pepNode = new CEMMLProfile();

	//now we have the node create a new element as assign the name
	pepNode->AddProfileName(sName, iLen);

	return pepNode;
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CEMMLProfile * CEMML::CEMMLProfile::FindEMMLProfileNode(CEMMLProfile * pepNode, LPCWSTR sName)
{

	int iLen = wcslen(sName);

	int iStart = 0;
	//check for leading spaces
	while(SkipChar(*(sName+iStart)))
	{
		++iStart;
		--iLen;
	}

	//remove trailing invalid chars
	while(iLen >= 0 && SkipChar(*(sName+iStart+(iLen-1))))
		iLen--;

	if(iLen <=0)
		return NULL;


	while( pepNode !=NULL )
	{
		//we compare the names:
		//	if it's greater it goes to the right of the tree
		//	if it's less it goes to the left
		//  if it's the same, we found it
		int dwCompare = wcsnicmp(pepNode->m_szProfileName, sName+iStart
			, ((wcslen(pepNode->m_szProfileName) > (size_t)iLen) ? wcslen(pepNode->m_szProfileName) : (size_t)iLen));

		if(dwCompare > 0)
			pepNode = pepNode->m_pepRightEl;
		
		else if(dwCompare < 0)
			pepNode = pepNode->m_pepLeftEl;
		
		else
			return pepNode;
		
	}

	return pepNode;
}


////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
DWORD CEMML::ParseEMMLTag(LPCWSTR szHttpEquiv, LPCWSTR szContents, EMMLPROC lpEMMLProc)
{
	wchar_t szHttpEquivResult[MAXURL];
	wchar_t szContentsResult[MAXURL];

	memset(szHttpEquivResult, 0, MAXURL*sizeof(wchar_t));
	memset(szContentsResult, 0, MAXURL*sizeof(wchar_t));

	//  DCC - Testing this with EMML Tags (on a page) shows a 10x performance increase
	//  in this function when m_bDoRegEx is false (~40ms --> ~4ms)
	if(m_bDoRegEx)
	{
		CRegEx* regexObj = new CRegEx();
		wcscpy(szHttpEquivResult, szHttpEquiv);
		wcscpy(szContentsResult, szContents);
		
		pbRegExPair * pbCurPair = m_RegExPatternsHE;
		while(pbCurPair != NULL)
		{
			if (-1 != regexObj->FindAndReplace(szHttpEquivResult, pbCurPair->szSearchPattern, 
				pbCurPair->szReplacePattern, NULL, &szHttpEquivResult[0]))
			{
				//  A Regular Expression was applied, log this so customers have more of an idea how to port their Browser application.
				int iLogLength = 5 + wcslen(szHttpEquivResult) + wcslen(szHttpEquiv) + 
					wcslen(pbCurPair->szSearchPattern) + wcslen(pbCurPair->szReplacePattern) + 
					wcslen(L"A Regular Expression has been applied to the Module name ( => )  RegEx.xml entry is \"<Expression patternEx=\"\" replaceEx=\"\"/>\"");
				WCHAR* szLog = new WCHAR[iLogLength];
				memset(szLog, 0, iLogLength * sizeof(WCHAR));
				if (szLog && szHttpEquiv && szHttpEquivResult)
				{
					wsprintf(szLog, L"A Regular Expression has been applied to the Module name (%s => %s).  RegEx.xml entry is \"<Expression patternEx=\"%s\" replaceEx=\"%s\"/>\"", 
						szHttpEquiv, szHttpEquivResult, pbCurPair->szSearchPattern, pbCurPair->szReplacePattern);
#ifndef PB_EMMP_PLG_EXPORTS
					Log(PB_LOG_INFO, szLog, _T(__FUNCTION__), __LINE__, L"EMML Parser");
#endif
					delete[] szLog;
				} 
			}
			pbCurPair = pbCurPair->lpNextNode;
		}
		pbCurPair = m_RegExPatternsCT;
		while(pbCurPair != NULL)
		{
			if (-1 != regexObj->FindAndReplace(szContentsResult, pbCurPair->szSearchPattern, 
				pbCurPair->szReplacePattern, NULL, &szContentsResult[0]))
			{
				//  A Regular Expression was applied, log this so customers have more of an idea how to port their Browser application.
				int iLogLength = 5 + wcslen(szContentsResult) + wcslen(szContents) + 
					wcslen(pbCurPair->szSearchPattern) + wcslen(pbCurPair->szReplacePattern) + 
					wcslen(L"A Regular Expression has been applied to the Module method or parameter ( => )  RegEx.xml entry is \"<Expression patternEx=\"\" replaceEx=\"\"/>\"");
				WCHAR* szLog = new WCHAR[iLogLength];
				memset(szLog, 0, iLogLength * sizeof(WCHAR));
				if (szLog && szContents && szContentsResult)
				{
					wsprintf(szLog, L"A Regular Expression has been applied to the Module method or parameter (%s => %s)  RegEx.xml entry is \"<Expression patternEx=\"%s\" replaceEx=\"%s\"/>\"", 
						szContents, szContentsResult, pbCurPair->szSearchPattern, pbCurPair->szReplacePattern);
#ifndef PB_EMMP_PLG_EXPORTS
					Log(PB_LOG_INFO, szLog, _T(__FUNCTION__), __LINE__, L"EMML Parser");
#endif
					delete[] szLog;
				} 
			}
			pbCurPair = pbCurPair->lpNextNode;
		}
		
		delete regexObj;
		regexObj = NULL;
	}




	if(*szHttpEquivResult == NULL)
		wcscpy(szHttpEquivResult, szHttpEquiv);

	if(*szContentsResult == NULL)
		wcscpy(szContentsResult, szContents);



	//if there is no http-equiv, nothing to do so return
	if(szHttpEquivResult == NULL || lpEMMLProc == NULL)
		return PET_NOTHINGTODO;

	CNameValuePair* pnvpHead = NULL;		//pointer to the start of the named value pair list
	CNameValuePair* pnvpCurr = NULL;		//pointer to the current named value pair
	
	//build named value pairs link list of parameters from szContentsResult
	const wchar_t *sString, *sEnd;

	//bool to indicate that the current string is within quotes so should be ignored until a closing quote
	bool bInSingleQuoteSection = false;
	bool bInDoubleQuoteSection = false;
	int iInRoundBracketedSection = 0 ;
	int iInSquareBracketedSection = 0;
	int iInCurlyBracketedSection = 0;

	sString = sEnd = szContentsResult;
	int iLen = wcslen(szContentsResult);

	
	while(sString < szContentsResult+iLen)
	{
		switch (*sEnd)
		{
			case L'{':
				//increment the count
				iInCurlyBracketedSection++;

				++sEnd;
				break;
			
			case L'}':
				//decrement the count
				iInCurlyBracketedSection--;

				++sEnd;
				break;

			case L'[':
				//increment the count
				iInSquareBracketedSection++;

				++sEnd;
				break;
			
			case L']':
				//decrement the count
				iInSquareBracketedSection--;

				++sEnd;
				break;

			case L'(':
				//increment the count
				iInRoundBracketedSection++;

				++sEnd;
				break;
			
			case L')':
				//decrement the count
				iInRoundBracketedSection--;

				++sEnd;
				break;

			case L'\\':
				//it's an escape character so skip a char
				sEnd+=2;
				break;
			case L'\'':
				//invert the single quote section state
				bInSingleQuoteSection = !bInSingleQuoteSection;

				++sEnd;
				break;
			
			case L'\"':
				//invert the double quote section state
				bInDoubleQuoteSection = !bInDoubleQuoteSection;

				++sEnd;
				break;

			case L';': //check for a null or semi-colon
				//break out if we are within a quotes section
				if(bInSingleQuoteSection || bInDoubleQuoteSection
					|| iInCurlyBracketedSection > 0
					|| iInSquareBracketedSection > 0
					|| iInRoundBracketedSection > 0)
				{
					++sEnd;
					break;
				}

			case L'\0': 	

				//either it's the first element or we've filled the current so create a new one
				if(pnvpCurr == NULL || pnvpCurr->GetNVPPointer()->szValue)
				{
					if(pnvpCurr)
						//add a new NVP to the current element's Next Element pointer
						//then set the current elelemt to this pointer
						pnvpCurr = pnvpCurr->m_pnvpNextEl = new CNameValuePair();
					else 
						pnvpCurr = new CNameValuePair();

					//if the head is null, this is the first element so set to this element
					if(pnvpHead == NULL)
						pnvpHead = pnvpCurr;
				}

				//this is a value or method so add it to the current NVP
				pnvpCurr->AddValue(sString, (int)(sEnd - sString));

				//move to the next character
				sString = ++sEnd;
				break;

			case L':': //check for a colon
				//break out if we are within a quotes section
				if(bInSingleQuoteSection || bInDoubleQuoteSection
					|| iInCurlyBracketedSection > 0
					|| iInSquareBracketedSection > 0
					|| iInRoundBracketedSection > 0)
				{
					++sEnd;
					break;
				}

				//either it's the first element or we've filled the current so create a new one
				if(pnvpCurr == NULL || pnvpCurr->GetNVPPointer()->szValue)
				{
					if(pnvpCurr)
						//add a new NVP to the current element's Next Element pointer
						//then set the current elelemt to this pointer
						pnvpCurr = pnvpCurr->m_pnvpNextEl = new CNameValuePair();
					else 
						pnvpCurr = new CNameValuePair();

					//if the head is null, this is the first element so set to this element
					if(pnvpHead == NULL)
						pnvpHead = pnvpCurr;
				}

				//this is a name so add it to the current NVP
				pnvpCurr->AddName(sString, (int)(sEnd - sString));

				//move to the next character
				sString = ++sEnd;
				break;

			default:
				//move to the next character
				++sEnd;
				break;
		}
	}
	
	
	wchar_t* pszModule = NULL;
	//look for a hypen in the http-equiv value

	iLen = wcslen(szHttpEquivResult);

	//store the module
	int iStart = 0;
	
	//check for leading spaces
	while(*(szHttpEquivResult+iStart) == L' ')
	{
		++iStart;
		--iLen;
	}

	pszModule = new wchar_t[iLen+1];
	wcsncpy(pszModule, szHttpEquivResult+iStart, iLen);
	pszModule[iLen] = NULL;

	sString = szHttpEquivResult+iStart;
	
	while(sString < ((szHttpEquivResult+iStart)+iLen))
	{
		if((wchar_t)*(sString++) == L'-')  
		{
			//found hypen so store the end of the module name
			//remove leading spaces
			int iHyphenPos = (sString-(szHttpEquivResult+iStart))-1;
			while(iHyphenPos >= 0 && pszModule[iHyphenPos-1] == L' ')
				iHyphenPos--;
			
			pszModule[iHyphenPos] = NULL;

			//find first unnamed value and assume it's paired with hyphened parameter name
			pnvpCurr = pnvpHead;
			while(pnvpCurr)
			{
				if(pnvpCurr->GetNVPPointer()->szName == NULL)
				{
					pnvpCurr->AddName(sString, wcslen(sString));
					break;
				}
				
				//move to the next element, or if there is no next el, there
				//are no unnamed pairs, assume it's a method (like "send" or "show")
				//and add the method to the end of the list
				if(pnvpCurr->m_pnvpNextEl != NULL)
					pnvpCurr = pnvpCurr->m_pnvpNextEl;
				else
				{
					pnvpCurr = pnvpCurr->m_pnvpNextEl = new CNameValuePair();
					pnvpCurr->AddValue(sString, wcslen(sString));
					break;
				}
			}
			
			break;
		}
	}

	//loop through the link list and call out all the meta functions
	pnvpCurr = pnvpHead;
	LPPBNAMEVALUE ppbNVP = NULL;

	while(pnvpCurr)
	{
		
		pnvpCurr->AddModule(pszModule, wcslen(pszModule));

		//ppbNVP = pnvpCurr->GetNVPPointer();
		//changed to this function so the name and value are the correct way round if it's a method
		ppbNVP = pnvpCurr->MakeMethod();

		if( ((ppbNVP->szValue == NULL || *(ppbNVP->szValue) == NULL) &&
			(ppbNVP->szName != NULL && *(ppbNVP->szName) != NULL))
			||
			((ppbNVP->szName == NULL || *(ppbNVP->szName) == NULL) &&
			(ppbNVP->szValue != NULL && *(ppbNVP->szValue) != NULL))
			||
			((ppbNVP->szValue != NULL && *(ppbNVP->szValue) != NULL) &&
			(ppbNVP->szName != NULL && *(ppbNVP->szName) != NULL)) )
		{
			(*lpEMMLProc)(ppbNVP);
		}

		pnvpCurr = pnvpCurr->m_pnvpNextEl;
	}

	
	delete [] pszModule;
	pszModule = NULL;

	//delete the link list
	delete pnvpHead;
	pnvpHead = NULL;

	return PET_SUCCESS;
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		November 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////

BOOL CEMML::AddRegExPair_HE(LPCWSTR lpcSearchPattern, LPCWSTR lpcReplacePattern)
{
	return AddRegExPair(&m_RegExCurNodeHE, lpcSearchPattern, lpcReplacePattern);
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		November 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
BOOL CEMML::AddRegExPair_CT(LPCWSTR lpcSearchPattern, LPCWSTR lpcReplacePattern)
{
	return AddRegExPair(&m_RegExCurNodeCT, lpcSearchPattern, lpcReplacePattern);
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		November 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
BOOL CEMML::AddRegExPair(pbRegExPair *** RegExCurNode, LPCWSTR lpcSearchPattern, LPCWSTR lpcReplacePattern)
{
	//check we have valid pointers
	//otherwise return false to indicate a failure
	if(lpcSearchPattern == NULL || lpcReplacePattern == NULL)
		return FALSE;

	//create a copy of the strings on the heap
	//if either fails, cleanup and return a failure
	wchar_t * wcSearch = new wchar_t[wcslen(lpcSearchPattern) + 1];
	if(wcSearch == NULL)
	{
		return FALSE;
	}

	wchar_t * wcReplace = new wchar_t[wcslen(lpcReplacePattern) + 1];
	if(wcReplace == NULL)
	{
		delete [] wcSearch;
		wcSearch = NULL;
		return FALSE;
	}
	memset(wcSearch, 0, (wcslen(lpcSearchPattern) + 1) * sizeof(wchar_t));
	memset(wcReplace, 0, (wcslen(lpcReplacePattern) + 1) * sizeof(wchar_t));
	wcscpy(wcSearch, lpcSearchPattern);
	wcscpy(wcReplace, lpcReplacePattern);

	//create a new node at the end of the list
	pbRegExPair* TempNode = new pbRegExPair;
	if(TempNode == NULL)
	{
		//failed to create a new node so delete the heap
		//copies of the patterns
		delete [] wcSearch;
		wcSearch = NULL;
		delete [] wcReplace;
		wcReplace = NULL;

		return FALSE;
	}
	//copy the pointers
	TempNode->szSearchPattern = wcSearch;
	TempNode->szReplacePattern = wcReplace;
	TempNode->lpNextNode = NULL;
	

	**RegExCurNode = TempNode;
	
	//set the end pointer
	*RegExCurNode = &(TempNode->lpNextNode);

	return TRUE;

}


////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
DWORD CEMML::ImportEMMLProfilePage(LPCWSTR szProfileFileName, LPCWSTR szNamedProfileListName /* = NULL */)
{

	DWORD dwProfileCount = 0;

	char* szcProfileContent;
	LPWSTR szProfile;

	//open the profile page
	HANDLE hEMMPFile = CreateFile(szProfileFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

	if(hEMMPFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwErr = GetLastError();
		CloseHandle(hEMMPFile);
		return NULL;
	}

	//get the size of the file
	DWORD dwFileSize = GetFileSize(hEMMPFile, NULL);

	if(dwFileSize == 0)
	{
		CloseHandle(hEMMPFile);
		return NULL;
	}

	
	//allocate a buffer to the size of the file
	szcProfileContent = new char[dwFileSize+1];

	//read in the contents of the file
	if(!ReadFile(hEMMPFile, (LPVOID)szcProfileContent, dwFileSize, &dwFileSize, NULL))
	{
		delete [] szcProfileContent;
		szcProfileContent = NULL;
		CloseHandle(hEMMPFile);
		return NULL;
	}

	//close the file, we are done with it
	CloseHandle(hEMMPFile);
	
	szcProfileContent[dwFileSize] = NULL;

	//convert the contents to unicode
	szProfile = new wchar_t[dwFileSize+1];
	mbstowcs(szProfile, szcProfileContent, dwFileSize);
	szProfile[dwFileSize] = NULL;

	//delete the multi-byte copy
	delete [] szcProfileContent;
	szcProfileContent = NULL;	

	CEMMLProfile* pepHead = NULL //pointer to the start of the profile tree
		, * pepCurr = NULL;
	
	//build named value pairs link list of parameters from szContents
	const wchar_t *sString, *sEnd, *sMemEnd = NULL;
	
	//bool to indicate that the current string is within quotes so should be ignored until a closing quote
	bool bInCommentSection = false;

	DWORD dwSearchState = EP_LOOKINGFORSTARTOFNAME;

	sString = sEnd = szProfile;
	int iLen = wcslen(szProfile);
	
	while(sEnd < szProfile+iLen)
	{
		
		//this switch is purely for checking if it's a comment section
		switch (*sEnd)
		{
		case L'/':  //could be the start of a comment section
			if( *(sEnd+1) == L'*')
			{
				bInCommentSection = true;
				
				//just remember this as this what follows is not part of the name
				if(dwSearchState == EP_LOOKINGFORENDOFNAME)
					sMemEnd = sEnd;
			}

			sEnd++;
			break;
		
		case L'*':  //could be the end of a comment section
			if( *(sEnd+1) == L'/')
				bInCommentSection = false;

			sEnd+=2;
			continue;
			break;

		default:
			break;
		}

		if(bInCommentSection)
		{
			sEnd++;
			continue;
		}

		switch (dwSearchState)
		{
		case EP_LOOKINGFORSTARTOFNAME:
			//the start of a profile name must start with a fullstop (period)
			//the name is then the text which follow that up until the first curly brace "{"
			if(*(sEnd++) == L'.')
			{
				sString = sEnd;
				dwSearchState = EP_LOOKINGFORENDOFNAME;
			}
			break;

		case EP_LOOKINGFORENDOFNAME:
			if(*(sEnd++) == L'{')
			{

				//the sMemEnd thing is purely incase someone puts a comment directly after the profile name
				if(sMemEnd == NULL)
					sMemEnd = sEnd;

			
				//if current is null then we should create a new profile as we are at the start
				//otherwise, this is a new profile, so create a new one
				pepCurr = CEMMLProfile::CreateNewEMMLProfileNode(pepHead, sString, (int)(sMemEnd-sString)-1);

				sMemEnd = NULL;

				//inc the profile count
				++dwProfileCount;

				//either it's the first element or we've filled the current so create a new one
				if(pepHead == NULL)
					pepHead = pepCurr;
				
				dwSearchState = EP_LOOKINGFORENDOFDATASECTION;
				sString = sEnd;
			}
			break;
		case EP_LOOKINGFORENDOFDATASECTION:
			if(*(sEnd++) == L'}')
			{
				
				//parse the meta data to get the name value pair link list
				if(pepCurr != NULL)
					pepCurr->m_pnvpHeadEl = ParseEMMLProfileData(sString, (int)(sEnd-sString)-1);

				//find the next profile
				dwSearchState = EP_LOOKINGFORSTARTOFNAME;
			}
			break;

		default:
			sEnd++;
			break;
		}
	}

	//delete the unicode version
	delete [] szProfile;
	szProfile = NULL;

	//if there is a named profilelist name, we must 
	//store the list in the NamedProfiles link list
	if(szNamedProfileListName != NULL)
	{
		CNamedProfileList* pnplNode = NULL;
		//create a new node
		pnplNode = CNamedProfileList::CreateNewEMMLProfileListNode(m_pNamedEMMLPLHead, szNamedProfileListName);

		//if the head is null, set that as the new node
		if(m_pNamedEMMLPLHead == NULL)
			m_pNamedEMMLPLHead = pnplNode;

		pnplNode->AddProfileTree(pepHead);

	}
	else
	{
		//set the member to the head
		m_pEMMLProfiles = pepHead;
	}

	return dwProfileCount;
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CNameValuePair* CEMML::ParseEMMLProfileData(LPCWSTR szProfileData, int iLen)
{

	//if there is no length, nothing to do so return
	if(iLen == 0)
		return NULL;

	BOOL bInCommentSection = false;

	CNameValuePair* pnvpHead = NULL;		//pointer to the start of the named value pair list
	CNameValuePair* pnvpCurr = NULL;		//pointer to the current named value pair
	
	//build named value pairs link list of parameters from szContents
	const wchar_t *sString, *sEnd;

	//bool to indicate that the current string is within quotes so should be ignored until a closing quote
	bool bInSingleQuoteSection = false;
	bool bInDoubleQuoteSection = false;
	int iInSquareBracketedSection = 0;
	int iInRoundBracketedSection = 0;

	sString = sEnd = szProfileData;
		
	while(sString < szProfileData+iLen)
	{
		//this switch is purely for checking if it's a comment section
		switch (*sEnd)
		{
		case L'/':  //could be the start of a comment section
			if( *(sEnd+1) == L'*')
			{
				bInCommentSection = true;
				sEnd++;
			}

			sEnd++;
			break;
		
		case L'*':  //could be the end of a comment section
			if( *(sEnd+1) == L'/')
			{
				bInCommentSection = false;
				sString = (++sEnd)+1;
			}

			sEnd++;
			continue;
			break;

		default:
			break;
		}

		if(bInCommentSection)
		{
			sEnd++;
			continue;
		}

		switch (*sEnd)
		{
			case L'\\':
				//it's an escape character so skip a char
				sEnd+=2;
				break;
			case L'\'':
				//invert the single quote section state
				bInSingleQuoteSection = !bInSingleQuoteSection;

				++sEnd;
				break;
			
			/*case L'{':
				//increment the count
				iInCurlyBracketedSection++;

				++sEnd;
				break;
			
			case L'}':
				//decrement the count
				iInCurlyBracketedSection--;

				++sEnd;
				break;
			*/

			case L'[':
				//increment the count
				iInSquareBracketedSection++;

				++sEnd;
				break;
			
			case L']':
				//decrement the count
				iInSquareBracketedSection--;

				++sEnd;
				break;

			case L'(':
				//increment the count
				iInRoundBracketedSection++;

				++sEnd;
				break;
			
			case L')':
				//decrement the count
				iInRoundBracketedSection--;

				++sEnd;
				break;
			
			case L'\"':
				//invert the double quote section state
				bInDoubleQuoteSection = !bInDoubleQuoteSection;

				++sEnd;
				break;

			 //check for a null or semi-colon or close curly braces
			case L'}':
			case L';':
				//break out if we are within a quotes section
				if(bInSingleQuoteSection || bInDoubleQuoteSection 
					|| iInRoundBracketedSection > 0 
					|| iInSquareBracketedSection > 0)
				{
					++sEnd;
					break;
				}

			case L'\0': 	

				//if there is already a value, then this is bad data so skip it
				if(pnvpCurr->GetNVPPointer()->szValue == NULL)
					//this is a value or method so add it to the current NVP
					pnvpCurr->AddValue(sString, (int)(sEnd - sString));

				//move to the next character
				sString = ++sEnd;

				/*if(pnvpCurr)
					//add a new NVP to the current element's Next Element pointer
					//then set the current elelemt to this pointer
					pnvpCurr = pnvpCurr->m_pnvpNextEl = new CNameValuePair();
				else 
					pnvpCurr = new CNameValuePair();
*/

				break;

			case L':': //check for a colon
				//break out if we are within a quotes section
				if(bInSingleQuoteSection || bInDoubleQuoteSection
					|| iInRoundBracketedSection > 0 
					|| iInSquareBracketedSection > 0)
				{
					++sEnd;
					break;
				}

				//either it's the first element or we've filled the current so create a new one
				if(pnvpCurr == NULL || (pnvpCurr->GetNVPPointer()->szModule && pnvpCurr->GetNVPPointer()->szName))
				{
					if(pnvpCurr)
						//add a new NVP to the current element's Next Element pointer
						//then set the current elelemt to this pointer
						pnvpCurr = pnvpCurr->m_pnvpNextEl = new CNameValuePair();
					else 
						pnvpCurr = new CNameValuePair();

					//if the head is null, this is the first element so set to this element
					if(pnvpHead == NULL)
						pnvpHead = pnvpCurr;
				}

				if(pnvpCurr->GetNVPPointer()->szModule)
					//this is a name so add it to the current NVP
					pnvpCurr->AddName(sString, (int)(sEnd - sString));
				else
					//this is a module so add it to the current NVP
					pnvpCurr->AddModule(sString, (int)(sEnd - sString));

				//move to the next character
				sString = ++sEnd;
				break;
			case L'-': //check for a hyphen
				//break out if we are within a quotes section
				if(bInSingleQuoteSection || bInDoubleQuoteSection
					|| iInRoundBracketedSection > 0 
					|| iInSquareBracketedSection > 0)
				{
					++sEnd;
					break;
				}
				
				if(pnvpCurr && ((pnvpCurr->GetNVPPointer()->szModule && pnvpCurr->GetNVPPointer()->szName) && !(pnvpCurr->GetNVPPointer()->szValue)))
				{
					++sEnd;
					break;
				}
				//either it's the first element or we've filled the current so create a new one
				if(pnvpCurr == NULL || pnvpCurr->GetNVPPointer()->szModule)
				{
					if(pnvpCurr)
						//add a new NVP to the current element's Next Element pointer
						//then set the current elelemt to this pointer
						pnvpCurr = pnvpCurr->m_pnvpNextEl = new CNameValuePair();
					else 
						pnvpCurr = new CNameValuePair();

					//if the head is null, this is the first element so set to this element
					if(pnvpHead == NULL)
						pnvpHead = pnvpCurr;
				}
				//this is a name so add it to the current NVP
				pnvpCurr->AddModule(sString, (int)(sEnd - sString));

				//move to the next character
				sString = ++sEnd;
				break;
			default:
				//move to the next character
				++sEnd;
				break;
		}
	}
	
	//return the head pointer
	return pnvpHead;
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
DWORD CEMML::ApplyEMMLProfile(LPCWSTR szProfileName, EMMLPROFILEPROC lpEMMLProc, LPARAM lParam)
{
	CEMMLProfile* pepFind = NULL;
	
	//if there is a dot in the profile name then use the named list
	//else use the temp list

	if(wcsstr(szProfileName, L".") != NULL)
	{
		CNamedProfileList* pnplFind;
		wchar_t * szName = new wchar_t[wcslen(szProfileName)+1];
		wcscpy(szName, szProfileName);
		*wcsstr(szName, L".") = NULL;

		pnplFind = CNamedProfileList::FindEMMLProfileListNode(m_pNamedEMMLPLHead, szName);
		delete [] szName;
		szName = NULL;

		if(pnplFind == NULL)
			return EP_NOTFOUND;

		pepFind = m_pEMMLProfiles->FindEMMLProfileNode(pnplFind->GetEPPointer(), wcsstr(szProfileName, L".")+1);
	}
	else
	{
		pepFind = m_pEMMLProfiles->FindEMMLProfileNode(m_pEMMLProfiles, szProfileName);
	}

	if(pepFind == NULL)
	{
		return EP_NOTFOUND;
	}

	CNameValuePair* pnvpCurr = pepFind->m_pnvpHeadEl;
	
	
	//loop through the link list and call out all the meta functions
	LPPBNAMEVALUE ppbNVP = NULL;

	while(pnvpCurr)
	{
		
		//ppbNVP = pnvpCurr->GetNVPPointer();
		ppbNVP = pnvpCurr->MakeMethod();

		if( ((ppbNVP->szValue == NULL || *(ppbNVP->szValue) == NULL) &&
			(ppbNVP->szName != NULL && *(ppbNVP->szName) != NULL))
			||
			((ppbNVP->szName == NULL || *(ppbNVP->szName) == NULL) &&
			(ppbNVP->szValue != NULL && *(ppbNVP->szValue) != NULL))
			||
			((ppbNVP->szValue != NULL && *(ppbNVP->szValue) != NULL) &&
			(ppbNVP->szName != NULL && *(ppbNVP->szName) != NULL)) )
		{
			(*lpEMMLProc)(ppbNVP, lParam);
		}

		pnvpCurr = pnvpCurr->m_pnvpNextEl;
	}

	return EP_SUCCESS;
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CEMML(BOOL bRegex)
	:m_pEMMLProfiles(NULL)
	,m_pNamedEMMLPLHead(NULL)
	,m_bDoRegEx(bRegex)
	,m_RegExPatternsHE(NULL)
	,m_RegExPatternsCT(NULL)
	,m_DefaultTags(NULL)
	,m_DefaultCurrent(NULL)
{
	m_RegExCurNodeHE = &m_RegExPatternsHE;
	m_RegExCurNodeCT = &m_RegExPatternsCT;

}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::~CEMML()
{
	//delete the profiles (named and temporary)
	delete m_pEMMLProfiles;
	m_pEMMLProfiles = NULL;

	delete m_pNamedEMMLPLHead;
	m_pNamedEMMLPLHead = NULL;

	//delete the reg ex patterns
	pbRegExPair * pbCurPair = m_RegExPatternsHE;
	while(pbCurPair != NULL)
	{
		delete [] pbCurPair->szSearchPattern;
		pbCurPair->szSearchPattern = NULL;
		delete [] pbCurPair->szReplacePattern;
		pbCurPair->szReplacePattern = NULL;
		
		pbRegExPair * pbDelPair = pbCurPair;
		pbCurPair = pbCurPair->lpNextNode;

		delete pbDelPair;
		pbDelPair = NULL;
	}

	pbCurPair = m_RegExPatternsCT;
	while(pbCurPair != NULL)
	{
		delete [] pbCurPair->szSearchPattern;
		pbCurPair->szSearchPattern = NULL;
		delete [] pbCurPair->szReplacePattern;
		pbCurPair->szReplacePattern = NULL;
		
		pbRegExPair * pbDelPair = pbCurPair;
		pbCurPair = pbCurPair->lpNextNode;

		delete pbDelPair;
		pbDelPair = NULL;
	}

}



////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CNamedProfileList::CNamedProfileList(void)
	:m_pnplNextEl(NULL)
	,m_pnplPrevEl(NULL)
	,m_sNamedProfilesName(NULL)
	,m_pProfileList(NULL)
{
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CNamedProfileList::~CNamedProfileList(void)
{
	delete m_pnplNextEl;	
	m_pnplNextEl = NULL;
	m_pnplPrevEl = NULL;

	delete [] m_sNamedProfilesName;		
	m_sNamedProfilesName = NULL;

	delete m_pProfileList;		
	m_pProfileList = NULL;
}


////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
void CEMML::CNamedProfileList::AddName(LPCWSTR sName, int iLen)
{
	//just check it's not already been assigned
	delete [] m_sNamedProfilesName;
	m_sNamedProfilesName = NULL;

	int iStart = 0;
	//check for leading spaces
	while(SkipChar(*(sName+iStart)))
	{
		++iStart;
		--iLen;
	}
	//remove trailing invalid chars
	while(iLen >= 0 && SkipChar(*(sName+iStart+(iLen-1))))
		iLen--;

	if(iLen < 0)
		return;
	
	//allocate and assign
	m_sNamedProfilesName = new wchar_t[iLen+1];

	//copy data char by char as we want to force to lower case
	for(int i=0; i<iLen; i++)
	{
		if(*(sName+iStart+i) >= L'A' && *(sName+iStart+i) <= 'Z')
			*(m_sNamedProfilesName+i) = *(sName+iStart+i)|L' ';
		else
			*(m_sNamedProfilesName+i) = *(sName+iStart+i);
	}

	
	m_sNamedProfilesName[iLen] = L'\0';

}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
void CEMML::CNamedProfileList::AddProfileTree(CEMMLProfile *pepHead)
{

	//just check it's not already been assigned
	delete [] m_pProfileList;
	m_pProfileList = NULL;

	//now assign it
	m_pProfileList = pepHead;

}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CEMMLProfile* CEMML::CNamedProfileList::GetEPPointer()
{
	return m_pProfileList;
}

////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
BOOL CEMML::CNamedProfileList::SkipChar(TCHAR tcCharToCheck)
{
	//check that the char is not to be skipped
	int i=0;
	static const wchar_t RemoveChars[] = {L" \t\r\n"};

	while(RemoveChars[i] != NULL)
	{
		if(tcCharToCheck == RemoveChars[i++])
			return TRUE;
	}
	return FALSE;
}


////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CNamedProfileList * CEMML::CNamedProfileList::CreateNewEMMLProfileListNode(CEMML::CNamedProfileList * pnplNode, LPCWSTR szName)
{

	UINT iLen = wcslen(szName);
	UINT iStart = 0;
	//check for leading spaces
	while(SkipChar(*(szName+iStart)))
	{
		++iStart;
		--iLen;
	}

	//remove trailing invalid chars
	while(iLen >= 0 && SkipChar(*(szName+iStart+(iLen-1))))
		iLen--;

	if(iLen <=0)
		return NULL;

	while( pnplNode !=NULL )
	{
		//we compare the names:
		//	if it doesn't compare, move down the list
		//  if it's the same, it's a duplicate so delete the current entry
		int dwCompare = wcsnicmp(pnplNode->m_sNamedProfilesName, szName+iStart
			, ((wcslen(pnplNode->m_sNamedProfilesName) > iLen) ? wcslen(pnplNode->m_sNamedProfilesName) : iLen));

		if(dwCompare != 0)
		{
			if(pnplNode->m_pnplNextEl == NULL)
			{

				//create a new node
				pnplNode->m_pnplNextEl = new CNamedProfileList();
				//set the new node's previous element to the current node
				pnplNode->m_pnplNextEl->m_pnplPrevEl = pnplNode;
				//set the current node to the new node
				pnplNode = pnplNode->m_pnplNextEl;
				break;
			}
			else
				pnplNode = pnplNode->m_pnplNextEl;
		}
		else
		{
			delete pnplNode->m_pProfileList;
			pnplNode->m_pProfileList = NULL;
			
			return pnplNode;
		}
	}

	//if the node is null, it was the head of the tree
	if(pnplNode == NULL)
		pnplNode = new CNamedProfileList();

	//now we have the node create a new element as assign the name
	pnplNode->AddName(szName, wcslen(szName));

	return pnplNode;
}


////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CNamedProfileList * CEMML::CNamedProfileList::FindEMMLProfileListNode(CEMML::CNamedProfileList * pnplNode, LPCWSTR szName)
{

	UINT iLen = wcslen(szName);
	UINT iStart = 0;
	//check for leading spaces
	while(SkipChar(*(szName+iStart)))
	{
		++iStart;
		--iLen;
	}

	//remove trailing invalid chars
	while(iLen >= 0 && SkipChar(*(szName+iStart+(iLen-1))))
		iLen--;

	if(iLen <=0)
		return NULL;

	while( pnplNode !=NULL )
	{
		//we compare the names:
		//	if it doesn't compare, move down the list
		//  if it's the same, return the pointer
		int dwCompare = wcsnicmp(pnplNode->m_sNamedProfilesName, szName+iStart
			, ((wcslen(pnplNode->m_sNamedProfilesName) > iLen) ? wcslen(pnplNode->m_sNamedProfilesName) : iLen));

		if(dwCompare != 0)
			pnplNode = pnplNode->m_pnplNextEl;
		else
			break;
		
	}

	return pnplNode;
}


////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
CEMML::CNamedProfileList * CEMML::CNamedProfileList::DeleteEMMLProfileListNode(CEMML::CNamedProfileList * pnplNode, LPCWSTR szName)
{

	UINT iLen = wcslen(szName);
	UINT iStart = 0;

	CEMML::CNamedProfileList * pnplHead = pnplNode;

	//check for leading spaces
	while(SkipChar(*(szName+iStart)))
	{
		++iStart;
		--iLen;
	}

	//remove trailing invalid chars
	while(iLen >= 0 && SkipChar(*(szName+iStart+(iLen-1))))
		iLen--;

	if(iLen <=0)
		return pnplHead;

	while( pnplNode !=NULL )
	{
		//we compare the names:
		//	if it doesn't compare, move down the list
		//  if it's the same, return the pointer
		int dwCompare = wcsnicmp(pnplNode->m_sNamedProfilesName, szName+iStart
			, ((wcslen(pnplNode->m_sNamedProfilesName) > iLen) ? wcslen(pnplNode->m_sNamedProfilesName) : iLen));

		if(dwCompare != 0)
			pnplNode = pnplNode->m_pnplNextEl;
		else
			break;
		
	}

	if(pnplNode == NULL)
		return pnplHead; //couldn't find the list
	
	//if it's the only node, just delete it
	if(pnplNode->m_pnplPrevEl == NULL && pnplNode->m_pnplNextEl == NULL)
	{
		delete pnplNode;
		pnplNode = NULL;
		pnplHead = NULL;
	}
	//if it's the head, just set the next's prev to NULL and delete
	else if(pnplNode->m_pnplPrevEl == NULL)
	{
		//store the new head
		pnplHead = pnplNode->m_pnplNextEl;
		
		//set the next's prev to NULL so it's a head
		pnplNode->m_pnplNextEl->m_pnplPrevEl = NULL;

		//set node's next to NULL so that the delete does not cascade down the list;
		pnplNode->m_pnplNextEl = NULL;

		delete pnplNode;
		pnplNode = NULL;
	}
	//if it's the tail, just set the prev's next to NULL and delete
	else if(pnplNode->m_pnplNextEl == NULL)
	{
		//set the previous node's next to null so it's a tail
		pnplNode->m_pnplPrevEl->m_pnplNextEl = NULL;
		
		delete pnplNode;
		pnplNode = NULL;
	}
	//it's in the middle somewhere
	else
	{
		//set the previous node's next to the next node
		pnplNode->m_pnplPrevEl->m_pnplNextEl = pnplNode->m_pnplNextEl;

		//set the next node's previous to the previous
		pnplNode->m_pnplNextEl->m_pnplPrevEl = pnplNode->m_pnplPrevEl;

		//clear the next node to avoid cascading deletes
		pnplNode->m_pnplNextEl = NULL;

		//delete the node
		delete pnplNode;
		pnplNode = NULL;
	}

	return pnplHead;
}


////////////////////////////////////////////////////////////////////////
// Authors:			James Morley-Smith (JMS)
// Change History:	
//		July 2008 - JMS First Created
////////////////////////////////////////////////////////////////////////
void CEMML::DeleteEMMLProfilePage(LPCWSTR szNamedProfileListName /* = NULL */)
{

	//we are not naming this profile list so must delete the temp list
	//in preparation to replace it
	if(szNamedProfileListName == NULL || *szNamedProfileListName == NULL)
	{
		delete m_pEMMLProfiles;
		m_pEMMLProfiles = NULL;
	}
	else
	{
		m_pNamedEMMLPLHead = CNamedProfileList::DeleteEMMLProfileListNode(m_pNamedEMMLPLHead, szNamedProfileListName);
	}
	
	return;
}

/*********************************************************************************************/
DWORD CEMML::AddStaticTag(LPCWSTR szHttpEquiv, LPCWSTR szContents)
{
	wchar_t szHttpEquivResult[MAXURL] = {0};
	wchar_t szContentsResult[MAXURL] = {0};

	if(*szHttpEquivResult == NULL) wcscpy(szHttpEquivResult, szHttpEquiv);
	if(*szContentsResult == NULL) wcscpy(szContentsResult, szContents);

	//if there is no http-equiv, nothing to do so return
	if(szHttpEquivResult == NULL)	
		return PET_NOTHINGTODO;

	CNameValuePair* pnvpCurr = NULL;				//pointer to the created node
	pnvpCurr = new CNameValuePair();
	
	if (m_DefaultCurrent)
	{
		m_DefaultCurrent->m_pnvpNextEl = pnvpCurr;
	}
	else
	{
		m_DefaultTags = pnvpCurr;
	}

	//  We are not actually adding a parsed name value pair, this is a fix so I am reusing that
	//  same structure to just store strings for the http-equiv and contents.
	pnvpCurr->AddModule(szHttpEquivResult,wcslen(szHttpEquivResult));					//Module Name
	pnvpCurr->AddDefaultMetaTagName(szContentsResult,wcslen(szContentsResult));
	
	m_DefaultCurrent = pnvpCurr;

	return EP_SUCCESS;
}
/*********************************************************************************************/
DWORD CEMML::RunDefaultTags(EMMLPROC lpEMMLProc)
{
	return ExecMetaTags(m_DefaultTags,lpEMMLProc );
}
/*********************************************************************************************/
DWORD CEMML::ExecMetaTags(CNameValuePair *pnvpCurr,EMMLPROC lpEMMLProc)
{
	//loop through the link list and call out all the meta functions
	LPPBNAMEVALUE ppbNVP = NULL;

	while(pnvpCurr)
	{
		ParseEMMLTag(pnvpCurr->GetNVPPointer()->szModule, pnvpCurr->GetNVPPointer()->szName, lpEMMLProc);
		pnvpCurr = pnvpCurr->m_pnvpNextEl;
	}
	return EP_SUCCESS;
}
