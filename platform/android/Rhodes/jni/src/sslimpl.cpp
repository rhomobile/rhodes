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
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/pkcs12.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/x509v3.h>

#include "rhodes/sslimpl.h"
#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_socket_SSLImpl.h"

#include <arpa/inet.h>

#include <cstring>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SSLImpl"

void print_last_openssl_error()
{
    char err_buffer[255] = {};
    auto code = ERR_peek_last_error();
    ERR_error_string_n(code, err_buffer, sizeof(err_buffer));
    RAWLOG_ERROR1("openssl return error: %s", err_buffer);
}

int add_ext(X509 *cert, int nid, char *value)
{
    X509_EXTENSION *ex;
    X509V3_CTX ctx;
    X509V3_set_ctx_nodb(&ctx);

    X509V3_set_ctx(&ctx, cert, cert, NULL, NULL, 0);
    ex = X509V3_EXT_conf_nid(NULL, &ctx, nid, value);
    if (!ex)
        return 0;

    X509_add_ext(cert, ex, -1);
    X509_EXTENSION_free(ex);
    return 1;
}

EVP_PKEY* generate_private_key()
{
    EVP_PKEY* private_key = EVP_PKEY_new();

    if (!private_key)
    {
        print_last_openssl_error();
        return nullptr;
    }

    RSA* rsa_key = RSA_generate_key(2048, RSA_F4, nullptr, nullptr);
    if (!EVP_PKEY_assign_RSA(private_key, rsa_key))
    {
        print_last_openssl_error();
        EVP_PKEY_free(private_key);
        return nullptr;
    }

    return private_key;
}

X509* generate_cert(EVP_PKEY* private_key, bool isClient)
{
    X509* x509 = X509_new();
    if (!x509)
    {
        print_last_openssl_error();
        return nullptr;
    }

    X509_set_version(x509, 2);
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 0);
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 60L * 60L * 24L * 365L);
    X509_set_pubkey(x509, private_key);
    X509_NAME* name = X509_get_subject_name(x509);

    if (!X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char *)"US", -1, -1, 0))
    {
        print_last_openssl_error();
        X509_free(x509);
        return nullptr;
    }

    if (!X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"localhost", -1, -1, 0))
    {
        print_last_openssl_error();
        X509_free(x509);
        return nullptr;
    }

    if (!X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char *)"TauTechnolgies", -1, -1, 0))
    {
        print_last_openssl_error();
        X509_free(x509);
        return nullptr;
    }


    if (!X509_set_issuer_name(x509, name))
    {
        print_last_openssl_error();
        X509_free(x509);
        return nullptr;
    }

    if (isClient)
    {
        add_ext(x509, NID_ext_key_usage, (char*)"clientAuth");
        add_ext(x509, NID_netscape_cert_type, (char*)"client");
    }

    if (!X509_sign(x509, private_key, EVP_sha1()))
    {
        print_last_openssl_error();
        X509_free(x509);
        return nullptr;
    }

    return x509;
}

const char* extract_raw_data(X509* cert)
{
    BIO* bio = BIO_new(BIO_s_mem());
    if(!PEM_write_bio_X509(bio, cert))
    {
        print_last_openssl_error();
        BIO_free(bio);
        return nullptr;
    }

    char* data = nullptr;
    BUF_MEM* buff_mem = nullptr;

    BIO_get_mem_data(bio, &data);
    BIO_get_mem_ptr(bio, &buff_mem);
    BIO_set_close(bio, BIO_NOCLOSE);

    buff_mem->data = nullptr;
    buff_mem->length = 0;
    buff_mem->max = 0;

    BUF_MEM_free(buff_mem);
    BIO_free(bio);

    return (const char*)data;
}

