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

#pragma once

#include "net/ssl.h"
#include <ppltasks.h>

using namespace Windows::Networking::Sockets;
using namespace Windows::Networking;
using namespace Windows::Storage::Streams;
using namespace Concurrency;

namespace rho
{
namespace net
{

struct ssl_data_t {
	ssl_data_t(){
		m_sslSocket = ref new StreamSocket();
	}
	~ssl_data_t(){
		if(m_sslSocket)
			delete m_sslSocket;
	}
	StreamSocket^ m_sslSocket;
};

class SSLImpl : public ISSL
{
public:
	SSLImpl() {};
	~SSLImpl() {};
    void *createStorage();
    void freeStorage(void *ptr);
    
    CURLcode connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage, char* host_name);
    void shutdown(void *storage);
    ssize_t send(const void *mem, size_t len, void *storage);
    ssize_t recv(char *buf, size_t size, int *wouldblock, void *storage);
	virtual bool rand(unsigned char *entropy, size_t length);
};

} // namespace net
} // namespace rho