/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#ifndef RHO_RAW_SOCKET_F5FFD21AD3EE463E850C5E2C789397BD
#define RHO_RAW_SOCKET_F5FFD21AD3EE463E850C5E2C789397BD

#include "common/RhoStd.h"
#include "logging/RhoLog.h"

#if !defined(WINDOWS_PLATFORM)
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
