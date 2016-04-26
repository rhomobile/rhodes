#include "PBlib.h"

//  Private Function Declarations
UINT CopyString (TCHAR *Dst, TCHAR *Src);
//  End Private Function Declarations

//////////////////////////////////////////////////
//												//
//		Begin Exported Functions				//
//												//
//////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Function:	PBformat
// Description:	Function is used instead of the wsprintf function in the navigate functions.
//				This override accepts a variable length list of arguements.  iNoParams is the length
//				of the list.
// Author:		Darryn Campbell
// Date:		July 2008
////////////////////////////////////////////////////////////////////////
void PBformat(BOOL bEscapeOutput, int iNoParams, TCHAR *Destination, TCHAR *Format, ...) 
{
	//Declare a va_list macro and initialize it with va_start to handle the variable parameters
	TCHAR *l_ParamVal;
	va_list l_Arg;
	va_start(l_Arg, Destination);
	l_ParamVal = va_arg(l_Arg,TCHAR*);	// Move past the Format String

	//  First loop through the variable parameters list and store them, avoids
	//  possible n-squared complexity accessing numbered positional parameters
	TCHAR** parameterList = new TCHAR*[iNoParams];
	for(int param = 0; param < iNoParams; param++)
	{
		TCHAR* parameter = new TCHAR[MAXURL+1];
		memset(parameter, 0, sizeof(TCHAR) * MAXURL);
		parameterList[param] = parameter;
		l_ParamVal = va_arg(l_Arg, TCHAR*);
		if ((l_ParamVal != NULL) && (l_ParamVal[0] != NULL))
			wcscpy(parameterList[param], l_ParamVal);
	}
	//  now we have made a copy of the parameters we can close the variable list
	va_end(l_Arg);

	PBformat(bEscapeOutput, iNoParams, Destination, Format, parameterList);
}

////////////////////////////////////////////////////////////////////////
// Function:	PBformat
// Description:	Function is used instead of the wsprintf function in the navigate functions.  
//				This override accepts an array of TCHAR*.  iNoParams is the number of elements
//				in the array.
// Author:		Darryn Campbell
// Date:		August 2008
////////////////////////////////////////////////////////////////////////
void PBformat(BOOL bEscapeOutput, int iNoParams, TCHAR *Destination, TCHAR *Format, TCHAR** parameterList) 
{
	if ((Destination == NULL) || (Format == NULL)) return;
	if (Format[0] == NULL) return;

	int parametersLeftToProcess = iNoParams;
	//  Currently copying-to character in the Destination array
	UINT DestPtr = 0;	
	//  Variable used to keep track of the currently processing parameter, used
	//  to enable a mixiture of %s and %<number>
	UINT currentParameter = 0;
	
	//  Look for %s or %<number> in the Format string and 
	//  replace with the appropriate list arguement
	for (DWORD i = 0; i < wcslen(Format); i++)
	{
		//  Handle the character sequence "\%", we wish to replace this in the destination
		//  string with the % character and not action the character after it.
		if (parametersLeftToProcess > 0 && Format[i] == L'\\' && i+1 < wcslen(Format))
		{
			if (Format[i+1] == '%')
			{
				//  We are escaping the % character, copy the % and advance the Dest 
				//  and format character pointers
				Destination[DestPtr++] = Format[i+1];
				i+=2;
			}
		}
		//  If there are parameters left to process and the currently 
		//  considering character is a parameter then process it
		if (parametersLeftToProcess > 0 && Format[i] == L'%')
		{
			//  Replace the parameter with the appropriate value in the destination
			//  string
			//  Check whether format specifier is %s or %<number>
			if (wcslen(&Format[i]) > 1 && (Format[i+1] == L's' || Format[i+1] == L'S')) 
			{
				//  %s format used, replace with next parameter
				if (wcslen(parameterList[currentParameter]) + DestPtr + 1 <= MAXURL)
				{
					DestPtr += CopyString(&Destination[DestPtr], parameterList[currentParameter]);
					DestPtr += wcslen(parameterList[currentParameter]);
					i++;
				}
				else
				{
					//  Replacing this parameter would exceed the maximum URL
					int length = wcslen(parameterList[currentParameter]);
					break;
				}
			}
			else if (wcslen(&Format[i]) > 1)
			{
				//  %<number> format used, check number is valid
				int replacingParameter = _wtoi(&Format[i+1]);
				if (replacingParameter > 0 && replacingParameter <= iNoParams)
				{
					//  Replace with the specified parameter
					if (wcslen(parameterList[replacingParameter-1]) + DestPtr + 1 <= MAXURL)
					{
						DestPtr += CopyString(&Destination[DestPtr], parameterList[replacingParameter-1]);
						DestPtr += wcslen(parameterList[replacingParameter-1]);
						TCHAR replacingParameterAsString[5];
						memset(replacingParameterAsString, 0, sizeof(TCHAR) * 5);
						_itow(replacingParameter, replacingParameterAsString, 10);
						i+=wcslen(replacingParameterAsString);
					}
					else
					{
						//  Replacing this parameter would exceed the maximum URL
						break;
					}
				}
				else
				{
					//  character following '%' is out of range or not a number,
					//  copy the '%' character to the destination string
					Destination[DestPtr++] = Format[i];
					continue;
				}
			}
			else
			{
				//  We are processing a '%' at the end of the Format string, do a direct copy
				Destination[DestPtr++] = Format[i];
				continue;
			}

			parametersLeftToProcess--;
			currentParameter++;
		}
		else
		{
			//  The currently considering character does not need to be 
			//  replaced with a parameter.
			if (parametersLeftToProcess == 0)
			{			
				//  No Parameters left to process, copy remainder of String and finish
				CopyString(&Destination[DestPtr], &Format[i]);				
				break;
			}
			else
			{	
				//  Copy a single character of the format string to the destination.
				Destination[DestPtr++] = Format[i];				
			}
		}	
	}

	if (bEscapeOutput)
		Escape(Destination);

	//  Delete the Parameter List Array
	for (int param = 0; param < iNoParams; param++)
	{
		delete [] parameterList[param];
	}
	delete [] parameterList;
	return;
}

