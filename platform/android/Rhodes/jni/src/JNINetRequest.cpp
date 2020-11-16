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

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Net"

extern "C" void rho_net_impl_network_indicator(int active);

IMPLEMENT_LOGCLASS(rho::net::JNINetRequest, "Net");

class JNINetResponseImpl : public rho::net::INetResponse
{
    rho::String m_data;
    int   m_nRespCode;
    rho::String m_cookies;
    rho::String m_errorMessage;

public:
    JNINetResponseImpl(char const *data, size_t size, int nRespCode)
            :m_nRespCode(nRespCode)
    {
        m_data.assign(data, size);
    }

    virtual const char* getCharData() const
    {
        return m_data.c_str();
    }

    virtual unsigned int getDataSize() const
    {
        return m_data.size();
    }

    virtual int getRespCode() const
    {
        return m_nRespCode;
    }

    virtual rho::String getCookies() const
    {
        return m_cookies;
    }

    virtual rho::String getErrorMessage() const
    {
        return m_errorMessage;
    }

    virtual void setCharData(const char* szData)
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

};

rho::net::INetResponse* rho::net::JNINetRequest::doRequest(const char *method, const String &strUrl, const String &strBody, IRhoSession *oSession, Hashtable<String, String> *pHeaders) {
    return nullptr;
}

void rho::net::JNINetRequest::cancel() {

}

rho::net::INetResponse* rho::net::JNINetRequest::pullFile(const rho::String &strUrl, common::CRhoFile &oFile, IRhoSession *oSession, Hashtable<String, rho::String> *pHeaders) {
    return doPull("GET", strUrl, String(), &oFile, oSession, pHeaders);
}

rho::net::INetResponse* rho::net::JNINetRequest::createEmptyNetResponse() {
    return nullptr;
}

rho::net::INetResponse* rho::net::JNINetRequest::pushMultipartData(const rho::String &strUrl, VectorPtr<CMultipartItem *> &arItems, IRhoSession *oSession, Hashtable<rho::String, rho::String> *pHeaders) {
    return nullptr;
}

rho::net::INetResponse* rho::net::JNINetRequest::doPull(const char *method, const rho::String &strUrl, const rho::String &strBody, common::CRhoFile *oFile, IRhoSession *oSession, Hashtable<rho::String, rho::String> *pHeaders) {

}