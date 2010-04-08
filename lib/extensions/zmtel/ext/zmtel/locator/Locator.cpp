#include "../stdafx.h"
#include "Locator.h"

#include "ws2tcpip.h"

// 
CLocator::CLocator()
  : m_sockBroadcast(INVALID_SOCKET)
  , m_OnRecv(0)
  , m_pObject(0)
  , m_ClientCount(0)
{
}
CLocator::~CLocator()
{
  Deinit();
}

// Locator nitialization
bool CLocator::Init(void *pObject, fxnRecv fxnCallback, const char* pIp, unsigned short Port)
{
  // Winsock initialization
  WORD wVersionRequested;
  WSADATA wsaData;

  wVersionRequested = MAKEWORD(2, 2);
  WSAStartup(wVersionRequested, &wsaData);

  // Create socket
  m_sockBroadcast = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(INVALID_SOCKET == m_sockBroadcast)
  {
    return false;
  }

  // Bind socket with all network interface
  struct sockaddr_in   addr_src;
  addr_src.sin_family = AF_INET;
  addr_src.sin_port = htons(Port);
  addr_src.sin_addr.S_un.S_addr = inet_addr(pIp);

  int res = bind(m_sockBroadcast, (const sockaddr*) &addr_src, sizeof(addr_src));
  if(SOCKET_ERROR == res)
  {
    closesocket(m_sockBroadcast);
    return false;
  }
  //
  
  // Set broadcast option
  bool bEnable = true;
  res = setsockopt(m_sockBroadcast, SOL_SOCKET, SO_BROADCAST, (const char*)&bEnable, sizeof(bEnable));
  if(SOCKET_ERROR == res)
  {
  }

  // Set event of received packets
  m_pObject = pObject;
  m_OnRecv = fxnCallback;

  // Clear other locators list
  {tLock lock(m_csList);
    m_ClientCount = 0;
    memset(m_aClient, 0, sizeof(m_aClient));
  }

  // Start 
  if (!tThreadObjEx::Start())
  {
    closesocket(m_sockBroadcast);
    return false;
  }

  // Sent salutation message
  SendAll(LOCATOR_HI);
  return true;
}

bool CLocator::Deinit()
{
  if(INVALID_SOCKET != m_sockBroadcast)
  {
    // Send bye message
    SendAll(LOCATOR_BYE);
    // Close session for TCP (recv and send function)
    //shutdown(m_sockBroadcast,SD_BOTH);
    // Close socket
    closesocket(m_sockBroadcast);
    m_sockBroadcast = INVALID_SOCKET;
  }

  // Wait threading edn
  tThreadObjEx::WaitExit(SPIRIT_INFINITE);
  tThreadObjEx::CloseDesc();
  m_OnRecv = 0;

  // Deinitialitation sockets
  WSACleanup();
  return true;
}

// Send salutation message
bool CLocator::Hi()
{
  OnSendHi();
  return SendAll(LOCATOR_HI);
}

// Get othe
bool CLocator::GetClientList(sLocatorClient *pList, size_t *pCount)
{
  if(NULL == pList || NULL == pCount)
    return false;

  if(*pCount > LOCATOR_CLIENT_MAX_COUNT)
    return false;

  {tLock  lock(m_csList);
    for(size_t i = 0; i < m_ClientCount; ++i)
      pList[i] = m_aClient[i].Client;
    *pCount = m_ClientCount;
  }
  return true;
}

bool CLocator::Call(const char* pIp, unsigned short Port)
{
  return SendTo(LOCATOR_CALL, pIp, Port);
}

bool CLocator::Hangup(const char* pIp, unsigned short Port)
{
  return SendTo(LOCATOR_HANGUP, pIp, Port);
}

// Send "hi"-message to all other client, all client from list 
bool CLocator::OnSendHi(unsigned short Port)
{
  tLock lock(m_csList);

  if(m_ClientCount == 0)
    return false;

  size_t i;
  for(i = 0; i < m_ClientCount - 1; i++)
  {
    if(m_aClient[i].iLiveTime > 0)
    {
      m_aClient[i].iLiveTime--;
    }
    else
    {
      m_ClientCount--;
      memcpy(&m_aClient[i], &m_aClient[i + 1], (m_ClientCount - i) * sizeof(sLocatorClientEx));
    }
  }
  // 
  i = m_ClientCount - 1;
  if(m_aClient[i].iLiveTime > 0)
  {
    m_aClient[i].iLiveTime--;
  }
  else
  {
    memset(&m_aClient[i], 0, sizeof(sLocatorClientEx));
    m_ClientCount--;
  }

  return true;
}

