#include <windows.h>
#include "logging/RhoLog.h"
#include "IstDll.h"

const wchar_t* const IST_MODULE_NAME = L"\\Windows\\ISTAPI32.dll";

bool screenorientation::CIstDll::IsPresent()
{
	DWORD attribs = ::GetFileAttributes(IST_MODULE_NAME);
	return ((0xFFFFFFFF != attribs) && ((attribs & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY));
}

screenorientation::CIstDll::CIstDll() :	m_hISTModule(NULL),
						m_hIST(NULL)
{

}

screenorientation::CIstDll::~CIstDll()
{
	Close();
}

/**
 * Load the IST dll and cmake an open call to IST API
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @return bool 
 */
bool screenorientation::CIstDll::Open()
{
	bool opStatus = false;
	if (this->IsPresent())
	{
		if (this->LoadDll(IST_MODULE_NAME))
		{
			if (this->ISTOpen(IST_DEFAULT_DEVICE_NAME))
			{
				opStatus = true;
			}
		}
	}

	return opStatus;
}

/**
 * Make a close call to the IST api and unload the dll.
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @return bool 
 */
bool screenorientation::CIstDll::Close()
{
	bool opStatus = false;
	if (this->ISTClose())
	{
		this->UnloadDll();
		opStatus = true;
	}

	return opStatus;
}

/**
 * 
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @param enable if true turns on the AutoRotate feature, false 
 *  			 turns it off
 */
void screenorientation::CIstDll::EnableAutoRotate(bool enable)
{
	DWORD dwConfig = 0;
	if (this->ISTGetSystemConfig(dwConfig))
	{
		bool enabled = (dwConfig & IST_AUTO_ROTATION) == IST_AUTO_ROTATION;
		bool doIt = false;
		// if the autorotate is currently enabled and
		// the request is to disable it
		if (enabled & !enable)
		{
			dwConfig &= (~IST_AUTO_ROTATION);
			doIt = true;
		}
		else if (!enabled & enable) // if the autorotate is currently disabled and request is to enable it
		{
			dwConfig |= IST_AUTO_ROTATION;
			doIt = true;
		}

		if (doIt)
		{
			this->ISTSetSystemConfig(dwConfig);
		}
	}
}

/**
 * Get the existing state of the autorotate state.
 * 
 * @author GXV738 (6/12/2013)
 * 
 * @return bool true if autorotate is enabled and false if 
 *  	   autorotate is disabled. A false is returned when
 *  	   there is an error.
 */
bool screenorientation::CIstDll::IsAutoRotateEnabled()
{
	bool enabled = false;
	DWORD dwConfig = 0;
	if (this->ISTGetSystemConfig(dwConfig))
	{
		enabled = (dwConfig & IST_AUTO_ROTATION) == IST_AUTO_ROTATION;
	}
	return enabled;
}

bool screenorientation::CIstDll::LoadDll(const wchar_t *szDll)
{
	if ((NULL == szDll) || (0 == *szDll))
	{
		::SetLastError(ERROR_FILE_NOT_FOUND);
		return false;
	}

	HMODULE hModule = ::LoadLibrary(szDll);
	if (NULL == hModule)	
		return false;
	
	m_hISTModule = hModule;

	return true;
}

void screenorientation::CIstDll::UnloadDll()
{
	if (NULL != m_hISTModule)
	{
		::FreeLibrary(m_hISTModule);
		m_hISTModule = NULL;
	}

}

bool screenorientation::CIstDll::ISTOpen(const wchar_t *pszDev)
{
	bool opStatus = false;
	typedef DWORD (*PFN_IST_OPEN)(TCHAR*, HANDLE*);

	if ((NULL == pszDev) || (0 == *pszDev))
	{
		::SetLastError(ERROR_INVALID_NAME);
		return opStatus;
	}

	PFN_IST_OPEN pfnISTOpen = (PFN_IST_OPEN)::GetProcAddress(m_hISTModule, L"IST_Open");
	if (NULL == pfnISTOpen)
		return opStatus;

	wchar_t* szDeviceName = reinterpret_cast<wchar_t*>(_alloca((wcslen(pszDev) + 1) * sizeof(wchar_t)));
	wcscpy(szDeviceName, pszDev);

	HANDLE hIST = NULL;
	DWORD dwISTStatus = pfnISTOpen(szDeviceName, &hIST);
	if (IST_ERROR_SUCCESS == dwISTStatus)
	{
		m_hIST = hIST;
		opStatus = true;
	}
	else
	{
		::SetLastError(dwISTStatus);
	}

	return opStatus;

}

bool screenorientation::CIstDll::ISTClose()
{
	typedef DWORD (*PFN_IST_CLOSE)(HANDLE);

	DWORD dwISTStatus = IST_ERROR_SUCCESS;
	bool opStatus = false;

	if ((NULL != m_hISTModule) && (NULL != m_hIST))
	{
		PFN_IST_CLOSE pfnISTClose = (PFN_IST_CLOSE)::GetProcAddress(m_hISTModule, L"IST_Close");
		if (NULL != pfnISTClose)
		{
			dwISTStatus = pfnISTClose(NULL);
			if (IST_ERROR_SUCCESS == dwISTStatus)
			{
				m_hIST = NULL;
				opStatus = true;
			}
			else
			{
				::SetLastError(dwISTStatus);
			}			
		}
	}
	else
	{
		::SetLastError(ERROR_INVALID_HANDLE);
	}

	return opStatus;
}

bool screenorientation::CIstDll::ISTGetSystemConfig(DWORD& dwConfig)
{
	typedef DWORD (*PFN_IST_GET_SYSTEM_CONFIG)(HANDLE, DWORD*);

	DWORD dwISTStatus = IST_ERROR_SUCCESS;
	bool opStatus = false;

	if ((NULL != m_hISTModule) && (NULL != m_hIST))
	{
		PFN_IST_GET_SYSTEM_CONFIG pfnISTGetSystemConfig = (PFN_IST_GET_SYSTEM_CONFIG)::GetProcAddress(m_hISTModule, L"IST_GetSystemConfig");
		if (NULL != pfnISTGetSystemConfig)
		{
			dwISTStatus = pfnISTGetSystemConfig(m_hIST, &dwConfig);
			if (IST_ERROR_SUCCESS == dwISTStatus)
				opStatus = true;
			else 
				::SetLastError(dwISTStatus);
		}
	}
	else
	{
		::SetLastError(ERROR_INVALID_HANDLE);
	}

	return opStatus;
}

bool screenorientation::CIstDll::ISTSetSystemConfig(DWORD dwConfig)
{
	typedef DWORD (*PFN_IST_SET_SYSTEM_CONFIG)(HANDLE, DWORD);

	DWORD dwISTStatus = IST_ERROR_SUCCESS;
	bool opStatus = false;

	if ((NULL != m_hISTModule) && (NULL != m_hIST))
	{
		PFN_IST_SET_SYSTEM_CONFIG pfnISTSetSystemConfig = (PFN_IST_SET_SYSTEM_CONFIG)::GetProcAddress(m_hISTModule, L"IST_SetSystemConfig");
		if (NULL != pfnISTSetSystemConfig)
		{
			dwISTStatus = pfnISTSetSystemConfig(m_hIST, dwConfig);
			if (IST_ERROR_SUCCESS == dwISTStatus)
				opStatus = true;
			else
				::SetLastError(dwISTStatus);			
		}
	}
	else
		::SetLastError(ERROR_INVALID_HANDLE);
	

	return opStatus;
}