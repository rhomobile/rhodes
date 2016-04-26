#include "StrW.h"




//friend function
CStrW operator+(const LPWSTR pStr,const CStrW & rhStr)
{
	CStrW strTemp;
	if(wcslen(pStr)){
		strTemp = pStr;///< L"string" + L"string2"

	}
	if(rhStr.m_iLen){
		strTemp +=rhStr;
	}
	return strTemp;
}




CStrW::CStrW(void)
{
	m_pStr = NULL;
	m_iLen = 0;
	m_nChunkSize = 0;
	m_uBufferSize = 0;
}

CStrW::CStrW(char *pStr)
{
	m_pStr = NULL;
	m_nChunkSize = 0;
	m_uBufferSize = 0;
	this->operator = (pStr);
	
}

CStrW::CStrW(LPWSTR pStr)
{
	m_pStr = NULL;
	m_nChunkSize = 0;
	m_uBufferSize = 0;
	this->operator = (pStr);
	
}
CStrW::CStrW(CStrW & str)
{
	m_uBufferSize = 0;
	m_iLen = str.m_iLen;
	m_pStr = NULL;
	if(str.m_iLen){
		m_uBufferSize = str.m_iLen + 1;
		m_pStr = new WCHAR[m_uBufferSize];
		wcscpy(m_pStr,str.m_pStr);
	}
	m_nChunkSize = 0;
	
}

CStrW::~CStrW(void)
{
	delete [] m_pStr;
}


CStrW CStrW::operator+(const CStrW & rhStr)
{
	if(!rhStr.m_iLen) return *this;
	CStrW strTemp;
	strTemp = m_pStr;
	strTemp += rhStr;
	return strTemp;

}

CStrW CStrW::operator+(const LPWSTR pStr)
{
	size_t iLen = 0;
	CStrW strTemp;
	if(pStr){
		if((iLen = wcslen(pStr))){
			if(m_iLen){
				//this string and the new string are both valid
				
				strTemp.operator = (m_pStr);
				strTemp.operator += (pStr);
			}
			else{
				this->operator = (pStr);
			}
		}
		
	}
	return strTemp;
}

CStrW CStrW::operator+(const WCHAR ch)
{
	size_t iLen = 0;
	CStrW strTemp;
	
	strTemp.operator = (m_pStr);
	strTemp.operator += (ch);
	
	return strTemp;
}


CStrW  CStrW::operator=(const CStrW & rhStr)
{	
	return this->operator = (rhStr.m_pStr);
	
}

CStrW CStrW::operator=(const LPWSTR pStr)
{
	
	size_t iLen = 0;
	if(pStr && (iLen = wcslen(pStr))){
		//check if allocation is necessary
		if(iLen >=  m_uBufferSize){
			delete [] m_pStr;
			while(iLen >=  m_uBufferSize)
			{
				//either add another chunk
				if(m_nChunkSize){
					m_uBufferSize += m_nChunkSize;
					continue;
					
				}
				//no chunk set, allocate based on size
				m_uBufferSize = iLen + 1;
			}
			m_pStr = new WCHAR[m_uBufferSize];
		}
		wcscpy(m_pStr,pStr);
	}
	else{// no string has been added
		delete [] m_pStr;
		m_pStr = NULL;
		m_uBufferSize = 0;

	}
	m_iLen = iLen;
	return *this;
}

CStrW CStrW::operator=(const char *pStr)
{
	
	size_t iLen = 0;
	if(pStr && (iLen = strlen(pStr))){
		//check if allocation is necessary
		if(iLen >=  m_uBufferSize){
			delete [] m_pStr;
			while(iLen >=  m_uBufferSize)
			{
				//either add another chunk
				if(m_nChunkSize){
					m_uBufferSize += m_nChunkSize;
					continue;
					
				}
				//no chunk set, allocate based on size
				m_uBufferSize = iLen + 1;
			}
			m_pStr = new WCHAR[m_uBufferSize];
		}
		mbstowcs(m_pStr,pStr,iLen + 1);
		
	}
	else{// no string has been added
		delete [] m_pStr;
		m_pStr = NULL;
		m_uBufferSize = 0;

	}
	m_iLen = iLen;
	return *this;
}
	
	


void CStrW::operator+=(const WCHAR ch)
{
	if(ch !=L''){
		if((m_iLen + 2) > m_uBufferSize){
			while((m_iLen + 2) > m_uBufferSize)
			{
				//either add another chunk
				if(m_nChunkSize){
					m_uBufferSize += m_nChunkSize;
					continue;
					
				}
				//no chunk set, allocate based on size
				
				m_uBufferSize = m_iLen + 2;
			}
			LPWSTR pTemp = new WCHAR[m_uBufferSize];
			wcscpy(pTemp,m_pStr);
			
			delete [] m_pStr;
			m_pStr = pTemp;
			
		}
		m_pStr[m_iLen]= ch;
		m_iLen++;
		m_pStr[m_iLen] = NULL;
		
		
	}
	
	
}
void CStrW::operator+=(const LPWSTR pStr)
{
	if(!m_pStr || m_iLen==0){
	 this->operator = (pStr);	//if the user tries to concatenate to an empty buffer
		return;
	}
	size_t iLen = 0;
	
	if(pStr){
		if((iLen = wcslen(pStr))){
			if((iLen + m_iLen + 1) > m_uBufferSize){
				CStrW strTemp,strTemp2;
				strTemp = m_pStr;
				strTemp2 = pStr;
				delete [] m_pStr;
				
				while((iLen + m_iLen + 1) > m_uBufferSize)
				{
					//either add another chunk
					if(m_nChunkSize){
						m_uBufferSize += m_nChunkSize;
						continue;
						
					}
					//no chunk set, allocate based on size
					m_uBufferSize = iLen + m_iLen + 1;
				}
				
				m_pStr = new WCHAR[m_uBufferSize];
				wcscpy(m_pStr,strTemp.m_pStr);
				wcscat(m_pStr,strTemp2.m_pStr);
							
				
			}
			else{ //the buffer is already large enough
				wcscat(m_pStr,pStr);
				
			}
			m_iLen += iLen;
		}
	}
}

