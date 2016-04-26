#include "StdAfx.h"
#include "Config.h"

CConfig::CConfig()
{
	m_pXML		= NULL;
	m_xAppRoot	= NULL;
}

CConfig::~CConfig(void)
{
	delete m_pXML;
	
}

BOOL CConfig::Save()
{
	return m_pXML->SaveAs(m_szConfigFilePath);
}


BOOL CConfig::Init(LPCTSTR pXMLFile)
{
	_tcscpy(m_szConfigFilePath,pXMLFile);
	
	m_pXML = new CXMLReader;
	if(m_pXML){
		if(m_pXML->ParseFile(pXMLFile) == SUCCESS){
			if(setAppRoot()){
				return TRUE;
			}
			
		}
	}
	return FALSE;
}




BOOL CConfig::setAppRoot()//sets up the member tag to point to the application root
{
	m_xAppRoot = GetTag(L"Configuration\\Applications",0);
	return m_xAppRoot == FALSE ? FALSE :TRUE;
}


//return the pAttName of the XML path
LPCTSTR CConfig::GetSettingPtr(LPCTSTR pXmlPath,LPTSTR pAttName)
{
	XML_ATT xAtt = GetSetting(pXmlPath,pAttName);
	if(xAtt){
		return xAtt->lpValue; 
	}
	return NULL;
}

//can be called with or without 'xFromTag'
XML_TAG CConfig::GetTag(LPCTSTR pXmlPath,int iIndex,XML_TAG xFromTag)
{
	LPCTSTR		pTagStart,pStr;
	XML_TAG		xTag[20];//20 levels deep is the max query EG: \one\two\three == three levels
	WCHAR		szTag[MAXLEN_TAGNAME + 1];
	int			iLen,iTagCount,iTagLen; 
	
	if(!pXmlPath || *pXmlPath==NULL || !m_pXML){
		return NULL;
	}
	
	iLen = (int)wcslen(pXmlPath);
		

	if(xFromTag){
		xTag[0] = xFromTag;
		iTagCount	= 1;
	}
	else{//xFromTag == NULL
		iTagCount	= 0;
	}
	
	
	
	
	iTagLen		= 0;	
	if(iLen && m_pXML){
		
		for(pStr = pTagStart = pXmlPath;;pStr++)
		{
			if(*pStr==L'\\'){
				//if(!iTagLen) continue;
				wcsncpy(szTag,pTagStart,iTagLen);
				szTag[iTagLen]=NULL;
				iTagLen = 0;
				pTagStart = pStr + 1; 
				if(iTagCount == 0){
					xTag[iTagCount] = m_pXML->GetElement(szTag,0);
					if(!xTag[iTagCount]){
						break;
					}

				}
				else{
					xTag[iTagCount] = m_pXML->GetChildElement(xTag[iTagCount - 1],szTag,0);
					if(!xTag[iTagCount]){
						break;;
					}
				}
				iTagCount++;
				continue;
				
			}
			
			else if(*pStr == NULL){
				wcsncpy(szTag,pTagStart,iTagLen);
				szTag[iTagLen]=NULL;
				xTag[iTagCount] =  m_pXML->GetChildElement(xTag[iTagCount - 1],szTag,iIndex);
				if(!xTag[iTagCount]){
					break;
				}
				return xTag[iTagCount];
			}
			iTagLen++;
		}

	}
	return NULL;	
}

XML_TAG CConfig::GetAppByName(LPCTSTR pAppName)
{
	XML_TAG xApp,xSet;
	
	int iLoop;
	
	if(m_xAppRoot){
		for(iLoop = 0;;iLoop++)//loop through the applications until we find a matching name
		{
			if((xApp = m_pXML->GetChildElement(m_xAppRoot,L"Application",iLoop))==NULL){//get the nth application
				break;//no matching app found
			}
			//get the application name
			if((xSet = GetTag(L"General\\StartPage",0,xApp))==NULL){
				break;//error
			}
			
			//is this the right app name?
			if(!_tcscmp(m_pXML->GetAttrByName(xSet,L"name")->lpValue,pAppName)){
				//we have found a match
				return xApp;
			}
			
		}
	}
	return NULL;
}

