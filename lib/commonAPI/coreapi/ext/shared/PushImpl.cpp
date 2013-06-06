/*
#-------------------------------------------------------------------------------
# (The MIT License)
#
# Copyright (c) 2013 Rhomobile, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# http://rhomobile.com
#-------------------------------------------------------------------------------
*/

#include "Push.h"
#include "RhoStd.h"

namespace rho {
namespace push {

//----------------------------------------------------------------------------------------------------------------------
class CPushSingleton: public CPushManager {
    VectorPtr<IPush*> m_clients;
public:
    virtual ~CPushSingleton() {}
    String getInitialDefaultID();
    void enumerate(rho::apiGenerator::CMethodResult& oResult);
    void addClient(IPush* pClient) { m_clients.addElement(pClient); }
    IPush* getClient(const String& id);
};

//----------------------------------------------------------------------------------------------------------------------
class CPushFactory : public CPushFactoryBase
{
public:
    virtual ~CPushFactory() {}
    IPushSingleton* createModuleSingleton()
    {
        return new CPushSingleton();
    }
    IPush* getModuleByID(const rho::String& strID);
};

//----------------------------------------------------------------------------------------------------------------------
CPushManager* CPushManager::getInstance()
{
    return reinterpret_cast<CPushManager*>(CPushFactoryBase::getInstance()->getModuleSingleton());
}

//----------------------------------------------------------------------------------------------------------------------
String CPushSingleton::getInitialDefaultID()
{
    if (m_clients.empty())
    {
        return "";
    }
    else
    {
        CMethodResult result;
        m_clients.front()->getId(result);
        return result.getString();
    }
}

//----------------------------------------------------------------------------------------------------------------------
void CPushSingleton::enumerate(rho::apiGenerator::CMethodResult& oResult)
{
    Vector<String> ids;
    for(VectorPtr<IPush*>::const_iterator it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        CMethodResult result;
        m_clients.front()->getId(result);
        ids.push_back(result.getString());
    }
    oResult.set(ids);
}

//----------------------------------------------------------------------------------------------------------------------
IPush* CPushSingleton::getClient(const String& id)
{
    for(VectorPtr<IPush*>::const_iterator it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        CMethodResult result;
        (*it)->getId(result);
        if(result.getString() == id)
        {
            return *it;
        }
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------
IPush* CPushFactory::getModuleByID(const rho::String& strID)
{
    return (reinterpret_cast<CPushSingleton*>(getModuleSingleton()))->getClient(strID);
}

//----------------------------------------------------------------------------------------------------------------------

}}

extern "C" void Init_Push()
{
    rho::CPushFactoryBase::setInstance( new rho::push::CPushFactory() );
    rho::Init_Push_API();
}
//----------------------------------------------------------------------------------------------------------------------
