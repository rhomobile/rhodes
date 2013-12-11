//
//  NewORMModelImpl.cpp
#include "generated/cpp/NewORMModelBase.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoTime.h"
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
            freezed_(false),
            fixed_schema_(false),
            modelProperties_(),
            schemaIndices_(),
            belongsTo_()
        {
            rho::apiGenerator::CMethodAccessor< INewORMModel >* fixedSchemaAccessor = new rho::apiGenerator::CMethodAccessor<INewORMModel>(&INewORMModel::getFixed_schema);
            fixedSchemaAccessor -> addSetter(new rho::apiGenerator::CMethodAccessor<INewORMModel>::CSetter<bool, bool>(&INewORMModel::setFixed_schema));
            rho::apiGenerator::CMethodAccessor< INewORMModel >* freezedModelAccessor = new rho::apiGenerator::CMethodAccessor<INewORMModel>(&INewORMModel::getFreezed);
            freezedModelAccessor -> addSetter(new rho::apiGenerator::CMethodAccessor<INewORMModel>::CSetter<bool, bool>(&INewORMModel::setFreezed));
            
            m_mapPropAccessors["fixed_schema"] = fixedSchemaAccessor;
            m_mapPropAccessors["freezed"] = freezedModelAccessor;
        }

        static void init_once()
        {
            if(reserved_names_.empty())
            {
                reserved_names_["object"] = 1;
                reserved_names_["source_id"] = 1;
                reserved_names_["update_type"] = 1;
                reserved_names_["attrib_type"] = 1;
                reserved_names_["set_notification"] = 1;
                reserved_names_["clear_notification"] = 1;
            }
        }

        bool fixed_schema()
        {
            return fixed_schema_;
        }

        bool freezed_model()
        {
            return freezed_;
        }

        void setFreezed(const bool newVal, rho::apiGenerator::CMethodResult& oResult)
        {
            freezed_ = newVal;
            m_hashProps["freezed"] = rho::common::convertToStringA(newVal);
        }

        void setFixed_schema(const bool newVal, rho::apiGenerator::CMethodResult& oResult)
        {
            fixed_schema_ = newVal;
            m_hashProps["fixed_schema"] = rho::common::convertToStringA(newVal);
        }

        void getFixed_schema(rho::apiGenerator::CMethodResult& oResult)
        {
            oResult.set(fixed_schema_);
        }

        void getFreezed(rho::apiGenerator::CMethodResult& oResult)
        {
            oResult.set(freezed_);
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

        void validateFreezedAttributes(const rho::Hashtable<rho::String, rho::String>& attrs, rho::apiGenerator::CMethodResult& oResult)
        {
            oResult.set(true);
            if(!fixed_schema() && !freezed_model()) 
                return;

            for(Hashtable<rho::String, rho::String>::const_iterator cIt = attrs.begin();
                cIt != attrs.end();
                ++cIt)
            {
                validateFreezedAttribute(cIt -> first, oResult);
                if(oResult.isError())
                    return;
            }
        }

        void validateFreezedAttribute(const rho::String& attrName, rho::apiGenerator::CMethodResult& oResult)
        {
            oResult.set(true);
            if(!fixed_schema() && !freezed_model()) 
                return;

            if(attrName == "object")
                return;

            // non-reserved attr name must exist
            if(!modelProperties_.containsKey(attrName)) 
            {
                rho::String errStr("Non-exist property : ");
                errStr += attrName;
                errStr += ". For model:  ";
                errStr += name();
                oResult.setError(errStr);
                return;
            }  
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

            if(!freezed_model())
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
            db::CDBAdapter& db = _get_db(oResult);
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

        void getCount(rho::apiGenerator::CMethodResult& oResult)
        { 
            db::CDBAdapter& db = _get_db(oResult);
            oResult.set(0);
            if(fixed_schema()) {
                rho::String strSQL("SELECT COUNT(*) FROM ");
                strSQL += name();
                IDBResult res = db.executeSQL(strSQL.c_str());
                if(!res.isEnd())
                    oResult.set(res.getIntByIdx(0));
            }
            else
            {
                IDBResult res = db.executeSQL("SELECT COUNT(DISTINCT object) FROM object_values WHERE source_id=?", name().c_str());
                if(!res.isEnd())
                    oResult.set(res.getIntByIdx(0));
            }

            LOG(INFO) + name() + ", getCount: " +  oResult.getInt();
        }

        void getBackendRefreshTime(rho::apiGenerator::CMethodResult& oResult)
        { 
            db::CDBAdapter& db = _get_db(oResult);
            int nTime = 0;
            IDBResult res = db.executeSQL("SELECT backend_refresh_time FROM sources WHERE source_id=?", name().c_str());
            if(!res.isEnd())
                nTime = res.getIntByIdx(0);
            oResult.set(CLocalTime(nTime).formatStr("%Y-%m-%d %H:%M:%S %z"));
        }

        void find(const rho::String& args, rho::apiGenerator::CMethodResult& oResult)
        {
            LOG(INFO) + name() + ", find: Params are: " + args;
            //for(size_t i = 0; i < args.size(); ++i) 
            //    LOG(INFO) + args[i];

            rho::Vector<rho::Hashtable<rho::String, rho::String> > retVals;
            if(!fixed_schema()) {
                oResult.set(retVals);
                return;
            }

            rho::String attribs("*");
            rho::String strSQL("SELECT ");
            strSQL += attribs;
            strSQL += " FROM ";
            strSQL += name();
            db::CDBAdapter& db = _get_db(oResult);
            IDBResult res = db.executeSQL(strSQL.c_str());
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                return;
            }
            for(; !res.isEnd(); res.next()) {
                int ncols = res.getColCount();
                Hashtable<rho::String, rho::String> obj_hash;
                for(int i = 0; i < ncols; ++i) {
                    obj_hash.put(res.getColName(i), res.getStringByIdx(i));
                }
                retVals.push_back(obj_hash);
            }
            oResult.set(retVals);
        }

        void createObject(const Hashtable<rho::String, rho::String>& attrs, rho::apiGenerator::CMethodResult& oResult)
        {
            getProperty("source_id", oResult);
            rho::String source_id = oResult.getString();
            getProperty("sync_type", oResult);
            bool is_sync_source = (oResult.getString() != "none");
            db::CDBAdapter& db = _get_db(oResult);

            db.startTransaction();
            if(is_sync_source) {
                IDBResult res = db.executeSQL("INSERT INTO changed_values(update_type,attrib,source_id,object) VALUES(?,?,?,?)", "create", "object", source_id, attrs.get("object"));
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }
            }

            if(fixed_schema()) {
                rho::Vector<rho::String> quests;
                rho::String strSQL = _make_insert_attrs_sql_script(attrs.get("object"), attrs, quests);

                LOG(INFO) + "createObject: MZV_DEBUG: we have the following sqlSQL:" + strSQL;
                for(size_t i = 0; i < quests.size(); ++i)
                    LOG(INFO) + quests[i];
            
                IDBResult res = db.executeSQLEx(strSQL.c_str(), quests);
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }    
            }
            else
            {
                rho::String strSQL("INSERT INTO object_values (source_id,object,attrib,value) VALUES(?,?,?,?);\r\n");
                for(Hashtable<rho::String, rho::String>::const_iterator cIt = attrs.begin();
                    cIt != attrs.end();
                    ++cIt)
                {
                    if(_is_reserved_name(cIt -> first))
                        continue;   
                    
                    IDBResult res = db.executeSQL(strSQL.c_str(), source_id, attrs.get("object"), cIt -> first, cIt -> second);
                    if(!res.getDBError().isOK()) {
                        oResult.setError(res.getDBError().getError());
                        db.rollback();
                        return;
                    }
                } 
            }

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

        db::CDBAdapter& _get_db(rho::apiGenerator::CMethodResult& oResult)
        {
            getProperty("partition", oResult);
            rho::String partition = oResult.getString();
            return db::CDBAdapter::getDB(partition.c_str());
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

        static bool _is_reserved_name(const rho::String& attrName)
        {
            return reserved_names_.containsKey(attrName);
        }

        const rho::String _make_insert_attrs_sql_script(const rho::String& objectId, 
                                                        const Hashtable<rho::String, rho::String>& attrs, 
                                                        Vector<rho::String>& quests) const
        {
            rho::String strCols, strQuests;
            quests.push_back(objectId);
            for(Hashtable<rho::String, rho::String>::const_iterator cIt = attrs.begin();
                cIt != attrs.end();
                ++cIt)
            {
                if(_is_reserved_name(cIt -> first))
                    continue;
                if(strCols.size())
                    strCols += ",";
                strCols += "\"";
                strCols += cIt -> first + "\"";
                if(strQuests.size())
                    strQuests += ",";
                strQuests += "?";
                quests.push_back(cIt -> second);
            }

            return rho::String("INSERT INTO ") + name() + "(\"object\"," + strCols + ") VALUES (?," + strQuests + ");\r\n";   
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
        static Hashtable<rho::String, int> reserved_names_;

        rho::String id_;
        rho::String name_;
        bool fixed_schema_;
        bool freezed_;
        Hashtable<rho::String, ModelPropertyDef> modelProperties_;
        Hashtable<rho::String, SchemaIndexDef > schemaIndices_;
        Hashtable<rho::String, rho::String> belongsTo_;
    };
    HashtablePtr<rho::String, CNewORMModelImpl*> CNewORMModelImpl::models_;
    Hashtable<rho::String, int> CNewORMModelImpl::reserved_names_;

    class CNewORMModelSingletonImpl: public CNewORMModelSingletonBase
    {
    public:
        CNewORMModelSingletonImpl(): CNewORMModelSingletonBase()
        {
            CNewORMModelImpl::init_once();
        }
        
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