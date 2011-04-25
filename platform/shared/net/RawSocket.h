#ifndef RHO_RAW_SOCKET_F5FFD21AD3EE463E850C5E2C789397BD
#define RHO_RAW_SOCKET_F5FFD21AD3EE463E850C5E2C789397BD

#include "common/RhoStd.h"
#include "logging/RhoLog.h"

#if !defined(OS_WINDOWS) && !defined(OS_WINCE)
typedef int SOCKET;
#  define INVALID_SOCKET -1
#  define SOCKET_ERROR -1
#  define RHO_NET_ERROR_CODE errno
#  define closesocket close
#else
#  if defined(OS_WINCE)
#    include <winsock2.h>
#    include <ws2tcpip.h>
#  endif
#  define RHO_NET_ERROR_CODE ::WSAGetLastError()
#endif

namespace rho
{
namespace net
{

#if !defined(OS_WINCE)

class RawSocket
{
public:
    RawSocket(const String& host, const String& port)  {}
    virtual ~RawSocket() {}

    bool init() { return false; }

    bool send(const String& sendData) { return false; }

    bool isInit() const { return false; }
};

#else                 

class RawSocket
{
private:
    SOCKET m_clientSocket;

    std::string m_hostName;
    std::string m_hostPort;

    bool m_isInit;
public:
    RawSocket(const String& host, const String& port) 
         : m_hostName(host) , m_hostPort(port), m_isInit(false) {}
    virtual ~RawSocket() { cleanup(); }

    bool init();

    bool send(const String& sendData);

    bool isInit() const { return m_isInit; }

private:
    bool create();

    void cleanup();
};

#endif

} // end of net
} // end if rho

#endif