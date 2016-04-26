#include "RegularExpressions.h"

//////////////////////////////////////
//  Construction / Destruction      //
//////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			July 2008
///	Change History:	Initially Created
////////////////////////////////////////////////////////////////////////////////
CRegEx::CRegEx()
{}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			July 2008
///	Change History:	Initially Created
////////////////////////////////////////////////////////////////////////////////
CRegEx::~CRegEx()
{}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			July 2008
///	Change History:	Initially Created
////////////////////////////////////////////////////////////////////////////////
int CRegEx::Find(LPCWSTR inputString, LPCWSTR patternToMatch)
{
	//  Simply return the result of a find and replace with no replace expression
	return FindAndReplace(inputString, patternToMatch, NULL, NULL, NULL);
}

////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			July 2008
///	Change History:	Initially Created
////////////////////////////////////////////////////////////////////////////////
int CRegEx::FindAndReplace(LPCWSTR inputString, LPCWSTR patternToMatch, LPCWSTR replaceExpression, int* lengthOfResult, TCHAR* result)
{
	//  Default value to return;
	int returnVal = -1;
	//  local copy of the length of the result string, this value will be returned 
	//  in the lengthOfResult parameter if applicable
	int lengthOfResultLocal = 0;

	//  User has specified null input, return no match.
	if (inputString == NULL || patternToMatch == NULL)
		return returnVal;

	//  Create an instance of the open source CRegExp class
	CRegExp* reLogic = new CRegExp();

	//  CRegExp requites standard char* strings, therefore convert the wide input parameters into
	//  compatible char arrays
	char *inputStringAsChar = (char*)malloc(wcslen(inputString)+1);
	char *patternToMatchAsChar = (char*)malloc(wcslen(patternToMatch)+1);
	int iReplaceExpLength;
	if (replaceExpression == NULL)
		iReplaceExpLength = 0;
	else
		iReplaceExpLength = wcslen(replaceExpression);
	char *replaceExpressionAsChar = (char*)malloc((iReplaceExpLength)+1);

	memset(inputStringAsChar,0, wcslen(inputString)+1);
	wcstombs(inputStringAsChar, inputString, wcslen(inputString));
	memset(patternToMatchAsChar, 0, wcslen(patternToMatch)+1);
	wcstombs(patternToMatchAsChar, patternToMatch, wcslen(patternToMatch));
	
	//  The replace expressions may not be specified if the user only wishes to perform a find
	if (replaceExpression != NULL)
	{
		memset(replaceExpressionAsChar, 0, wcslen(replaceExpression)+1);
		wcstombs(replaceExpressionAsChar, replaceExpression, wcslen(replaceExpression));
	}

	//  The Pattern needs to be less than 120 characters, anything over this 
	//  is too complex for the regular expression parser.
	int nPos;
	//  Compile the specified regular expression
	reLogic->CompileRegularExpression(patternToMatchAsChar);

	//  Attempt to match the given regular expression to the Input String
	nPos = reLogic->MatchString(inputStringAsChar);

	if (nPos != -1)
	{
		//  Successfully parsed
		//  Do the replace if we have been provided with a replace expression and storage
		//  buffer for the output.
		if (replaceExpression != NULL)
		{	
			char *pReplaceStr = reLogic->ReplaceString(replaceExpressionAsChar);
			lengthOfResultLocal = strlen(pReplaceStr);
			if (result != NULL)
				mbstowcs(result, pReplaceStr, lengthOfResultLocal+1);

			//pReplaceStr goes out of scope here
			//memoery was allocated in reLogic but not deleted
			delete [] pReplaceStr;
			pReplaceStr = NULL;

		}
		returnVal = nPos;
	}
	else
	{
		//  No match Found
		//  Copy inputString to replace expression
		lengthOfResultLocal = wcslen(inputString);
		if (result != NULL)
		{
			wcscpy(result, inputString);
		}

		returnVal = nPos;
	}

	//  Populate the lengthOfResult int*
	//  +1 for the terminating NULL character
	if (lengthOfResult != NULL)
		*lengthOfResult = lengthOfResultLocal + 1;

	//  Free the resources used by this function
	free(inputStringAsChar);
	free(patternToMatchAsChar);
	free(replaceExpressionAsChar);
	delete reLogic;
	return returnVal;
}