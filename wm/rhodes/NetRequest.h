#pragma once

#include "stdafx.h"
#include <connmgr.h>
#include <wininet.h>

class CNetRequest
{
public:
	CNetRequest(bool local = true);
	virtual ~CNetRequest(void);

public:
	char* doRequest(LPWSTR verb, char* url,
					char* headers, size_t headers_size,
					char* body, size_t body_size, 
					bool bGetHeaders = false, bool bGetRawData = false, 
					bool bCheckSession = false, DWORD* pdwDataSize = NULL);

protected:
	bool SetupInternetConnection(LPCTSTR url);

	void alloc_url_components(URL_COMPONENTS *uri, char *url);
	void free_url_components(URL_COMPONENTS *uri);
	void ErrorMessage(LPTSTR pszFunction);

private:
	HANDLE m_hConnection;
	bool m_localhostConn;
};
