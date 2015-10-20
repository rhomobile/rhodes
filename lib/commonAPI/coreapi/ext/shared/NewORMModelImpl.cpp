//
//  NewORMModelImpl.cpp
#include "NewORMModelImpl.h"
#include "generated/cpp/NewORMModelBase.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoTime.h"
#include "logging/RhoLog.h"
#include "db/DBAdapter.h"
#include "sync/RhoconnectClientManager.h"


const rho::String& normalizeString(const rho::String& _in, rho::String& buffer)
{
    if (_in.size() > 1)
    {
        char begin = *(_in.begin());
        char end = *(_in.rbegin());
        if ((begin == end) && (begin == '\'' || begin == '"'))
        {
            buffer = _in.substr(1, _in.length() - 2);
            LOG(WARNING) + "ESCAPING " + _in + " TO " + buffer;
            return buffer;
        }
        else
        {
            return _in;
        }
    }

    return _in;
}

rho::CNewORMModelImpl::CNewORMModelImpl(const rho::String& strID)
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

void rho::CNewORMModelImpl::init_once()
{
    if(reserved_names_.empty())
    {
        reserved_names_[""] = 1;
        reserved_names_["object"] = 1;
        reserved_names_["source_id"] = 1;
        reserved_names_["update_type"] = 1;
        reserved_names_["attrib_type"] = 1;
        reserved_names_["set_notification"] = 1;
        reserved_names_["clear_notification"] = 1;
    }
}

void rho::CNewORMModelImpl::setFreezed(const bool newVal, rho::apiGenerator::CMethodResult& oResult)
{
    freezed_ = newVal;
    m_hashProps["freezed"] = rho::common::convertToStringA(newVal);
}

void rho::CNewORMModelImpl::setFixed_schema(const bool newVal, rho::apiGenerator::CMethodResult& oResult)
{
    fixed_schema_ = newVal;
    m_hashProps["fixed_schema"] = rho::common::convertToStringA(newVal);
}

void rho::CNewORMModelImpl::getFixed_schema(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set(fixed_schema_);
}

void rho::CNewORMModelImpl::getFreezed(rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set(freezed_);
}

//methods
void rho::CNewORMModelImpl::init(const rho::String& modelName, rho::apiGenerator::CMethodResult& oResult)
{
    LOG(INFO) + "Initializing new Model: " + modelName;
    setModel_name(modelName, oResult);
    name_ = modelName;
    // init_defaults
    init_defaults();
    models_.put(modelName, this);
}

//methods
void rho::CNewORMModelImpl::destroy(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(INFO) + "Destroying Model: " + name_;
    models_.erase(name_);
    name_ = "";
    id_ = "";
    modelProperties_.clear();
    schemaIndices_.clear();
    belongsTo_.clear();
    clearAllProperties(oResult);
}

void rho::CNewORMModelImpl::set(const rho::String& propName, const rho::String& propValue, rho::apiGenerator::CMethodResult& oResult)
{
    // 'sync' property require special processing
    LOG(INFO) + ",set :" + propName + ":" + propValue;
    if(propName == "sync")
    {
        if(propValue == "true")
        {
            getSync_type(oResult);
            if(oResult.getString() == "none")
                setSync_type("incremental", oResult);
            // sync type also controls the partition name
            getPartition(oResult);
            if(oResult.getString() == "local")
                setPartition("user", oResult);
        }
        else
        {
            setSync_type("none", oResult);
            setPartition("local", oResult);
        }

    }
    // all other properties
    else
        setProperty(propName, propValue, oResult);
}

void rho::CNewORMModelImpl::enable(const rho::String& propName, rho::apiGenerator::CMethodResult& oResult)
{
    LOG(INFO) + ",enable :" + propName;
    set(propName, "true", oResult);
}

void rho::CNewORMModelImpl::setBelongsTo(const rho::String& propName, const rho::String& sourceName, rho::apiGenerator::CMethodResult&)
{
    belongsTo_[propName].push_back(sourceName);
}

void rho::CNewORMModelImpl::getBelongsTo(const rho::String& propName, rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<rho::String, rho::Vector<rho::String> >::const_iterator cIt = belongsTo_.find(propName);
    if(cIt != belongsTo_.end()) {
        oResult.set(cIt -> second);
    }
}

void rho::CNewORMModelImpl::setModelProperty(const rho::String& propName,
                      const rho::String& propType,
                      const rho::String& option,
                      rho::apiGenerator::CMethodResult&)
{
    modelProperties_[propName] = ModelPropertyDef(propName, propType, option);
}

void rho::CNewORMModelImpl::getModelProperty(const rho::String& propName, rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<rho::String, ModelPropertyDef>::const_iterator cIt = modelProperties_.find(propName);
    if(cIt != modelProperties_.end()) {
        Hashtable<rho::String, rho::String> retVals;
        retVals["name"] = propName;
        retVals["type"] = cIt -> second.type_;
        retVals["option"] = cIt -> second.option_;
        oResult.set(retVals);
    }
}

void rho::CNewORMModelImpl::setSchemaIndex(const rho::String& indexName, const rho::Vector<rho::String>& indexColumns, bool bUniqueIndex, rho::apiGenerator::CMethodResult& oResult)
{
    LOG(INFO) + "setSchemaIndex: " + indexName + ", is_unique: " + bUniqueIndex;
    for(int i = 0; i < indexColumns.size(); ++i)
        LOG(INFO) + "indexCol: " + indexColumns[i];
    schemaIndices_[indexName] = SchemaIndexDef(indexName, indexColumns, bUniqueIndex);
}

void rho::CNewORMModelImpl::validateFreezedAttributes(const rho::Hashtable<rho::String, rho::String>& attrs, rho::apiGenerator::CMethodResult& oResult)
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

void rho::CNewORMModelImpl::validateFreezedAttribute(const rho::String& attrName, rho::apiGenerator::CMethodResult& oResult)
{
    oResult.set(true);
    if(!fixed_schema() && !freezed_model())
        return;

    if(_is_reserved_name(attrName))
        return;

    // non-reserved attr name must exist
    if(!modelProperties_.containsKey(attrName))
    {
        rho::String errStr("Non-exist property : ");
        errStr += attrName;
        errStr += ". For model:  ";
        errStr += name();
        oResult.setArgError(errStr);
        return;
    }
}

int rho::CNewORMModelImpl::get_start_id(const rho::String& partition)
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

void rho::CNewORMModelImpl::initModel(rho::apiGenerator::CMethodResult& oResult)
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
    setLoaded(true, oResult);
}

void rho::CNewORMModelImpl::initAssociations(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(INFO) +  "initAssociations: " + name();

    for(Hashtable<rho::String, rho::Vector<rho::String> >::const_iterator cIt = belongsTo_.begin();
        cIt != belongsTo_.end(); ++cIt)
    {
        const rho::String& property_name = cIt -> first;
        const rho::Vector<rho::String>& sources = cIt -> second;

        for(size_t i = 0; i < sources.size(); ++i)
        {
            const rho::String& source_name = sources[i];
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
}

void rho::CNewORMModelImpl::initBlobAttributes(rho::apiGenerator::CMethodResult& oResult)
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
            blob_attribs += prop_def.name_ + ",";
            blob_attribs += (prop_def.option_ == "overwrite" ? "1" : "0");

        }
    }
    setProperty("blob_attribs", blob_attribs, oResult);
}

