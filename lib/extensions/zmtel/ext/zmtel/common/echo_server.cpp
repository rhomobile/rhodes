// Test only sevrver

#include "echo_server.h"
#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

tEchoServer::tEchoServer(tUI& ui,teEchoServerType eType)
  : m_UI(ui),m_Type(eType)
{
  m_Socket = INVALID_SOCKET;

#ifdef _WIN32
  WORD wVersionRequested;
  WSADATA wsaData;

  wVersionRequested = MAKEWORD( 2, 2 );
  WSAStartup( wVersionRequested, &wsaData );
#endif // _WIN32
}

tEchoServer::~tEchoServer()
{
  Stop();
#ifdef _WIN32
  WSACleanup( );
#endif // _WIN32
}

bool tEchoServer::Start(uint32 ip, uint16 port)
{
  struct sockaddr_in   addr;

  Stop();
  // Create socket of echo server
  m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if((SOCKET)INVALID_SOCKET == m_Socket) {
    //PRINTF(_T("Error: echo server can not create socket, '") CHAR_STR _T("'\n"),strIPPort);
    return false;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(ip);
    //(((long)IpAddr.ip.i8[0]) << 24) | \
    //                     (((long)IpAddr.ip.i8[1]) << 16) | \
    //                     (((long)IpAddr.ip.i8[2]) << 8) | \
    //                     ((long)IpAddr.ip.i8[3]);

  if((int)SOCKET_ERROR == bind(m_Socket, (const sockaddr*) &addr, sizeof(addr)))
  {
    //PRINTF(_T("Error: echo server can not bind socket, '") CHAR_STR _T("'\n"),strIPPort);
    closesocket(m_Socket);
    m_Socket = INVALID_SOCKET;
    return false;
  }

  int sock_buf_size = 256 * 1024;
  setsockopt( m_Socket, SOL_SOCKET, SO_RCVBUF, ( const char* ) &sock_buf_size, sizeof( sock_buf_size ) );
  setsockopt( m_Socket, SOL_SOCKET, SO_SNDBUF, ( const char* ) &sock_buf_size, sizeof( sock_buf_size ) );

  if (!tThreadObjEx::Start()) {
    //PRINTF(_T("Error: echo server cannot create thread, '") CHAR_STR _T("'\n"),strIPPort);
    closesocket(m_Socket); m_Socket = INVALID_SOCKET;
    return false;
  }

  return true;
}

void tEchoServer::Stop()
{

  if (m_Socket != (SOCKET)INVALID_SOCKET) {
    shutdown(m_Socket,SD_BOTH);
    closesocket(m_Socket);m_Socket = (SOCKET)INVALID_SOCKET;
  }

  tThreadObjEx::WaitExit(SPIRIT_INFINITE);
  tThreadObjEx::CloseDesc();
}


// Echo (receive->corrupt SSRC->send)
bool tEchoServer::Process()
{
  sockaddr_in   addr_tmp;
  socklen_t size_addr_tmp = sizeof(addr_tmp);

  //data buffer
  //we assume max RTP packet size would not exceed 1500 bytes
  char aBuffer[1500];

  //receive buffer
  int size = recvfrom(m_Socket, aBuffer, sizeof(aBuffer), 0, (sockaddr*)&addr_tmp, &size_addr_tmp);
  if(size <= 0) return false;

  //we have to change SSRC to protect MediaManager from SSRC conflict (input and output streams has the same SSRC)
  //
  switch (m_Type)
  {
  case EST_TYPE_RTP:
    aBuffer[8] = aBuffer[8] + 1;
    break;

  case EST_TYPE_RTCP:
    aBuffer[4] = aBuffer[4] + 1;
    break;
  }

  // Sent the buffer back
  sendto(m_Socket, aBuffer, size, 0, (const sockaddr*)&addr_tmp, size_addr_tmp);

  return true;
}

void tEchoServer::ThreadProc()
{
  while(1)
  {
    //lets check should if we should stop processing
    if (SPIRIT_WAIT_OBJECT_0 == Event_Wait(m_TerminateEvent.get(),0))
      break;

    Process();
  }
}