void CStrW::operator+=(const char *pStr)
{
	if(!m_pStr || m_iLen==0){
	 this->operator = (pStr);	//if the user tries to concatenate to an empty buffer
		return;
	}
	size_t iLen = 0;
	
	if(pStr){
		if((iLen = strlen(pStr))){
			if((iLen + m_iLen + 1) > m_uBufferSize){
				CStrW strTemp,strTemp2;
				strTemp = m_pStr;
				strTemp2 = pStr;
				delete [] m_pStr;
				
				while((iLen + m_iLen + 1) > m_uBufferSize)
				{
					//either add another chunk
					if(m_nChunkSize){
						m_uBufferSize += m_nChunkSize;
						continue;
						
					}
					//no chunk set, allocate based on size
					m_uBufferSize = iLen + m_iLen + 1;
				}
				
				m_pStr = new WCHAR[m_uBufferSize];
				wcscpy(m_pStr,strTemp.m_pStr);
				wcscat(m_pStr,strTemp2.m_pStr);
							
				
			}
			else{ //the buffer is already large enough
				mbstowcs(m_pStr + m_iLen,pStr,iLen + 1);
				//wcscat(m_pStr,pStr);
				m_iLen += iLen;
			}
		}
	}
}


void CStrW::operator+=(const CStrW & rhStr)
{
	if(!m_pStr || !wcslen(m_pStr)){
		this->operator = (rhStr);	//if the user tries to concatenate to an empty buffer
	}
	else{
		this->operator += (rhStr.m_pStr);
	}
		
}


int CStrW::operator==(const CStrW & rhStr)
{
	return !memcmp(m_pStr,rhStr.m_pStr,m_iLen + 1);
}

int CStrW::operator==(const LPWSTR pStr)
{
	return !memcmp(m_pStr,pStr,m_iLen + 1);
}

int CStrW::operator!=(const CStrW & rhStr)
{
	return memcmp(m_pStr,rhStr.m_pStr,m_iLen + 1);
}

int CStrW::operator!=(const LPWSTR pStr)
{
	return memcmp(m_pStr,pStr,m_iLen + 1);
}

int CStrW::operator>(const CStrW & rhStr)
{
	if(wcscmp(m_pStr,rhStr.m_pStr)>0){
		return 1;
	}
	return 0;

}

int CStrW::operator>(const LPWSTR pStr)
{
	if(wcscmp(m_pStr,pStr)>0){
		return 1;
	}
	return 0;
	
}
int CStrW::operator<(const CStrW & rhStr)
{
	if(wcscmp(m_pStr,rhStr.m_pStr)<0){
		return 1;
	}
	return 0;
}

int CStrW::operator<(const LPWSTR pStr)
{
	if(wcscmp(m_pStr,pStr)<0){
		return 1;
	}
	return 0;
}

WCHAR CStrW::operator[](const size_t iIndex)
{
	WCHAR chTemp = NULL;
	if(iIndex < m_iLen){
		chTemp = m_pStr[iIndex];
	}
	return chTemp;
}



size_t CStrW::ToCharArray(char *pStr,size_t iCharsToCopy)
{
	size_t iRet = 0;
	if(m_iLen && iCharsToCopy){
		size_t iLenPlusNull = m_iLen+1;
		
		if(iCharsToCopy <= iLenPlusNull){
			wcstombs(pStr,m_pStr,iCharsToCopy);
			iRet = iCharsToCopy;
		}
		else{
			wcstombs(pStr,m_pStr,iLenPlusNull);
			iRet = iLenPlusNull;
		}
	}
	return iRet;

}

size_t CStrW::ToWCharArray(LPWSTR pWStr,size_t iCharsToCopy)
{
	size_t iRet = 0;
	if(m_iLen && iCharsToCopy){
		size_t iLenPlusNull = m_iLen+1;
		if(iCharsToCopy <= iLenPlusNull){
			wcsncpy(pWStr,m_pStr,iCharsToCopy);
			iRet = iCharsToCopy;
		}
		else{
			wcsncpy(pWStr,m_pStr,iLenPlusNull);
			iRet = iLenPlusNull;
		}
	}
	return iRet;
}

bool CStrW::InsertAt(unsigned int iIndex,WCHAR wc)
{
	if(iIndex < m_iLen){
		m_pStr[iIndex] = wc;
		return true;
	}
	return false;


}
bool CStrW::InsertAt(unsigned int iIndex,char wc)
{
	if(iIndex < m_iLen){
		m_pStr[iIndex] = m_pStr[iIndex] & 0x0;
		m_pStr[iIndex] = m_pStr[iIndex] | wc;
		return true;
	}
	return false;


}