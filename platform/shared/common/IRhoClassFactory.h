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

#include "common/RhoStd.h"
#include "net/INetRequest.h"

namespace rho {

namespace net
{
    struct ISSL;
}

struct ICallbackObject
{
    virtual ~ICallbackObject(){};
    virtual unsigned long getObjectValue() = 0;
};

namespace common {
struct IRhoThreadImpl;
struct IRhoCrypt;
class ISecurityTokenGenerator;

struct IRhoClassFactory
{
    virtual ~IRhoClassFactory(void){;}

    virtual IRhoThreadImpl* createThreadImpl() = 0;
    virtual net::ISSL* createSSLEngine() = 0;

    virtual IRhoCrypt* createRhoCrypt() = 0;

    virtual net::INetRequestImpl* createNetRequestImpl() = 0;
    virtual void registerNetRequestBuilder( const String& name, net::INetRequestBuilder* builder, bool setAsDefault = true ) = 0;
    virtual void setDefaultNetRequestBuilder( const String& name ) = 0;

    virtual const ISecurityTokenGenerator* createSecurityTokenGenerator() = 0;
};

//abstract base class stubbing some of IRhoClassFactory.
//override for specific platforms.
class CBaseRhoClassFactory : public IRhoClassFactory
{
public:

    CBaseRhoClassFactory()
        : mSelectedNetRequestBuilder(0)
    {

    }

    virtual void registerNetRequestBuilder( const String& name, net::INetRequestBuilder* builder, bool setAsDefault = true )
    {
        mNetRequestBuilders[name] = builder;

        if ( setAsDefault ) {
            setDefaultNetRequestBuilder( name );
        }
    }

    virtual void setDefaultNetRequestBuilder( const String& name )
    {
        mSelectedNetRequestBuilder = 0;

        auto search = mNetRequestBuilders.find( name );
        if ( search != mNetRequestBuilders.end() ) {
            mSelectedNetRequestBuilder = search->second;
        }
    }

    virtual net::INetRequestImpl* createNetRequestImpl()
    {
        net::INetRequestImpl* ret = 0;
        if ( mSelectedNetRequestBuilder != 0 ) {
            ret = mSelectedNetRequestBuilder->build();
        }
        return ret;
    }

private:
    std::map<String, net::INetRequestBuilder*> mNetRequestBuilders;
    net::INetRequestBuilder* mSelectedNetRequestBuilder;
};

}
}

rho::common::IRhoClassFactory* rho_get_RhoClassFactory();

