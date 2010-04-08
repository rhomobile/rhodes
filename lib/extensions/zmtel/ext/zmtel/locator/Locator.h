#ifndef __LOCATOR__H__
#define __LOCATOR__H__

#include <windows.h>
#include <winsock2.h>

#include "system/spirit_system.h"

#define   LOCATOR_PORT_DEFAULT      10000

#define   LOCATOR_CLIENT_NAME_SIZE  64
#define   LOCATOR_CLIENT_MAX_COUNT  16
#define   LOCATOR_CLIENT_LIVE_TIME  3
#define   LOCATOR_REPEAT_COUNT      3

// Packets types
typedef enum _eLocatorPacketType
{
  LOCATOR_HI      = 0,
  LOCATOR_ACK     = 1,
  LOCATOR_BYE     = 2,
  LOCATOR_CALL    = 3,
  LOCATOR_HANGUP  = 4,
  LOCATOR_OK      = 5,
  LOCATOR_CANCEL  = 6
}eLocatorPacketType;

// Event reciev packets
typedef bool (*fxnRecv)(void *pObject, eLocatorPacketType Type, const char* pIp, unsigned short Port);

// 
typedef struct _sLocatorClient
{
  u_long  ulIp;
  char    aName[LOCATOR_CLIENT_NAME_SIZE];
}sLocatorClient;

typedef struct _sLocatorClientEx
{
  sLocatorClient  Client;
  int             iLiveTime;
}sLocatorClientEx;

class CLocator : public tThreadObjEx
{
  SOCKET    m_sockBroadcast;

  void      *m_pObject;
  fxnRecv   m_OnRecv;

  tCritSec  m_csList;

  size_t    m_ClientCount;
  sLocatorClientEx m_aClient[LOCATOR_CLIENT_MAX_COUNT];
public:
  CLocator();
  ~CLocator();

  bool Init(void *pObject, fxnRecv fxnCallback, const char* pIp = "0.0.0.0"/*ADDR_ANY*/, unsigned short Port = LOCATOR_PORT_DEFAULT);
  bool Deinit();

  bool Hi();

  bool GetClientList(sLocatorClient *pList, /*in/out*/size_t *pCount);

  bool Call(const char* pIp, unsigned short Port = LOCATOR_PORT_DEFAULT);
  bool Hangup(const char* pIp, unsigned short Port = LOCATOR_PORT_DEFAULT);

protected:
  virtual void ThreadProc();

  bool OnSendHi(unsigned short Port = LOCATOR_PORT_DEFAULT);
  bool OnAck(const char* pIp, unsigned short Port = LOCATOR_PORT_DEFAULT);
  bool OnBye(const char* pIp, unsigned short Port = LOCATOR_PORT_DEFAULT);
  bool OnCall(const char* pIp, unsigned short Port = LOCATOR_PORT_DEFAULT);

  bool SendAll(eLocatorPacketType Type, unsigned short Port = LOCATOR_PORT_DEFAULT);
  bool SendTo(eLocatorPacketType Type, const char* pIp, unsigned short Port = LOCATOR_PORT_DEFAULT);
};

#endif // __LOCATOR__H__

