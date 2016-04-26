#include "Scanner.h"

//	Compares two strings or part of strings returns TRUE if 
//	same else returns FALSE calls parent class implementation.
BOOL CScanner::cmp(LPCTSTR tc1, LPCTSTR tc2)
{
	return m_pModule->cmp(tc1, tc2);
}

int CScanner::StringToInt(LPCTSTR wInString)
{
	if (!wInString)
		return 0;
	else
		return _httoi(wInString);
}

BOOL CScanner::EnabledValue(LPCTSTR szString)
{
	//  Returns true if the parameter passed to it represents enable or enabled.
	//  Gives backward compatibility with earlier Browser syntax.
	if (NULL == szString)
		return FALSE;
	else if (wcsicmp(szString, L"Enable") == 0)
		return TRUE;
	else if (wcsicmp(szString, L"Enabled") == 0)
		return TRUE;
	else
		return FALSE;
}

BOOL CScanner::DisabledValue(LPCTSTR szString)
{
	//  Returns true if the parameter passed to it represents disable or disabled.
	//  Gives backward compatibility with earlier Browser syntax.
	if (NULL == szString)
		return FALSE;
	else if (wcsicmp(szString, L"Disable") == 0)
		return TRUE;
	else if (wcsicmp(szString, L"Disabled") == 0)
		return TRUE;
	else
		return FALSE;
}

unsigned int CScanner::StringLength(DWORD iNumber)
{
	//  Only works for positive numbers
	//  a = b^c then loga (base)b = c
	return ((int)(log10((double)iNumber))) + 1;
}

int CScanner::ProcessDecoderPropertyForReturn(LPCTSTR szPropertyValue, WCHAR* szPropertyBuffer)
{
	int iRetVal = wcslen(szPropertyValue) + 1;
	if (szPropertyBuffer)
		if (wcscpy_s(szPropertyBuffer, iRetVal, szPropertyValue) == EINVAL)
			iRetVal = -1;
	return iRetVal;
}

int CScanner::ProcessDecoderPropertyForReturn(DWORD iPropertyValue, WCHAR* szPropertyBuffer)
{
	int iRetVal = StringLength(iPropertyValue) + 1;
	if (szPropertyBuffer)
		wsprintf(szPropertyBuffer, L"%i", iPropertyValue);
	return iRetVal;
}

int CScanner::_httoi(const TCHAR *value)
{
  if (wcslen(value) <= 2 || (wcslen(value) > 2 && (wcsnicmp(value, L"0x", 2) != 0)))
  {
    return _wtoi(value);
  }
  struct CHexMap
  {
    TCHAR chr;
    int value;
  };
  const int HexMapL = 16;
  CHexMap HexMap[HexMapL] =
  {
    {'0', 0}, {'1', 1},
    {'2', 2}, {'3', 3},
    {'4', 4}, {'5', 5},
    {'6', 6}, {'7', 7},
    {'8', 8}, {'9', 9},
    {'A', 10}, {'B', 11},
    {'C', 12}, {'D', 13},
    {'E', 14}, {'F', 15}
  };
  TCHAR *mstr = _tcsupr(_tcsdup(value));
  TCHAR *s = mstr;
  int result = 0;
  if (*s == '0' && *(s + 1) == 'X') s += 2;
  bool firsttime = true;
  while (*s != '\0')
  {
    bool found = false;
    for (int i = 0; i < HexMapL; i++)
    {
      if (*s == HexMap[i].chr)
      {
        if (!firsttime) result <<= 4;
        result |= HexMap[i].value;
        found = true;
        break;
      }
    }
    if (!found) break;
    s++;
    firsttime = false;
  }
  free(mstr);
  return result;
}
