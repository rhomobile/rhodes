#include "windows.h"
#include <math.h>
#include "../logging/RhoLog.h"
#include "RhoUtil.h"

namespace rho {
	namespace common {
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
		void GetDataURI (BYTE* bData, int iLength, rho::String& data)
		{
			//  Convert the Signature to base 64, this representation is about 1/3
			//  larger than the binary input
			char* szEncodedString = new char[iLength * 2 + 30];
			memset(szEncodedString, 0, iLength * 2 + 30);

			// Start with the data header
			strcpy(szEncodedString, "data:image/jpeg;base64,");

			// Now append the encoded data itself
			rho::common::EncodeToBase64(bData, iLength, szEncodedString + strlen (szEncodedString));

			// Copy it to the caller
			data = szEncodedString;

			// Clean up
			delete[] szEncodedString;
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
		bool GetJpegResolution(BYTE* pData, DWORD buffSize, int& nWidth, int& nHeight)
		{
			bool bRetStatus = false;
			unsigned int bufferIndex = 0; //use to iterate over the buffer
			nWidth = 0;
			nHeight=0;
			//Check whether file starts with 0xFFD8 and 0xFFE0
			//Any jpeg image should start with these markers
			if(pData[bufferIndex] == 0xFF && pData[bufferIndex+1] == 0xD8 && 
				pData[bufferIndex+2] == 0xFF && pData[bufferIndex+3] == 0xE0) 
			{
				//move to segment length of section:0xFFE0
				bufferIndex = bufferIndex + 4;
				// Check for "JFIF", see this in the data section of the segment, if true proceed further
				if(pData[bufferIndex+2] == 'J' && pData[bufferIndex+3] == 'F' && pData[bufferIndex+4] == 'I' && 
					pData[bufferIndex+5] == 'F' && pData[bufferIndex+6] == 0x00) 
				{
					//get block length of each segment and skip segments, till we reach our marker 0xFFC0
					unsigned short segment_length = pData[bufferIndex] << 8 | pData[bufferIndex+1]; //convert big endian issue by shift operater (two bytes is the segment length)
					while( bufferIndex < buffSize)
					{
						bufferIndex = bufferIndex + segment_length; // skip the segment and point to the next segment in the buffer

						WCHAR szLog[512];
								wsprintf(szLog, L"bufferIndex = %d, segment_length = %d, buff size =%d", 
									bufferIndex, segment_length, buffSize);
								LOG(INFO) + szLog;
						if(bufferIndex >= buffSize) 
						{
							LOG(INFO) + L"segment is corrupted";
							break;
						}
						if(pData[bufferIndex] == 0xFF) 
						{	
							//check whether the segment is start of frame 0xFFC0
							if(pData[bufferIndex+1] == 0xC0)
							{   
								//start of frame segment looks as shown below
								//[0xFFC0][unsigned short][unsigned char precision][unsigned short][unsigned short]
								//              |                                     |                |
								//              |                                     |                |
								//              -->length of segment                  |                |
								//                                                    -->height        |
								//                                                                     -->width
								//--------------------------------------------------------------------------------------


								nHeight = (pData[bufferIndex+5]<< 8) | pData[bufferIndex+6];
								nWidth = (pData[bufferIndex+7] << 8) | pData[bufferIndex+8];
								bRetStatus = true;
								break;
							}
							else
							{
								bufferIndex = bufferIndex +2;//skip segment marker, sothat it points length of the segment
								segment_length = (pData[bufferIndex] << 8) | pData[bufferIndex+1];   //Go to next segment
							}
						}
						else
						{
							LOG(INFO) + L"segment is corrupted, not pointing to the start of next segment";
							break;
						}
					}
					if(false == bRetStatus)
					{
						LOG(INFO) + L"Could not get the size info";
					}
				}
				else
				{ 
					LOG(INFO) + L"Could not find JFIF in marker 0xFFE0";
				}                  

			}
			else
			{ 
				LOG(INFO) + L"Invalid SOI header";
			}                   
			return bRetStatus;
		}
		bool GetJpegResolution(LPCTSTR szFileName, int& nWidth, int& nHeight)
		{
			bool bRetStatus = false;
			//  read the file from disk.
			HANDLE hFile = CreateFile(szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, NULL);

			if(hFile)
			{
				LPVOID pImageBuffer;///< Buffer store the image						
				DWORD dwFileSize = GetFileSize(hFile, NULL);
				if (dwFileSize > 0)
				{		
					DWORD dwBytesRead = 0;
					pImageBuffer = new BYTE[dwFileSize];
					if(pImageBuffer)
					{
						bool bFileReadSuccess = true;
						do
						{
							if (!ReadFile(hFile, pImageBuffer, dwFileSize, &dwBytesRead, NULL))
							{
								//  Some error has occured reading the file
								LOG(INFO) + L"Unable to read image";	
								bFileReadSuccess = false;
								break;
							}
						}while (dwBytesRead != 0);

						if(bFileReadSuccess)
						{
							bRetStatus = GetJpegResolution((BYTE*)pImageBuffer, dwFileSize, nWidth, nHeight);
						}
						delete[] pImageBuffer;
						pImageBuffer = NULL;
					}


				}
				CloseHandle(hFile);
			}
			else
			{
				LOG(INFO) + L"Unable to find the image";
			}
			return bRetStatus;
		}
		BOOL cmp(LPCTSTR tc1, LPCTSTR tc2)
		{
			if (!tc1 || !tc2)
				return false;
			return !_tcsicmp(tc1, tc2);

		}

	}
}