// Add client to list
bool CLocator::OnAck(const char* pIp, unsigned short Port)
{
  unsigned long ip = inet_addr(pIp);
  char aName[256] = {0};
  //if(!gethostname(aName, sizeof(aName)))
  //{
  //  hostent *pLocalHost = gethostbyname(aName);
  //  if(pLocalHost)
  //  {
  //    int n = 0;
  //    while(pLocalHost->h_addr_list[n])
  //    {
  //      if((*((in_addr*)pLocalHost->h_addr_list[n])).S_un.S_addr == ip)
  //        return true;
  //      n++;
  //    }
  //  }
  //}

  //getnameinfo();
  hostent *pHost = gethostbyaddr((char*) &ip , sizeof(unsigned long), AF_INET);

  tLock lock(m_csList);

  for(size_t i = 0; i < m_ClientCount; ++i)
  {
    if(m_aClient[i].Client.ulIp == ip)
    {
      m_aClient[i].iLiveTime = LOCATOR_CLIENT_LIVE_TIME;
      return true;
    }
  }

  if(m_ClientCount + 1 >= LOCATOR_CLIENT_MAX_COUNT)
    return false;

  m_aClient[m_ClientCount].iLiveTime = LOCATOR_CLIENT_LIVE_TIME;
  m_aClient[m_ClientCount].Client.ulIp = ip;

  if(pHost && pHost->h_name && strcmp(pHost->h_name, "localhost"))
  {
    sprintf(m_aClient[m_ClientCount].Client.aName, "%s (%s)",
      pHost->h_name,
      pIp
    );
  }
  else
  {
    strcpy(m_aClient[m_ClientCount].Client.aName, pIp);
  }

  m_ClientCount++;
  return true;
}

// Delete client from list
bool CLocator::OnBye(const char* pIp, unsigned short Port)
{
  tLock lock(m_csList);

  if(m_ClientCount == 0)
    return false;

  unsigned long ip = inet_addr(pIp);
  size_t i;
  for(i = 0; i < m_ClientCount - 1; i++)
  {
    if(m_aClient[i].Client.ulIp == ip)
    {
      m_ClientCount--;
      memcpy(&m_aClient[i], &m_aClient[i + 1], (m_ClientCount - i) * sizeof(sLocatorClientEx));
    }
  }
  // 
  i = m_ClientCount - 1;
  if(m_aClient[i].Client.ulIp == ip)
  {
    memset(&m_aClient[i], 0, sizeof(sLocatorClientEx));
    m_ClientCount--;
  }
  return true;
}

bool CLocator::OnCall(const char* pIp, unsigned short Port)
{
  return SendTo(LOCATOR_OK, pIp, Port);
}
bool CLocator::SendTo(eLocatorPacketType Type, const char* pIp, unsigned short Port)
{
  struct sockaddr_in addr_dst;
  addr_dst.sin_family = AF_INET;
  addr_dst.sin_port = htons(Port);
  addr_dst.sin_addr.S_un.S_addr = pIp ? inet_addr(pIp) : INADDR_BROADCAST;

  int res = 0;
  for(int i = 0; i < LOCATOR_REPEAT_COUNT; i++)
  {
    res = sendto(
                m_sockBroadcast, 
                (const char*)&Type, 
                sizeof(Type), 
                0, 
                (const sockaddr*)&addr_dst, 
                sizeof(addr_dst)
    );
  }
  return true;  
}

bool CLocator::SendAll(eLocatorPacketType Type, unsigned short Port)
{
  return SendTo(Type, 0, Port);
}
// Working tread
void CLocator::ThreadProc()
{
  sockaddr_in   addr_tmp;
  int           size_addr_tmp;

  int res = 0;
  eLocatorPacketType  type;

  while(1)
  {
    if (SPIRIT_WAIT_OBJECT_0 == Event_Wait(m_TerminateEvent.get(),0))
      break;

    size_addr_tmp = sizeof(addr_tmp);
    res = recvfrom(m_sockBroadcast, (char*)&type, sizeof(eLocatorPacketType), 0, (sockaddr*)&addr_tmp, &size_addr_tmp);

    if(res >= sizeof(eLocatorPacketType))
    {
      switch(type)
      {
      case LOCATOR_HI:
        SendTo(LOCATOR_ACK, inet_ntoa(addr_tmp.sin_addr), ntohs(addr_tmp.sin_port));
      case LOCATOR_ACK:
        OnAck(inet_ntoa(addr_tmp.sin_addr), ntohs(addr_tmp.sin_port));
        break;
      case LOCATOR_BYE:
        OnBye(inet_ntoa(addr_tmp.sin_addr), ntohs(addr_tmp.sin_port));
        break;
      case LOCATOR_CALL:
        OnCall(inet_ntoa(addr_tmp.sin_addr), ntohs(addr_tmp.sin_port));
        break;
      //case LOCATOR_HANGUP:
      //  break;
      //case LOCATOR_OK:
      //  break;
      //case LOCATOR_CANCEL:
      //  break;
      }
      if(m_OnRecv)
        m_OnRecv(m_pObject, type, inet_ntoa(addr_tmp.sin_addr), ntohs(addr_tmp.sin_port));  
    }
  }
}

