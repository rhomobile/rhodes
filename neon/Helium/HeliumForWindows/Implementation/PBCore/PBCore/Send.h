#pragma once

#include <Wininet.h>
#include "..\..\Common\Public\PB_Defines.h"
#define APP_NAME_TEST _T("Test")

#define SEND_GENERAL_ERR	1
#define SEND_CONNECT_ERR	2
#define SEND_INTERNET_ERR	3
#define SEND_REQUEST_ERR	4
#define SEND_PARAM_ERR		5
#define SEND_RECEIVE_ERR	6



//#define MAX_URL 2049


class CSend
{
public:
	CSend(void);
	CSend(HINTERNET	hInet);//second constructor so that we can use an existing connection
	~CSend(void);
	bool Open(LPCTSTR pHostAdd,LPCTSTR pUser,LPCTSTR pPass);
	bool SetUserPass(LPCTSTR pUser,LPCTSTR pPass);
	int Send();//return error code
	bool CrackURL(const LPCTSTR pURL);
	void AddNameValue(LPCTSTR pName,LPCTSTR pValue);
	void SetPort(WORD nPort){m_nPort= nPort;};

	LPTSTR			m_SzResponse;
protected:
	int		open();
	void	GenerateMultipartBoundary(LPTSTR boundaryPrefix);
	void	close();
	
	
	TCHAR			m_szPostData[MAX_URL+1];
	HINTERNET		m_hConnection;
	HINTERNET		m_hInet;		///<  Handle to the HTTP request (get or post)
	
	LPTSTR			m_szHost;
	LPTSTR			m_SzUser;
	LPTSTR			m_SzPass;
	WORD			m_nPort;		
	LPTSTR			m_szPath;
};
