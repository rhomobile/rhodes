#include "generated/cpp/NewORMBase.h"
#include "common/RhoAppAdapter.h"
#include "db/DBAdapter.h"
#include "logging/RhoLog.h"
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

    virtual void databaseClientReset(bool resetLocalSources, rho::apiGenerator::CMethodResult& oResult) {

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