void rho::CNewORMModelImpl::initSyncSourceProperties(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(INFO) +  "initSyncSourceProperties: " + name();
    if(!rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        return;

    getProperty("source_id", oResult);
    int source_id = -1;
    convertFromStringA(oResult.getString().c_str(), source_id);
    if(source_id == -1) {
        oResult.setArgError("Invalid SourceID -1");
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
    rho::sync::RhoconnectClientManager::set_source_property(source_id, "freezed", modelProps.c_str());
}

void rho::CNewORMModelImpl::initDbSource(rho::apiGenerator::CMethodResult& oResult)
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
                setSql += cIt -> first + "=\"" + cIt -> second + "\"";
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
    // reload BlobAttrs
    db.getAttrMgr().loadBlobAttrs(db);
}

void rho::CNewORMModelImpl::initDbSchema(rho::apiGenerator::CMethodResult& oResult)
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
    // nothing needs to be done
    if (bTableExists && !bMigrateSchema)
        return;

    // result MUST be set to nil in order to execute next operation (drop table)
    res = NULL;

    rho::String strCreateSQL = _make_create_sql_script();
    db.startTransaction();
    setProperty("sql", strCreateSQL, oResult);
    if(bMigrateSchema) {
        // destroy previous version of the table
        rho::String dropTableSql("DROP TABLE ");
        dropTableSql += name();
        res = db.executeSQL(dropTableSql.c_str());
        if(!res.getDBError().isOK()) {
            oResult.setError(res.getDBError().getError());
            db.rollback();
            return;
        }
    }
    // create new table
    res = db.executeSQL(strCreateSQL.c_str());
    if(!res.getDBError().isOK()) {
        oResult.setError(res.getDBError().getError());
        db.rollback();
        return;
    }
    // create all indices
    rho::Vector<rho::String> indices = _create_sql_schema_indices();
    for(size_t i = 0; i < indices.size(); ++i) {
        res = db.executeSQL(indices[i].c_str());
        if(!res.getDBError().isOK()) {
            oResult.setError(res.getDBError().getError());
            db.rollback();
            return;
        }
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

void rho::CNewORMModelImpl::getCount(rho::apiGenerator::CMethodResult& oResult)
{
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    Hashtable<rho::String, rho::String> attrsSet;
    db::CDBAdapter& db = _get_db(oResult);
    oResult.set(0);
    if(fixed_schema()) {
        rho::String strSQL("SELECT COUNT(*) FROM ");
        strSQL += name();
        IDBResult res = db.executeSQL(strSQL.c_str());
        _processDbResult(res, "count", attrsSet, true, oResult);
    }
    else
    {
        rho::String strSQL("SELECT COUNT(DISTINCT object) FROM object_values WHERE source_id=");
        strSQL += source_id;
        IDBResult res = db.executeSQL(strSQL.c_str());
        _processDbResult(res, "count", attrsSet, true, oResult);
    }
}

void rho::CNewORMModelImpl::getBackendRefreshTime(rho::apiGenerator::CMethodResult& oResult)
{
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    db::CDBAdapter& db = _get_db(oResult);
    int nTime = 0;
    IDBResult res = db.executeSQL("SELECT backend_refresh_time FROM sources WHERE source_id=?", source_id);
    if(!res.isEnd())
        nTime = res.getIntByIdx(0);
    oResult.set(CLocalTime(nTime).formatStr("%Y-%m-%d %H:%M:%S %z"));
}

void rho::CNewORMModelImpl::findObjects(const rho::String& what,
                 const Hashtable<rho::String, rho::String>& strOptions,
                 const rho::Vector<rho::String>& quests,
                 const rho::Vector<rho::String>& select_attrs,
                 const rho::Vector<rho::String>& order_attrs,
                 rho::apiGenerator::CMethodResult& oResult)
{
    if(what.empty()) {
        oResult.setArgError("findObjects: Invalid Empty First Argument passed.");
        return;
    }
    findObjectsFixedSchema(what, strOptions, quests, select_attrs, order_attrs, oResult);
}

void rho::CNewORMModelImpl::find_by_sql(const rho::String& sqlQuery, rho::apiGenerator::CMethodResult& oResult)
{
    if(!fixed_schema()) {
        oResult.setError("find_by_sql only works with FixedSchema models");
        return;
    }

    db::CDBAdapter& db = _get_db(oResult);
    IDBResult res = db.executeSQL(sqlQuery.c_str());
    if(!res.getDBError().isOK()) {
        oResult.setError(res.getDBError().getError());
        return;
    }
    Vector<Hashtable<rho::String, rho::String> > retVals;
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

void rho::CNewORMModelImpl::deleteObjects(const Hashtable<rho::String, rho::String>& strOptions, const Vector<rho::String>& quests, rho::apiGenerator::CMethodResult& oResult)
{
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    getProperty("sync_type", oResult);
    bool is_sync_source = (oResult.getString() != "none");
    db::CDBAdapter& db = _get_db(oResult);
    db.startTransaction();
    findObjectsFixedSchema("all", strOptions, quests, rho::Vector<rho::String>(), rho::Vector<rho::String>(), oResult);
    if(oResult.isError()) {
        db.rollback();
        return;
    }
    const rho::Vector<Hashtable<rho::String, rho::String> >& listObjs = oResult.getHashArray();
    for(size_t i = 0; i < listObjs.size(); ++i) {
        const Hashtable<rho::String, rho::String>& obj_data = listObjs[i];
        Hashtable<rho::String, rho::String>::const_iterator cObjIt = obj_data.find("object");
        if(cObjIt == obj_data.end()) {
            oResult.setError("Cannot delete an object without Object ID");
            db.rollback();
            return;
        }
        const rho::String& objId = cObjIt -> second;
        _deleteObject(db, is_sync_source, source_id, objId, obj_data, oResult);
        if(oResult.isError())
        {
            db.rollback();
            return;
        }
    }
    db.endTransaction();
}

void rho::CNewORMModelImpl::deleteObjectsPropertyBagByCondHash(const Hashtable<rho::String, rho::String>& conditions,
                    const Hashtable<rho::String, rho::String>& strOptions,
                    rho::apiGenerator::CMethodResult& oResult)
{
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    getProperty("sync_type", oResult);
    bool is_sync_source = (oResult.getString() != "none");
    db::CDBAdapter& db = _get_db(oResult);
    db.startTransaction();
    Vector<rho::String> selectAttrs;
    findObjectsPropertyBagByCondHash("all", conditions, strOptions, selectAttrs, oResult);
    if(oResult.isError()) {
        db.rollback();
        return;
    }
    const rho::Vector<Hashtable<rho::String, rho::String> >& listObjs = oResult.getHashArray();
    for(size_t i = 0; i < listObjs.size(); ++i) {
        const Hashtable<rho::String, rho::String>& obj_data = listObjs[i];
        Hashtable<rho::String, rho::String>::const_iterator cObjIt = obj_data.find("object");
        if(cObjIt == obj_data.end()) {
            oResult.setError("Cannot delete an object without Object ID");
            db.rollback();
            return;
        }
        const rho::String& objId = cObjIt -> second;
        _deleteObject(db, is_sync_source, source_id, objId, obj_data, oResult);
        if(oResult.isError())
        {
            db.rollback();
            return;
        }
    }
    db.endTransaction();
}

void rho::CNewORMModelImpl::deleteObjectsPropertyBagByCondArray(const rho::String& conditions,
                                    const Vector<rho::String>& quests,
                                    const Hashtable<rho::String, rho::String>& strOptions,
                                    rho::apiGenerator::CMethodResult& oResult)
{
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    getProperty("sync_type", oResult);
    bool is_sync_source = (oResult.getString() != "none");
    db::CDBAdapter& db = _get_db(oResult);
    db.startTransaction();
    Vector<rho::String> selectAttrs;
    findObjectsPropertyBagByCondArray("all", conditions, quests, strOptions, selectAttrs, oResult);
    if(oResult.isError()) {
        db.rollback();
        return;
    }
    const rho::Vector<Hashtable<rho::String, rho::String> >& listObjs = oResult.getHashArray();
    for(size_t i = 0; i < listObjs.size(); ++i) {
        const Hashtable<rho::String, rho::String>& obj_data = listObjs[i];
        Hashtable<rho::String, rho::String>::const_iterator cObjIt = obj_data.find("object");
        if(cObjIt == obj_data.end()) {
            oResult.setError("Cannot delete an object without Object ID");
            db.rollback();
            return;
        }
        const rho::String& objId = cObjIt -> second;
        _deleteObject(db, is_sync_source, source_id, objId, obj_data, oResult);
        if(oResult.isError())
        {
            db.rollback();
            return;
        }
    }
    db.endTransaction();
}

rho::String rho::CNewORMModelImpl::_make_select_attrs_str(const rho::Vector<rho::String>& select_attrs,
                                   rho::Hashtable<rho::String, rho::String>& attrsSet)
{
    if(select_attrs.empty())
        return rho::String("*");

    rho::String attrs_str;
    rho::String buffer;
    for(size_t i = 0; i < select_attrs.size(); ++i) {
        if(attrs_str.size())
            attrs_str += ",";
        const rho::String& attr = normalizeString(select_attrs[i], buffer);
        attrs_str += attr;
        attrsSet[attr] = "";
    }
    // object must be included always
    if(!attrsSet.containsKey("object")) {
        attrsSet["object"] = "";
        attrs_str += ",";
        attrs_str += "object";
    }
    return attrs_str;
}

rho::String rho::CNewORMModelImpl::_make_order_str(const Vector<rho::String>& order_attrs)
{
    rho::String order_str;
    rho::String order_attr_sql;
    rho::String buffer;
    if(order_attrs.empty())
        return order_str;
    for(size_t i = 0; i < order_attrs.size();)
    {
        if(order_attr_sql.size())
            order_attr_sql += ",";
        const rho::String& attr = normalizeString(order_attrs[i], buffer);
        order_attr_sql += rho::String("\"") + attr + "\" " + order_attrs[i + 1];
        i += 2;
    }
    if(order_attr_sql.size())
        order_str = rho::String(" ORDER BY ") + order_attr_sql;
    return order_str;
}

rho::String rho::CNewORMModelImpl::_make_limit_str(const Hashtable<rho::String, rho::String>& options)
{
    rho::String limit_str;
    int limit = -1, offset = 0;
    Hashtable<rho::String, rho::String>::const_iterator cIt = options.find("per_page");
    if(cIt != options.end()) {
        rho::common::convertFromStringA(cIt -> second.c_str(), limit);
    }
    cIt = options.find("offset");
    if(cIt != options.end()) {
        rho::common::convertFromStringA(cIt -> second.c_str(), offset);
    }

    if(limit > -1)
        limit_str = rho::String(" LIMIT " ) + rho::common::convertToStringA(limit) + " OFFSET " + rho::common::convertToStringA(offset);
    return limit_str;
}

void rho::CNewORMModelImpl::buildFindLimits(const rho::String& whatArg,
                     const Hashtable<rho::String, rho::String>& options,
                     rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<rho::String, rho::String> retVals;
    if(whatArg == "count") {
        oResult.set(retVals);
        return;
    }
    int iLimit = -1, iOffset = -1;
    if(whatArg == "first") {
        iLimit = 1;
        iOffset = 0;
    }
    Hashtable<rho::String, rho::String>::const_iterator cIt = options.find("offset");
    if(cIt != options.end()) {
        rho::common::convertFromStringA(cIt -> second.c_str(), iOffset);
        cIt = options.find("per_page");
        if(cIt != options.end())
            rho::common::convertFromStringA(cIt -> second.c_str(), iLimit);
    }
    if(iLimit != -1 && iOffset != -1) {
        retVals["per_page"] = rho::common::convertToStringA(iLimit);
        retVals["offset"] = rho::common::convertToStringA(iOffset);
    }
    oResult.set(retVals);
}

void rho::CNewORMModelImpl::buildFindOrder(const Vector<rho::String>& orderAttrs,
                    const Vector<rho::String>& orderOps,
                    rho::apiGenerator::CMethodResult& oResult)
{
    // if ORDER DIRECTION array is smaller - pad it
    Vector<rho::String> orderDirections(orderOps);
    int pad_number = orderAttrs.size() - orderOps.size();
    for(size_t i = 0; i < pad_number; ++i)
        orderDirections.push_back("ASC");
    rho::Vector<rho::String> retVals;
    rho::String buffer;
    for(size_t i = 0; i < orderAttrs.size(); ++i)
    {
        retVals.push_back(normalizeString(orderAttrs[i], buffer));
        retVals.push_back(orderDirections[i]);
    }
    oResult.set(retVals);
}

void rho::CNewORMModelImpl::buildFindOrderString(const Vector<rho::String>& orderAttrs,
                    const Vector<rho::String>& orderOps,
                    rho::apiGenerator::CMethodResult& oResult)
{
    buildFindOrder(orderAttrs, orderOps, oResult);
    rho::Vector<rho::String> orderArr = oResult.getStringArray();
    rho::String order_str = _make_order_str(orderArr);
    oResult.set(order_str);
}


void rho::CNewORMModelImpl::buildSimpleWhereCond(const rho::String& what,
                          const rho::Vector<rho::String>& conditions,
                          rho::apiGenerator::CMethodResult& oResult)
{
    if(conditions.size() > 0) {
        oResult.set(conditions);
        return;
    }
    // if conditions are empty - check if what is objId
    if(what != "all" && what != "count" && what != "first") {
        Vector<rho::String> values;
        values.push_back(what);
        buildComplexWhereCond("object", values, "=", "", oResult);
        return;
    }
    else { // all, first, count - build simple hash condition
        Vector<rho::String> values;
        buildComplexWhereCond("", values, "=", "", oResult);
        return;
    }
    oResult.set(conditions);
}

void rho::CNewORMModelImpl::buildComplexWhereCond(const rho::String& key,
                           const Vector<rho::String>& values,
                           const rho::String& val_op,
                           const rho::String& val_func,
                           rho::apiGenerator::CMethodResult& oResult)
{
    if(!key.size()) {
        oResult.setArgError("Invalid empty attribute name passed to WHERE condition");
    }
    rho::Vector<rho::String> retVals;
    retVals.push_back(rho::String());
    rho::String strSQL = _make_cond_where_ex(key, values, val_op, val_func, retVals);
    retVals[0] = strSQL;
    oResult.set(retVals);
}

bool rho::CNewORMModelImpl::_is_non_str_func_where(const rho::String& val_func)
{
    if(!val_func.size())
        return false;
    if(!strcasecmp(val_func.c_str(), "length"))
        return true;
    if(!strcasecmp(val_func.c_str(), "count"))
        return true;
    return false;
}

rho::String rho::CNewORMModelImpl::_make_cond_where_ex(const rho::String& key,
                                const Vector<rho::String>& values,
                                const rho::String& val_op,
                                const rho::String& val_func,
                                Vector<rho::String>& quests)
{
    rho::String strSQL;
    rho::String buffer;
    if(fixed_schema()) {
        if(!key.size())
            return strSQL;
        strSQL += (val_func.size() ? val_func + "(" + key + ")" : key);
        strSQL += " ";
    }
    else {
        rho::apiGenerator::CMethodResult oResult;
        if(!key.size())
            return strSQL;
        if(key != "object")
        {
            strSQL += " attrib=?";
            quests.push_back(normalizeString(key, buffer));
            strSQL += " AND ";
            strSQL += (val_func.size() ? val_func + "(value)" : "value");
            strSQL += " ";
        }
        else {
            strSQL += " object ";
        }
    }

    if(val_op == "IN" || val_op == "NOT IN")
    {
        strSQL += val_op + " (";
        rho::String valsStr;
        for(size_t i = 0; i < values.size(); ++i)
        {
            if(valsStr.size())
                valsStr += ",";
            valsStr += "?";
            quests.push_back(normalizeString(values[i], buffer));
        }
        strSQL += valsStr + ")";
    }
    else
    {
        if(!values.size())
            strSQL += " IS NULL";
        else
        {
            // do not add embedded \" in case of Integer Func like COUNT
            if(_is_non_str_func_where(val_func))
            {
                strSQL += val_op + rho::String(" ") + values[0];
            }
            else {
                strSQL += val_op + "?";
                quests.push_back(normalizeString(values[0], buffer));
            }
        }
    }

    return strSQL;
}

void rho::CNewORMModelImpl::findObjectsFixedSchema(const rho::String& what,
                 const Hashtable<rho::String, rho::String>& strOptions,
                 const rho::Vector<rho::String>& quests,
                 const rho::Vector<rho::String>& select_attrs,
                 const rho::Vector<rho::String>& order_attrs,
                 rho::apiGenerator::CMethodResult& oResult)
{
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    Hashtable<rho::String, rho::String> attrsSet;
    rho::String attrs_str = _make_select_attrs_str(select_attrs, attrsSet);
    rho::String order_str = _make_order_str(order_attrs);
    rho::String limit_str = _make_limit_str(strOptions);
    rho::String where_str;
    Hashtable<rho::String, rho::String>::const_iterator cIt = strOptions.find("conditions");
    if(cIt != strOptions.end())
        where_str = cIt -> second;
    rho::Vector<rho::String> questParams;
    rho::String buffer;
    questParams.reserve(quests.size());
    for(rho::Vector<rho::String>::const_iterator cIt = quests.begin(); cIt != quests.end(); ++cIt) {
        questParams.push_back(normalizeString(*cIt, buffer));
    }
    // build the SQL
    rho::String strSQL("SELECT ");
    if(what == "count") {
        strSQL += "COUNT(*) ";
    }
    else 
    {
        strSQL += attrs_str;
    }
    strSQL += rho::String(" FROM ") + name();
    if(where_str.size())
        strSQL += rho::String(" WHERE ") + where_str;
    if(order_str.size())
        strSQL += order_str;
    if(limit_str.size())
        strSQL += limit_str;

    db::CDBAdapter& db = _get_db(oResult);
    IDBResult res = db.executeSQLEx(strSQL.c_str(), questParams);
    _processDbResult(res, what, attrsSet, true, oResult);
}

void rho::CNewORMModelImpl::findObjectsPropertyBagByCondHash(const rho::String& what,
                                                             const Hashtable<rho::String, rho::String>& conditions,
                                                             const Hashtable<rho::String, rho::String>& strOptions,
                                                             const Vector<rho::String>& select_attr,
                                                             rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<rho::String, rho::String> attrsSet;
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    Hashtable<rho::String, rho::String> attrSet;
    rho::String selStr = _make_select_attrs_str(select_attr, attrSet);

    // make intersect SQL statement
    rho::String strSQL;
    rho::Vector<rho::String> quests;
    if(!conditions.size()) {
        strSQL = "SELECT object ";
        strSQL += " FROM object_values WHERE source_id=?";
        quests.push_back(source_id);
    }
    else
    {
        for(Hashtable<rho::String, rho::String>::const_iterator cIt = conditions.begin(); cIt != conditions.end(); ++cIt) {
            if(strSQL.size() > 0)
            {
                strSQL += "\nINTERSECT\n";
            }
            rho::String strCondStatement("SELECT object ");
            strCondStatement += " FROM object_values WHERE source_id=? AND ";
            quests.push_back(source_id);
            const rho::String& key = cIt -> first;
            const rho::String& value = cIt -> second;
            strCondStatement += rho::String("attrib=? AND value");
            strCondStatement += (value.empty() ? " IS NULL" : "=?");  
            //strCondStatement += rho::String("attrib=? AND value=?");
            quests.push_back(key);
            if(value.size())
                quests.push_back(value);
            strSQL += strCondStatement;
        }
    }

    db::CDBAdapter& db = _get_db(oResult);
    IDBResult res = db.executeSQLEx(strSQL.c_str(), quests);
    if(!res.getDBError().isOK()) {
        oResult.setError(res.getDBError().getError());
        return;
    }
    // now , we got objects, time to retrive all attribs
    Vector<Hashtable<rho::String, rho::String> > retVals;
    Hashtable<rho::String, Hashtable<rho::String, rho::String> > obj_hashes;
    for(; !res.isEnd(); res.next()) {
        Hashtable<rho::String, rho::String> objHash;
        rho::String objId = res.getStringByIdx(0);
        objHash["source_id"] = source_id;
        objHash["object"] = objId;
        _get_object_attrs(objId, objHash, attrSet, oResult);
        if(oResult.isError()) {
            return;
        }
        retVals.push_back(objHash);
    }
    // if count - return integer
    if(what == "count") {
        oResult.set((int)retVals.size());
        return;
    }
    oResult.set(retVals);
}

void rho::CNewORMModelImpl::findObjectsPropertyBagByCondArray(const rho::String& what,
                                                              const rho::String& conditions,
                                                              const Vector<rho::String>& quests,
                                                              const Hashtable<rho::String, rho::String>& strOptions,
                                                              const Vector<rho::String>& select_attr,
                                                              rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<rho::String, rho::String> attrsSet;
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    rho::String where_str(conditions);
    rho::Vector<rho::String> questParams(quests);
    // count returns an integer
    rho::String strSQL;
    
    if(what.empty()) {
        oResult.setArgError("findObjectsPropertyBagByCondArray: Invalid Empty First Argument passed.");
        return;
    }
    
    if(what == "count")
    {
        strSQL = "SELECT COUNT(DISTINCT object)";
    }

    Hashtable<rho::String, rho::String> attrSet;
    _make_select_attrs_str(select_attr, attrSet);

    if(!where_str.size()) 
    {
        if(what != "all" && what != "first" && what != "count") {
            where_str = "object=?";
            questParams.clear();
            questParams.push_back(what);
        }

        else if(what == "all" || what == "first"){
            int limit = -1;
            rho::String limit_str;

            if(what == "all"){
                limit_str = _make_limit_str(strOptions);
            }
            else if(what == "first"){
                limit_str = rho::String(" LIMIT 1 OFFSET 0");
            }

            if(limit_str.size()){
                rho::String sql = "SELECT distinct(object) FROM object_values WHERE source_id=";
                sql += source_id;
                sql += limit_str;

                db::CDBAdapter& db = _get_db(oResult);
                IDBResult res = db.executeSQL(sql.c_str());
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    return;
                }
                // now , we got objects, time to retrive all attribs
                Vector<Hashtable<rho::String, rho::String> > retVals;
                Hashtable<rho::String, Hashtable<rho::String, rho::String> > obj_hashes;
                for(; !res.isEnd(); res.next()) {
                    Hashtable<rho::String, rho::String> objHash;
                    rho::String objId = res.getStringByIdx(0);
                    objHash["source_id"] = source_id;
                    objHash["object"] = objId;
                    _get_object_attrs(objId, objHash, attrSet, oResult);
                    if(oResult.isError()) {
                        return;
                    }
                    retVals.push_back(objHash);
                }
                oResult.set(retVals);
                return;
            }
        }
        rho::String order_str = "";
        rho::String attrs_str = "object,attrib,value";
        if(!strSQL.size()) 
        {
            strSQL = "SELECT ";
            strSQL += attrs_str; 
        }
        strSQL += " FROM object_values";
        strSQL += " WHERE source_id=";
        strSQL += source_id;
        if(where_str.size())
            strSQL += rho::String(" AND ") + where_str;

        db::CDBAdapter& db = _get_db(oResult);
        IDBResult res = db.executeSQLEx(strSQL.c_str(), questParams);
        _processDbResult(res, what, attrSet, false, oResult);
        return;
    }
    else 
    {
        rho::String limit_str = "";
        rho::String order_str;
        rho::String order_attr_sql;

        Hashtable<rho::String, rho::String>::const_iterator cIt = strOptions.find("order");
        if(cIt != strOptions.end())
            order_str = cIt -> second;

        limit_str = _make_limit_str(strOptions);

        if(what != "count") 
        {
            if(what == "first")
                limit_str = rho::String(" LIMIT 1 OFFSET 0");

            if(where_str.size())
                strSQL = "SELECT * FROM (\n";
        }
        else 
        {
            strSQL += " FROM (\n";
        }
        strSQL += "SELECT object";

        rho::Vector<rho::String> col_attrib = _get_attribs_of_PropertyBagModel(oResult);

        for(size_t i = 0; i < col_attrib.size(); ++i)
        {
            if(_is_reserved_name(col_attrib[i]))
                continue;
            strSQL += ",\n";
            strSQL += "MAX(CASE WHEN attrib = '";
            strSQL += col_attrib[i];
            strSQL += "' THEN value ELSE NULL END) AS \'";
            strSQL += col_attrib[i];
            strSQL += "\'";
        }
        strSQL += " FROM object_values ov \n";
        strSQL += " WHERE source_id=";
        strSQL += source_id;
        strSQL += "\n";
        strSQL += "GROUP BY object\n";
        strSQL += ") WHERE ";
        strSQL += where_str;

        if(order_str.size())
            strSQL += order_str;
        if(limit_str.size())
            strSQL += limit_str;

        db::CDBAdapter& db = _get_db(oResult);
        IDBResult res = db.executeSQLEx(strSQL.c_str(), questParams);
        _processDbResult(res, what, attrSet, true, oResult);
    }
}

void rho::CNewORMModelImpl::_processDbResult(IDBResult& res, 
                                             const rho::String& what,
                                             const Hashtable<rho::String, rho::String>& attrSet,
                                             const bool tableResults, 
                                             rho::apiGenerator::CMethodResult& oResult)
{
    if(!res.getDBError().isOK()) {
        oResult.setError(res.getDBError().getError());
        return;
    }

    if(what == "count")
    {
        if(!res.isEnd())
            oResult.set(res.getIntByIdx(0));
        return;   
    }

    // fixed_schema and inclusion SELECT for PropertyBag returns col/value table-format rows
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    if(tableResults) {
        Vector<Hashtable<rho::String, rho::String> > retVals;
        rho::String datum;
        for(; !res.isEnd(); res.next()) {
            int ncols = res.getColCount();
            Hashtable<rho::String, rho::String> obj_hash;
            for(int i = 0; i < ncols; ++i) {
                if (res.getStringOrNil(i, datum)) {
                    // include only the requested attributes
                    if(attrSet.size() && !attrSet.containsKey(res.getColName(i)))
                        continue;
                    obj_hash.put(res.getColName(i), datum);
                }
            }
            obj_hash["source_id"] = source_id;
            retVals.push_back(obj_hash);
        }
        oResult.set(retVals);
    }
    // propertyBag search returns attrib,value,object rows
    else {
        Vector<Hashtable<rho::String, rho::String> > retVals;
        Hashtable<rho::String, Hashtable<rho::String, rho::String> > obj_hashes;
        rho::String buffer;
        for(; !res.isEnd(); res.next()) {
            // include only the requested attributes
            if(attrSet.size() && !attrSet.containsKey(res.getStringByIdx(1)))
                continue;
            if (res.getStringOrNil(2,buffer)){
                obj_hashes[res.getStringByIdx(0)][res.getStringByIdx(1)] = buffer;
            }
        }
        for(Hashtable<rho::String, Hashtable<rho::String, rho::String> >::iterator cResIt = obj_hashes.begin(); cResIt != obj_hashes.end(); ++cResIt) {
            Hashtable<rho::String, rho::String>& obj_hash = cResIt -> second;
            const rho::String& objId = cResIt -> first;
            obj_hash["object"] = objId;
            obj_hash["source_id"] = source_id;
            retVals.push_back(obj_hash);
        }
        oResult.set(retVals);
    }
}

void rho::CNewORMModelImpl::hasChanges(const rho::String& objId, rho::apiGenerator::CMethodResult& oResult)
{
    db::CDBAdapter& db = _get_db(oResult);
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    IDBResult res = db.executeSQL("SELECT object from changed_values WHERE source_id=? and object=? LIMIT 1 OFFSET 0", source_id, objId);
    bool has_changes = false;
    if(!res.isEnd())
        has_changes = (res.getStringByIdx(0).size() > 0);
    oResult.set(has_changes);
}

void rho::CNewORMModelImpl::anyChangedObjects(rho::apiGenerator::CMethodResult& oResult)
{
    db::CDBAdapter& db = _get_db(oResult);
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    IDBResult res = db.executeSQL("SELECT object from changed_values WHERE source_id=? LIMIT 1 OFFSET 0", source_id);
    bool has_changes = false;
    if(!res.isEnd())
        has_changes = (res.getStringByIdx(0).size() > 0);
    oResult.set(has_changes);
}

void rho::CNewORMModelImpl::canModify(const rho::String& objId, rho::apiGenerator::CMethodResult& oResult)
{
    db::CDBAdapter& db = _get_db(oResult);
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    IDBResult res = db.executeSQL("SELECT object from changed_values WHERE source_id=? and object=? and sent>1 LIMIT 1 OFFSET 0", source_id, objId);
    bool can_modify = res.isEnd();
    oResult.set(can_modify);
}

void rho::CNewORMModelImpl::createInstance(const Hashtable<rho::String, rho::String>& attrs, rho::apiGenerator::CMethodResult& oResult)
{
    validateFreezedAttributes(attrs, oResult);
    if(oResult.isError())
        return;
    Hashtable<rho::String, rho::String> retInstance(attrs);
    // remove incorrect attributes
    retInstance.remove("");
    if(!retInstance.containsKey("object"))
    {
        rho::CNewORMFactoryBase::getNewORMSingletonS()->generateId(oResult);
        if(oResult.isError())
            return;
        retInstance["object"] = rho::common::convertToStringA(oResult.getInt());
    }
    getProperty("source_id", oResult);
    retInstance["source_id"] = oResult.getString();
    oResult.set(retInstance);
}

void rho::CNewORMModelImpl::createObject(const Hashtable<rho::String, rho::String>& objHash, rho::apiGenerator::CMethodResult& oResult)
{
    createInstance(objHash, oResult);
    if(oResult.isError())
        return;
    Hashtable<rho::String, rho::String>& attrs = oResult.getStringHash();

    // use another CMethodResult for intermediate operations
    // to preserve attrs hash
    // (except when we're returning errors)
    rho::apiGenerator::CMethodResult createResult;
    getProperty("source_id", createResult);
    rho::String source_id = createResult.getString();
    getProperty("sync_type", createResult);
    bool is_sync_source = (createResult.getString() != "none");
    db::CDBAdapter& db = _get_db(createResult);

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

void rho::CNewORMModelImpl::updateObject(const rho::String& objId,
                  const Hashtable<rho::String, rho::String>& oldAttrs,
                  const Hashtable<rho::String, rho::String>& newAttrs,
                  rho::apiGenerator::CMethodResult& oResult)
{
    Hashtable<rho::String, rho::String> retAttrs;
    validateFreezedAttributes(newAttrs, oResult);
    if(oResult.isError())
        return;

    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    int iSrcId = -1;
    convertFromStringA(source_id.c_str(), iSrcId);

    db::CDBAdapter& db = _get_db(oResult);
    bool createOrUpdate = false;
    bool ignore_changed_values = true;
    getProperty("sync_type", oResult);
    bool is_sync_source = (oResult.getString() != "none");
    getProperty("full_update", oResult);
    bool is_full_update = (oResult.getString() != "");
    rho::String update_type("update");
    rho::String existing_update_type;

    db.startTransaction();
    if(is_sync_source) {
        IDBResult res = db.executeSQL("SELECT update_type FROM changed_values WHERE object=? AND source_id=? AND sent=0",
                                        objId, source_id);
        if(!res.getDBError().isOK()) {
            oResult.setError(res.getDBError().getError());
            db.rollback();
            return;
        }
        if(!res.isEnd())
        {
            existing_update_type = res.getStringByIdx(0);
        }
        ignore_changed_values = (existing_update_type == "create");
    }

    for(Hashtable<rho::String, rho::String>::const_iterator cIt = newAttrs.begin(); cIt != newAttrs.end(); ++cIt)
    {
        const rho::String& attrKey = cIt -> first;
        if(_is_reserved_name(attrKey))
            continue;
        const rho::String& attrValue = cIt -> second;
        bool is_modified = false;
        Hashtable<rho::String, rho::String>::const_iterator cOldIt = oldAttrs.find(attrKey);
        if(cOldIt != oldAttrs.end())
        {
            const rho::String& oldAttrValue = cOldIt -> second;
            is_modified = (attrValue != oldAttrValue);
        }
        else
            is_modified = (attrValue.size() > 0);

        // if the object's value doesn't match the database record
        // then we procede with update
        if(is_modified)
        {
            Vector<rho::String> quests;
            rho::String sqlScript = _make_insert_or_update_attr_sql_script(source_id, objId, attrKey, attrValue, quests);
            IDBResult res = db.executeSQLEx(sqlScript.c_str(), quests);
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                db.rollback();
                return;
            }
            if(!ignore_changed_values)
            {
                if(existing_update_type.size() > 0)
                {
                    rho::String sqlDelStr("DELETE FROM changed_values WHERE object=? AND attrib=? AND source_id=? AND sent=0");
                    res = db.executeSQL(sqlDelStr.c_str(), objId, attrKey, source_id);
                    if(!res.getDBError().isOK()) {
                        oResult.setError(res.getDBError().getError());
                        db.rollback();
                        return;
                    }
                }

                rho::String attrib_type = (db.getAttrMgr().isBlobAttr(iSrcId, attrKey.c_str()) ? "blob.file" : "");
                rho::String insertScript("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type) VALUES (?,?,?,?,?,?)");
                res = db.executeSQL(insertScript.c_str(), source_id, objId, attrKey, attrValue, update_type, attrib_type);
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }
            }

            // to update in-memory object
            retAttrs[attrKey] = attrValue;
        }
    }
    if (is_full_update && !ignore_changed_values) {
        rho::String sqlDelStr("DELETE FROM changed_values WHERE object=? AND source_id=? AND sent=0");
        IDBResult res = db.executeSQL(sqlDelStr.c_str(), objId, source_id);
        if(!res.getDBError().isOK()) {
            oResult.setError(res.getDBError().getError());
            db.rollback();
            return;
        }
        Hashtable<rho::String, rho::String> obj_hash;
        if (fixed_schema()) {
            rho::String strSQL("SELECT * FROM ");
            strSQL += name();
            strSQL += rho::String(" WHERE object=?");
            res = db.executeSQL(strSQL.c_str(), objId);
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                return;
            }
            // Fill out hashtable
            for(; !res.isEnd(); res.next()) {
                int ncols = res.getColCount();
                for(int i = 0; i < ncols; ++i) {
                    obj_hash.put(res.getColName(i), res.getStringByIdx(i));
                }
            }
        } else { // Property Bag
            rho::String strSQL("SELECT attrib,value FROM object_values WHERE object=? AND source_id=?");
            res = db.executeSQL(strSQL.c_str(), objId, source_id);
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                return;
            }
            for(; !res.isEnd(); res.next()) {
                obj_hash.put(res.getStringByIdx(0), res.getStringByIdx(1));
            }
        }
        for(Hashtable<rho::String, rho::String>::const_iterator cIt = obj_hash.begin(); cIt != obj_hash.end(); ++cIt)
        {
            const rho::String& attrKey = cIt -> first;
            const rho::String& attrValue = cIt -> second;
            if(_is_reserved_name(attrKey)) continue;

            rho::String attrib_type = (db.getAttrMgr().isBlobAttr(iSrcId, attrKey.c_str()) ? "blob.file" : "");
            res = db.executeSQL(
                "INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type) VALUES (?,?,?,?,?,?)",
                source_id, objId, attrKey, attrValue, update_type, attrib_type);
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                return;
            }
        }
    }

    db.endTransaction();
    oResult.set(retAttrs);
}

