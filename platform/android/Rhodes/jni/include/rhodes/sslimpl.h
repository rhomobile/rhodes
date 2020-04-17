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

#ifndef RHO_SSL_IMPL_EC601E9D39EFA5CBA3F57583992797A
#define RHO_SSL_IMPL_EC601E9D39EFA5CBA3F57583992797A

#include <net/ssl.h>
#include <jni.h>

namespace rho {
namespace net {

class CertificateStorage
{
public:
    static CertificateStorage* instance();
    ~CertificateStorage();

private:
    const char* raw_cert = nullptr;
    const X509* cert = nullptr;
    const X509* client_cert = nullptr;
    const EVP_PKEY* private_key = nullptr;

    std::string pwd;
    std::vector<char> raw_p12;

public:
    CertificateStorage();

    const EVP_PKEY* getPrivateKey() const
    {
        return private_key;
    }

    const X509* getCertificate() const
    {
        return cert;
    }

    const X509* getClientCertificate() const
    {
        return client_cert;
    }

    const char* getRawCertificate() const
    {
        return raw_cert;
    }

    const std::vector<char>& getRawP12() const
    {
        return raw_p12;
    }

    const std::string& getPasswordForP12() const
    {
        return pwd;
    }
};

class SSLImpl : public ISSL
{
public:
    SSLImpl();

    void* createStorage();
    void freeStorage(void *ptr);

    CURLcode connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage, char* host_name);
    void shutdown(void *storage);
    ssize_t send(const void *mem, size_t len, void *storage);
    ssize_t recv(char *buf, size_t size, int *wouldblock, void *storage);
    bool rand(unsigned char *entropy, size_t length);

    const EVP_PKEY* getPrivateKey() const override
    {
        return CertificateStorage::instance()->getPrivateKey();
    }

    const X509* getCertificate() const override
    {
        return CertificateStorage::instance()->getCertificate();
    }

    const X509* getClientCertificate() const override
    {
        return CertificateStorage::instance()->getClientCertificate();
    }

private:
    jclass cls;
    jmethodID midConstructor = nullptr;
    jmethodID midConnect = nullptr;
    jmethodID midShutdown = nullptr;
    jmethodID midSend = nullptr;
    jmethodID midRecv = nullptr;
    jmethodID midRand = nullptr;
    jmethodID midSetRawCertificate = nullptr;
    jmethodID midSetP12 = nullptr;
};

}
}

#endif // RHO_SSL_IMPL_EC601E9D39EFA5CBA3F57583992797A

