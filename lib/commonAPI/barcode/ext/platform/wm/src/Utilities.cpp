#include "Scanner.h"

static const char scn_base64_chars[] = 
						 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
						 "abcdefghijklmnopqrstuvwxyz"
						 "0123456789+/";

//	Compares two strings or part of strings returns TRUE if 
//	same else returns FALSE calls parent class implementation.
BOOL CScanner::cmp(LPCTSTR tc1, LPCTSTR tc2)
{
	if (!tc1 || !tc2)
		return false;
	return !_wcsicmp(tc1, tc2);
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
	else if (wcsicmp(szString, L"True") == 0)
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
	else if (wcsicmp(szString, L"False") == 0)
		return TRUE;
	else
		return FALSE;
}

unsigned int CScanner::StringLength(DWORD iNumber)
{
	//  Only works for positive numbers
	//  a = b^c then loga (base)b = c
	if (iNumber == 0)
		return 1;
	else
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
		return _wtoi(value);
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

//  Encodes the specified byte array into base64 encoding, this is used for
//  DataURI encoding.
//  szEncoded data MUST be allocated by the caller and must be of an appropriate
//  length, note: base 64 encoded data is about 1/3 larger than the byte equivilent
//  iLen is the Length of the bytes to be encoded.
void CScanner::EncodeToBase64(unsigned char const* bytes_to_encode, unsigned int iLen, char* szEncodedData)
{
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (iLen--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) 
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
			{
				*szEncodedData = scn_base64_chars[char_array_4[i]];
				szEncodedData++;
			}
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
		{
			*szEncodedData = scn_base64_chars[char_array_4[j]];
			szEncodedData++;
		}

		while((i++ < 3))
		{
			*szEncodedData = '=';
			szEncodedData++;
		}
	}
}