void rho::CNewORMModelImpl::saveObject(const rho::String& objId,
                const Hashtable<rho::String, rho::String>& attrs,
                rho::apiGenerator::CMethodResult& oResult)
{
    validateFreezedAttributes(attrs, oResult);
    if(oResult.isError())
        return;

    db::CDBAdapter& db = _get_db(oResult);
    db.Lock();
    Hashtable<rho::String, rho::String> oldAttrs;
    Hashtable<rho::String, rho::String> selectAttrs;
    bool object_exists = _get_object_attrs(objId, oldAttrs, selectAttrs, oResult);
    if(!object_exists) {
        createObject(attrs, oResult);
        db.Unlock();
        return;
    }
    else
    {
        updateObject(objId, oldAttrs, attrs, oResult);
        db.Unlock();
        return;
    }
}

void rho::CNewORMModelImpl::deleteObject(const rho::String& objId,
                  rho::apiGenerator::CMethodResult& oResult)
{
    // delete all attrs first
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    getProperty("sync_type", oResult);
    bool is_sync_source = (oResult.getString() != "none");
    db::CDBAdapter& db = _get_db(oResult);
    db.startTransaction();
    Hashtable<rho::String, rho::String> attrs;
    Hashtable<rho::String, rho::String> selectAttrs;
    bool object_exists = _get_object_attrs(objId, attrs, selectAttrs, oResult);
    if(object_exists)
        _deleteObject(db, is_sync_source, source_id, objId, attrs, oResult);
    if(oResult.isError())
        db.rollback();
    else
        db.endTransaction();
}