XML_ATT CConfig::GetAppSettingByName(int iIndex, LPCTSTR pAppName,LPCTSTR pXmlPath,LPTSTR pAttName)
{
	XML_TAG xTag,xApp = GetAppByName(pAppName);
	if(xApp){
		xTag = GetTag(pXmlPath,iIndex,xApp);//get the nth pXmlPath setting 
		if(xTag){
			return m_pXML->GetAttrByName(xTag,pAttName);

		}	
	}
	return NULL;
}


LPCTSTR CConfig::GetAppSettingPtr(int iAppID,LPCTSTR pXmlPath,LPTSTR pAttName)
{
	XML_ATT xAtt;
	xAtt = GetAppSetting(iAppID,pXmlPath,pAttName);
	if(xAtt && xAtt->lpValue){
		return xAtt->lpValue;
	}
	return NULL;
}

XML_ATT CConfig::GetAppSetting(int iAppID,LPCTSTR pXmlPath,LPTSTR pAttName)
{
	XML_TAG	xApp, xTag;
	
	if(m_xAppRoot){
		
		xApp = m_pXML->GetChildElement(m_xAppRoot,L"Application",iAppID);//get the nth application
		//xApp = m_pXML->GetElement(m_xAppRoot,L"Application",iIndex);//get the nth application
		if(!xApp){
			return NULL;
		}
		xTag = GetTag(pXmlPath,0,xApp);
		if(xTag){
			return m_pXML->GetAttrByName(xTag,pAttName);

		}

	}
	

	return NULL;
	
}

XML_ATT CConfig::GetSetting(LPCTSTR pXmlPath,LPTSTR pAttName,int iIndex)
{
	XML_TAG xTag = GetTag(pXmlPath,iIndex);
	if(xTag){
		return m_pXML->GetAttrByName(xTag,pAttName);
	}
	return NULL;
	
}


XML_ATT CConfig::GetPreLoad(int iAppID,int iPreLoadIndex)
{
	XML_TAG	xApp, xTag;
	
	if(m_xAppRoot){
		
		xApp = m_pXML->GetChildElement(m_xAppRoot,L"Application",iAppID);//get the nth application
		//xApp = m_pXML->GetElement(m_xAppRoot,L"Application",iIndex);//get the nth application
		if(!xApp){
			return NULL;
		}
		xTag = GetTag(L"Preloads\\Preload",iPreLoadIndex,xApp);
		if(xTag){
			return m_pXML->GetAttrByName(xTag,L"Value");

		}

	}
	

	return NULL;
}
LPCTSTR CConfig::GetPreLoadPtr(int iAppID,int iPreLoadIndex)
{
	XML_ATT xAtt = GetPreLoad(iAppID,iPreLoadIndex);
	if(xAtt && xAtt->lpValue){
		return xAtt->lpValue;
	}
	return NULL;
	
}


LPCTSTR CConfig::GetDefaultMetaTagsPtr(int iAppID,int iMetaIndex)
{
	XML_ATT xAtt = GetDefaultMetaTags(iAppID,iMetaIndex);
	if(xAtt && xAtt->lpValue){
		return xAtt->lpValue;
	}
	return NULL;
}


