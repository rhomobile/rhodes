#include "StdAfx.h"
#include "XMLReader.h"



CTag::CTag(void)
{
	m_pSzName		= NULL;
	m_pSzData		= NULL;
	m_pAttrbs		= NULL;
	m_pChildTag		= NULL;
	m_pSibTag		= NULL;
	m_iTagtype		= INVALID_TAG;
	
}

CTag::~CTag(void)
{
	delete	[] m_pSzName;
	delete	[] m_pSzData;
	delete	m_pChildTag;
	delete	m_pSibTag;
	m_pAttrbs = deleteAttribs(m_pAttrbs);
	
}

BOOL CTag::Init(LPCTSTR pTagStr,int iLen)
{
	return parseTag(pTagStr,iLen);
}


//get attrib by index
XML_ATT CTag::GetAttr(int iIndex)
{
	XML_ATT pAtt;
	int iCount= iIndex;
	for(pAtt = m_pAttrbs;pAtt;pAtt = pAtt->pAttrbs)
	{
		
		if(!iCount){
			return pAtt;
		}
		iCount--;
							
	}
	return NULL;	
}
//get attrib by name
XML_ATT CTag::GetAttr(LPCTSTR attribName)
{
	XML_ATT pAtt;
	for(pAtt = m_pAttrbs;pAtt;pAtt = pAtt->pAttrbs)
	{
				
		if(!wcsicmp(pAtt->lpName,attribName)){
			return pAtt;
		}
					
	}
	return NULL;		
			
}

BOOL CTag::setTagName(LPCTSTR pName,int iLen)
{
	delete []m_pSzName;
	m_pSzName = new WCHAR[iLen + 1];
	if(!m_pSzName){
		return FALSE;
	}
	wcsncpy(m_pSzName,pName,iLen);
	m_pSzName[iLen]= NULL;
	return TRUE;
}

//if stAttribs == NULL then it will return the count of attribs
BOOL CTag::parseTag(LPCTSTR pTagStr,int iLen)
{
	BOOL bAtts = TRUE;

	
	if(pTagStr[0] !=L'<' && pTagStr[iLen-1]!=L'>')
		return 0;
	
	//set the tag type
	m_iTagtype = getTagType(pTagStr,iLen);
	if(m_iTagtype == INVALID_TAG){
		return FALSE;
	}
	
	//get tag name
	int iIndex,iCount;
	
	switch(m_iTagtype)
	{
		case OPEN_TAG:
		case SELF_CLOSE:
			iIndex = 1;
			break;
		
		case CLOSE_TAG:
			iIndex = 2;
			break;
		case COMMENT_TAG:
			setTagName(L"Comment",7);
			m_pSzData = new TCHAR[iLen - 1];
			if(!m_pSzData){
				return FALSE;
			}
			_tcsncpy(m_pSzData,pTagStr+1,iLen-2);
			m_pSzData[iLen-2]= NULL;
			return TRUE;//no further processing required
		case HEADER_TAG:
			setTagName(L"Header",7);
			m_pSzData = new TCHAR[iLen - 1];
			if(!m_pSzData){
				return FALSE;
			}
			_tcsncpy(m_pSzData,pTagStr+1,iLen-2);
			m_pSzData[iLen-2]= NULL;
			return TRUE;//no further processing required
		default:
			return TRUE;//no further processing required
	}
	
	
	LPCTSTR pName = &pTagStr[iIndex];
	for(iCount = 0;pTagStr[iIndex]!=L'\t' && pTagStr[iIndex]!=L' ' && pTagStr[iIndex]!=L'\r'&& pTagStr[iIndex]!=L'\n' && iIndex < iLen ;iIndex++)
	{
		
		if(pTagStr[iIndex]==L'>'){
			bAtts = FALSE;//error
			break;
		}
		iCount++;
	}
	
	if(!setTagName(pName,iCount)){
		return FALSE;
	}
	

	if(!bAtts){
		return TRUE;//there are no attribs in this tag
	}

	LPCTSTR pStr = &pTagStr[iIndex];
	//LPTSTR pAtt;
	WCHAR szAttName[MAXLEN_ATTRIBUTE_NAME];
	WCHAR szAttValue[MAXLEN_ATTRIBUTE_VALUE];
	BOOL bStartAtt,bStartVal,bEndVal,bEndAtt;
	bStartAtt	= FALSE;
	bStartVal	= FALSE;
	bEndVal		= FALSE;
	bEndAtt		= FALSE;

	for(iIndex = 0;*pStr!=L'>';pStr++)
	{
		
		if(*pStr==L' ' || *pStr==L'\r' || *pStr==L'\n' ||*pStr==L'\t'){
			if(bStartAtt && !bEndAtt){
				szAttName[iIndex]=NULL;
				bEndAtt = TRUE;
			}
			if(bStartVal && !bEndVal){
				szAttValue[iIndex]=*pStr;
				
			}
			
			
		}
		else if(*pStr==L'='){
			if(bStartVal && !bEndVal){//if we are between quotes
				szAttValue[iIndex]=*pStr;
				
			}
			if(bStartAtt && !bEndAtt){
				szAttName[iIndex]=NULL;
				bEndAtt = TRUE;
			}

			
		}
		else if(*pStr==L'\"'){
			if(bStartVal){
				bEndVal	= TRUE;
				szAttValue[iIndex]=NULL;
				if(bStartVal && bEndVal && bStartAtt && bEndAtt){
					if(!AddAttrib(szAttName,szAttValue)){
						return FALSE;
					}
					bStartVal = bEndVal =  bStartAtt = bEndAtt = FALSE;
				}
			}
			else{//bStartVal == TRUE
				bStartVal = TRUE;
				iIndex = 0;
				continue;
			}
		}
		else if(*pStr==L'/'){
			if(bStartVal && bEndVal){
				bEndVal	= TRUE;
				szAttValue[iIndex]=NULL;
				if(bStartVal && bEndVal && bStartAtt && bEndAtt){
					if(!AddAttrib(szAttName,szAttValue)){
						return FALSE;
					}
					break;
				}
			}
			if(bStartVal && !bEndVal){//if we are between quotes
				szAttValue[iIndex]=*pStr;
				
			}
		}
		else{//any other character
			if(!bStartAtt){
				bStartAtt = TRUE;
				iIndex = 0;
			}
			if(bStartAtt && !bEndAtt){
				szAttName[iIndex] = *pStr;
			}
			else if(bStartVal && !bEndVal){
				szAttValue[iIndex] = *pStr;
			}
		}


		
		if(++iIndex >= MAXLEN_ATTRIBUTE_VALUE){
			return FALSE;//the XML attribute value string is too large
		}
		
	}
	return TRUE;

}