DH* get_safe_dh_params()
{
    static const char* dh_param = 
        "-----BEGIN DH PARAMETERS-----\n"
        "MIIBCAKCAQEApXLjKj+Xoh+mgTKVbA750GEpDvDRxdzCQGfp7YAVTrCFPCD/KFBT\n"
        "n3jAfYJ2cpSg6300tsHDNwoS9SEiqSVTtJe7Y6BDU8NsLimDJyHV4f85SBsKEltx\n"
        "34H3CXzih/wIpMkkjpgX92MuJgMqpM6cg/t4dNda7fMP/9jEWnZ1tZkg2RpEqiHA\n"
        "yTlfB7dT+YaKThbq8oq1EyZxaS31Ud7tU73FHdAz+beSr6NGc4NJB0GSdjtI7eSX\n"
        "Gm5NggSN2lsbKpLMPtISkXGz/veF/ArYoLPAjGcvYy3xul4ND+BzVW63ySbmqUWW\n"
        "YPebgsdPcragYyE9F6c7YSf8sxBS+d2/kwIBAg==\n"
        "-----END DH PARAMETERS-----";

    BIO* bio = BIO_new_mem_buf(dh_param, (int)strlen(dh_param));
    DH* dh = PEM_read_bio_DHparams(bio, nullptr, nullptr, nullptr);
    if (!dh)
    {
        print_last_openssl_error();
        BIO_free(bio);
        return nullptr;
    }

    BIO_free(bio);

    return dh;
}

const char* create_p12(EVP_PKEY* private_key, X509* cert, size_t* outLen, std::string& outPwd)
{
    unsigned char symbols[8] = {};
    unsigned char base64_pwd[256] = {};
    RAND_bytes(symbols, sizeof(symbols));
    int pwdLen = 0;

    EVP_ENCODE_CTX* ctx = EVP_ENCODE_CTX_new();
    EVP_EncodeInit(ctx);
    EVP_EncodeUpdate(ctx, base64_pwd, &pwdLen, symbols, sizeof(symbols));
    EVP_EncodeFinal(ctx, base64_pwd, &pwdLen);
    EVP_ENCODE_CTX_free(ctx);
    outPwd.assign((const char*)base64_pwd, pwdLen - 1);


    BIO* bio = BIO_new(BIO_s_mem());
    PKCS12* p12 = PKCS12_create(outPwd.c_str(), "taup12", private_key, cert, nullptr, 0, 0, 0, 0, 0);

    if (!p12)
    {
        print_last_openssl_error();
        BIO_free(bio);
        return nullptr;
    }

    if (!i2d_PKCS12_bio(bio, p12))
    {
        print_last_openssl_error();
        BIO_free(bio);
        PKCS12_free(p12);
        return nullptr;
    }

    char* data = nullptr;
    BUF_MEM* buff_mem = nullptr;

    BIO_get_mem_data(bio, &data);
    BIO_get_mem_ptr(bio, &buff_mem);
    BIO_set_close(bio, BIO_NOCLOSE);

    *outLen = buff_mem->length;
    buff_mem->data = nullptr;
    buff_mem->length = 0;
    buff_mem->max = 0;

    BUF_MEM_free(buff_mem);
    BIO_free(bio);
    PKCS12_free(p12);

    return (const char*)data;
}

rho::net::CertificateStorage* rho::net::CertificateStorage::instance()
{
    static CertificateStorage _storage;
    return &_storage;
}

rho::net::CertificateStorage::~CertificateStorage()
{
    if(raw_cert)
        OPENSSL_free((void*)raw_cert);
    X509_free((X509*)cert);
    EVP_PKEY_free((EVP_PKEY*)private_key);
}

rho::net::CertificateStorage::CertificateStorage()
{
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    private_key = generate_private_key();
    if(!private_key) return;

    cert = generate_cert((EVP_PKEY*)private_key, false);
    if(!cert) 
    {
        EVP_PKEY_free((EVP_PKEY*)private_key);
        return;
    }

    raw_cert = extract_raw_data((X509*)cert);
    if(!raw_cert)    
    {
        RAWLOG_INFO("Failed to generate certificate!!!");
        X509_free((X509*)cert);
        EVP_PKEY_free((EVP_PKEY*)private_key);
        return;
    }

    EVP_PKEY* private_key_client = generate_private_key();
    if(!private_key_client) return;

    client_cert = generate_cert(private_key_client, true);
    if(!client_cert) 
    {
        EVP_PKEY_free(private_key_client);
        return;
    }

    size_t outLen = 0;
    const char* p12 = create_p12(private_key_client, (X509*)client_cert, &outLen, pwd);

    dh_params = get_safe_dh_params();
        
    //X509_free(cert_client);
    EVP_PKEY_free(private_key_client);

    if(!p12)
    {
        RAWLOG_INFO("Failed to generate p12!!!");
    }
    else
    {
        raw_p12.reserve(outLen);    
        raw_p12.resize(outLen);
        memcpy(raw_p12.data(), p12, outLen);
        OPENSSL_free((void*)p12);
    }

}

