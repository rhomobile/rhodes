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

#include "SSLImpl.h"

#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SSL"

namespace rho
{
	namespace net
	{

		void * SSLImpl::createStorage()
		{
			return new ssl_data_t();
		}

		void SSLImpl::freeStorage(void *ptr)
		{
			if(ptr)
			{
				delete ptr;
				ptr = null;
			}
		}

		CURLcode SSLImpl::connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage)
		{
			struct sockaddr_in adr_inet;
			int len_inet;
			int res;
			char buffer [33];
			CURLcode retCode = CURLE_SSL_CONNECT_ERROR;

			ssl_data_t* Storage = (ssl_data_t*)storage;
			if(Storage == null)
				return CURLE_SSL_CONNECT_ERROR;

			res = getpeername(sockfd, (struct sockaddr *)&adr_inet, &len_inet);
			if(res == -1)
				return CURLE_SSL_CONNECT_ERROR;

			Platform::String^ host = A2PS(inet_ntoa(adr_inet.sin_addr));
			Platform::String^ port = A2PS(itoa(adr_inet.sin_port, buffer, 10));

			task<void>(Storage->m_sslSocket->ConnectAsync(ref new HostName(host), port, Sockets::SocketProtectionLevel::Ssl)).then([this, Storage, &retCode] (task<void> previousTask) {
				try
				{
					previousTask.get();
					retCode = CURLE_OK;
				}
				catch (Platform::Exception^ exception)
				{
					delete Storage->m_sslSocket;
					Storage->m_sslSocket = nullptr;
					RAWLOG_ERROR("SSL connection error");
					retCode = CURLE_SSL_CONNECT_ERROR;
				}

			}).wait();

			return retCode;
		}

		void SSLImpl::shutdown(void *storage)
		{
			ssl_data_t* st = (ssl_data_t*)storage;
			if (st == nullptr)
				return;

			delete st->m_sslSocket;
			st->m_sslSocket = nullptr;
		}

		ssize_t SSLImpl::send(const void *mem, size_t len, void *storage)
		{
			int retCode = -1;
			
			ssl_data_t* Storage = (ssl_data_t*)storage;

			if(Storage == null)
				return -1;

			IOutputStream^ os = Storage->m_sslSocket->OutputStream;

			auto writer = ref new DataWriter(os);

			auto data = ref new Platform::Array<uint8>(len);

			memcpy(data->Data, mem, len);

			writer->WriteBytes(data);

			task<unsigned int>(writer->StoreAsync()).then([this, &retCode] (task<unsigned int> bytesStored) {
				try
				{
					retCode = bytesStored.get();
				}
				catch (Platform::Exception^ exception)
				{
					RAWLOG_ERROR("SSL send failed");
					retCode = -1;
				}

			}).wait();

			if(retCode == CURLE_SSL_CONNECT_ERROR)
				return CURLE_SSL_CONNECT_ERROR;

			task<bool>(writer->FlushAsync()).then([this, &retCode] (task<bool> isFlushed) {
				try
				{
					if(!isFlushed.get())
						retCode = -1;
				}
				catch (Platform::Exception^ exception)
				{
					RAWLOG_ERROR("SSL send failed");
					retCode = CURLE_SSL_CONNECT_ERROR;
				}

			}).wait();

			return retCode;
		}

		ssize_t SSLImpl::recv(char *buf, size_t size, int *wouldblock, void *storage)
		{
			int retCode = -1;
			
			ssl_data_t* Storage = (ssl_data_t*)storage;

			if(Storage == null)
				return -1;

			IInputStream^ is = Storage->m_sslSocket->InputStream;
		
			auto reader = ref new DataReader(is);

			auto data = ref new Platform::Array<uint8>(size);

			task<unsigned int>(reader->LoadAsync(size)).then([this, reader, &retCode, data] (task<unsigned int> bytesLoaded) {
				try
				{
					retCode = bytesLoaded.get();								
			        reader->ReadBytes(data);
				}
				catch (Platform::Exception^ exception)
				{
					RAWLOG_ERROR("SSL read failed");
					retCode = -1;
				}

			}).wait();

			*wouldblock = 0;

			return retCode;
		}

	} // namespace net
} // namespace rho
