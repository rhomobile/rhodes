#include "generated/cpp/NewORMBase.h"
#include "common/RhoAppAdapter.h"
#include "db/DBAdapter.h"
#include "common/RhoTime.h"
#include "logging/RhoLog.h"
#include "common/RhoConf.h"
#include "sync/RhoconnectClientManager.h"
#include "NewORMModelImpl.h"

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
        // clean local data
        for(HashtablePtr<String, CNewORMModelImpl*>::iterator cIt = CNewORMModelImpl::models().begin(); 
            cIt != CNewORMModelImpl::models().end(); 
            ++cIt)
        {
            CNewORMModelImpl* model = cIt -> second;
            model -> getProperty("partition", oResult);
            rho::String partition = oResult.getString();
            model -> getProperty("source_id", oResult);
            rho::String source_id = oResult.getString();
            if(partition != "local")
                continue;

            db::CDBAdapter& db = model -> _get_db(oResult);
            db.startTransaction();

            if(model -> fixed_schema()) {
                rho::String sqlStr = rho::String("DELETE FROM ") + model -> name();
                IDBResult res = db.executeSQL(sqlStr.c_str());
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }      
            }
            else {
                rho::String sqlStr = rho::String("DELETE FROM object_values WHERE source_id=?");
                IDBResult res = db.executeSQL(sqlStr.c_str(), source_id);
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }       
            } 
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
        }

        // clean the data
        for(HashtablePtr<String, CNewORMModelImpl*>::iterator cIt = CNewORMModelImpl::models().begin(); 
            cIt != CNewORMModelImpl::models().end(); 
            ++cIt)
        {
            CNewORMModelImpl* model = cIt -> second;
            model -> getProperty("partition", oResult);
            rho::String partition = oResult.getString();
            model -> getProperty("source_id", oResult);
            rho::String source_id = oResult.getString();
            if(!resetLocalSources && partition == "local")
                continue;

            db::CDBAdapter& db = model -> _get_db(oResult);
            db.startTransaction();

            if(model -> fixed_schema()) {
                rho::String sqlStr = rho::String("DELETE FROM ") + model -> name();
                res = db.executeSQL(sqlStr.c_str());
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }      
            }
            else {
                rho::String sqlStr = rho::String("DELETE FROM object_values WHERE source_id=?");
                res = db.executeSQL(sqlStr.c_str(), source_id);
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }       
            } 
        }

        if(rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        {
            rho::sync::RhoconnectClientManager::set_pollinterval(old_poll_interval);
        }
    }

    void databaseFullResetEx(const Vector<rho::String>& modelNames,
                             const bool resetClientInfo,
                             const bool resetLocalModels,
                             rho::apiGenerator::CMethodResult& oResult)
    {
        LOG(INFO) + "MZV_DEBUG, databaseFullResetEx params are : ";
        for(size_t i = 0; i < modelNames.size(); ++i) {
            LOG(INFO) + "MZV_DEBUG, model is : " + modelNames[i];
        }
        
        if(modelNames.empty()) {
            databaseFullReset(resetClientInfo, resetLocalModels, oResult);
            return;
        }
        
        if(modelNames.size() && resetClientInfo) {
            oResult.setError("reset_client_info should not be true if reset selected models");
            return;
        }

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
        IDBResult res = userdb.executeSQL("UPDATE client_info SET reset=1");
        if(!res.getDBError().isOK()) {
            oResult.setError(res.getDBError().getError());
            return;
        }
        
        RHOCONF().setString("reset_models", "", true);
        rho::String resetModelStr;

        for(size_t i = 0; i < modelNames.size(); ++i) {
            HashtablePtr<String, CNewORMModelImpl*>::iterator cModIt = CNewORMModelImpl::models().find(modelNames[i]);
            if(cModIt == CNewORMModelImpl::models().end()) {
                rho::String errStr = rho::String("model: ") + modelNames[i] + " doesn't exist.";
                oResult.setError(errStr);
                return;
            }
            
            CNewORMModelImpl* model = cModIt -> second;
            model -> getProperty("partition", oResult);
            rho::String partition = oResult.getString();
            model -> getProperty("source_id", oResult);
            rho::String source_id = oResult.getString();
            if(partition == "local" && !resetLocalModels)
                continue;

            if(partition != "local") {
                if(resetModelStr.size())
                    resetModelStr += ",";
                resetModelStr += modelNames[i];
            }

            db::CDBAdapter& db = model -> _get_db(oResult);
            db.startTransaction();

            res = db.executeSQL("UPDATE sources SET token=0 WHERE name=?", modelNames[i]);
            if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
            } 

            if(model -> fixed_schema()) {
                rho::String sqlStr = rho::String("DELETE FROM ") + modelNames[i];
                res = db.executeSQL(sqlStr.c_str());
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }      
            }
            else {
                rho::String sqlStr = rho::String("DELETE FROM object_values WHERE source_id=?");
                res = db.executeSQL(sqlStr.c_str(), source_id);
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }       
            }

            RHOCONF().setString("reset_models", resetModelStr, true);
            db.endTransaction();

        }

        if(rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        {
            rho::sync::RhoconnectClientManager::set_pollinterval(old_poll_interval);
        }
    }

    void databaseFullReset(const bool resetClientInfo, const bool resetLocalModels, rho::apiGenerator::CMethodResult& oResult)
    {
        LOG(INFO) + ", databaseFullReset : MZV_DEBUG: resetClientInfo : " + resetClientInfo + ", resetLocalModels : " + resetLocalModels;
    
        int old_poll_interval = -1;
        if(rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        {
            old_poll_interval = rho::sync::RhoconnectClientManager::set_pollinterval(0);
            rho::sync::RhoconnectClientManager::stop();
            if(rho::sync::RhoconnectClientManager::has_bulksyncstate())
                rho::sync::RhoconnectClientManager::set_bulksyncstate(0);
        }
        if(resetClientInfo && RHOCONF().isExist("push_pin")) {
            RHOCONF().setString("push_pin", "", false);
        } 
        
        // clean client info
        db::CDBAdapter& userdb = db::CDBAdapter::getUserDB();
        IDBResult res = userdb.executeSQL("UPDATE client_info SET reset=1");
        if(!res.getDBError().isOK()) {
            oResult.setError(res.getDBError().getError());
            return;
        }

        // clean the db's
        if(resetClientInfo) {
            res = userdb.executeSQL("DELETE FROM client_info");
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                return;
            }    
        }

        for(HashtablePtr<String,db::CDBAdapter*>::iterator cDbIt = db::CDBAdapter::getDBPartitions().begin();
            cDbIt != db::CDBAdapter::getDBPartitions().end();
            ++cDbIt)
        {
            db::CDBAdapter* db = cDbIt -> second;
            const rho::String& partition = cDbIt -> first;
            if(!resetLocalModels && partition == "local")
                continue;

            res = db -> executeSQL("UPDATE sources SET token=0");
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                return;
            }
        }

        // restore schemas
        for(HashtablePtr<String, CNewORMModelImpl*>::iterator cIt = CNewORMModelImpl::models().begin(); 
            cIt != CNewORMModelImpl::models().end(); 
            ++cIt)
        {
            CNewORMModelImpl* model = cIt -> second;
            model -> getProperty("partition", oResult);
            rho::String partition = oResult.getString();
            model -> getProperty("source_id", oResult);
            rho::String source_id = oResult.getString();
            if(!resetLocalModels && partition == "local")
                continue;

            db::CDBAdapter& db = model -> _get_db(oResult);
            if(model -> fixed_schema()) {
                rho::String sqlStr = rho::String("DELETE FROM ") + model -> name();
                res = db.executeSQL(sqlStr.c_str());
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }      
            }
            else {
                rho::String sqlStr = rho::String("DELETE FROM object_values WHERE source_id=?");
                res = db.executeSQL(sqlStr.c_str(), source_id);
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }       
            }
        }

        if(rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        {
            rho::sync::RhoconnectClientManager::set_pollinterval(old_poll_interval);
        }
    }

    void databaseFullResetAndLogout(rho::apiGenerator::CMethodResult& oResult)
    {
        databaseFullReset(false, true, oResult);
        if(rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        {
            rho::sync::RhoconnectClientManager::logout();
        }
    }
    
    void databaseFullclientResetAndLogout(rho::apiGenerator::CMethodResult& oResult)
    {
        databaseFullReset(true, true, oResult);
        if(rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        {
            rho::sync::RhoconnectClientManager::logout();
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