//edits a named attribute or adds an attribute if it does not exist
BOOL CTag::SetAttrib(LPCTSTR pAttName,LPCTSTR pNewValue)
{
	XML_ATT xAtt = GetAttr(pAttName);
	if(xAtt){//attribute found so change it
		delete [] xAtt->lpValue;
		xAtt->lpValue = new TCHAR[_tcslen(pNewValue)+ 1];
		if(xAtt->lpValue){
			_tcscpy(xAtt->lpValue,pNewValue);
			return TRUE;
		}
	}
	else{//attribute not found so add it 
		return AddAttrib(pAttName,pNewValue);

	}
	return FALSE;



}


BOOL CTag::AddAttrib(LPCTSTR szAttName,LPCTSTR szAttValue)
{
	int iNameLen,iValLen;
	iNameLen	= wcslen(szAttName);
	iValLen		= wcslen(szAttValue);
	if(iNameLen > MAXLEN_ATTRIBUTE_NAME && iNameLen)
		return FALSE;
	if(iValLen > MAXLEN_ATTRIBUTE_VALUE && iValLen)
		return FALSE;
	
	//create a new attrib record 
	stAttribs *pAtt = NULL; 
	if(!m_pAttrbs){
		m_pAttrbs = new stAttribs;
		if(!m_pAttrbs){
			return FALSE;//mem error
		}
		pAtt = m_pAttrbs;

	}
	else{
		for(pAtt = m_pAttrbs;pAtt->pAttrbs;pAtt = pAtt->pAttrbs);
		pAtt->pAttrbs = new stAttribs;
		if(!pAtt->pAttrbs){
			return FALSE;//mem error
		}
		pAtt = pAtt->pAttrbs;
	}
	memset(pAtt,0, sizeof(stAttribs));


	pAtt->lpName	= new WCHAR[iNameLen + 1];
	pAtt->lpValue	= new WCHAR[iValLen + 1];
	if(pAtt->lpName && pAtt->lpValue){
		wcsncpy(pAtt->lpName,szAttName,iNameLen);
		wcsncpy(pAtt->lpValue,szAttValue,iValLen);
		pAtt->lpName[iNameLen]	= NULL;
		pAtt->lpValue[iValLen]	= NULL;
		return TRUE;
	}
	delete []	pAtt->lpName;
	delete []	pAtt->lpValue;
	delete		pAtt;
	return FALSE;
}


stAttribs * CTag::deleteAttribs(stAttribs *pAtt)
{
	if(pAtt){
		if(pAtt->pAttrbs)
			pAtt->pAttrbs = deleteAttribs(pAtt->pAttrbs);
		delete []	pAtt->lpName;
		delete []	pAtt->lpValue;
		delete		pAtt;
	}
	return NULL;
}