XML_ATT CConfig::GetDefaultMetaTags(int iAppID,int iMetaIndex)
{
	XML_TAG	xApp, xTag;
	
	if(m_xAppRoot){
		
		xApp = m_pXML->GetChildElement(m_xAppRoot,L"Application",iAppID);//get the nth application
		//xApp = m_pXML->GetElement(m_xAppRoot,L"Application",iIndex);//get the nth application
		if(!xApp){
			return NULL;
		}
		xTag = GetTag(L"DefaultMetaTags\\MetaTag",iMetaIndex,xApp);
		if(xTag){
			return m_pXML->GetAttrByName(xTag,L"Value");

		}

	}
	

	return NULL;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set config
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//set the value attribute if the specified XML path
BOOL CConfig::SetAppValue(int iAppID,LPCTSTR pXmlPath,LPCTSTR pValue)
{
	XML_TAG	xApp, xTag;
	
	if(m_xAppRoot){
		
		xApp = m_pXML->GetChildElement(m_xAppRoot,L"Application",iAppID);//get the nth application
		//xApp = m_pXML->GetElement(m_xAppRoot,L"Application",iIndex);//get the nth application
		if(!xApp){
			return NULL;
		}
		xTag = GetTag(pXmlPath,0,xApp);
		if(xTag){
			return xTag->SetAttrib(L"Value",pValue);
		}

	}
		
	return FALSE;
}
BOOL CConfig::SetAppValue(LPCTSTR pAppName,LPCTSTR pXmlPath,LPCTSTR pValue)
{
	XML_TAG	xApp, xTag;
	
	if(m_xAppRoot){
		xApp = GetAppByName(pAppName);//get the application
		
		if(!xApp){
			return NULL;
		}
		xTag = GetTag(pXmlPath,0,xApp);
		if(xTag){
			return xTag->SetAttrib(L"Value",pValue);
		}

	}
		
	return FALSE;
}

BOOL CConfig::SetGlobalValue(LPCTSTR pXmlPath,LPCTSTR pValue)
{
	XML_TAG xTag = GetTag(pXmlPath,0);
	if(xTag){
		return xTag->SetAttrib(L"Value",pValue);
	}
		
	return FALSE;

}
BOOL CConfig::AddPreload(int iAppID,LPCTSTR pPreloadStr)
{
	XML_TAG	xApp, xTag,xPre;
	LPTSTR pTagStr;
	int iLen = (int)wcslen(pPreloadStr);
	BOOL bRet= FALSE;

	pTagStr = new WCHAR[iLen + 25];
	if(!pTagStr){
		return FALSE;
	}
	//build the string that will be used to initialise the new tag
	wcscpy(pTagStr,L"<Preload value=\"");
	wcscat(pTagStr,pPreloadStr);
	wcscat(pTagStr,L"\"/>\0");


	if(m_xAppRoot){
		
		xApp = m_pXML->GetChildElement(m_xAppRoot,L"Application",iAppID);//get the nth application
		//xApp = m_pXML->GetElement(m_xAppRoot,L"Application",iIndex);//get the nth application
		if(xApp){
			xTag = GetTag(L"Preloads",0,xApp);
			if(xTag){
				xPre = xTag->NewChild(pTagStr);
				if(xPre){
					bRet = TRUE;
				}
				
			}
		
		}

	}
	
	delete [] pTagStr;
	return bRet;

}


//adds a default meta tag to the indexed application
//EG: AddDefaultMeta(0,L"Signal~Left:10;Top:250;color:#009900;visibility:visible");
//EG: AddDefaultMeta(0,L"Signal~Left:10");
BOOL CConfig::AddDefaultMeta(int iAppID,LPCTSTR metaTag)
{
	XML_TAG	xApp, xTag,xMeta;
	LPTSTR pTagStr;
	int iLen = (int)wcslen(metaTag);
	BOOL bRet= FALSE;

	pTagStr = new WCHAR[iLen + 25];
	if(!pTagStr){
		return FALSE;
	}
	//build the string that will be used to initialise the new tag
	wcscpy(pTagStr,L"<MetaTag value=\"");
	wcscat(pTagStr,metaTag);
	wcscat(pTagStr,L"\"/>\0");


	if(m_xAppRoot){
		
		xApp = m_pXML->GetChildElement(m_xAppRoot,L"Application",iAppID);//get the nth application
		//xApp = m_pXML->GetElement(m_xAppRoot,L"Application",iIndex);//get the nth application
		if(xApp){
			xTag = GetTag(L"DefaultMetaTags",0,xApp);
			if(xTag){
				xMeta = xTag->NewChild(pTagStr);
				if(xMeta){
					bRet = TRUE;
				}
				
			}
		
		}

	}
	
	delete [] pTagStr;
	return bRet;
}


