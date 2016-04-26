#include "windows.h"
#include <math.h>
static const char base64_chars[] = 
						 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
						 "abcdefghijklmnopqrstuvwxyz"
						 "0123456789+/";

unsigned int StringLength(DWORD iNumber)
{
	//  Only works for positive numbers
	//  a = b^c then loga (base)b = c
	if (iNumber <= 0)
		return 1;
	else
		return ((int)(log10((double)iNumber))) + 1;
}

//  Encodes the specified byte array into base64 encoding, this is used for
//  DataURI encoding.
//  szEncoded data MUST be allocated by the caller and must be of an appropriate
//  length, note: base 64 encoded data is about 1/3 larger than the byte equivilent
//  iLen is the Length of the bytes to be encoded.
void EncodeToBase64(unsigned char const* bytes_to_encode, unsigned int iLen, char* szEncodedData)
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
				*szEncodedData = base64_chars[char_array_4[i]];
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
			*szEncodedData = base64_chars[char_array_4[j]];
			szEncodedData++;
		}

		while((i++ < 3))
		{
			*szEncodedData = '=';
			szEncodedData++;
		}
	}
}

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

void DecodeFromBase64(unsigned char const* encoded_string, int iLengthOfEncodedString, char* szDecodedData) 
{
  int in_len = iLengthOfEncodedString;
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
	  {
		  for(unsigned int j = 0; j < strlen(base64_chars); j++)
		  {
			if (base64_chars[j] == char_array_4[i])
			{
				char_array_4[i] = j;
				break;
			}
		  }
	  }

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
	  {
		  *szDecodedData = char_array_3[i];
		  *szDecodedData++;
	  }
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
	{

		  for(unsigned int m = 0; m < strlen(base64_chars); m++)
		  {
			if (base64_chars[m] == char_array_4[j])
			{
				char_array_4[j] = m;
				break;
			}
		  }
	}

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) 
	{
		  *szDecodedData = char_array_3[j];
		  *szDecodedData++;
	}
  }
}

void GetApplicationVersion(int* iV1, int* iV2, int* iV3, int* iV4)
{
#ifdef _WIN32_WCE
	VS_FIXEDFILEINFO m_vsAPPVer;
	memset(&m_vsAPPVer, 0, sizeof(VS_FIXEDFILEINFO));
	DWORD dwLen, dwUseless = 0;
    LPTSTR lpVI;
    DWORD dwVersion = 0;

	wchar_t wcFilename[256];
	if(wcslen(L"")==0)
		GetModuleFileName(NULL, wcFilename, 255);
    dwLen = GetFileVersionInfoSize(wcFilename, &dwUseless);
    if (dwLen==0) 
	{
        return;
	}
    lpVI = (LPTSTR) GlobalAlloc(GPTR, dwLen);
    if (lpVI)
    {
        DWORD dwBufSize;
        VS_FIXEDFILEINFO* lpFFI;
        GetFileVersionInfo(wcFilename, NULL, dwLen, lpVI);

        if (VerQueryValue(lpVI, (L"\\"), (LPVOID *) &lpFFI, (UINT *) &dwBufSize))
		{
			memcpy(&m_vsAPPVer, lpFFI, sizeof(VS_FIXEDFILEINFO));
            dwVersion = lpFFI->dwFileVersionMS;
		}
        GlobalFree((HGLOBAL)lpVI);
    }

	*iV1 = HIWORD(m_vsAPPVer.dwFileVersionMS);
	*iV2 = LOWORD(m_vsAPPVer.dwFileVersionMS);
	*iV3 = HIWORD(m_vsAPPVer.dwFileVersionLS);
	*iV4 = LOWORD(m_vsAPPVer.dwFileVersionLS);
#endif// _WIN32_WCE
}