int CTag::getTagType(LPCTSTR pTagStr,int iLen)
{
	m_bOpen = FALSE;
	if(pTagStr[0] !=L'<')
		return INVALID_TAG;
	
	if(pTagStr[1]== L'/'){
		return CLOSE_TAG;
	}
	else if(pTagStr[1]== L'?' && pTagStr[iLen-2]==L'?'){
		return HEADER_TAG;
	}
	if(pTagStr[iLen-2]== L'/'){
		return SELF_CLOSE;
	}
	else if(pTagStr[1]== L'!'&& pTagStr[2]== L'-'&& pTagStr[3]== L'-' && pTagStr[iLen-2]==L'-' && pTagStr[iLen-3]==L'-'){
		return COMMENT_TAG;
	}
	m_bOpen	= TRUE;
	return OPEN_TAG;

}

BOOL CTag::AddData(LPCTSTR pData,int iLen)
{
	delete[] m_pSzData;
	m_pSzData = new WCHAR[iLen + 1];
	if(m_pSzData){
		wcsncpy(m_pSzData,pData,iLen);
		m_pSzData[iLen] = NULL;
		return TRUE;
	}
	return FALSE;
}

//when adding tags to the tree manually
//if(xDate = xAction->NewChild(L"<Date>",SELF_CLOSE)){
//	xDate->AddAttrib(L"Day",lpSzDay);
//	xDate->AddAttrib(L"Month",lpSzMonth);
//	xDate->AddAttrib(L"Year",lpSzYear);
CTag *CTag::NewChild(LPCTSTR tagStr,int iTagType)
{
	XML_TAG xNC = NewChild(tagStr);
	if(xNC){
		xNC->m_iTagtype = iTagType;
		m_bOpen = FALSE;
		return xNC;
	}
	return NULL;
}

//adds a child tag to the XML tree
CTag *CTag::NewChild(LPCTSTR tagStr)
{
	XML_TAG xTemp,pTag = this;
	
	if(pTag->m_pChildTag){
		//iterate through the siblings to find the end of the linked list
		xTemp = pTag->m_pChildTag;
		while(xTemp)
		{
			if(xTemp->m_pSibTag){
				xTemp = xTemp->m_pSibTag;
			}
			else{
				xTemp->m_pSibTag = new CTag();
				if(xTemp->m_pSibTag){
					if(xTemp->m_pSibTag->Init(tagStr,wcslen(tagStr))){
						return xTemp->m_pSibTag;
					}
				}
			}
		}
	}
	else{//there are no children so create a new child
		pTag->m_pChildTag = new CTag();
		if(pTag->m_pChildTag){
			if(pTag->m_pChildTag->Init(tagStr,wcslen(tagStr))){ 
				return pTag->m_pChildTag;
			}
		}
	}
	
	return NULL;
}



CXMLReader::CXMLReader(void)
{
	//PB_DEBUG_CONSTRUCTOR
	
	m_lpStrXML		= NULL;
	m_hWFile		= NULL;
	m_hFile			= NULL;
	m_tagHead		= NULL;
	m_pTblTags		= NULL;
	m_iTagCount		= 0;
	
}
	

CXMLReader::~CXMLReader(void)
{
	
	delete m_tagHead;
	delete [] m_pTblTags;
	delete [] m_lpStrXML;
	
}

