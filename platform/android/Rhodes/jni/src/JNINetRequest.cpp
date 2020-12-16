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

#include "rhodes/JNINetRequest.h"

#include <common/RhodesApp.h>
#include <common/RhoConf.h>
#include <net/URI.h>

#include "rhodes/JNIRhodes.h"

#include <algorithm>
#include <random>
#include <sstream>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Net"

extern "C" void rho_net_impl_network_indicator(int active);

IMPLEMENT_LOGCLASS(rho::net::JNINetRequest, "Net");

class JNetResponseImpl : public rho::net::INetResponse
{
    rho::String m_data;
    int   m_nRespCode;
    rho::String m_cookies;
    rho::String m_errorMessage;
    jcharArray body = nullptr;

public:
    JNetResponseImpl(char const *data, size_t size, int nRespCode) : m_nRespCode(nRespCode) {
        m_data.assign(data, size);
    }

    JNetResponseImpl(const rho::String& body, int nRespCode) : m_nRespCode(nRespCode) {
        m_data = std::move(body);
    }

    virtual const char* getCharData() const override
    {
        return m_data.c_str();
    }

    virtual unsigned int getDataSize() const override
    {
        return m_data.size();
    }

    virtual int getRespCode() const override
    {
        return m_nRespCode;
    }

    virtual rho::String getCookies() const override
    {
        return m_cookies;
    }

    virtual rho::String getErrorMessage() const override
    {
        return m_errorMessage;
    }

    virtual void setCharData(const char* szData) override
    {
        m_data = szData;
    }

    void setRespCode(int nRespCode)
    {
        m_nRespCode = nRespCode;
    }

    rho::boolean isOK() const
    {
        return m_nRespCode == 200 || m_nRespCode == 206;
    }

    rho::boolean isUnathorized() const
    {
        return m_nRespCode == 401;
    }

    rho::boolean isSuccess() const
    {
        return m_nRespCode > 0 && m_nRespCode < 400;
    }

    rho::boolean isResponseRecieved(){ return m_nRespCode!=-1;}

    void setCharData(const rho::String &data)
    {
        m_data = data;//.assign(data.begin(), data.end());
    }

    void setCookies(rho::String s)
    {
        m_cookies = s;
    }

    virtual ~JNetResponseImpl() {
        JNIEnv *env = jnienv();
        if(body)
            env->DeleteGlobalRef(body);
    }

};

void rho::net::JNINetRequest::ProxySettings::initFromConfig() {
    port = 0;

    if (RHOCONF().isExist("http_proxy_host")) {
        host = RHOCONF().getString("http_proxy_host");

        String strPort;

        if (RHOCONF().isExist("http_proxy_port")) {
            strPort = RHOCONF().getString("http_proxy_port");
            port = atoi(strPort.c_str());
        }

        if (RHOCONF().isExist("http_proxy_login")) {
            username = RHOCONF().getString ("http_proxy_login");
        }

        if (RHOCONF().isExist("http_proxy_password")) {
            password = RHOCONF().getString("http_proxy_password");
        }

        LOG(INFO) + "PROXY: " + host + " PORT: " + strPort + " USERNAME: " + username + " PASSWORD: " + password;
    }
}

