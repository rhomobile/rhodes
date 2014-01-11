#include "generated/cpp/NewORMBase.h"
#include "common/RhoAppAdapter.h"
#include "db/DBAdapter.h"
#include "common/RhoTime.h"
#include "logging/RhoLog.h"
#include "common/RhoConf.h"
#include "sync/RhoconnectClientManager.h"
#include "NewORMModelImpl.cpp"

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
public:
    CNewORMSingleton()
    {
        struct tm bench_time;
        bench_time.tm_sec = 0;
        bench_time.tm_min = 0;
        bench_time.tm_hour = 0;
        bench_time.tm_mday = 1;
        bench_time.tm_mon = 0;
        bench_time.tm_year = 2009 - 1900;
        time_t bench_time_ms = mktime(&bench_time)*1000;
        unsigned long now_time_ms = CTimeInterval::getCurrentTime().toULong();
        LOG(INFO) + "Times in ms are: " + bench_time_ms + ", " + now_time_ms;
        CNewORMSingleton::base_temp_obj_id_ = now_time_ms - bench_time_ms;
    }

    ~CNewORMSingleton()
    {
    }

    virtual void generateId(rho::apiGenerator::CMethodResult& oResult) 
    {
        CNewORMSingleton::base_temp_obj_id_ += 1;
        LOG(INFO) + "Generated id is : " + CNewORMSingleton::base_temp_obj_id_;
        oResult.set(CNewORMSingleton::base_temp_obj_id_);
    }
    
    virtual void useNewOrm(rho::apiGenerator::CMethodResult& oResult) 
    {
        LOG(INFO) + "MZV_DEBUG: calling useNewOrm";
        oResult.set( RHOCONF().getInt("use_new_orm") ? true : false );
    }

    virtual void getClientId(rho::apiGenerator::CMethodResult& oResult) 
    {
        LOG(INFO) + "calling getClientId";
        db::CDBAdapter& db = db::CDBAdapter::getUserDB();
        String clientID = "";
        IDBResult res = db.executeSQL("SELECT client_id,reset from client_info limit 1");
        if ( !res.isEnd() )
            oResult.set(res.getStringByIdx(0));
    }

    virtual void haveLocalChanges(rho::apiGenerator::CMethodResult& oResult) 
    {
        LOG(INFO) + "calling haveLocalChanges";
        db::CDBAdapter& db = db::CDBAdapter::getUserDB();
        IDBResult res = db.executeSQL("SELECT object FROM changed_values WHERE sent<=1 LIMIT 1 OFFSET 0");
        if ( !res.isEnd() )
            oResult.set(true);
        else
            oResult.set(res.getStringByIdx(0).length() > 0 ? true : false);
    }

    virtual void databaseLocalReset(rho::apiGenerator::CMethodResult& oResult) 
    {

        LOG(INFO) + " Calling databaseLocalReset";
        Vector<String> exclude_tables;
        exclude_tables.push_back("sources");
        exclude_tables.push_back("client_info");

        // clean all tables
        db::CDBAdapter& db = db::CDBAdapter::getDB("local");
        db.destroy_tables(Vector<String>(), exclude_tables);

        // restore schemas
        for(HashtablePtr<String, CNewORMModelImpl*>::iterator cIt = CNewORMModelImpl::models().begin(); 
            cIt != CNewORMModelImpl::models().end(); 
            ++cIt)
        {
            CNewORMModelImpl* model = cIt -> second;
            model -> getProperty("partition", oResult);
            rho::String partition = oResult.getString();
            if(partition != "local")
                continue;

            model -> initDbSource(oResult);
            if(oResult.isError())
                return;
            model -> initDbSchema(oResult);
            if(oResult.isError())
                return;
        }
    }

    virtual void databaseClientReset(bool resetLocalSources, rho::apiGenerator::CMethodResult& oResult) 
    {
        LOG(INFO) + " Calling databaseClientReset";
        Vector<String> exclude_tables;
        exclude_tables.push_back("sources");
        exclude_tables.push_back("client_info");

        int old_poll_interval = -1;
        if(rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        {
            old_poll_interval = rho::sync::RhoconnectClientManager::set_pollinterval(0);
            rho::sync::RhoconnectClientManager::stop();
            if(rho::sync::RhoconnectClientManager::has_bulksyncstate())
                rho::sync::RhoconnectClientManager::set_bulksyncstate(0);
        }

        // clean client info
        db::CDBAdapter& userdb = db::CDBAdapter::getUserDB();
        IDBResult res = userdb.executeSQL("UPDATE client_info SET client_id=?, token=?, token_sent=?", "", "", 0);
        if(!res.getDBError().isOK()) {
            oResult.setError(res.getDBError().getError());
            return;
        }

        // clean the db's
        for(HashtablePtr<String,db::CDBAdapter*>::iterator cDbIt = db::CDBAdapter::getDBPartitions().begin();
            cDbIt != db::CDBAdapter::getDBPartitions().end();
            ++cDbIt)
        {
            db::CDBAdapter* db = cDbIt -> second;
            const rho::String& partition = cDbIt -> first;
            if(!resetLocalSources && partition == "local")
                continue;

            res = db -> executeSQL("UPDATE sources SET token=0");
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                return;
            }
            db -> destroy_tables(Vector<String>(), exclude_tables);
        }

        // restore schemas
        for(HashtablePtr<String, CNewORMModelImpl*>::iterator cIt = CNewORMModelImpl::models().begin(); 
            cIt != CNewORMModelImpl::models().end(); 
            ++cIt)
        {
            CNewORMModelImpl* model = cIt -> second;
            model -> getProperty("partition", oResult);
            rho::String partition = oResult.getString();
            if(!resetLocalSources && partition == "local")
                continue;

            model -> initDbSource(oResult);
            if(oResult.isError())
                return;
            model -> initDbSchema(oResult);
            if(oResult.isError())
                return;
        }

        if(rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        {
            rho::sync::RhoconnectClientManager::set_pollinterval(old_poll_interval);
        }
    }

private:
    static unsigned long base_temp_obj_id_;
};
unsigned long CNewORMSingleton::base_temp_obj_id_;


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