//
//	open the XML file, read the data into a string buffer and then 
//	parse it into a dynamic structure.
//
int CXMLReader::ParseFile(LPCTSTR m_szLocaFile)
{
	
	DWORD dwFileSizeHigh,dwNumberOfBytesToRead = INVALID_FILE_SIZE,dwNumberOfBytesRead;
	char *pSzFile = NULL;
	BOOL bFileRead;
	int iRet = ERR;
	
	delete [] m_lpStrXML;
	m_lpStrXML = NULL;
	
	//open the XML file
	
	m_hFile = CreateFile (m_szLocaFile,GENERIC_READ,
		FILE_SHARE_READ ,NULL,OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,NULL);

	if(m_hFile == INVALID_HANDLE_VALUE){
		m_hFile = NULL;
		return FILE_ERR;
	}
	
	dwNumberOfBytesToRead = GetFileSize(m_hFile,&dwFileSizeHigh);
	if(dwNumberOfBytesToRead == INVALID_FILE_SIZE){
		iRet = FILE_ERR;
		goto _cleanup;
	}

	//create the file buffer based on the size of the file
	pSzFile = new char[dwNumberOfBytesToRead + 1];
	if(pSzFile == NULL){
		iRet = MEM_ALLOC_ERR;
		goto _cleanup;
	}
			
	//read the file into buffer
	bFileRead = ReadFile(m_hFile,pSzFile,dwNumberOfBytesToRead , &dwNumberOfBytesRead,NULL);
	if(dwNumberOfBytesToRead != dwNumberOfBytesRead || !bFileRead){
		iRet = FILE_ERR;
		goto _cleanup;
	}
	
	pSzFile[dwNumberOfBytesToRead]= NULL;

	//copy the file data into the CXMLReader buffer 
	m_lpStrXML = new TCHAR[dwNumberOfBytesToRead + 1];
	if(m_lpStrXML == NULL){
		iRet = MEM_ALLOC_ERR;
		goto _cleanup;
	}
	//int i = strlen(pSzFile);
	mbstowcs(m_lpStrXML,pSzFile,dwNumberOfBytesToRead);
	m_lpStrXML[dwNumberOfBytesToRead]=NULL;

	//  The entire Config.xml file has been read into memory, replace
	//  the substitution variables
	int iLenSubstitutedString = 0;
	bool bSubstitutionComplete = !ProcessSubstitutionVariables(m_lpStrXML, NULL, &iLenSubstitutedString);
	while (true)
	{
		//  There is a variable to substitute
		WCHAR* tcSubstitutedString = new WCHAR[iLenSubstitutedString + 1];
		memset(tcSubstitutedString, 0, (iLenSubstitutedString + 1) * sizeof(WCHAR));
		ProcessSubstitutionVariables(m_lpStrXML, 
			tcSubstitutedString, &iLenSubstitutedString);
		int iLength = iLenSubstitutedString;
		//  Need to call ProcessSubstitutionVariables again to find out
		//  if we have finished substituting
		bool bSubstitutionComplete = !ProcessSubstitutionVariables(tcSubstitutedString, NULL, &iLenSubstitutedString);
		delete[] m_lpStrXML;
		m_lpStrXML = new WCHAR[iLength + 1];
		memset(m_lpStrXML, 0, (iLength + 1) * sizeof(WCHAR));
		wcscpy(m_lpStrXML, tcSubstitutedString);
		delete[] tcSubstitutedString;		
		if (bSubstitutionComplete)
			break;
	}

	
	if(PreprocessXML()){
		if(buildTagIndex()){
			iRet = parseTagData();
			iRet = SUCCESS;
		}
	}
	
	
_cleanup:

	CloseHandle(m_hFile);
	m_hFile = NULL;
	delete [] pSzFile;
	delete [] m_lpStrXML;
	m_lpStrXML = NULL;
	delete [] m_pTblTags;
	m_pTblTags = NULL;
	return iRet;

}


void CXMLReader::indent(int iVal)
{
	int iLoop;
	for(iLoop = iVal;iLoop;iLoop--)
	{
		writeChar(L'\t');
	}
}

//recursive function
void CXMLReader::memToFile(CTag *pTag,int iIndent)
{
	stAttribs	*pAtt;
	indent(iIndent);
	switch(pTag->m_iTagtype)
	{
		
		
		case OPEN_TAG:
			writeChar(L'<');
			writeStr(pTag->m_pSzName);
			//WriteFile(m_hWFile,pTag->m_pSzName,wcslen(pTag->m_pSzName),&dwBytesWritten,NULL);			
			pAtt = pTag->m_pAttrbs;
			while(pAtt)//now write out all attribs
			{
				writeChar(L' ');
				writeStr(pAtt->lpName);
				writeStr(L"=\"");
				writeStr(pAtt->lpValue);
				writeChar(L'\"');
				pAtt = pAtt->pAttrbs;

			}
			writeStr(L">\n");
			if(pTag->m_pSzData){
				indent(iIndent + 1);
				writeStr(pTag->m_pSzData);
				writeStr(L"\n");
			}
			if(pTag->m_pChildTag){
				memToFile(pTag->m_pChildTag,iIndent + 1);	
			}
			indent(iIndent);
			writeStr(L"</");
			writeStr(pTag->m_pSzName);
			writeStr(L">\n");	
			if(pTag->m_pSibTag){
				memToFile(pTag->m_pSibTag,iIndent);	
			}
			break;
		case SELF_CLOSE:
			writeChar(L'<');
			writeStr(pTag->m_pSzName);
			pAtt = pTag->m_pAttrbs;
			while(pAtt)//now write out all attribs
			{
				writeChar(L' ');
				writeStr(pAtt->lpName);
				writeStr(L"=\"");
				writeStr(pAtt->lpValue);
				writeChar(L'\"');
				pAtt = pAtt->pAttrbs;

			}
			writeChar(L'/');
			writeStr(L">\n");
			if(pTag->m_pSibTag){
				memToFile(pTag->m_pSibTag,iIndent);
			}
			break;
		
		
		case COMMENT_TAG:
			writeChar(L'<');
			writeStr(pTag->m_pSzData);
			writeStr(L">\n");
			if(pTag->m_pSibTag){
				memToFile(pTag->m_pSibTag);
			}
			break;//no further processing required
		case HEADER_TAG:
			writeChar(L'<');
			writeStr(pTag->m_pSzData);
			writeStr(L">\n");
			if(pTag->m_pSibTag){
				memToFile(pTag->m_pSibTag);
			}
			break;//no further processing required

		
	}
	
}
BOOL CXMLReader::SaveAs(LPCTSTR m_szLocaFile)
{
	
	BOOL bRet = FALSE;
	CTag *pTag;
	
	
	//check for a valid XML stucture 
	if(m_tagHead){
		
		//open the XML file
		m_hWFile = CreateFile (m_szLocaFile,GENERIC_WRITE,
			FILE_SHARE_READ ,NULL,CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,NULL);
		
		if(m_hWFile){
			pTag = m_tagHead;
			memToFile(pTag);
			
			CloseHandle(m_hWFile);
			m_hWFile = NULL;
			bRet = TRUE;
		}
	}
	return bRet;

}

