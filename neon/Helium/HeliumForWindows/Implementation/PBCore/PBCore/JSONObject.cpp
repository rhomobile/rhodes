#include "JSONObject.h"

extern BOOL Log	(LogTypeInterface logSeverity,LPCTSTR pLogComment, 
									LPCTSTR pFunctionName, DWORD dwLineNumber,LPCTSTR pCallingModule);


JSONObject::JSONObject() 
{

}

JSONObject::~JSONObject() 
{
	//  Free the memory associated with each of the Tuples
	for (uint i = 0; i < vTuples.size(); i++)
	{
		delete[] vTuples.at(i);
	}
	vTuples.clear();
}

//  Relies on 2D Javascript notation not putting a gap between the 'new array' and the '('
int JSONObject::put(TCHAR* tcName, LPCTSTR tcValue) 
{
	int iLengthOfAddedString = 0;
	//  tcValue may be a value, a javascript array or a javascript 2D array
	//  tcName may be a name or comma separated names (for a 2D array)
	TCHAR* tcPositionOfNewArray = NULL;
	if (tcName == NULL || tcValue == NULL)
		return 0;
	else
	{
		tcPositionOfNewArray = wcsistr(tcValue, L"new Array(");
		if (tcPositionOfNewArray)
		{
			//  The value contains a specification of a Javascript array, check
			//  to see if it is a 2D specification
			tcPositionOfNewArray = wcsistr(tcPositionOfNewArray + wcslen(L"new Array("), L"new Array(");
			if (tcPositionOfNewArray)
			{
				//  tcValue contains the definition of a 2D javascript array
				//  tuple should be of the form name:[{name:value,name:value},{name:value,name:value},{...}]
				//  Javascript is of the form "new Array(new Array(value, value), new Array(value, value))
				//  tcName contains values in the form: ComponentVersions=Module,Version Where ComponentVersions
				//  is the name of an array of objects and Module / Version are the IDs of the values in the object
				//  First separate the tcName into its constituents

				CStr tempTuple;
				tempTuple.SetChunkSize(MAXURL/2);
				//TCHAR tempTuple[MAXURL];  //  Temp area to hold 2D data as this will get too complicated to assign memory each time
				//memset(tempTuple, 0, MAXURL * sizeof(TCHAR));
		
				TCHAR* tcNamesArray[15];	//  Increased Array Size to 15 to cater for new RFID plugin
				TCHAR tcNameCopy[MAXURL];
				wcscpy(tcNameCopy, tcName);
				TCHAR* tcNamesToken = wcstok(tcNameCopy, L",= ");
				int iNamesFound = 0;
				while (tcNamesToken != NULL)
				{
					//  While there are names left to put in the Names Array
					tcNamesArray[iNamesFound] = new TCHAR[wcslen(tcNamesToken) + 1];
					wcscpy(tcNamesArray[iNamesFound], tcNamesToken);
					tcNamesToken = wcstok(NULL, L",= ");
					iNamesFound++;
				}
				//  tcNamesArray now contains all the names to provide the LHS of the JSON objects
				if (iNamesFound < 1)
					return 0;
				tempTuple = L"'";
				tempTuple += tcNamesArray[0];
				tempTuple += L"':[";
				
				//  Loop through the tcValue string looking for 'new array', each time a 'new array' is found
				//  then a new JSON object should be created
				TCHAR* tcNewArrayContents = tcPositionOfNewArray;
				while (tcNewArrayContents != NULL)
				{
					tcNewArrayContents += wcslen(L"new Array(");
					//  Create the JSON object for each 'new array('
					//  Expect to find iNamesFound - 1 attributes in each array
					TCHAR* tcPositionOfCloseBracket = wcsistr(tcNewArrayContents, L")");
					if (tcPositionOfCloseBracket)
					{
						int iCurrentName = 1;
						//  Start new Object
						tempTuple += L"{";
						//  Write the name associated with this value
						tempTuple += L"'";
						tempTuple += tcNamesArray[iCurrentName];
						
						tempTuple += L"':";
						iCurrentName++;
						//  Tokenize the Values found between 'tcNewArrayContents and tcPositionOfCloseBracket
						bool bInString = false;
						for (int iVal = 0; iVal < (tcPositionOfCloseBracket - tcNewArrayContents); iVal++)
						{
							if (tcNewArrayContents[iVal] == '\'')
							{
								//  Start / end of String
								bInString = !bInString;
								tempTuple += tcNewArrayContents[iVal];
								//wcsncat(tempTuple, tcNewArrayContents + iVal, 1);
							}
							else if (tcNewArrayContents[iVal] == ',' && !bInString)
							{
								//  Check to see if number of names and values correspond (not too many values)
								if (iCurrentName == iNamesFound)
									return 0;
								//  End of this particular value, write the naxt name
								tempTuple += L",'";
								tempTuple += tcNamesArray[iCurrentName];
								tempTuple += L"':";
								//wcscat(tempTuple, L",'");
								//wcscat(tempTuple, tcNamesArray[iCurrentName]);
								//wcscat(tempTuple, L"':");
								iCurrentName++;

							}
							else
							{
								tempTuple += tcNewArrayContents[iVal];
								//wcsncat(tempTuple, tcNewArrayContents + iVal, 1);
							}
						}
						//  End New Object
						tempTuple += L"},";
						//wcscat(tempTuple, L"},");
					}
					//  Find next 'new array('
					tcNewArrayContents = wcsistr(tcPositionOfCloseBracket, L"new Array(");
				}
				//  Remove the final ','
				if (tempTuple[tempTuple.GetLength() - 1] == L',')
					tempTuple.InsertAt(tempTuple.GetLength() - 1,L']');
					//tempTuple[wcslen(tempTuple) - 1] = L']';
				else
					tempTuple+= L"]";
					//wcscat(tempTuple, L"]");
				//TCHAR* tcNewTuple = new TCHAR[wcslen(tempTuple) + 1];
				TCHAR* tcNewTuple = new TCHAR[tempTuple.GetLength() + 1];
				tempTuple.ToWCharArray(tcNewTuple,tempTuple.GetLength() + 1);
				//wcscpy(tcNewTuple, );
				vTuples.push_back(tcNewTuple);
				iLengthOfAddedString = wcslen(tcNewTuple);

				//  Clean up
				//  Delete all Names created in the Names Array
				for (int i = 0; i < iNamesFound; i++)
					delete[] tcNamesArray[i];
			}
			else
			{
				//  tcValue contains the definition of a 1D javascript array.
				//  tuple should be of the form name:[value,value,value,value]
				//  Format of JS Array is:	new Array('1','2','3')
				//  Format of JSON Array is:['1','2','3'] 
				TCHAR* tcPositionOfNewArray = wcsistr(tcValue, L"new array(");
				int iLengthOfNamePortion = wcslen(tcName);
				int iLengthOfValuePortion = wcslen(tcPositionOfNewArray + wcslen(L"new array("));
				TCHAR* tcPositionOfCloseBracket = wcsistr(tcPositionOfNewArray, L")");
				if (tcPositionOfNewArray && tcPositionOfCloseBracket)
				{
					TCHAR* tcNewTuple = new TCHAR[iLengthOfNamePortion + iLengthOfValuePortion + wcslen(L"'':[] ")];
					wcscpy(tcNewTuple, L"'");
					wcscat(tcNewTuple, tcName);
					wcscat(tcNewTuple, L"':[");
					wcsncat(tcNewTuple, tcPositionOfNewArray + wcslen(L"new array("),
						tcPositionOfCloseBracket - (tcPositionOfNewArray + wcslen(L"new array(")));
					wcscat(tcNewTuple, L"]");
					vTuples.push_back(tcNewTuple);
					iLengthOfAddedString = wcslen(tcNewTuple);
				}
			}
		}
		else
		{
			//  Tuple is the simple case of name:value where name and value are both strings
			TCHAR* tcNewTuple = new TCHAR[wcslen(tcName) + (2 * wcslen(tcValue)) + 6];  //  +6 for '', : and \0 and assume all value characters escaped
			if (tcNewTuple == NULL)
			{
				WCHAR* szLog = new WCHAR[70 + wcslen(tcName)];
				if (szLog)
				{
					wsprintf(szLog, L"Unable to allocate sufficient memory for JSON parameter \"%s\"", tcName);
					Log(PB_LOG_WARNING, szLog,
						_T(__FUNCTION__), __LINE__, L"JSONObject");
					delete[] szLog;
				}
				return 0;
			}
			else
			{
				wcscpy(tcNewTuple, L"'");
				wcscat(tcNewTuple, tcName);
				wcscat(tcNewTuple, L"':'");
				TCHAR* tcEscapedValue = new TCHAR[(2 * wcslen(tcValue)) + 1];
				memset(tcEscapedValue, 0, (2 * wcslen(tcValue)) + 1);
				CopyString(tcEscapedValue, (TCHAR*)tcValue, true);
				wcscat(tcNewTuple, tcEscapedValue);
				wcscat(tcNewTuple, L"'");
				vTuples.push_back(tcNewTuple);
				iLengthOfAddedString = wcslen(tcNewTuple);
				delete[] tcEscapedValue;
			}
		}
	}
	return iLengthOfAddedString;
}

