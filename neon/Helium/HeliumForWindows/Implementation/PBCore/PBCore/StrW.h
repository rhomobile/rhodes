#pragma once

#include "Windows.h"


#ifdef UNICODE
#define CStr CStrW
#endif

class CStrW
{
public:
	CStrW(void);
	CStrW(char *pStr);
	CStrW(LPWSTR pStr);
	CStrW(CStrW &str);
	~CStrW(void);
	
	CStrW operator+(const CStrW & rhStr);
	CStrW operator+(const LPWSTR pStr);
	CStrW operator+(const WCHAR ch);
	friend CStrW operator+(const LPWSTR pStr,const CStrW & rhStr);


	CStrW  operator=(const CStrW & rhStr);
	CStrW operator=(const LPWSTR pStr);
	CStrW operator=(const char *pStr);
	
	void operator+=(const CStrW & rhStr);
	void operator+=(const LPWSTR pStr);
	void operator+=(const WCHAR ch);
	void operator+=(const char *pStr);
	
	int operator==(const CStrW & rhStr);
	int operator==(const LPWSTR pStr);

	int operator!=(const CStrW & rhStr);
	int operator!=(const LPWSTR pStr);


	int operator>(const CStrW & rhStr);
	int operator>(const LPWSTR pStr);

	int operator<(const CStrW & rhStr);
	int operator<(const LPWSTR pStr);

	WCHAR operator[](size_t iIndex);

	bool InsertAt(unsigned int iIndex,WCHAR wc);
	bool InsertAt(unsigned int iIndex,char wc);

	

	size_t GetLength(){return m_iLen;};
	
	void SetChunkSize(size_t nChunkSize){m_nChunkSize = nChunkSize;};
	
	// will add the NULL if iCharsToCopy is at least one larger than the wrapped string
	size_t ToCharArray(char *pStr,size_t iCharsToCopy);
	size_t ToWCharArray(LPWSTR pWStr,size_t iCharsToCopy);
protected:
	LPWSTR		m_pStr;
	size_t		m_iLen;
	size_t		m_uBufferSize;

	size_t m_nChunkSize;
};