BOOL CXMLReader::writeChar(TCHAR tCh)
{
	char ch;
	DWORD dwBytesWritten;
	
	if(m_hWFile){
		wcstombs(&ch, &tCh,1);
		WriteFile(m_hWFile,&ch,1,&dwBytesWritten,NULL);
		if(dwBytesWritten == 1)
			return TRUE;
	}

	return FALSE;

}


//writes a string of data to the output file
int CXMLReader::writeStr(LPTSTR lpSzChars)
{
	DWORD	dwByteToWrite,dwBytesWritten = 0;
	BOOL	bRet= FALSE;
	char *pStr;
	
	dwByteToWrite = wcslen(lpSzChars);
	pStr = new char[dwByteToWrite + 1];
	if(pStr){
		if(wcstombs(pStr,lpSzChars,dwByteToWrite)== dwByteToWrite){
			if(m_hWFile){
				WriteFile(m_hWFile,pStr,dwByteToWrite,&dwBytesWritten,NULL);
			}
		}
	}
	delete [] pStr;
	return dwBytesWritten;
}


LPWSTR CXMLReader::wstristr(LPCTSTR pToSearch,LPCTSTR pToFind)
{
	LPWSTR pRet = NULL;
	int iIndex,iLenSearch	= wcslen(pToSearch);
	int iLenFind	= wcslen(pToFind);

	LPWSTR  pFound = NULL,pToSearchLow = NULL,pToFindLow = NULL;
	int differ = L'A'- L'a';


	pToSearchLow = new WCHAR[iLenSearch + 1];
	if(pToSearchLow){
		LPWSTR pToFindLow = new WCHAR[iLenFind];
		if(pToFindLow){
			//wcsncpy(pToSearchLow,pToSearch,iLenSearch);
			//wcsncpy(pToFindLow,pToFind,iLenFind);
			//now set both strings to lowercase
			for(iIndex = 0; iIndex < iLenSearch;iIndex++)
			{
				if (pToSearch[iIndex]>=L'A' && pToSearch[iIndex]<=L'Z'){
					pToSearchLow[iIndex] = pToSearch[iIndex]- differ;
				}
				else{
					pToSearchLow[iIndex]= pToSearch[iIndex];
				}
			}
			for(iIndex = 0; iIndex < iLenFind;iIndex++)
			{
				if (pToFind[iIndex]>=L'A' && pToFind[iIndex]<=L'Z'){
					pToFindLow[iIndex] = pToFind[iIndex]- differ;
				}
				else{
					pToFindLow[iIndex]= pToFind[iIndex];
				}
				
			}
			pToSearchLow[iLenSearch] = NULL;
			pToFindLow[iLenFind]= NULL;
			pFound = wcsstr(pToSearchLow,pToFindLow);
			//this will return the pointer to the dynamic arrays, so convert to original string
			if(pFound){
				iIndex = (int)(pFound - pToSearchLow);
				pRet = (LPWSTR)&pToSearch[pFound - pToSearchLow];
			}
		}
	}
	
	delete [] pToSearchLow;
	delete [] pToFindLow;
	return pRet;
		
}