void rho::CNewORMModelImpl::_deleteObject(db::CDBAdapter& db,
                   const bool is_sync_source,
                   const rho::String& source_id,
                   const rho::String& objId,
                   const Hashtable<rho::String, rho::String>& attrs,
                   rho::apiGenerator::CMethodResult& oResult)
{
    // delete all attrs first
    if(fixed_schema()) {
        rho::String deleteSql("DELETE FROM ");
        deleteSql += name() + " WHERE object=?";
        IDBResult res = db.executeSQL(deleteSql.c_str(), objId);
        if(!res.getDBError().isOK()) {
            oResult.setError(res.getDBError().getError());
            return;
        }
    }
    else
    {
        rho::String deleteSql("DELETE FROM object_values WHERE object=? AND source_id=?");
        IDBResult res = db.executeSQL(deleteSql.c_str(), objId, source_id);
        if(!res.getDBError().isOK()) {
            oResult.setError(res.getDBError().getError());
            return;
        }
    }

    bool ignore_changed_values = true;
    if(is_sync_source) {
        IDBResult res = db.executeSQL("SELECT update_type FROM changed_values WHERE object=? AND update_type=\"create\" AND sent=0", objId);
        if(!res.getDBError().isOK()) {
            oResult.setError(res.getDBError().getError());
            return;
        }
        // if object hasn't been created - then, no need to queue delete to changed values, just do the cleanup
        ignore_changed_values = !res.isEnd();
        res = db.executeSQL("DELETE FROM changed_values WHERE object=? AND source_id=? AND sent=0", objId, source_id);
        if(!res.getDBError().isOK()) {
            oResult.setError(res.getDBError().getError());
            return;
        }
        // update changed values with delete request
        if(!ignore_changed_values) {
            for(Hashtable<rho::String, rho::String>::const_iterator cIt = attrs.begin(); cIt != attrs.end(); ++cIt)
            {
                const rho::String& attrKey = cIt -> first;
                const rho::String& attrValue = cIt -> second;
                if(_is_reserved_name(attrKey) || !attrValue.size())
                    continue;
                res = db.executeSQL("INSERT INTO changed_values (source_id,object,attrib,value,update_type) VALUES (?,?,?,?,?)",
                                    source_id, objId, attrKey, attrValue, "delete");
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    return;
                }
            }
        }
    }
    oResult.set(true);
}