rho::net::JNINetRequest::JNINetRequest()
{
    JNIEnv *env = jnienv();    
    if (!env) {   
        RAWLOG_ERROR("JNI init failed: jnienv is null");   
        return;    
    }

    cls = getJNIClass(RHODES_JAVA_CLASS_NETREQUEST);
    if (!cls) 
    {
        RAWLOG_ERROR("Class com/rhomobile/rhodes/NetRequest not found!");
        return;
    }

    clsHashMap = getJNIClass(RHODES_JAVA_CLASS_HASHMAP);
    if (!clsHashMap) return;
    midHashMapConstructor = getJNIClassMethod(env, clsHashMap, "<init>", "()V");
    if (!midHashMapConstructor) return;
    midPut = getJNIClassMethod(env, clsHashMap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (!midPut) return;

    jmethodID constructorNetRequest = getJNIClassMethod(env, cls, "<init>", "()V");
    if (!constructorNetRequest) 
    {
        RAWLOG_ERROR("Constructor for class com/rhomobile/rhodes/NetRequest not found!");
        return;
    }

    netRequestObject = env->NewObject(cls, constructorNetRequest);
    if(!netRequestObject)
    {
        RAWLOG_ERROR("NetRequest object not created!");
        return;
    }

    jobject _netRequestObject = env->NewGlobalRef(netRequestObject);
    env->DeleteLocalRef(netRequestObject);
    netRequestObject = _netRequestObject;

    midDoPull = getJNIClassMethod(env, cls, 
           "doPull", 
           "("
           "Ljava/lang/String;"
           "Ljava/lang/String;"
           "Ljava/lang/String;"
           "I"
           "Ljava/util/HashMap;"
           "ZJ"
           ")I");

    if(!midDoPull) {
        RAWLOG_ERROR("doPull method not found!");
        return;
    }

    midgetValuesFromResponseHeaders = getJNIClassMethod(env, cls, "getValuesFromResponseHeaders", "()[Ljava/lang/String;" );
    midgetKeysFromResponseHeaders = getJNIClassMethod(env, cls, "getKeysFromResponseHeaders", "()[Ljava/lang/String;" );
    midgetResponseBody = getJNIClassMethod(env, cls, "getResponseBody", "()Ljava/lang/String;" );
    midAddMultiPartData = getJNIClassMethod(env, cls, "AddMultiPartData", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );

    midSetAuthSettings = getJNIClassMethod(env, cls, "SetAuthSettings", "(Ljava/lang/String;Ljava/lang/String;Z)V" );

    timeout = rho_conf_getInt("net_timeout");
    if (timeout == 0)
        timeout = 30; // 30 seconds by default

}

jobject rho::net::JNINetRequest::makeJavaHashMap(const Hashtable<String,String>& table)
{
    JNIEnv *env = jnienv();
    jobject v = env->NewObject(clsHashMap, midHashMapConstructor);
    if (!v) return nullptr;

    for(const auto& row : table)
    {
        jhstring key = rho_cast<jstring>(row.first);
        jhstring value = rho_cast<jstring>(row.second);
        env->CallObjectMethod(v, midPut, key.get(), value.get());
    }

    return v;
}

rho::net::INetResponse* rho::net::JNINetRequest::doRequest(const char *method, const String &strUrl, const String &strBody, IRhoSession *oSession, Hashtable<String, String> *pHeaders) {
    return doPull(method, strUrl, strBody, nullptr, oSession, pHeaders);
}

void rho::net::JNINetRequest::cancel() {

}

rho::net::INetResponse* rho::net::JNINetRequest::pullFile(const rho::String &strUrl, common::CRhoFile &oFile, IRhoSession *oSession, Hashtable<String, rho::String> *pHeaders) {
    return doPull("GET", strUrl, String(), &oFile, oSession, pHeaders);
}

rho::net::INetResponse* rho::net::JNINetRequest::createEmptyNetResponse() {
    return new JNetResponseImpl("", 0, -1);
}

size_t rho::net::JNINetRequest::processMultipartItems(VectorPtr<CMultipartItem*>& arItems) {

    JNIEnv *env = jnienv();
    std::random_device device_engine;
    std::uniform_int_distribution<int> dist(0, 254);
    std::ostringstream ss;

    for(int i = 0; i < 8; i++) {
        int value = dist(device_engine);
        ss << std::hex << value;
    }

    m_multipartBoundary = ss.str();
    m_multipartPostfix = "\r\n";
    m_multipartPostfix += "--";
    m_multipartPostfix += m_multipartBoundary;
    m_multipartPostfix += "--\r\n";

    int nSize = 0;
    for( int i = 0; i < (int)arItems.size(); i++ ) {
        CMultipartItem& oItem = *arItems.elementAt(i);

        if ( oItem.m_strName.length() == 0 )
            oItem.m_strName = "blob";

        if ( oItem.m_strFileName.length() == 0 ) {
            if (oItem.m_strFilePath.length() > 0)  {
                //common::CFilePath oPath(oItem.m_strFilePath);
                //oItem.m_strFileName = oPath.getBaseName();
            }
        }

        oItem.m_strDataPrefix = i > 0 ? "\r\n" : "";
        oItem.m_strDataPrefix += "--";
        oItem.m_strDataPrefix += m_multipartBoundary;
        oItem.m_strDataPrefix += "\r\n";
        oItem.m_strDataPrefix += "Content-Disposition: form-data; name=\"";
        oItem.m_strDataPrefix += oItem.m_strName + "\"";
        if (oItem.m_strFileName.length() > 0) {
            oItem.m_strDataPrefix += "; filename=\"" + oItem.m_strFileName + "\"";
        }

        oItem.m_strDataPrefix += "\r\n";
        if (oItem.m_strContentType.length() > 0) {
            oItem.m_strDataPrefix += "Content-Type: " + oItem.m_strContentType + "\r\n";
        }

        int nContentSize = 0;
        if (oItem.m_strFilePath.length() > 0) {
            common::CRhoFile oFile;
            if (oFile.open(oItem.m_strFilePath.c_str(),common::CRhoFile::OpenReadOnly))
                nContentSize = oFile.size();
        }
        else {
            nContentSize = (int)oItem.m_strBody.length();
        }

        oItem.m_strDataPrefix += "Content-Length: " + common::convertToStringA(nContentSize) + "\r\n";
        oItem.m_strDataPrefix += "\r\n";
        nSize += oItem.m_strDataPrefix.length() + nContentSize;

        jhstring j_strFilePath = rho_cast<jstring>(env, oItem.m_strFilePath);
        jhstring j_strBody = rho_cast<jstring>(env, oItem.m_strBody);
        jhstring j_strName = rho_cast<jstring>(env, oItem.m_strName);
        jhstring j_strFileName = rho_cast<jstring>(env, oItem.m_strFileName);
        jhstring j_strContentType = rho_cast<jstring>(env, oItem.m_strContentType);
        jhstring j_strDataPrefix = rho_cast<jstring>(env, oItem.m_strDataPrefix);
        env->CallVoidMethod(netRequestObject, midAddMultiPartData,
                            j_strFilePath.get(),
                            j_strBody.get(),
                            j_strName.get(),
                            j_strFileName.get(),
                            j_strContentType.get(),
                            j_strDataPrefix.get());
    }

    nSize += m_multipartPostfix.length();
    return nSize;
}

rho::net::INetResponse* rho::net::JNINetRequest::pushMultipartData(const rho::String &strUrl, VectorPtr<CMultipartItem *> &arItems, IRhoSession *oSession, Hashtable<rho::String, rho::String> *pHeaders) {

    RAWLOG_INFO1("POST request (Push): %s", strUrl.c_str());
    rho_net_impl_network_indicator(1);

    ProxySettings proxySettings;
    proxySettings.initFromConfig();

    processMultipartItems(arItems);

    char multi_part_header_value[128] = {};
    sprintf(multi_part_header_value, "multipart/form-data; boundary=%s", m_multipartBoundary.c_str());
    pHeaders->emplace("Content-type", multi_part_header_value);

    rho_net_impl_network_indicator(0);

    return doPull("POST", strUrl, m_multipartPostfix, nullptr, oSession, pHeaders);
}

rho::net::INetResponse* rho::net::JNINetRequest::doPull(const char *method, const rho::String &strUrl,
        const rho::String &strBody, common::CRhoFile *oFile, IRhoSession *oSession, Hashtable<rho::String,
        rho::String> *pHeaders) {

    int nRespCode = -1;
    long nStartFrom = 0;
    if (oFile) {
        nStartFrom = oFile->size();
    }

    if( !RHODESAPP().isBaseUrl(strUrl.c_str()) )
    {
        //Log every non localhost requests
        RAWLOG_INFO2("%s request (Pull): %s", method, strUrl.c_str());
        rho_net_impl_network_indicator(1);
    }

    JNIEnv *env = jnienv();

    ProxySettings proxySettings;
    proxySettings.initFromConfig();

    Hashtable<rho::String, rho::String> empty_headers;
    Hashtable<rho::String, rho::String>* _pHeaders = pHeaders ? pHeaders : &empty_headers;
    set_options(strUrl, strBody, oFile, oSession, *_pHeaders);

    jhobject headers = makeJavaHashMap(*_pHeaders);
    jhstring jurl = rho_cast<jstring>(env, strUrl);
    jhstring jmethod = rho_cast<jstring>(env, method);
    jint jfd = rho_cast<jint>(env, oFile ? oFile->getFD() : -1);
    jhstring jbody = rho_cast<jstring>(env, strBody);
    jint _code = env->CallIntMethod(netRequestObject, midDoPull, jurl.get(), jmethod.get(), jbody.get(),
                                    jfd, headers.get(), m_sslVerifyper, timeout);
    nRespCode = rho_cast<int>(env, _code);

    if( !RHODESAPP().isBaseUrl(strUrl.c_str()) ) {
        rho_net_impl_network_indicator(0);
    }

    jhstring jresponse_body = static_cast<jstring>(env->CallObjectMethod(netRequestObject, midgetResponseBody));
    rho::String response_body = jresponse_body ? rho_cast<rho::String>(env, jresponse_body.get()) : "";
    getResponseHeader(*_pHeaders);

    if(oFile) {
        oFile->write(response_body.c_str(), response_body.length());
        oFile->flush();
    }

    if (m_pCallback)
    {
        NetResponse r = makeResponse(response_body, nRespCode);
        m_pCallback->didReceiveData(response_body.c_str(), response_body.length());
        m_pCallback->didReceiveResponse(r, _pHeaders);
        if (r.isOK()) {
            m_pCallback->didFinishLoading();
        }
        else {
            m_pCallback->didFail(r);
        }
    }

    if(oSession && nRespCode == 401)
        oSession->logout();

    return makeResponse(response_body, nRespCode);
}

void rho::net::JNINetRequest::set_options(const String &strUrl, const String &strBody,
        rho::common::CRhoFile *oFile, rho::net::IRhoSession *pSession,
        rho::Hashtable<rho::String, rho::String> headers) {

    rho::String session;
    if (pSession)
        session = pSession->getSession();

    if (!session.empty()) {
        //RAWTRACE1("Set cookie: %s", session.c_str());
        //curl_easy_setopt(m_curl, CURLOPT_COOKIE, session.c_str());
    }

    if (RHODESAPP().isBaseUrl(strUrl))
        timeout = 10*24*60*60;

    headers.emplace("Expect", "");
    headers.emplace("Connection", "Keep-Alive");

    auto it = headers.find("content-type");
    bool hasContentType = (it != headers.end());

    if (!hasContentType && strBody.length() > 0) {
        rho::String strHeader = "Content-Type";
        if ( pSession)
            headers.emplace(strHeader, pSession->getContentType().c_str());
        else
            headers.emplace(strHeader, "application/x-www-form-urlencoded");
    }

    JNIEnv *env = jnienv();
    switch (m_authMethod) {
        case AUTH_DIGEST:
        case AUTH_BASIC:
        {
            jhstring user = rho_cast<jstring>(env, m_authUser);
            jhstring pwd = rho_cast<jstring>(env, m_authPassword);
            env->CallVoidMethod(netRequestObject, midSetAuthSettings, user.get(), pwd.get(), m_authMethod == AUTH_DIGEST);
            break;
        }
        default:
            break;
    }


}

rho::net::INetResponse* rho::net::JNINetRequest::makeResponse(const rho::Vector<char> &body, int nErrorCode) {
    if(body.size() > 0)
        return makeResponse(&body[0], body.size(), nErrorCode);
    else
        return makeResponse(0, 0, nErrorCode);
}

rho::net::INetResponse* rho::net::JNINetRequest::makeResponse(const char *body, size_t bodysize, int nErrorCode) {
    RAWTRACE1("JNetResponseImpl::makeResponse - nErrorCode: %d", nErrorCode);
    if (!body) {
        body = "";
        bodysize = 0;
    }

    return new JNetResponseImpl(body, bodysize, nErrorCode);
}

rho::net::INetResponse* rho::net::JNINetRequest::makeResponse(const rho::String &body, int nErrorCode) {
    return new JNetResponseImpl(body, nErrorCode);
}

void rho::net::JNINetRequest::getResponseHeader(rho::Hashtable<rho::String, rho::String> &headers) {
    JNIEnv *env = jnienv();
    jharray keys = static_cast<jobjectArray>(env->CallObjectMethod(netRequestObject, midgetKeysFromResponseHeaders));
    jharray values = static_cast<jobjectArray>(env->CallObjectMethod(netRequestObject, midgetValuesFromResponseHeaders));

    std::vector<rho::String> v_keys, v_values;
    if(keys.get()) JArraytoVectorString(keys, v_keys);
    if(values.get()) JArraytoVectorString(values, v_values);

    if(v_keys.size() == v_values.size()) {
        for(size_t i = 0; i < v_keys.size(); ++i) {
            headers.emplace(v_keys[i], v_values[i]);
        }
    }

}

rho::net::JNINetRequest::~JNINetRequest() noexcept {
    JNIEnv *env = jnienv();
    if(netRequestObject) env->DeleteGlobalRef(netRequestObject);
}

void rho::net::JNINetRequest::JArraytoVectorString(const jharray &array, std::vector<rho::String>& v) {
    JNIEnv *env = jnienv();
    jsize count = env->GetArrayLength(array.get());

    for(jsize i = 0; i < count; ++i) {
        jhstring item = static_cast<jstring>(env->GetObjectArrayElement(array.get(), i));
        v.push_back(rho_cast<rho::String>(env, item.get()));
    }
}