//private
//the first character must be L'<'
BOOL CXMLReader::buildTagIndex()
{
	BOOL bDataSection,bValidData = FALSE;
	m_pTblTags = new stTagIndex[m_iTagCount];
	if(m_pTblTags){
		memset(m_pTblTags,0,sizeof(stTagIndex) * m_iTagCount);
		
		if(m_lpStrXML){
			int iOffset,
				iLen,
				iIndex = -1;
				
				//iDataSection;
			
			iLen = wcslen(m_lpStrXML);
			
			for(iOffset = 0;iOffset < iLen;iOffset++)
			{
				if(m_lpStrXML[iOffset]==L'<'){
					iIndex++;
					bDataSection = FALSE;//end of data section
					m_pTblTags[iIndex].pSzTagStr = &m_lpStrXML[iOffset];//set the start of the data
					m_pTblTags[iIndex].iLenTag = 0;
					if(!bValidData && iIndex){
						m_pTblTags[iIndex -1].iLenData = 0;
						m_pTblTags[iIndex - 1].pSzDataStr = NULL;
					}
					
				}
				else if(m_lpStrXML[iOffset]==L'>'){
					bDataSection	= TRUE;//indicates when we are parsing in between the tags looking for data
					m_pTblTags[iIndex].iLenData		= 0;
					bValidData		= FALSE;
					
					
				}
				else if(bDataSection){//if this is true then we are parsing between the tags looking for data
					if(!bValidData){
						if(m_lpStrXML[iOffset]==L' ' || m_lpStrXML[iOffset]==L'\t' || m_lpStrXML[iOffset]==L'\n' || m_lpStrXML[iOffset]==L'\r'){
							//ignore control characters
							continue;	
						}
					}
					//XML data cannot contain control characters so don't start counting the length until we have a valid character
					if(m_pTblTags[iIndex].iLenData == 0){
						bValidData = TRUE;
						m_pTblTags[iIndex].pSzDataStr	= &m_lpStrXML[iOffset];	
					}
					m_pTblTags[iIndex].iLenData++;
					
				}
				
				
				m_pTblTags[iIndex].iLenTag++;
			}
			/*
#ifdef _DEBUG
			WCHAR TempStr[200];
			int iLoop;
			for(iLoop = 0;iLoop < m_iTagCount;iLoop++)
			{
				wcsncpy(TempStr,m_pTblTags[iLoop].pSzTagStr,m_pTblTags[iLoop].iLenTag);
				TempStr[m_pTblTags[iLoop].iLenTag]=NULL;
				OutputDebugString(TempStr);
				OutputDebugString(L"\n");
				if(m_pTblTags[iLoop].pSzDataStr){
					wcsncpy(TempStr,m_pTblTags[iLoop].pSzDataStr,m_pTblTags[iLoop].iLenData);
					TempStr[m_pTblTags[iLoop].iLenData]=NULL;
					OutputDebugString(TempStr);
					OutputDebugString(L"\n");
				}
				

			}
	
#endif
*/

			return TRUE;
		}
	}
	delete [] m_pTblTags;
	m_pTblTags = NULL;
	return FALSE;
}


//this will do some basic validation and count the tags
BOOL CXMLReader::PreprocessXML()
{
	//matching number of opening to closing tags
	LPWSTR pStart = NULL;
	int iIndex,iOpenBrack,iCloseBrack,iCloseTag,iOpenTag;
	
	iOpenBrack = iCloseBrack = iCloseTag = iOpenTag = 0;
	
	if(m_lpStrXML){
		m_iTagCount = 0;
		for(iIndex = 0;m_lpStrXML[iIndex];iIndex++)
		{
			if(m_lpStrXML[iIndex] == L'<'){ 
				iOpenBrack++;
				m_iTagCount++;
				//is it a close tag?	
				if(m_lpStrXML[iIndex + 1] == L'/'){
					iCloseTag++;
				}
				//is it a header?
				else if(m_lpStrXML[iIndex + 1] == L'?'){
					continue;	
				}
				//is it a comment?
				else if(m_lpStrXML[iIndex + 1] == L'!'&& m_lpStrXML[iIndex + 2] == L'-'&& m_lpStrXML[iIndex + 3] == L'-'){
					continue;
				}
				else{
					iOpenTag++;
				}
						
			}
			else if(m_lpStrXML[iIndex] == L'>'){ 
				iCloseBrack++;
				if(iIndex){//only do this if iIndex > 0
					if(m_lpStrXML[iIndex - 1]==L'/'){
						iCloseTag++;
					}
				}

			}
		}
		if(iOpenBrack == iCloseBrack){//is '<' equal to '>'
			if(iOpenTag == iCloseTag){//uneven '/>' or '</' to opening brackets '<'
				return TRUE;
			}
		}
		
	}
	return FALSE;
}



