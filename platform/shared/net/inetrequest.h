#pragma once

#include "common/RhoStd.h"
#include "common/InputStream.h"

namespace rho {
namespace net {

struct INetData
{
    virtual ~INetData(void){;}
    virtual const char* getCharData() = 0;
    virtual unsigned int getDataSize() = 0;
};

struct INetRequest
{
    virtual ~INetRequest(void){;}

    virtual INetData* pullData(const String& strUrl ) = 0;
    virtual boolean pushData(const String& strUrl, const String& strBody)=0;
    virtual boolean pushFile(const String& strUrl, const String& strFileName)=0;
    virtual boolean pullCookies(const String& strUrl, const String& strBody)=0;
    //if strUrl.length() == 0 delete all cookies if possible
    virtual void deleteCookie(const String& strUrl)=0;

    virtual String resolveUrl(const String& strUrl)=0;

    virtual void cancelAll() = 0;

};

#define NetRequestStr(name,call)\
    rho::common::CAutoPtr<rho::net::INetData> p##name = call;\
    const char* name = p##name->getCharData();

}
}