////////////////////////////////////////////////////////////////////////
// Function:	Escape
// Description:	Function to convert the given string to its equivilent in Hex ASCII.  Upto a max length of	MAXURL
// Example:		Escape('hi world'); will convert the passed string to '%68%69%20%77%6f%72%69%64'
// Author:		Darryn Campbell
// Date:		July 2008
////////////////////////////////////////////////////////////////////////
void Escape(TCHAR *stringToEscape)
{
	//  Make a copy of the given String
	TCHAR destcpy[MAXURL+1];
 	wcscpy(destcpy, stringToEscape);
	TCHAR characterToCopy;
	//  Declare an additional pointer to the passed string, allows us to iterate over the passed string modifying
	//  it and still keep track of where the beginning of the string was
	TCHAR *stringIter = stringToEscape;

	//  Iterate over the copy of the original string received.  Note that the escaped string will
	//  be 3 * the length of the unescaped string, hence the limit being MAXURL / 3.
	for(int i = 0; i < MAXURL / 3; i++)
	{
		characterToCopy = destcpy[i];
		//  Test to see if end of copying string reached.
		if (characterToCopy == '\0')
			break;
		//  For each character in the original string modify the passed string
		//  to contain have 3 characters, one for a '%' and two to represent the copied character's
		//  ASCII code in hex.
		wsprintf(stringToEscape, L"%c", '%');
		stringToEscape++;
		wsprintf(stringToEscape, L"%02x", (int)characterToCopy);
		stringToEscape+=2;
	}
	//  Just in case the string isn't null terminated though it should be.
	wsprintf(stringToEscape, L"%c", '\0');
	//  Move passed string pointer back to the beginning of the string being escaped.
	stringToEscape = stringIter;
}

////////////////////////////////////////////////////////////////////////
// Function:	cmp
// Description:	compares to TCHARs - case insensitive
// Author:		James Morley-Smith
// Date:		December 2004
////////////////////////////////////////////////////////////////////////
BOOL cmp(LPCTSTR tc1, LPCTSTR tc2)
{
	if (!tc1 || !tc2)
		return false;

	return !_memicmp(tc1, tc2, wcslen(tc1)*sizeof(TCHAR));
}
////////////////////////////////////////////////////////////////////////
// Function:	cmp
// Description:	compares to TCHARs - case insensitive
// Author:		James Morley-Smith
// Date:		December 2004
////////////////////////////////////////////////////////////////////////
BOOL cmp(LPCTSTR tc1, LPCTSTR tc2, int iChars)
{
	if (!tc1 || !tc2)
		return false;

	return !_memicmp(tc1, tc2, iChars*sizeof(TCHAR));
}


//////////////////////////////////////////////////
//												//
//			End Exported Functions				//
//												//
//////////////////////////////////////////////////

//////////////////////////////////////////////////
//												//
//			Begin Private Functions				//
//												//
//////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
// Function:	CopyString
// Description:	Inserts '\' char before special chars like
//				'\n', '\r' etc while copying chars from Src to Dst 
//              To fix the Javascript errors
// Authors:		Sudhakar Murthy
// Date:		17/3/2008
// Modified:	The original placed a '\' before the control character when it needed to place 
// the appropriate break char such as '\n\r'		
// Author		Paul Henderson
// Date			20/5/2008
////////////////////////////////////////////////////////////////////////
UINT CopyString (TCHAR *Dst, TCHAR *Src)
{
	
	UINT cnt = 0; // spcial char counter
	TCHAR tc = 0;
	while (*Src != L'\0')
	{
		switch(*Src)
		{
		case L'\f':
			tc = L'f';
			break;
		case L'\n':
			tc = L'n';
			break;
		case L'\r':
			tc = L'r';
			break;
		case L'\"':
			tc = L'"';
			break;
		} // end of switch(Src[is])

		if(tc)
		{
			*Dst++	= L'\\';
			*Dst++	= tc;
			cnt++;
		}
		else
			*Dst++	= *Src;

		Src++;
		tc = 0;

	} // end of while (*Src != L'\0')

	*Dst = L'\0'; // NULL string char at the end

	return cnt; // return number of '\\' chars inserted during Src to Dst copy
}

//////////////////////////////////////////////////
//												//
//			End Private Functions				//
//												//
//////////////////////////////////////////////////
