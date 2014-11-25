/** Declaration if type for exported functions from DLL */
#define EXPORT_TYPE extern "C" __declspec(dllexport)

/** Macro giving count of elements in an array */
#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))

/** Macro to copy a wide string, guaranteed not to overrun the destination and always to be null terminated */
#define WSAFECOPY(d,s)	StringCchCopy(d,COUNTOF(d),s)

/** Macro to write to PBModule log automatically adding function name and line number */
/*#ifndef BUILD_AS_RHODES_EXTENSION
#define LOG(s,m) Log(s, m,_T(__FUNCTION__), __LINE__)
#endif*/

/**  Returns the length of the string which represents the specified positive number */
unsigned int StringLength(DWORD iNumber);

/**
*  Encodes the specified byte array into base64 encoding, this is used for
*  DataURI encoding.
*  \param bytes_to_encode The byte array to encode to base64
*  \param iLen The length of the bytes to be encoded
*  \param szEncodedData [out] Buffer into which encoded data is put.  This MUST 
*         be allocated by the caller and must be of an appropriate length, 
*         note: base 64 encoded data is about 1/3 larger than the byte equivilent
*/
void EncodeToBase64(unsigned char const* bytes_to_encode, unsigned int iLen, char* szEncodedData);

/**
*  Decodes the specified base64 string into binary data from radix-64 encoding.
*  \param encoded_string The radix-64 encoded ASCII string to be converted to binary.
*  \param iLengthOfEncodedString The length of the string being decoded.
*  \param szDecodedData [out] Buffer into which the decoded data is put.  This MUST
*			be allocated ty the caller and must be of an appropriate length,
*			note: base 64 encoded data is about 1/3 larger than the byte equivilent.
*/
void DecodeFromBase64(unsigned char const* encoded_string, int iLengthOfEncodedString, char* szDecodedData);

void GetApplicationVersion(int* iV1, int* iV2, int* iV3, int* iV4);