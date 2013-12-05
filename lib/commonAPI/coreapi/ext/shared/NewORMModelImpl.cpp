//
//  NewORMModelImpl.cpp
#include "generated/cpp/NewORMModelBase.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "logging/RhoLog.h"
#include "db/DBAdapter.h"
#include "sync/RhoconnectClientManager.h"

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;
    
    class CNewORMModelImpl : public CNewORMModelBase
    {
    public:
        struct SchemaIndexDef
        {
            rho::String name_;
            Vector<rho::String> columns_;
            bool unique_;

            SchemaIndexDef()
                : name_(),
                  columns_(),
                  unique_(false)
                {}

            SchemaIndexDef(const rho::String& name,
                     const rho::Vector<rho::String>& cols,
                     const bool bUnique)
                : name_(name),
                  columns_(cols),
                  unique_(bUnique)
                {}
        };

        struct ModelPropertyDef
        {
            rho::String name_;
            rho::String type_;
            rho::String option_;

            ModelPropertyDef()
                : name_(),
                  type_(),
                  option_()
                {}

            ModelPropertyDef(const rho::String& name,
                     const rho::String& propType,
                     const rho::String& option)
                : name_(name),
                  type_(propType),
                  option_(option)
                {}
        };

        CNewORMModelImpl(const rho::String& strID)
        : id_(strID),
            name_(),
            modelProperties_(),
            schemaIndices_(),
            belongsTo_()
        {
        }

        bool fixed_schema()
        {
            rho::apiGenerator::CMethodResult oResult;
            getProperty("fixed_schema", oResult);
            bool retVal = false;
            convertFromStringA(oResult.getString().c_str(), retVal);
            return retVal;
        }

        virtual ~CNewORMModelImpl() {}

        const rho::String& name() const { return name_; }

        const rho::String& id() const { return id_; }

        //methods
        void init(const rho::String& modelName, rho::apiGenerator::CMethodResult& oResult)
        {
            LOG(INFO) + "Initializing new Model: " + modelName;
            setModel_name(modelName, oResult);
            name_ = modelName;
            // init_defaults
            init_defaults();
            models_.put(modelName, this);
        }

        //methods
        void destroy(rho::apiGenerator::CMethodResult& oResult)
        {
            LOG(INFO) + "Destroying Model: " + name_;
            models_.remove(name_);
            name_ = "";
            id_ = "";
            modelProperties_.clear();
            schemaIndices_.clear();
            belongsTo_.clear();
            clearAllProperties(oResult);
        }

        static HashtablePtr<rho::String, CNewORMModelImpl*>& models() { return models_; }

        void setBelongsTo(const rho::String& propName, const rho::String& sourceName, rho::apiGenerator::CMethodResult&)
        {
            belongsTo_[propName] = sourceName;
        }

        void setModelProperty(const rho::String& propName, 
                              const rho::String& propType, 
                              const rho::String& option,
                              rho::apiGenerator::CMethodResult&)
        {
            modelProperties_[propName] = ModelPropertyDef(propName, propType, option);
        }

        void setSchemaIndex(const rho::String& indexName, const rho::Vector<rho::String>& indexColumns, bool bUniqueIndex, rho::apiGenerator::CMethodResult& oResult)
        {
            LOG(INFO) + "setSchemaIndex: " + indexName + ", is_unique: " + bUniqueIndex;
            for(int i = 0; i < indexColumns.size(); ++i) 
                LOG(INFO) + "indexCol: " + indexColumns[i]; 
            schemaIndices_[indexName] = SchemaIndexDef(indexName, indexColumns, bUniqueIndex);
        }

        static int get_start_id(const rho::String& partition)
        {
            int start_id = _get_partition_start_id(partition);
            db::CDBAdapter& db = db::CDBAdapter::getDB(partition.c_str());
            IDBResult res = db.executeSQL("SELECT source_id,name from sources");
    
            for(; !res.isEnd(); res.next()) {
                int src_id = res.getIntByIdx(0);
                if(src_id > start_id)
                    start_id = src_id;
            }
      
            start_id += 1;
          
            LOG(INFO) + "_get_start_id:" + start_id;
            return start_id;
        }

        void initModel(rho::apiGenerator::CMethodResult& oResult)
        {
            initAssociations(oResult);
            if(oResult.isError())
                return;
            initBlobAttributes(oResult);
            if(oResult.isError())
                return;
            initDbSource(oResult);
            if(oResult.isError())
                return;
            initDbSchema(oResult);
            if(oResult.isError())
                return;
            initSyncSourceProperties(oResult);
            if(oResult.isError())
                return;
        }

        void initAssociations(rho::apiGenerator::CMethodResult& oResult)
        {
            LOG(INFO) +  "initAssociations: " + name();

            for(Hashtable<rho::String, rho::String>::const_iterator cIt = belongsTo_.begin(); 
                cIt != belongsTo_.end(); ++cIt) 
            {
                const rho::String& property_name = cIt -> first;
                const rho::String& source_name = cIt -> second;

                HashtablePtr<rho::String, CNewORMModelImpl*>::iterator modelIt = CNewORMModelImpl::models().find(source_name);
                if(modelIt == CNewORMModelImpl::models().end())
                {
                    LOG(ERROR) + "Invalid belongs_to : source name '" + source_name + "' does not exist.";
                    continue;
                }

                CNewORMModelImpl* associate_with_model = modelIt -> second;
                associate_with_model -> getProperty("associations", oResult);
                rho::String existing_associations = oResult.getString();
                if(existing_associations.size() > 0)
                    existing_associations += ",";
                existing_associations += name() + "," + property_name;
                associate_with_model -> setProperty("associations", existing_associations, oResult);
            }
        }

        void initBlobAttributes(rho::apiGenerator::CMethodResult& oResult)
        {
            LOG(INFO) +  "initBlobAttributes: " + name();

            setProperty("blob_attribs", "", oResult);
            rho::String blob_attribs;
            for(Hashtable<rho::String, ModelPropertyDef>::const_iterator cIt = modelProperties_.begin();
                cIt != modelProperties_.end();
                ++cIt)
            {
                const ModelPropertyDef& prop_def = cIt -> second;
                if(prop_def.type_ == "blob") 
                {
                    if(blob_attribs.size() > 0)
                        blob_attribs += ",";
                    blob_attribs += prop_def.name_;
                    blob_attribs += (prop_def.option_ == "overwrite" ? "1" : "0");

                }
            }
            setProperty("blob_attribs", blob_attribs, oResult);
        }

        void initSyncSourceProperties(rho::apiGenerator::CMethodResult& oResult)
        {
            LOG(INFO) +  "initSyncSourceProperties: " + name();
            if(!rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
                return;

            getProperty("source_id", oResult);
            int source_id = -1;
            convertFromStringA(oResult.getString().c_str(), source_id);
            if(source_id == -1) {
                oResult.setError("Invalid SourceID -1");
                return;
            }
            Vector<String> sync_options;
            sync_options.push_back("pass_through");
            sync_options.push_back("full_update");
            for(int i = 0; i < sync_options.size(); ++i)
            {
                getProperty(sync_options[i], oResult);
                bool optValue = false;
                convertFromStringA(oResult.getString().c_str(), optValue);
                if(optValue)
                   rho::sync::RhoconnectClientManager::set_source_property(source_id, sync_options[i].c_str(), oResult.getString().c_str());   
            }

            getProperty("freezed", oResult);
            bool optValue = false;
            convertFromStringA(oResult.getString().c_str(), optValue);
            if(!optValue)
                return;

            rho::String modelProps;
            for(Hashtable<rho::String, ModelPropertyDef>::const_iterator cIt = modelProperties_.begin();
                cIt != modelProperties_.end();
                ++cIt)
            {
                const ModelPropertyDef& prop_def = cIt -> second;
                if(modelProps.size())
                    modelProps += ",";
                modelProps += prop_def.name_;
            }
            LOG(INFO) +  "initSyncSourceProperties1: " + name();
            rho::sync::RhoconnectClientManager::set_source_property(source_id, "freezed", modelProps.c_str());

            LOG(INFO) +  "initSyncSourceProperties2: " + name();
        }

        void initDbSource(rho::apiGenerator::CMethodResult& oResult)
        {
            LOG(INFO) +  "initDbSource: " + name();
            getPartition(oResult);
            rho::String partition = oResult.getString();
            getSync_priority(oResult);
            rho::String sync_priority = oResult.getString();
            getSync_type(oResult);
            rho::String sync_type = oResult.getString();
            getProperty("associations", oResult);
            rho::String associations = oResult.getString();
            getProperty("blob_attribs", oResult);
            rho::String blob_attribs = oResult.getString();
            getProperty("source_id", oResult);
            rho::String source_id = oResult.getString();

            LOG(INFO) + "initDbSource: model attribs: " + "name:" + name()
                + ". source_id:" + source_id 
                + ", partition:" + partition
                + ", sync_type:" + sync_type
                + ", sync_priority:" + sync_priority
                + ", associations: " + associations
                + ", blob_attribs:" + blob_attribs; 

            db::CDBAdapter& db = db::CDBAdapter::getDB(partition.c_str());
            IDBResult res = db.executeSQL("SELECT sync_priority,source_id,partition,sync_type,associations,blob_attribs,name FROM sources WHERE name=?", name().c_str());
            
            bool hasExistingAttribs = !res.isEnd();
            if(hasExistingAttribs) {
                Hashtable<rho::String, rho::String> existing_attribs;
                Hashtable<rho::String, rho::String> attribs_to_update;
                for(int iCol = 0; iCol < res.getColCount(); ++iCol) { 
                    existing_attribs.put(res.getColName(iCol), res.getStringByIdx(iCol));
                }
                // re-use existing source_id
                source_id = existing_attribs["source_id"];
                setProperty("source_id", existing_attribs["source_id"], oResult);

                if(existing_attribs["sync_priority"] !=sync_priority)
                    attribs_to_update.put("sync_priority", sync_priority);
                if(existing_attribs["sync_type"] !=sync_type)
                    attribs_to_update.put("sync_type", sync_type);
                if(existing_attribs["associations"] != associations)
                    attribs_to_update.put("associations", associations);
                if(existing_attribs["blob_attribs"] != blob_attribs)
                    attribs_to_update.put("blob_attribs", blob_attribs);
                
                if(attribs_to_update.size()) {
                    rho::String updateSqlStatement("UPDATE sources SET ");
                    rho::String setSql;
                    for(Hashtable<rho::String, rho::String>::const_iterator cIt = attribs_to_update.begin(); cIt != attribs_to_update.end(); ++cIt) {
                        if(setSql.size())
                            setSql += ",";
                        setSql += cIt -> first + "=" + cIt -> second;
                    }
                    updateSqlStatement += setSql + " WHERE name=?";
                    LOG(INFO) + "update_attribs: " + updateSqlStatement;
                    res = db.executeSQL(updateSqlStatement.c_str(), name());
                    if(!res.getDBError().isOK()) {
                        oResult.setError(res.getDBError().getError());
                        return;
                    }
                }
            }
            else {
                if(!source_id.size()) {
                    int start_id = get_start_id(partition);
                    source_id = convertToStringA(start_id);
                    setProperty("source_id", source_id, oResult);
                }
                res = db.executeSQL("INSERT INTO sources (source_id,name,sync_priority,sync_type,partition,associations,blob_attribs) VALUES(?,?,?,?,?,?,?)", 
                    source_id, name(), sync_priority, sync_type, partition, associations, blob_attribs);
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                }
            }
        }

        void initDbSchema(rho::apiGenerator::CMethodResult& oResult)
        {
            if(!fixed_schema())
                return; 

            // Schema models are freezed by default
            setProperty("freezed", "true", oResult);
            getProperty("schema_version", oResult);
            rho::String schema_version = oResult.getString();
            getProperty("partition", oResult);
            rho::String partition = oResult.getString();
            db::CDBAdapter& db = db::CDBAdapter::getDB(partition.c_str());
            IDBResult res = db.executeSQL("SELECT  schema_version FROM sources WHERE name=?", name().c_str());
            rho::String existing_schema_version;
            if(!res.isEnd())
                existing_schema_version = res.getStringByIdx(0);
            LOG(INFO) + "initDbSchema: model attribs: " + "name: " + name()
                + ", schema_version: " + schema_version
                + ", schema_version in DB: " + existing_schema_version;
            
            bool bTableExists = db.isTableExist(name());
            bool bMigrateSchema = (bTableExists && (existing_schema_version != schema_version));
            LOG(INFO) + "initDbSchema: bTableExists: " + bTableExists + ", bMigrateSchema: " + bMigrateSchema;

            // nothing needs to be done
            if (bTableExists && !bMigrateSchema)
                return;

            rho::String strCreateSQL = _make_create_sql_script();
            db.startTransaction();
            setProperty("sql", strCreateSQL, oResult);
            if(bMigrateSchema) {
                // destroy previous version of the table
                rho::Vector<rho::String> arIncludeTables;
                arIncludeTables.push_back(name());
                db.destroy_tables(arIncludeTables, rho::Vector<rho::String>());
            }
            // create new table
            res = db.executeSQL(strCreateSQL.c_str());
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                db.rollback();
                return;
            }
            // update attribs
            res = db.executeSQL("UPDATE sources SET schema=?, schema_version=? WHERE name=?", strCreateSQL, schema_version, name());
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                db.rollback();
            }
            else
                db.endTransaction();
        }

    private:
        void init_defaults()
        {
            rho::apiGenerator::CMethodResult oRes;
            setLoaded(false, oRes);
            setFixed_schema(false, oRes);
            setSync_type("none", oRes);
            setSync_priority(1000, oRes);
            setPartition("local", oRes);
        }

        static int _get_partition_start_id(const rho::String& partition)
        {
            if(partition == "user")
                return 0;
            else if(partition == "app")
                return 20000;
            else if(partition == "local")
                return 40000;

            return 0;
        }

        static const rho::String _get_db_type(const rho::String& propType)
        {
            rho::String strType = "varchar";
            if(propType == "integer")
                strType = "integer";
            if(propType == "float")
                strType = "float";
            if(propType == "date")
                strType = "integer";
            if(propType == "time")
                strType = "integer"; 
            return strType;  
        }

        const rho::String _make_create_sql_script() const
        {
            rho::String strCols;
            for(Hashtable<rho::String, ModelPropertyDef>::const_iterator cIt = modelProperties_.begin();
                cIt != modelProperties_.end();
                ++cIt)
            {
                const ModelPropertyDef& prop_def = cIt -> second;
                rho::String strDBType = _get_db_type(prop_def.type_);
                if(strCols.size())
                    strCols += ",";
                strCols += "\"";
                strCols += prop_def.name_ + "\" " + strDBType + " default null";
            }

            if(strCols.size())
                strCols += ",";
            strCols += "\"object\" varchar(255) PRIMARY KEY";

            rho::String strCreate = "CREATE TABLE \"" + name() + "\" ( " + strCols + " )";
            
            if(strCreate.size() > 0)
                strCreate += ";\r\n";
            strCreate += _create_sql_schema_indices();

            LOG(INFO) + "_make_createsql_script: " + name() + ", sql: " + strCreate;
  
            return strCreate;
        }

        const rho::String _create_sql_schema_indices() const
        {
            rho::String strSQLIndices;
            for(Hashtable<rho::String, SchemaIndexDef>::const_iterator cIt = schemaIndices_.begin();
                cIt != schemaIndices_.end();
                ++cIt) 
            {
                rho::String strCols;
                const SchemaIndexDef& index_def = cIt -> second;
                const rho::Vector<rho::String> cols = index_def.columns_;
                for(int i = 0; i < cols.size(); ++i)
                {
                    if(strCols.size())
                        strCols += ",";
                    strCols += rho::String("\"") + cols[i] + "\""; 
                }

                rho::String strIndex("CREATE ");
                if(index_def.unique_)
                    strIndex += "UNIQUE ";
                strIndex += rho::String("INDEX ") + "\"" + index_def.name_ + "\" ON \"" + name() + "(" + strCols + ");\r\n";
                strSQLIndices += strIndex; 
            }

            LOG(INFO) + "_create_sql_schema_indices: " + name() + ", sql: " + strSQLIndices;

            return strSQLIndices;
        }
        
        static HashtablePtr<rho::String, CNewORMModelImpl*> models_;

        rho::String id_;
        rho::String name_;
        Hashtable<rho::String, ModelPropertyDef> modelProperties_;
        Hashtable<rho::String, SchemaIndexDef > schemaIndices_;
        Hashtable<rho::String, rho::String> belongsTo_;
    };
    HashtablePtr<rho::String, CNewORMModelImpl*> CNewORMModelImpl::models_;

    class CNewORMModelSingletonImpl: public CNewORMModelSingletonBase
    {
    public:
        CNewORMModelSingletonImpl(): CNewORMModelSingletonBase(){}
        
        //methods
        // enumerate  
        virtual void enumerate(rho::apiGenerator::CMethodResult& oResult) {
            Vector<rho::String> ret_models;
            HashtablePtr<rho::String, CNewORMModelImpl*>& models = CNewORMModelImpl::models();
            for(HashtablePtr<rho::String, CNewORMModelImpl*>::iterator it = models.begin(); it != models.end(); ++it) {
                ret_models.push_back((*it).second -> id());
            }

            oResult.set(ret_models);
        } 

    };
    
    ////////////////////////////////////////////////////////////////////////
    
    class CNewORMModelFactory: public CNewORMModelFactoryBase    {
    public:
        CNewORMModelFactory()
        {}
        
        INewORMModelSingleton* createModuleSingleton()
        { 
            return new CNewORMModelSingletonImpl();
        }
        
        virtual INewORMModel* createModuleByID(const rho::String& strID){ return new CNewORMModelImpl(strID); };    
    };
    
}

extern "C" void Init_NewORMModel_extension()
{
    rho::CNewORMModelFactory::setInstance( new rho::CNewORMModelFactory() );
    rho::Init_NewORMModel_API();
}