rho::db::CDBAdapter& rho::CNewORMModelImpl::_get_db(rho::apiGenerator::CMethodResult& oResult)
{
    getProperty("partition", oResult);
    rho::String partition = oResult.getString();
    return db::CDBAdapter::getDB(partition.c_str());
}

void rho::CNewORMModelImpl::onSyncDeleteError(const rho::String& objId,
                                              const Hashtable<rho::String, rho::String>& attrHash,
                                              const rho::String& actionStr,
                                              rho::apiGenerator::CMethodResult& oResult)
{
    LOG(INFO) +  "onSyncDeleteError: " + actionStr;
    if(!rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        return;
    if(actionStr != "retry")
    {
        oResult.setArgError("on_sync_delete_error action should be `retry`");
        return;
    }
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    int iSrcId = -1;
    convertFromStringA(source_id.c_str(), iSrcId);
    getProperty("sync_type", oResult);
    bool is_sync_source = (oResult.getString() != "none");
    if(!is_sync_source)
        return;
    db::CDBAdapter& db = _get_db(oResult);

    db.startTransaction();
    for(Hashtable<rho::String, rho::String>::const_iterator cIt = attrHash.begin(); cIt != attrHash.end(); ++cIt)
    {
        const rho::String& attrName = cIt -> first;
        const rho::String& attrValue = cIt -> second;
        rho::String selectSQL("SELECT update_type FROM changed_values WHERE object=? AND source_id=? AND attrib=? and sent=0");
        IDBResult res = db.executeSQL(selectSQL.c_str(), objId, source_id, attrName);
        if(!res.isEnd())
            continue;

        rho::String attrib_type = (db.getAttrMgr().isBlobAttr(iSrcId, attrName.c_str()) ? "blob.file" : "");
        rho::String insertScript("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type) VALUES (?,?,?,?,?,?)");
        res = db.executeSQL(insertScript.c_str(), source_id, objId, attrName, attrValue, "delete", attrib_type);
        if(!res.getDBError().isOK()) {
            oResult.setError(res.getDBError().getError());
            db.rollback();
            return;
        }
    }
    db.endTransaction();
}

void rho::CNewORMModelImpl::onSyncUpdateError(const rho::String& objId,
                                              const Hashtable<rho::String, rho::String>& attrHash,
                                              const Hashtable<rho::String, rho::String>& rollbackHash,
                                              const rho::String& actionStr,
                                              rho::apiGenerator::CMethodResult& oResult)
{
    LOG(INFO) +  "onSyncUpdateError: " + actionStr;
    if(!rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        return;
    if(actionStr != "retry" && actionStr != "rollback")
    {
        oResult.setError("on_sync_update_error action should be `retry` or `rollback`");
        return;
    }
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    int iSrcId = -1;
    convertFromStringA(source_id.c_str(), iSrcId);
    getProperty("sync_type", oResult);
    bool is_sync_source = (oResult.getString() != "none");
    if(!is_sync_source)
        return;
    getProperty("full_update", oResult);
    bool is_full_update = false;
    convertFromStringA(oResult.getString().c_str(), is_full_update);

    db::CDBAdapter& db = _get_db(oResult);

    db.startTransaction();
    if(actionStr == "rollback")
    {
        for(Hashtable<rho::String, rho::String>::const_iterator cIt = rollbackHash.begin(); cIt != rollbackHash.end(); ++cIt)
        {
            const rho::String& attrName = cIt -> first;
            const rho::String& attrValue = cIt -> second;
            Vector<rho::String> quests;
            rho::String sqlScript = _make_insert_or_update_attr_sql_script(source_id, objId, attrName, attrValue, quests);
            IDBResult res = db.executeSQLEx(sqlScript.c_str(), quests);
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                db.rollback();
                return;
            }
        }
    }
    else
    {
        for(Hashtable<rho::String, rho::String>::const_iterator cIt = attrHash.begin(); cIt != attrHash.end(); ++cIt)
        {
            const rho::String& attrName = cIt -> first;
            const rho::String& attrValue = cIt -> second;
            rho::String selectSQL("SELECT update_type FROM changed_values WHERE object=? AND source_id=? AND attrib=? and sent=0");
            IDBResult res = db.executeSQL(selectSQL.c_str(), objId, source_id, attrName);
            if(!res.isEnd())
                continue;

            rho::String attrib_type = (db.getAttrMgr().isBlobAttr(iSrcId, attrName.c_str()) ? "blob.file" : "");
            rho::String insertScript("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type) VALUES (?,?,?,?,?,?)");
            res = db.executeSQL(insertScript.c_str(), source_id, objId, attrName, attrValue, "update", attrib_type);
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                db.rollback();
                return;
            }
        }
    }
    db.endTransaction();
}

