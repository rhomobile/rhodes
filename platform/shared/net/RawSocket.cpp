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

#include "net/RawSocket.h"
#include "common/RhodesApp.h"

#include <algorithm>

#if !defined(WINDOWS_PLATFORM)
#include <arpa/inet.h>
#endif

#if !defined(OS_WINCE)
#include <common/stat.h>

#ifdef EAGAIN
#undef EAGAIN
#endif
#define EAGAIN EWOULDBLOCK

#else

#include "CompatWince.h"

#if defined(OS_WINDOWS) || defined(OS_WINCE)
typedef unsigned __int16 uint16_t;

#  ifndef S_ISDIR
#    define S_ISDIR(m) ((_S_IFDIR & m) == _S_IFDIR)
#  endif

#  ifndef S_ISREG
#    define S_ISREG(m) ((_S_IFREG & m) == _S_IFREG)
#  endif

#  ifndef EAGAIN
#    define EAGAIN WSAEWOULDBLOCK
#  endif
#endif

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RawSocket"

namespace rho
{
namespace net
{

bool RawSocket::init()
{
    //RAWTRACE("Init raw socket");
    m_isInit = create();
    return m_isInit;
}

bool RawSocket::create()
{
    //RAWTRACE("Start create raw socket");
    cleanup();

    int iResult;
    addrinfo *addrInfo = NULL, hints;

    memset( &hints, 0, sizeof(hints) );
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    //RAWTRACE("Get host adress");

    // Resolve the server address and port
    iResult = getaddrinfo(m_hostName.c_str(), m_hostPort.c_str(), &hints, &addrInfo);

    if (iResult != 0) {
        //RAWTRACE2("Unable to get addres info for host %s, port %s", m_hostName.c_str(), m_hostPort.c_str());
        return false;
    }

    //RAWTRACE("Create socket");
    // Create a SOCKET for connecting to server
    m_clientSocket = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);

    if (m_clientSocket == INVALID_SOCKET) {
        //RAWTRACE2("Socket can`t create for host %s, port %s", m_hostName.c_str(), m_hostPort.c_str());
        freeaddrinfo(addrInfo);
        return false;
    }

    // Connect to server.
    //RAWTRACE("Connect to server");
    iResult = connect(m_clientSocket, addrInfo->ai_addr, (int)addrInfo->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        //RAWTRACE2("Can`t connect to host %s, port %s ", m_hostName.c_str(), m_hostPort.c_str());
        cleanup();
        return false;
    }

    freeaddrinfo(addrInfo);

    //RAWTRACE("End of socket creating");

    return true;
}

bool RawSocket::send(const String& sendData)
{
    int iResult = 0;

    // Send an initial buffer
    iResult = ::send(m_clientSocket, sendData.c_str(), (int) sendData.size(), 0);

    if (iResult == SOCKET_ERROR)
    {
        //RAWTRACE2("Data not send for host %s, port %s", m_hostName.c_str(), m_hostPort.c_str());
        cleanup();
        return false;
    }

    return true;
}

void RawSocket::cleanup()
{
    m_isInit = false;
    closesocket(m_clientSocket);
    m_clientSocket = INVALID_SOCKET;
}

} // namespace net
} // namespace rho

#endif
