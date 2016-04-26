#pragma once

#include <windows.h>
#include "..\..\Common\Public\PB_Defines.h"
#include "Config.h"

#define MAX_LEN_RETURN 10000


#define PB_READ_OK			0
#define PB_READ_ERR			1
#define PB_READ_NODATA		2
#define PB_READ_NOTFOUND	3
			

/*
Runs a thread to receive messages from the Generic ActiveX control
*/

/******************************************************************************/
class CGenericManager
{
private:
	HANDLE	hThread;
	HANDLE	hQuitEvent;
	TCHAR	m_szResponse[MAX_LEN_RETURN];
	//TCHAR	m_szJSON[10000];
	//CXMLReader *m_pXML;
	
	//LPCTSTR ReadSetting(LPCTSTR pFileName, LPCTSTR pXmlPath);
	//LPCTSTR buildRootJSON();
	//LPCTSTR buildJSON(XML_TAG xTag);
	//LPCTSTR GetJSON(LPCTSTR pXmlPath);
	//XML_TAG GetTag(LPCTSTR pXmlPath,int iIndex,XML_TAG xFromTag = NULL);
	
	static DWORD StaticWaitForRequest (LPVOID pparam);
	void WaitForRequest (void);

public:
	CGenericManager ();
	~CGenericManager ();
	

	int ReadSetting(LPCTSTR pSetting);
	int WriteSetting(LPCTSTR pSetting, LPCTSTR pValue);

	BOOL WriteConfigSetting(LPCTSTR pSetting,LPCTSTR pValue,LPCTSTR pAppName);
	int ReadConfigSetting(LPCTSTR pSetting,LPCTSTR pAppName);

	BOOL Translate(LPCTSTR pSettName,LPTSTR pRetStr);
	BOOL TranslateApp(LPCTSTR pSettName,LPTSTR pRetStr);
	
	int	 Escape(LPCTSTR pIn,LPTSTR pEscapedStr);
	
	void Start ();
	void Stop (void);



};
