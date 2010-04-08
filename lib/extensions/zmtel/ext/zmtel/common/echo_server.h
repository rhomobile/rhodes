#ifndef __ECHO_SERVER__H__
#define __ECHO_SERVER__H__

#include "system/spirit_system.h"
#include "../ui/ui.h"

typedef enum tag_teEchoServerType
{
  EST_TYPE_SIMPLE, // Simple echo server
  EST_TYPE_RTP,    // Modify SSRC of incoming RTP packet
  EST_TYPE_RTCP    // Modify SSRC of incoming RTCP packet
} teEchoServerType;

// This echo server for corrupt SSRC
class tEchoServer : public tThreadObjEx
{
public:
  //constructor - initializes default values
  tEchoServer(tUI& ui,teEchoServerType eType = EST_TYPE_SIMPLE);

  //destructor - Stops server if needed and frees any allocated resources
  ~tEchoServer();

  //opens socket, starts server thread and initiates data receiving 
  bool Start(uint32 ip, uint16 port);

  //stops thread and closes socket
  void Stop();

protected:
  //receives one packet changes it SSRC and then sends it back
  bool Process();

  virtual void ThreadProc();

private:  
	tUI              &m_UI;
	SOCKET            m_Socket;
	teEchoServerType  m_Type;
};

#endif // __ECHO_SERVER__H__