void rho::CNewORMModelImpl::onSyncCreateError(const Vector<rho::String>& objects,
                                              const rho::String& actionStr,
                                              rho::apiGenerator::CMethodResult& oResult)
{
    LOG(INFO) +  "onSyncCreateError: " + actionStr;
    if(!rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        return;
    if(actionStr != "delete" && actionStr != "recreate")
    {
        oResult.setError("on_sync_create_error action should be `delete` or `recreate`");
        return;
    }
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    getProperty("sync_type", oResult);
    bool is_sync_source = (oResult.getString() != "none");
    if(!is_sync_source)
        return;
    db::CDBAdapter& db = _get_db(oResult);

    db.startTransaction();
    for(size_t i = 0; i < objects.size(); ++i)
    {
        const rho::String& objId = objects[i];
        if(actionStr == "recreate")
        {
            rho::String selectSQL("SELECT object FROM changed_values WHERE update_type=? AND object=? AND source_id=?");
            IDBResult res = db.executeSQL(selectSQL.c_str(), "delete", objId, source_id);
            if(!res.isEnd())
                continue;

            rho::String deleteSQL("DELETE FROM changed_values WHERE source_id=? AND object=?");
            res = db.executeSQL(deleteSQL.c_str(), source_id, objId);
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                db.rollback();
                return;
            }

            rho::String insertScript("INSERT INTO changed_values (source_id,object,attrib,update_type) VALUES (?,?,?,?)");
            res = db.executeSQL(insertScript.c_str(), source_id, objId, "object", "create");
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                db.rollback();
                return;
            }
        }
        else // delete
        {
            rho::String deleteSQL("DELETE FROM changed_values WHERE source_id=? AND object=?");
            IDBResult res = db.executeSQL(deleteSQL.c_str(), source_id, objId);
            if(!res.getDBError().isOK()) {
                oResult.setError(res.getDBError().getError());
                db.rollback();
                return;
            }

            if(fixed_schema()) {
                rho::String deleteSourceSQL("DELETE FROM ");
                deleteSourceSQL += name() + " WHERE object=?";
                res = db.executeSQL(deleteSourceSQL.c_str(), objId);
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }
            }
            else
            {
                rho::String deleteSourceSQL("DELETE FROM object_values WHERE object=? AND source_id=?");
                res = db.executeSQL(deleteSourceSQL.c_str(), objId, source_id);
                if(!res.getDBError().isOK()) {
                    oResult.setError(res.getDBError().getError());
                    db.rollback();
                    return;
                }
            }
        }
    }
    db.endTransaction();
}