RHO_GLOBAL jobject JNICALL Java_com_rhomobile_rhodes_socket_SSLImpl_getRemoteSockAddr
  (JNIEnv *env, jobject, jint sock)
{
    jclass clsSockAddr = getJNIClass(RHODES_JAVA_CLASS_RHOSOCKADDR);
    if (!clsSockAddr) return NULL;
    jclass clsInetAddr = getJNIClass(RHODES_JAVA_CLASS_INETADDRESS);
    if (!clsInetAddr) return NULL;
    jmethodID midSockAddr = getJNIClassMethod(env, clsSockAddr, "<init>", "()V");
    if (!midSockAddr) return NULL;
    jmethodID midInetAddr = getJNIClassStaticMethod(env, clsInetAddr, "getByAddress", "(Ljava/lang/String;[B)Ljava/net/InetAddress;");
    if (!midInetAddr) return NULL;
    jfieldID fidInetAddr = getJNIClassField(env, clsSockAddr, "host", "Ljava/net/InetAddress;");
    if (!fidInetAddr) return NULL;
    jfieldID fidPort = getJNIClassField(env, clsSockAddr, "port", "I");
    if (!fidPort) return NULL;

    jholder<jbyteArray> array = jholder<jbyteArray>(env->NewByteArray(4));
    if (!array) return NULL;
    jbyte *arr = env->GetByteArrayElements(array.get(), NULL);
    if (!arr) return NULL;

    sockaddr_in sa;
    socklen_t salen = sizeof(sa);
    int e = ::getpeername(sock, (sockaddr*)&sa, &salen);
    if (e == -1) {
      RAWLOG_ERROR1("getpeername failed: %d", errno);
      return NULL;
    }
    uint32_t addr = ntohl(sa.sin_addr.s_addr);
    arr[3] = (jbyte)(addr & 0xFF);
    addr >>= 8;
    arr[2] = (jbyte)(addr & 0xFF);
    addr >>= 8;
    arr[1] = (jbyte)(addr & 0xFF);
    addr >>= 8;
    arr[0] = (jbyte)(addr & 0xFF);
    env->ReleaseByteArrayElements(array.get(), arr, 0);

    jhstring ipaddrObj = rho_cast<jstring>(env, ::inet_ntoa(sa.sin_addr));
    jhobject inetaddrObj = env->CallStaticObjectMethod(clsInetAddr, midInetAddr, ipaddrObj.get(), array.get() );
    if (!inetaddrObj) return NULL;

    jhobject sockaddrObj = env->NewObject(clsSockAddr, midSockAddr);
    if (!sockaddrObj) return NULL;

    env->SetObjectField(sockaddrObj.get(), fidInetAddr, inetaddrObj.get());
    env->SetIntField(sockaddrObj.get(), fidPort, ntohs(sa.sin_port));

    return sockaddrObj.release();
}

namespace rho
{
namespace net
{

SSLImpl::SSLImpl()
{
    JNIEnv *env = jnienv();
    if (!env) {
       return;
    }
    cls = getJNIClass(RHODES_JAVA_CLASS_SSLIMPL);
    if (!cls) return;
    midConstructor = getJNIClassMethod(env, cls, "<init>", "()V");
    if (!midConstructor) return;
    midConnect = getJNIClassMethod(env, cls, "connect", "(IZLjava/lang/String;)Z");
    if (!midConnect) return;
    midShutdown = getJNIClassMethod(env, cls, "shutdown", "()V");
    if (!midShutdown) return;
    midSend = getJNIClassMethod(env, cls, "send", "([B)Z");
    if (!midSend) return;
    midRecv = getJNIClassMethod(env, cls, "recv", "([B)I");
    if (!midRecv) return;
    midRand = getJNIClassStaticMethod(env, cls, "rand", "([B)Z"); 
    if(!midRand) return;
    midSetRawCertificate = getJNIClassStaticMethod(env, cls, "setRawCertificate", "([B)V");
    if (!midSetRawCertificate) return;
    midSetP12 = getJNIClassStaticMethod(env, cls, "setP12", "([BLjava/lang/String;)V");
    if (!midSetP12) return;

    CertificateStorage::instance();
}

void* SSLImpl::createStorage()
{
    JNIEnv *env = jnienv();
    jobject obj = env->NewObject(cls, midConstructor);
    jobject objG = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);

