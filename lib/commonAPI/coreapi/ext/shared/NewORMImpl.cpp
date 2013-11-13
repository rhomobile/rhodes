#include "generated/cpp/NewORMBase.h"
#include "common/RhoAppAdapter.h"
#include "db/DBAdapter.h"
#include "logging/RhoLog.h"

namespace rho {

using namespace apiGenerator;

class CNewORMImpl: public CNewORMBase
{
public:
    CNewORMImpl(const rho::String& strID): CNewORMBase()
    {
    }

};

class CNewORMSingleton: public CNewORMSingletonBase
{
    ~CNewORMSingleton(){}

    virtual void getClientId(rho::apiGenerator::CMethodResult& oResult) {
        LOG(INFO) + "calling getClientId";
        db::CDBAdapter& db = db::CDBAdapter::getUserDB();
        String clientID = "";
        IDBResult res = db.executeSQL("SELECT client_id,reset from client_info limit 1");
        if ( !res.isEnd() )
            oResult.set(res.getStringByIdx(0));
    }

    virtual void haveLocalChanges(rho::apiGenerator::CMethodResult& oResult) {
        LOG(INFO) + "calling haveLocalChanges";
        db::CDBAdapter& db = db::CDBAdapter::getUserDB();
        IDBResult res = db.executeSQL("SELECT object FROM changed_values WHERE sent<=1 LIMIT 1 OFFSET 0");
        if ( !res.isEnd() )
            oResult.set(true);
        else
            oResult.set(res.getStringByIdx(0).length() > 0 ? true : false);
    }

    virtual void databaseLocalReset(rho::apiGenerator::CMethodResult& oResult) {

        LOG(INFO) + " Calling databaseLocalReset";
        // retrieve all available sources
        //HashtablePtr<String, INewormSource*>& sources = ((CNewormSourceFactory*)CNewormSourceFactoryBase::getInstance()) -> get_all_sources();
        //for(HashtablePtr<String, INewormSource*>::iterator it = sources.begin(); it != sources.end(); ++it) {
        //    const String& name = (*it).first;
        //    LOG(INFO) + " and we have here in reset : source " + name;
        //}
        /*
        db::CDBAdapter& db = db::CDBAdapter::getUserDB();
        IDBResult res = db.executeSQL("SELECT object FROM changed_values WHERE sent<=1 LIMIT 1 OFFSET 0");
        if ( !res.isEnd() )
            oResult.set(true);
        else
            oResult.set(res.getStringByIdx(0).length() > 0 ? true : false);

        //puts "database_local_reset"
        
        // load all partitions
        RhoAppAdapter.loadAllSyncSources();
        
        Vector<String> exclude_tables;
        exclude_tables.push_back("sources");
        exclude_tables.push_back("client_info");
        
        HashtablePtr<String,db::CDBAdapter*>& partitions = db::CDBAdapter::getDBPartitions();
        for ( HashtablePtr<String,db::CDBAdapter*>::iterator it = partitions.begin();  it != partitions.end(); ++it ) {
            if((*it).first == "local")
                continue;
            db::CDBAdapter* db = (*it).second;
            db -> destroy_tables(Vector<String>(), exclude_tables);
        }
      
        //hash_migrate = {}
        //::Rho::RHO.init_schema_sources(hash_migrate) 
        */
    }
};

class CNewORMFactory: public CNewORMFactoryBase
{
    ~CNewORMFactory(){}
    virtual INewORMSingleton* createModuleSingleton();
    virtual INewORM* createModuleByID(const rho::String& strID);
};

extern "C" void Init_NewORMModel_extension();
extern "C" void Init_NewORM_extension()
{
    Init_NewORMModel_extension();
    CNewORMFactory::setInstance( new CNewORMFactory() );
    Init_NewORM_API();
}

INewORM* CNewORMFactory::createModuleByID(const rho::String& strID)
{
    return new CNewORMImpl(strID);
}

INewORMSingleton* CNewORMFactory::createModuleSingleton()
{
    return new CNewORMSingleton();
}

}