void rho::CNewORMModelImpl::pushChanges(rho::apiGenerator::CMethodResult& oResult)
{
    LOG(INFO) +  "pushChanges";
    if(!rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl())
        return;
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    getProperty("sync_type", oResult);
    bool is_sync_source = (oResult.getString() != "none");
    if(!is_sync_source)
        return;
    db::CDBAdapter& db = _get_db(oResult);
    rho::String insertScript("INSERT INTO changed_values (source_id,update_type) VALUES (?,?)");
    IDBResult res = db.executeSQL(insertScript.c_str(), source_id, "push_changes");
    if(!res.getDBError().isOK()) {
        oResult.setError(res.getDBError().getError());
    }
}

void rho::CNewORMModelImpl::init_defaults()
{
    CNewORMModelImpl::init_once();
    rho::apiGenerator::CMethodResult oRes;
    setLoaded(false, oRes);
    setFixed_schema(false, oRes);
    setSync_type("none", oRes);
    setSync_priority(1000, oRes);
    setPartition("local", oRes);
}

int rho::CNewORMModelImpl::_get_partition_start_id(const rho::String& partition)
{
    if(partition == "user")
        return 0;
    else if(partition == "app")
        return 20000;
    else if(partition == "local")
        return 40000;

    return 0;
}

rho::String rho::CNewORMModelImpl::_get_db_type(const rho::String& propType)
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