//returns an error
DWORD CXMLReader::parseTagData()
{
	//iterate through the index table built from the XML text
	CTag *pTag,*pTempTag;

	int iIndex,iErr = ERR;
	
	m_tagHead = new CTag();
	if(!m_tagHead){
		return MEM_ERR;
	}
	
	if(m_tagHead->Init(m_pTblTags[0].pSzTagStr,m_pTblTags[0].iLenTag)){
		pTag = m_tagHead;
		for(iIndex = 1;iIndex < m_iTagCount;iIndex++)
		{
			pTempTag = new CTag();
			if(!pTempTag){
				return MEM_ERR;
			}
			
			if(pTempTag->Init(m_pTblTags[iIndex].pSzTagStr,m_pTblTags[iIndex].iLenTag)){
				switch(pTempTag->m_iTagtype){
					case HEADER_TAG:
					case COMMENT_TAG:
					case SELF_CLOSE: //so the next tag in the string is a self closing tag EG: <Alias Name="RFID" />
					case OPEN_TAG:						// EG: <Plugin>
						if(pTag->m_bOpen){//is it a child tag
							pTag = pTag->m_pChildTag = pTempTag;
							
						}
						//ok it must be a sibling
						else{ 
							pTag = pTag->m_pSibTag = pTempTag;
							
						}
						if(m_pTblTags[iIndex].iLenData && m_pTblTags[iIndex].pSzDataStr){
							if(!pTag->AddData(m_pTblTags[iIndex].pSzDataStr,m_pTblTags[iIndex].iLenData)){
								return MEM_ERR;
							}
						}
						pTempTag = NULL;
						break;
						
					case CLOSE_TAG: //EG: </RFID>
						//find the last open tag with a matching name
						CTag *pTagToClose = findOpenTag(m_tagHead,pTempTag->m_pSzName);
						//close the last matching tag
						pTagToClose->m_bOpen = FALSE;
						delete pTempTag;
						pTag = pTagToClose;//reset the tree pointer 					
						break;
					
					}
			}
		}			
			
	}
	return iErr;
}


//search from the tag provided down, for the last matching tag
CTag *CXMLReader::findOpenTag(CTag *pTag,LPTSTR lpSzTagToFind )
{
	CTag *ptempTag;
	if(pTag->m_bOpen){
			if(isSame(pTag->m_pSzName,lpSzTagToFind))
				return pTag;

		}
	if(pTag->m_pSibTag){
		ptempTag = findOpenTag(pTag->m_pSibTag,lpSzTagToFind);
		if(ptempTag)
			return ptempTag;
	}
	if(pTag->m_pChildTag){
		ptempTag = findOpenTag(pTag->m_pChildTag,lpSzTagToFind);
		if(ptempTag)
			return ptempTag;
	}
	return NULL;
}


//strips leading and trailing spaces from the comparison
BOOL CXMLReader::isSame(LPTSTR lpStr,LPTSTR lpString)
{
	LPTSTR pStr,pString;

	pStr	= lpStr;
	pString = lpString;
	
	if(!lpStr ){
		return FALSE;
	}

	for(;*pStr;pStr++){
		if(*pStr == L' ')
			wcscpy(pStr,pStr + 1);
	}
	for(;*pString;pString++){
		if(*pString == L' ')
			wcscpy(pString,pString + 1);
	}
	
	
	return !wcscmp(lpStr,lpString);
}


BOOL CXMLReader::isHeader(LPTSTR lpSzTag)
{
	int iCharFound = 0;
	LPTSTR lpStr;

	
	for(lpStr = lpSzTag;*lpStr;lpStr++){
		if(*lpStr == L'?')
			iCharFound++;
		if(*lpStr == L'!'){
			if(*(lpStr+1) == L'-' &&*(lpStr+2) == L'-')
				return TRUE;//this is a comment header EG: <!--
		}
		if(iCharFound == 2)
			return TRUE;

	}
	return FALSE;
}



 
//if this encounters NULL before the character has been found, it returns 0
DWORD CXMLReader::getLenToChar(LPTSTR lpFileData,TCHAR tcEnd)
{
	DWORD dwCount;
	for(dwCount = 0;*lpFileData;lpFileData++){
		if(*lpFileData == tcEnd)
			return dwCount;
		dwCount++;
	}
	return 0;
}


//get the named attribute value
//returns TRUE if found

XML_ATT CXMLReader::GetAttrByName(XML_TAG xTag,LPTSTR attribName)
{
	
	CTag *pTag = (CTag *)xTag;
	if(pTag){
		return pTag->GetAttr(attribName);
	}
	return NULL;		
			
}



//get the nth attribute of the tag referenced by xTag
XML_ATT CXMLReader::GetAttr(XML_TAG xTag ,int iIndex)
{
	CTag *pTag = (CTag *)xTag;
	
	if(pTag){
		return pTag->GetAttr(iIndex);
		
	}
	return NULL;		
			
}






//does the same as GetElement, but does not check the siblings of xTag
XML_TAG CXMLReader::GetChildElement(XML_TAG xTag ,const LPCTSTR pElem,int iIndex)
{
	if(xTag->m_pChildTag)
		return findElement(xTag->m_pChildTag,pElem,iIndex);
	return 0;
}




