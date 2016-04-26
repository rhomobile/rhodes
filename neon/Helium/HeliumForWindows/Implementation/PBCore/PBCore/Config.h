#pragma once
#include "XMLReader.h"
#include "CriticalSect.h"

//#ifdef BUILD_AS_RHODES_EXTENSION
//typedef LPCTSTR (CONFIGURATIONCALLBACK) (LPCTSTR,LPTSTR);
//#endif

class CConfig:public Lockable
{
public:
//#ifdef BUILD_AS_RHODES_EXTENSION
//	CConfig(CONFIGURATIONCALLBACK *pConfigCB = NULL);
//#endif
	CConfig();
	~CConfig(void);
	BOOL Init(LPCTSTR pXMLFile);
	
	XML_TAG GetAppByName(LPCTSTR pAppName);
	XML_ATT GetAppSettingByName(int iIndex, LPCTSTR pAppName,LPCTSTR pXmlPath,LPTSTR pAttName);
	
	LPCTSTR GetSettingPtr(LPCTSTR pXmlPath,LPTSTR pAttName);
	LPCTSTR GetAppSettingPtr(int iAppID,LPCTSTR pXmlPath,LPTSTR pAttName);
	LPCTSTR GetDefaultMetaTagsPtr(int iAppID,int iMetaIndex);
	LPCTSTR GetPreLoadPtr(int iAppID,int iPreLoadIndex);
	
	XML_ATT GetSetting(LPCTSTR pXmlPath,LPTSTR pAttName,int iIndex=0);
	XML_ATT GetAppSetting(int iIndex,LPCTSTR pXmlPath,LPTSTR pAttName);
	XML_TAG GetTag(LPCTSTR pXmlPath,int iIndex,XML_TAG xFromTag = NULL);
	XML_ATT GetPreLoad(int iAppID,int iPreLoadIndex);
	XML_ATT GetDefaultMetaTags(int iAppID,int iMetaIndex);
	
	BOOL	Save();
////////////////////////////////////////////////////////////////////////////////////////////////
//	set XML values in memory
////////////////////////////////////////////////////////////////////////////////////////////////
		
	BOOL	SetAppValue		(int iAppID,LPCTSTR pXmlPath,LPCTSTR pValue);
	BOOL	SetAppValue		(LPCTSTR pAppName,LPCTSTR pXmlPath,LPCTSTR pValue);
	BOOL	SetGlobalValue	(LPCTSTR pXmlPath, LPCTSTR pValue);
	BOOL	AddPreload		(int iAppID,LPCTSTR pPreloadStr);
	BOOL	AddDefaultMeta	(int iAppID,LPCTSTR metaTag);
	

private:
	BOOL setAppRoot();
	

	XML_TAG	m_xAppRoot;
	CXMLReader *m_pXML;
	TCHAR m_szConfigFilePath[MAX_PATH + 1];
//#ifdef BUILD_AS_RHODES_EXTENSION
//	CONFIGURATIONCALLBACK *m_pConfigCB;
//#endif
};
