//
//  ConfigImpl.cpp
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "generated/cpp/ConfigBase.h"
#include "logging/RhoLog.h"

#include <algorithm>
#include <functional>

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;
    
	Hashtable<String, Vector<String>>::value_type convertHashPtrToHash(HashtablePtr<String, Vector<String>*>::value_type old)
	{
		return std::make_pair(old.first, *old.second);
	}

    class CConfigSingletonImpl: public CConfigSingletonBase
    {
    public:

		CConfigSingletonImpl() : CConfigSingletonBase(){}

		virtual void getConfigPath(rho::apiGenerator::CMethodResult& oResult) 
		{
			oResult.set(RHOCONF().getAppConfFilePath());
		}

		virtual void setConfigPath(const rho::String& configPath, rho::apiGenerator::CMethodResult& oResult) 
		{
			RHOCONF().setAppConfFilePath(configPath.c_str());
		}

		virtual void getPropertyString(const rho::String& name, rho::apiGenerator::CMethodResult& oResult) 
		{
			oResult.set(RHOCONF().getString(name.c_str()));
		}

		virtual void setPropertyString(const rho::String& name, const rho::String& value, bool saveToFile, rho::apiGenerator::CMethodResult& oResult) 
		{
			RHOCONF().setString(name.c_str(), value, saveToFile);
		}

		virtual void getPropertyInt(const rho::String& name, rho::apiGenerator::CMethodResult& oResult) 
		{
			oResult.set(RHOCONF().getInt(name.c_str()));
		}

		virtual void setPropertyInt(const rho::String& name, int value, bool saveToFile, rho::apiGenerator::CMethodResult& oResult) 
		{
			RHOCONF().setInt(name.c_str(), value, saveToFile);
		}

		virtual void getPropertyBool(const rho::String& name, rho::apiGenerator::CMethodResult& oResult) 
		{
			oResult.set(RHOCONF().getBool(name.c_str()));
		}

		virtual void setPropertyBool(const rho::String& name, bool value, bool saveToFile, rho::apiGenerator::CMethodResult& oResult)
		{
			RHOCONF().setBool(name.c_str(), value, saveToFile);
		}

		virtual void isPropertyExists(const rho::String& name, rho::apiGenerator::CMethodResult& oResult)
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
/*
        virtual void getConflicts(rho::apiGenerator::CMethodResult& oResult) 
		{
			rho::HashtablePtr<String, Vector<String>* > conflictsHashPtr = RHOCONF().getConflicts();
			rho::Hashtable<String, rho::Vector<String>> conflictsHash;

			std::transform(conflictsHashPtr.begin(), conflictsHashPtr.end(),
				std::inserter(conflictsHash, conflictsHash.begin()), &convertHashPtrToHash);

			oResult.set(conflictsHash);
        } 
*/
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