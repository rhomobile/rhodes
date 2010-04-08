#include "../stdafx.h"
#include "IpConfig.h"

CIpConfig::CIpConfig(void)
  : m_pAdapterInfo(NULL)
  , m_pCurrentAdapterInfo(NULL)
  , m_bExist(false)
{

}

CIpConfig::~CIpConfig(void)
{
  if(NULL != m_pAdapterInfo)
    free(m_pAdapterInfo);
}
// If item exist in the ip address list
bool CIpConfig::IsExist()
{
  return m_bExist;
}
// Update ip address list
bool CIpConfig::Update()
{
  ULONG Size = 0;
  if(m_pAdapterInfo)
  {
    free(m_pAdapterInfo);
    m_pAdapterInfo = 0;
  }

  DWORD res = GetAdaptersInfo(NULL, &Size);
  if(res == NO_ERROR || res == ERROR_BUFFER_OVERFLOW)
  {
    m_pAdapterInfo = (PIP_ADAPTER_INFO)malloc(Size);
    if(NULL != m_pAdapterInfo)
    {
      res = GetAdaptersInfo(m_pAdapterInfo, &Size);
      if(res == NO_ERROR)
      {
        m_bExist = true;
        return true;
      }
    }
  }
  
  m_bExist = false;
  return false;
}
// Get next item from ip address list
CString CIpConfig::Next()
{
  // m_pCurrentAdapterInfo->IpAddressList

  if(!m_pCurrentAdapterInfo)
    m_pCurrentAdapterInfo = m_pAdapterInfo;

  CString str = TEXT("0.0.0.0");
  if(!m_pCurrentAdapterInfo)
  {
    m_bExist = false;
    return str;
  }

  if(m_pCurrentAdapterInfo)
  {
    if(m_pCurrentAdapterInfo->CurrentIpAddress)
      str.Format( TEXT("%S"), m_pCurrentAdapterInfo->CurrentIpAddress->IpAddress.String );

    m_pCurrentAdapterInfo = m_pCurrentAdapterInfo->Next;
  }
  
  if(!m_pCurrentAdapterInfo)
  {
    free(m_pAdapterInfo);
    m_bExist = false;
  }
  return str;
}