bool rho::CNewORMModelImpl::_is_reserved_name(const rho::String& attrName)
{
    return reserved_names_.containsKey(attrName);
}

rho::String rho::CNewORMModelImpl::_make_insert_attrs_sql_script(const rho::String& objectId,
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

rho::String rho::CNewORMModelImpl::_make_create_sql_script() const
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

    return strCreate;
}

rho::Vector<rho::String> rho::CNewORMModelImpl::_create_sql_schema_indices() const
{
    rho::Vector<rho::String> retIndices;
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
        strIndex += rho::String("INDEX ") + index_def.name_ + " ON " + name() + " (" + strCols + ");\r\n";
        retIndices.push_back(strIndex);
    }

    return retIndices;
}

rho::String rho::CNewORMModelImpl::_make_insert_or_update_attr_sql_script(const rho::String srcId,
                                                        const rho::String& objId,
                                                        const rho::String& attrKey,
                                                        const rho::String& attrValue,
                                                        Vector<rho::String>& quests)
{
    rho::apiGenerator::CMethodResult oResult;
    db::CDBAdapter& db = _get_db(oResult);
    rho::String retScript;
    if(fixed_schema())
    {
        rho::String checkObjScript = rho::String("SELECT object FROM ") + name() + " WHERE object=?";
        IDBResult res = db.executeSQL(checkObjScript.c_str(), objId);
        bool insert_or_update = false;
        insert_or_update = res.isEnd();
        if(insert_or_update) {
            retScript = rho::String("INSERT INTO ") + name() + " (object," + attrKey + ") VALUES (?,?)";
            quests.push_back(objId);
            quests.push_back(attrValue);
        }
        else
        {
            retScript = rho::String("UPDATE ") + name() + " SET " + attrKey + "=? WHERE object=?";
            quests.push_back(attrValue);
            quests.push_back(objId);
        }
    }
    else
    {
        rho::String checkObjScript("SELECT source_id FROM object_values WHERE object=? AND attrib=? AND source_id=?");
        IDBResult res = db.executeSQL(checkObjScript.c_str(), objId, attrKey, srcId);
        bool insert_or_update = false;
        insert_or_update = res.isEnd();
        if(insert_or_update) {
            retScript = rho::String("INSERT INTO object_values (source_id,object,attrib,value) VALUES (?,?,?,?)");
            quests.push_back(srcId);
            quests.push_back(objId);
            quests.push_back(attrKey);
            quests.push_back(attrValue);
        }
        else {
            retScript = rho::String("UPDATE object_values SET value=? WHERE object=? AND source_id=? AND attrib=?");
            quests.push_back(attrValue);
            quests.push_back(objId);
            quests.push_back(srcId);
            quests.push_back(attrKey);
        }
    }

    return retScript;
}

bool rho::CNewORMModelImpl::_get_object_attrs(const rho::String& objId,
                        Hashtable<rho::String, rho::String>& attrs,
                        const Hashtable<rho::String, rho::String>& selectAttrs,
                        rho::apiGenerator::CMethodResult& oResult)
{
    if(objId.empty())
        return false;

    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();
    int iSrcId = -1;
    convertFromStringA(source_id.c_str(), iSrcId);

    db::CDBAdapter& db = _get_db(oResult);
    bool object_exists = false;
    if(fixed_schema()) {
        rho::String select_attr("*");
        if(selectAttrs.size()) {
            select_attr = "";
            for(Hashtable<rho::String, rho::String>::const_iterator cIt = selectAttrs.begin(); cIt != selectAttrs.end(); ++cIt) {
                if(select_attr.size())
                    select_attr += ",";
                select_attr += cIt -> first;
            }
        }
        rho::String checkSql("SELECT ");
        checkSql += select_attr + " FROM ";
        checkSql += name() + " WHERE object=? LIMIT 1 OFFSET 0";
        IDBResult res = db.executeSQL(checkSql.c_str(), objId);
        object_exists = !res.isEnd();
        if(object_exists)
        {
            int ncols = res.getColCount();
            for(int i = 0; i < ncols; ++i)
            {
                attrs[res.getColName(i)] = res.getStringByIdx(i);
            }
        }
    }
    else
    {
        rho::String checkSql("SELECT attrib,value FROM object_values WHERE object=? AND source_id=?");
        IDBResult res = db.executeSQL(checkSql.c_str(), objId, source_id);
        object_exists = !res.isEnd();
        if(object_exists)
        {
            for(; !res.isEnd(); res.next())
            {
                if(selectAttrs.size() && !selectAttrs.containsKey(res.getStringByIdx(0)))
                    continue;
                attrs[res.getStringByIdx(0)] = res.getStringByIdx(1);
            }
        }
    }

    return object_exists;
}

rho::String rho::CNewORMModelImpl::_strip_braces(const rho::String& str)
{
    rho::String retStr = str;
    if(retStr.size() && retStr[0] == '{')
        retStr = retStr.substr(1);
    if(retStr.size() && retStr[retStr.size() - 1] == '}')
        retStr = retStr.substr(0, retStr.size() - 1);
    return retStr;
}

rho::Vector<rho::String> rho::CNewORMModelImpl::_get_attribs_of_PropertyBagModel(rho::apiGenerator::CMethodResult& oResult)
{
    getProperty("source_id", oResult);
    rho::String source_id = oResult.getString();

    //Lets get all attrib as columns
    rho::Vector<rho::String> col_attributes;
    rho::String sql = "SELECT distinct(attrib) FROM object_values WHERE source_id=";
    sql += source_id;

    db::CDBAdapter& db = _get_db(oResult);
    IDBResult res = db.executeSQL(sql.c_str());
    if(!res.getDBError().isOK()) {
        oResult.setError(res.getDBError().getError());
        return col_attributes;
    }

    for(; !res.isEnd(); res.next()) {
        col_attributes.push_back(res.getStringByIdx(0));
    }

    return col_attributes;
}

// static definitions
rho::HashtablePtr<rho::String, rho::CNewORMModelImpl*> rho::CNewORMModelImpl::models_;
rho::Hashtable<rho::String, int> rho::CNewORMModelImpl::reserved_names_;

extern "C" void Init_NewORMModel_extension()
{
    rho::CNewORMModelFactory::setInstance( new rho::CNewORMModelFactory() );
    rho::Init_NewORMModel_API();
}