//
//return the address of the the nth child element found
//
// This function will look for matching tags names only
//
 XML_TAG CXMLReader::GetElement(XML_TAG xTag ,const LPCTSTR pElem,int iIndex)
 {
	return findElement(xTag,pElem,iIndex);
 }


//only checks the first child and it's siblings
XML_TAG CXMLReader::findElement(XML_TAG pTag,LPCTSTR pStrToFind,int iIndex)
 {
	XML_TAG	xTag = NULL;
	int ITagCount = iIndex;
	
	if(!pTag){
		return 0;	
	}
	if(pTag->m_pSzName){
		if(!_wcsicmp(pTag->m_pSzName,pStrToFind)){
			
			if(!ITagCount){
				return (XML_TAG)pTag;
			}
			ITagCount--;
			
		}
	
		
	}
	
	xTag = findElement(pTag->m_pSibTag,pStrToFind,ITagCount);
	if(xTag){
		return (XML_TAG)xTag;
	}
	

	return 0;
 }


 
//return a reference to the nth element(iIndex) with a matching name
//

 XML_TAG CXMLReader::GetElement(const LPTSTR pElem,int iIndex)
 {
	return findElement(m_tagHead,pElem,iIndex);
 }


//return the root tag
XML_TAG	CXMLReader::GetRoot()
{
	XML_TAG xTag;
	xTag = m_tagHead;
	while(xTag->m_iTagtype != OPEN_TAG)//igonore comments and headers
	{
		if(xTag->m_pSibTag) xTag = xTag->m_pSibTag;
	}
	return xTag;
}

bool CXMLReader::
	ProcessSubstitutionVariables(LPCWSTR tcOriginalString, 
								TCHAR* tcSubstitutedString, 
								int* iLengthOfSubstitutedString)
{
	bool bRetVal = false;
	if (tcOriginalString && 
		wcslen(tcOriginalString) > wcslen(L"%INSTALLDIR%") && 
		wcsstr(tcOriginalString, L"%INSTALLDIR%"))
	{
		//  String contains a variable which needs substituting
		bRetVal = true;
		//  Ascertain what our installation directory is
		TCHAR tcCurrentDirectory[MAX_PATH];
		memset(tcCurrentDirectory, 0, MAX_PATH * sizeof(TCHAR));
		if (RetrieveInstallationDirectory(tcCurrentDirectory, MAX_PATH - 1))
		{
			//  Work out the length of the substitution variable to return
			//  and substitute the string if the user has given us a valid 
			//  pointer.
			*iLengthOfSubstitutedString = wcslen(tcCurrentDirectory) + 
				wcslen(tcOriginalString) - wcslen(L"%INSTALLDIR%");
			
			if (tcSubstitutedString)
			{
				//  Substitute the variable into the string
				//  First copy the data from the original string before the 
				//  substitution variable
				wcscpy(tcSubstitutedString, L"");
				wcsncat(tcSubstitutedString, tcOriginalString, 
					(wcsstr(tcOriginalString, L"%INSTALLDIR%") - tcOriginalString));
				//  Now copy the substitution variable
				wcscat(tcSubstitutedString, tcCurrentDirectory);
				//  Now copy anything after the substitution variable
				wcscat(tcSubstitutedString, 
					(wcsstr(tcOriginalString, L"%INSTALLDIR%") +  wcslen(L"%INSTALLDIR%")));
			}
		}
		else
		{
			bRetVal = false;
		}
	}
	else
	{
		*iLengthOfSubstitutedString = wcslen(tcOriginalString);
		bRetVal = false;
		//  Would normally expect the user to provide a NULL pointer if 
		//  they are just trying to find out the length of the string to 
		//  substitute but makes sense to still copy it here if they do not.
		if (tcSubstitutedString)
			wcscpy(tcSubstitutedString, tcOriginalString);
	}
	return bRetVal;
}

//  Must be called with sufficient memory for tcInstallationDirectory [out], e.g. MAX_PATH
bool CXMLReader::RetrieveInstallationDirectory(TCHAR* tcInstallDirectory, unsigned int iMaxLen)
{
	bool bReturnValue = false;
	TCHAR tcDirectory[MAX_PATH];	
	if (GetModuleFileName(NULL, tcDirectory, MAX_PATH) > 0)
	{
		//  Remove the file name from the end of the value returned by 
		//  GetModuleFileName
		TCHAR* lastSlash = wcsrchr(tcDirectory, L'\\');
		if (lastSlash)
			memset(lastSlash, 0, wcslen(lastSlash) * sizeof(TCHAR));
	}
	if (wcslen(tcDirectory) <= iMaxLen)
	{
		wcscpy(tcInstallDirectory, tcDirectory);
		bReturnValue = true;
	}
	else
		bReturnValue = false;

	return bReturnValue;
}
 


