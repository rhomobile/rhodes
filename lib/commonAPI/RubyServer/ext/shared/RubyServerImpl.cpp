//
//  RubyServerImpl.cpp
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "generated/cpp/RubyServerBase.h"
#include "logging/RhoLog.h"
#include "rhoruby/api/RhoRuby.h"




class CRhoRubyServerRunMethodHolder : public  rho::ruby::IRunnable {
public:

    CRhoRubyServerRunMethodHolder(   const rho::String& classFullName,
                                    const rho::String& methodName,
                                    const rho::String& parameters_in_JSON,
                                    rho::apiGenerator::CMethodResult& mresult) {
        mClassName = classFullName;
        mMethodName = methodName;
        mParameters = parameters_in_JSON;
        mMethodResult = mresult;
    }

    virtual ~CRhoRubyServerRunMethodHolder() {}

    virtual void run() {
        rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
        rho::ruby::IString* rr_str = rr->executeRubyMethodWithJSON(mClassName.c_str(), mMethodName.c_str(), mParameters.c_str());
        rho::String res_str;
        if (rr_str != NULL) {
            res_str = rr_str->getUTF8();
        }
        mMethodResult.set(res_str);
        delete this;
    }

private:
    rho::String mClassName;
    rho::String mMethodName;
    rho::String mParameters;
    rho::apiGenerator::CMethodResult mMethodResult;

};

class CRhoRubyServerLoadRubyHolder : public  rho::ruby::IRunnable {
public:
    
    CRhoRubyServerLoadRubyHolder(   const rho::String& filename) {
        mFileName = filename;
    }
    
    virtual ~CRhoRubyServerLoadRubyHolder() {}
    
    virtual void run() {
        rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
        rr->loadRubyFile(mFileName.c_str());
        delete this;
    }
    
private:
    rho::String mFileName;
    
};

class CRhoRubyServerLoadModelHolder : public  rho::ruby::IRunnable {
public:
    
    CRhoRubyServerLoadModelHolder(   const rho::String& filename) {
        mFileName = filename;
    }
    
    virtual ~CRhoRubyServerLoadModelHolder() {}
    
    virtual void run() {
        rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
        rr->loadModel(mFileName.c_str());
        delete this;
    }
    
private:
    rho::String mFileName;
    
};


class CRhoRubyServerNativeCallbackHolder : public  rho::ruby::IRubyNativeCallback {
public:

    CRhoRubyServerNativeCallbackHolder(rho::apiGenerator::CMethodResult& mresult) {
        mMethodResult = mresult;
    }

    virtual ~CRhoRubyServerNativeCallbackHolder() {}

    virtual void onRubyNative(rho::ruby::IObject* param) {
        rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
        rho::String res_str;
        if (param != NULL) {
            rho::ruby::IString* str = rr->convertObject_to_JSON(param);
            if (str != NULL) {
                res_str = str->getUTF8();
            }
        }
        mMethodResult.set(res_str);
    }
    
    
private:
    rho::apiGenerator::CMethodResult mMethodResult;
    
};


class CRhoRubyServerLocalServerRequestCallbackHolder : public  rho::ruby::IRubyLocalServerRequestCallback {
public:
    
    CRhoRubyServerLocalServerRequestCallbackHolder(rho::apiGenerator::CMethodResult& mresult) {
        mMethodResult = mresult;
    }
    
    virtual ~CRhoRubyServerLocalServerRequestCallbackHolder() {}
    
    virtual void onLocalServerResponce(rho::ruby::IString* responce) {
        rho::String res_str;
        if (responce != NULL) {
           res_str = responce->getUTF8();
        }
        mMethodResult.set(res_str);
    }
    
    
private:
    rho::apiGenerator::CMethodResult mMethodResult;
    
};



namespace rho {

    using namespace apiGenerator;
    using namespace common;

    class CRubyServerSingletonImpl: public CRubyServerSingletonBase
    {
    public:

        CRubyServerSingletonImpl(): CRubyServerSingletonBase(){}

        //methods

        // serverURL getter for "serverURL" property
        virtual void getServerURL(rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("getServerURL","RubyServer");
            oResult.set(RHODESAPP().getRubyHomeURL());
        }

        // loadRubyFile Load ruby file into Ruby VM
        virtual void loadRubyFile( const rho::String& filepath, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("loadRubyFile","RubyServer");
            rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
            CRhoRubyServerLoadRubyHolder* rr_holder = new CRhoRubyServerLoadRubyHolder(filepath);
            rr->executeInRubyThread(rr_holder);
        }

        // loadModel Load ruby model file into Ruby VM
        virtual void loadModel( const rho::String& filepath, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("loadModel","RubyServer");
            rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
            CRhoRubyServerLoadModelHolder* rr_holder = new CRhoRubyServerLoadModelHolder(filepath);
            rr->executeInRubyThread(rr_holder);
        }

        // executeRubyMethodWithJSON Async call ruby code in Ruby thread and return result in callback
        virtual void executeRubyMethodWithJSON( const rho::String& classFullName,  const rho::String& methodName,  const rho::String& parameters_in_JSON, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("executeRubyMethodWithJSON","RubyServer");
            rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
            CRhoRubyServerRunMethodHolder* rr_holder = new CRhoRubyServerRunMethodHolder(classFullName, methodName, parameters_in_JSON, oResult);
            //rr_holder->run();
            rr->executeInRubyThread(rr_holder);
        }

        // executeGetRequestToRubyServer Async make GET request to Ruby local server and return responce in callback
        virtual void executeGetRequestToRubyServer( const rho::String& url, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("executeGetRequestToRubyServer","RubyServer");
            rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
            CRhoRubyServerLocalServerRequestCallbackHolder* rr_holder = new CRhoRubyServerLocalServerRequestCallbackHolder(oResult);
            rr->executeGetRequestToRubyServer(url.c_str(), rr_holder);
        }

        // addRubyNativeCallback add Ruby callback for call your code from Ruby. In Ruby : Rho::Ruby.callNativeCallback(callbackID, param)
        virtual void addRubyNativeCallback( const rho::String& callbackID, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("addRubyNativeCallback","RubyServer");
            rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
            CRhoRubyServerNativeCallbackHolder* rr_holder = new CRhoRubyServerNativeCallbackHolder(oResult);
            rr->addRubyNativeCallback(callbackID.c_str(), rr_holder);
        }

        // removeRubyNativeCallback remove previously added Ruby callback for call your code from Ruby. In Ruby : Rho::Ruby.callNativeCallback(callbackID, param)
        virtual void removeRubyNativeCallback( const rho::String& callbackID, rho::apiGenerator::CMethodResult& oResult) {
            // RAWLOGC_INFO("removeRubyNativeCallback","RubyServer");
            rho::ruby::IRhoRuby* rr = rho::ruby::RhoRubySingletone::getRhoRuby();
            rr->removeRubyNativeCallback(callbackID.c_str());
        }

    };

    class CRubyServerImpl : public CRubyServerBase
    {
    public:
        virtual ~CRubyServerImpl() {}

        //methods

    };

    ////////////////////////////////////////////////////////////////////////

    class CRubyServerFactory: public CRubyServerFactoryBase    {
    public:
        CRubyServerFactory(){}

        IRubyServerSingleton* createModuleSingleton()
        {
            return new CRubyServerSingletonImpl();
        }
        
        virtual IRubyServer* createModuleByID(const rho::String& strID){ return new CRubyServerImpl(); };

    };

}

extern "C" void Init_RubyServer_extension()
{
    rho::CRubyServerFactory::setInstance( new rho::CRubyServerFactory() );
    rho::Init_RubyServer_API();

}
