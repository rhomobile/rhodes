#ifndef __IP_CONFIG__H__
#define __IP_CONFIG__H__

#include <Iphlpapi.h>

// Get local ip address information
class CIpConfig
{
private:
  PIP_ADAPTER_INFO  m_pAdapterInfo;
  PIP_ADAPTER_INFO  m_pCurrentAdapterInfo;
  bool              m_bExist;

public:
  CIpConfig(void);
  ~CIpConfig(void);

  bool IsExist();
  bool Update();
  CString Next();
};

#endif // __IP_CONFIG__H__

