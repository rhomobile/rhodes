//
//  ConfigImpl.cpp
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "generated/cpp/ConfigBase.h"
#include "logging/RhoLog.h"

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;
    
    class CConfigSingletonImpl: public CConfigSingletonBase
    {
    public:
        
        CConfigSingletonImpl(): CConfigSingletonBase(){}
        
		virtual void getConfFilePath(rho::apiGenerator::CMethodResult& oResult)
        {
			oResult.set(RHOCONF().getConfFilePath().c_str());
		}

		virtual void setConfFilePath(const rho::String& confFilePath, rho::apiGenerator::CMethodResult& oResult) 
		{
			RHOCONF().setConfFilePath(confFilePath.c_str());
		}
		
		virtual void getAppConfFilePath(rho::apiGenerator::CMethodResult& oResult) 
		{
			oResult.set(RHOCONF().getAppConfFilePath());
		}

		virtual void setAppConfFilePath(const rho::String& appConfFilePath, rho::apiGenerator::CMethodResult& oResult)
		{
			RHOCONF().setAppConfFilePath(appConfFilePath.c_str());
		}

		virtual void getAppConfUserFilePath(rho::apiGenerator::CMethodResult& oResult) 
		{
			oResult.set(RHOCONF().getAppConfUserFilePath());
		}
		
		virtual void setAppConfUserFilePath(const rho::String& appConfUserFilePath, rho::apiGenerator::CMethodResult& oResult) 
		{
			RHOCONF().setAppConfUserFilePath(appConfUserFilePath.c_str());
		}
	
	    virtual void getString( const rho::String& name, rho::apiGenerator::CMethodResult& oResult)
		{
			oResult.set(RHOCONF().getString(name.c_str()));
        } 

		virtual void getPath( const rho::String& name, rho::apiGenerator::CMethodResult& oResult)
		{
			oResult.set(RHOCONF().getPath(name.c_str()));
        } 

		virtual void getInt( const rho::String& name, rho::apiGenerator::CMethodResult& oResult) 
		{
			oResult.set(RHOCONF().getInt(name.c_str()));
        } 

		virtual void getBool( const rho::String& name, rho::apiGenerator::CMethodResult& oResult)
		{
			oResult.set(RHOCONF().getBool(name.c_str()));
        } 

		virtual void isExist( const rho::String& name, rho::apiGenerator::CMethodResult& oResult)
		{
			oResult.set(RHOCONF().isExist(name.c_str()));
        } 

		virtual void removeProperty( const rho::String& name,  bool saveToFile, rho::apiGenerator::CMethodResult& oResult) 
		{
			RHOCONF().removeProperty(name.c_str(), saveToFile);
        } 

        virtual void loadFromFile(rho::apiGenerator::CMethodResult& oResult)
		{
			RHOCONF().loadFromFile();
        } 

        virtual void getConflicts(rho::apiGenerator::CMethodResult& oResult) 
		{
        } 

        virtual void conflictsResolved(rho::apiGenerator::CMethodResult& oResult) 
		{
			RHOCONF().conflictsResolved();
        } 
    };
    
    class CConfigImpl : public CConfigBase
    {
    public:
        virtual ~CConfigImpl() {}
    };
    
    ////////////////////////////////////////////////////////////////////////
    
    class CConfigFactory: public CConfigFactoryBase   
	{
    public:
        CConfigFactory(){}
        
        IConfigSingleton* createModuleSingleton()
        { 
            return new CConfigSingletonImpl();
        }
        
        virtual IConfig* createModuleByID(const rho::String& strID){ return new CConfigImpl(); };
    };
    
}

extern "C" void Init_Config_extension()
{
    rho::CConfigFactory::setInstance( new rho::CConfigFactory() );
    rho::Init_Config_API();
}