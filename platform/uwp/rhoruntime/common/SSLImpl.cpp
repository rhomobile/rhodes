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

#include "RhoConvertWP8.h"

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
				ptr = NULL;
			}
		}

		CURLcode SSLImpl::connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage, char* host_name)
		{
			struct sockaddr_in adr_inet= {0};
			int len_inet = sizeof(adr_inet);
			int res;
			CURLcode retCode = CURLE_SSL_CONNECT_ERROR;

			ssl_data_t* Storage = (ssl_data_t*)storage;
			if(Storage == NULL)
				return CURLE_SSL_CONNECT_ERROR;

			res = getpeername(sockfd, (struct sockaddr *)&adr_inet, &len_inet);
			if(res == -1)
				return CURLE_SSL_CONNECT_ERROR;

			//struct hostent *remoteHost;
			//remoteHost = gethostbyaddr((char *)&(adr_inet.sin_addr), 4, AF_INET);
			Platform::String^ host = rho::common::convertStringCToWP8(strtok (host_name, ":"));//remoteHost->h_name);
			Platform::String^ port = rho::common::convertStringCToWP8(strtok (NULL, ":"));

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

			*done = 1;
			return retCode;
		}

		void SSLImpl::shutdown(void *storage)
		{
			ssl_data_t* Storage = (ssl_data_t*)storage;
			if (Storage == nullptr)
				return;

			delete Storage->m_sslSocket;
			Storage->m_sslSocket = nullptr;
		}

		ssize_t SSLImpl::send(const void *mem, size_t len, void *storage)
		{
			int retCode = -1;
			
			ssl_data_t* Storage = (ssl_data_t*)storage;

			if(Storage == NULL)
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

			if(retCode == -1)
				return -1;

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

			if(Storage == NULL)
				return -1;

			IInputStream^ is = Storage->m_sslSocket->InputStream;
		
			auto reader = ref new DataReader(is);

			reader->InputStreamOptions = InputStreamOptions::Partial;

			task<unsigned int>(reader->LoadAsync(size)).then([this, reader, &retCode, buf] (task<unsigned int> bytesLoaded) {
				try
				{
					retCode = bytesLoaded.get();	
					auto data = ref new Platform::Array<uint8>(retCode);
			        reader->ReadBytes(data);
					memcpy(buf, data->Data, retCode);
				}
				catch (Platform::Exception^ exception)
				{
					RAWLOG_ERROR("SSL read failed");
					retCode = -1;
				}

			}).wait();
			reader->DetachStream();
			*wouldblock = (retCode > 0) ? 0 : 1;

			return retCode;
		}

		bool SSLImpl::rand(unsigned char *entropy, size_t length)
		{
			for (int i = 0; i < length; i++){entropy[i] = std::rand() % 256;}
			//TODO: make it more security random
			return true;
		}

	} // namespace net
} // namespace rho