bool JSONObject::toString(TCHAR* tcJSONAsString, int iMaximumLength) 
{
	//  Enumerate over the Tuples and create the JSON object
	int iLengthOfJSONObject = 1;  //  For opening {
	for (uint i = 0; i < vTuples.size(); i++)
	{
		iLengthOfJSONObject += wcslen(vTuples.at(i));
		iLengthOfJSONObject++;	//  For ,'s and final }
	}
	iLengthOfJSONObject++;  //  For \0
	if (iLengthOfJSONObject > iMaximumLength)
		return false;
	else
	{
		wcscpy(tcJSONAsString, L"{");
		for (uint j = 0; j < vTuples.size(); j++)
		{
			wcscat(tcJSONAsString, vTuples.at(j));
			if (j != vTuples.size() - 1)
				wcscat(tcJSONAsString, L",");
		}
		wcscat(tcJSONAsString, L"}");
		return true;
	}
}



////////////////////////////////////////////////////////////////////////////////
/// Author:			Darryn Campbell (DCC)
/// Date:			May 2008
///	Change History:	Initially Created
////////////////////////////////////////////////////////////////////////////////
TCHAR* wcsistr(LPCTSTR szStringToBeSearched, LPCTSTR szSubstringToSearchFor)
{
	TCHAR* pPos = NULL;
	//  Copies of strings to avoid lowering the case of the actual strings
	TCHAR* szCopy1 = NULL;
	TCHAR* szCopy2 = NULL;

	// verify parameters
	if ( szStringToBeSearched == NULL ||
			szSubstringToSearchFor == NULL ||
			wcslen(szStringToBeSearched) < wcslen(szSubstringToSearchFor))
	{
	  return NULL;
	}

	// empty substring - return input (consistent with strstr)
	if (wcslen(szSubstringToSearchFor) == 0 )
	{
		return (TCHAR*)szStringToBeSearched;
	}

	//  duplicate strings and convert to lower case
	szCopy1 = _wcslwr(_wcsdup(szStringToBeSearched));
	szCopy2 = _wcslwr(_wcsdup(szSubstringToSearchFor));

	if ( szCopy1 == NULL || szCopy2 == NULL  ) 
	{
		//  Problem converting strings to lower case, return
		free((void*)szCopy1);
		free((void*)szCopy2);
		return NULL;
	}

	//  use strstr on the lower case copies.
	pPos = wcsstr(szCopy1, szCopy2);

	if ( pPos != NULL ) {
		// map to the original string
		pPos = (TCHAR*)szStringToBeSearched + (pPos - szCopy1);
	}

	free((void*)szCopy1);
	free((void*)szCopy2);

	return pPos;
}
