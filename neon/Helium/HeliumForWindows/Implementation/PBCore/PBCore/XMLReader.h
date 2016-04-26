#pragma once


#include <windows.h> 



#include "Error.h"


#define MAXLEN_TAGNAME			500
#define MAXLEN_HEADER			1000
#define MAXLEN_TAG				1000
#define MAXLEN_ATTRIBUTE_NAME	500
#define MAXLEN_ATTRIBUTE_VALUE	500
#define OPEN_TAG_BRACKET		L"<"
#define CLOSE_TAG_BRACKET		L">"
#define FSLASH					L"/"
#define CARRIAGERETURN			0x0D
#define NEWLINE					0x0A
#define XMLCHUNKSIZE			500



#define MAX_LEN_DATA	200
#define MEM_ERR -1



enum tagType {
      INVALID_TAG,
	  OPEN_TAG,		//EG: <PLUGIN>
      CLOSE_TAG,	//EG: </PLUGIN>
      SELF_CLOSE,	//EG: <MODULE Name="RFID"/>
	  RE_TAG_DATA,		//EG  <PLUGIN>this is tag data</PLUGIN>
	  HEADER_TAG,	//EG: <?xml version = "1.0"?>
	  COMMENT_TAG	//EG: <!-- Regular expressions start here -->
   };

struct stInfo{
	int  iDataLen;///>the length of the tag data EG: the length of 'Hello World!'<MyData>Hello World!</MyData>
	int  iTagType;///> OPEN_TAG,CLOSE_TAG,SELF_CLOSE,COMMENT_TAG,HEADER_TAG or RE_TAG_DATA
	int  iTaglen;///> the length of the tag name
};


struct stAttribs{
	LPTSTR		lpName;
	LPTSTR		lpValue;
	stAttribs*	pAttrbs;
};

typedef stAttribs*	XML_ATT;




class CTag{
	
public:
	CTag(void);
	~CTag(void);
	
	BOOL		Init(LPCTSTR pTagStr,int iLen);
	int			getTagType(LPCTSTR pTagStr,int iLen);
	BOOL		AddData(LPCTSTR pData,int iLen);
	BOOL		parseTag(LPCTSTR pTagStr,int iLen);
	BOOL		AddAttrib(LPCTSTR szAttName,LPCTSTR szAttValue);
	XML_ATT		GetAttr(LPCTSTR attribName);
	XML_ATT		GetAttr(int iIndex);
	BOOL		setTagName(LPCTSTR pName,int iLen);
	BOOL		SetAttrib(LPCTSTR pAttName,LPCTSTR pNewValue);
	
	CTag		*NewChild(LPCTSTR tagStr);//adds a child tag to the XML tree
	CTag		*NewChild(LPCTSTR tagStr,int iTagType);//used when adding to XML programatically
	LPTSTR		m_pSzName;
	LPTSTR		m_pSzData;
	stAttribs	*m_pAttrbs;
	CTag		*m_pChildTag;
	CTag		*m_pSibTag;
	BOOL		m_bOpen;
	int			m_iTagtype;
private:
	
	stAttribs	*deleteAttribs(stAttribs *pAtt);
};

struct stTagIndex{
	
	LPTSTR	pSzTagStr;
	LPTSTR	pSzDataStr;
	int		iLenTag;
	int		iLenData;
};


typedef CTag*		XML_TAG;



class CXMLReader
	
{
public:
	CXMLReader(void);

	~CXMLReader(void);
	BOOL	ParseFile			(LPCTSTR m_szLocaFile);							//read a XML file into memory
	BOOL	SaveAs				(LPCTSTR m_szLocaFile);							//write memory to a XML file
	XML_TAG	GetElement			(const LPTSTR pElem,int iIndex);				//searches from the start of the XML data
	XML_TAG GetElement			(XML_TAG xTag ,const LPCTSTR pElem,int iIndex);	//starts the search from the referenced tag
	XML_TAG GetChildElement		(XML_TAG xTag ,const LPCTSTR pElem,int iIndex);	//starts the search from the referenced tag's children
	XML_ATT GetAttr				(XML_TAG xTag ,int iIndex);						//gets the attributes for the element referenced
	XML_ATT GetAttrByName		(XML_TAG xTag,LPTSTR attribName);			//gets the attributes of the named attribute. If not found returns NULL
	BOOL	SetAttrVal			(XML_TAG xTag,LPCTSTR attribName,LPTSTR lpNewValue);
	XML_TAG	GetRoot				();
protected:
	//functions
	BOOL		buildTagIndex();
	LPWSTR		wstristr(LPCTSTR pToSearch,LPCTSTR pToFind);
	BOOL		PreprocessXML();//does some basic validation on the XML target file
	void		writeElement	(CTag *pTag);
	
	void		memToFile(CTag *pTag,int iIndent = 0);
	void		indent(int iVal);
	/**
	*  Function to handle substitution variables in the configuration XML files.
	*  Substitutes %InstallDir% with the Installation directory (minus the 
	*  last \)
	*  \param	tcOriginalString String with the Substitution variable
	*  \param	tcSubstitutedString [out] string with the substitution variable
	*			substituted.
	*  \param	iLengthOfSubstitutedString Length of the string after substitution,
	*			call this function with a NULL Substituted string to determine
	*			the amount of memory to allocate.  This value does not include 
	*			the NULL terminating character.
	*  \return	True if the original string had a variable which needed 
	*			substituting, else false.
	*/
	bool ProcessSubstitutionVariables(LPCWSTR tcOriginalString, 
										TCHAR* tcSubstitutedString, 
										int* iLengthOfSubstitutedString);
	bool RetrieveInstallationDirectory(TCHAR* tcInstallDirectory, unsigned int iMaxLen);





	
	BOOL		isHeader		(LPTSTR lpSzTag);
	DWORD		parseTagData	();
	CTag		*findOpenTag	(CTag *pTag,LPTSTR lpSzTagToFind);
	BOOL		isSame			(LPTSTR lpStr,LPTSTR lpString);
	
	DWORD		getLenToChar	(LPTSTR lpFileData,TCHAR tcEnd);
	BOOL		writeChar		(TCHAR tCh);
	int			writeStr		(LPTSTR lpSzChars);
	XML_TAG		findElement		(CTag *pTag,LPCTSTR pStrToFind,int iIndex);
	
	
	//////////////////////////////////////////////////////////////////////////
	// Variables
	//////////////////////////////////////////////////////////////////////////
	

	stTagIndex *m_pTblTags;
	int			m_iTagCount;
	CTag		*m_tagHead;
	
	HANDLE		m_hFile;//main file handle
	HANDLE		m_hWFile;//main file handle wide version for writing
	LPTSTR		m_lpStrXML;
	
};