    const char* raw_cert = CertificateStorage::instance()->getRawCertificate();
    if (raw_cert)
    {
        size_t cert_len = strlen(raw_cert);
        jholder<jbyteArray> cert = env->NewByteArray(cert_len);
        env->SetByteArrayRegion(cert.get(), 0, cert_len, (jbyte const *)raw_cert);
        env->CallStaticVoidMethod(cls, midSetRawCertificate, cert.get());

        auto &raw_p12 = CertificateStorage::instance()->getRawP12();
        const std::string &pwd = CertificateStorage::instance()->getPasswordForP12();

        if (!raw_p12.empty())
        {
            jholder<jbyteArray> p12 = env->NewByteArray(raw_p12.size());
            env->SetByteArrayRegion(p12.get(), 0, raw_p12.size(), (jbyte const *)raw_p12.data());
            jhstring _pwd = rho_cast<jstring>(env, pwd.c_str());
            env->CallStaticVoidMethod(cls, midSetP12, p12.get(), (jstring)_pwd.get());

            CertificateStorage::instance()->clearRaws();
        }
    }

    return objG;
}

void SSLImpl::freeStorage(void *ptr)
{
    if (!ptr) return;
    jobject obj = (jobject)ptr;
    jnienv()->DeleteGlobalRef(obj);
}

CURLcode SSLImpl::connect(int sockfd, int nonblocking, int *done, int ssl_verify_peer, void *storage, char* host_name)
{
    if (!storage) return CURLE_SSL_CONNECT_ERROR;

    jobject obj = (jobject)storage;

    JNIEnv* env = jnienv();
    jhstring jHostname = rho_cast<jstring>(env, host_name);

    jboolean result = jnienv()->CallBooleanMethod(obj, midConnect, sockfd, (jboolean)ssl_verify_peer, (jstring)jHostname.get());
    if (!result)
        return CURLE_SSL_CONNECT_ERROR;

    *done = 1;
    return CURLE_OK;
}

void SSLImpl::shutdown(void *storage)
{
    if (!storage) return;

    RAWTRACE("shutdown");

    jobject obj = (jobject)storage;
    jnienv()->CallVoidMethod(obj, midShutdown);
}

ssize_t SSLImpl::send(const void *mem, size_t len, void *storage)
{
    if (!storage) return -1;

    JNIEnv *env = jnienv();
    jholder<jbyteArray> array = jholder<jbyteArray>(env->NewByteArray(len));
    if (!array) return -1;
    jbyte *arr = env->GetByteArrayElements(array.get(), NULL);
    if (!arr) return -1;
    std::memmove(arr, mem, len);
    env->ReleaseByteArrayElements(array.get(), arr, 0);

    jobject obj = (jobject)storage;
    jboolean result = env->CallBooleanMethod(obj, midSend, array.get());
    if (!result) return -1;
    return len;
}

bool SSLImpl::rand(unsigned char *entropy, size_t length)
{
    JNIEnv *env = jnienv();
    jholder<jbyteArray> array = env->NewByteArray(length);
    if(!array) return false;
    
    jboolean result = env->CallStaticBooleanMethod(cls, midRand, array.get());
    if(result)
    {
        jbyte *arr = env->GetByteArrayElements(array.get(), NULL);
        std::memmove(entropy, arr, length);
    }

    return result;    
}

ssize_t SSLImpl::recv(char *buf, size_t size, int *wouldblock, void *storage)
{
    *wouldblock = 0;
    if (!storage) return -1;

    jobject obj = (jobject)storage;
    JNIEnv *env = jnienv();

    static jfieldID fid = 0;
    if(!fid)
    {
        jclass clsSock = getJNIObjectClass(env, obj);
        if (!clsSock) return -1;

        fid = getJNIClassField(env, clsSock, "sockfd", "I");

        env->DeleteLocalRef(clsSock);
    }
    if (!fid) return -1;

    jint sock = env->GetIntField(obj, fid);

    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(sock, &rfd);
    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    int n = ::select(sock + 1, &rfd, NULL, NULL, &tv);
    if (n == 0) {
        *wouldblock = 1;
        return -1;
    }

    jholder<jbyteArray> array = env->NewByteArray(size);
    jint result = env->CallIntMethod(obj, midRecv, array.get());

    if (result > 0) {
        jbyte *arr = env->GetByteArrayElements(array.get(), NULL);
        if (!arr) return -1;
        std::memmove(buf, arr, result);
        env->ReleaseByteArrayElements(array.get(), arr, JNI_ABORT);
    }
    return result;
}

} // namespace net
} // namespace rho

