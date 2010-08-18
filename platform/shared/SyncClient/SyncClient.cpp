#include "SyncClient.h"

#include "stdafx.h"

#include "common/RhodesAppBase.h"
#include "sync/SyncThread.h"
#include "common/RhoFile.h"
#include "common/Tokenizer.h"
#include "common/RhoConf.h"
#include "common/RhoError.h"
#include "common/RhoTime.h"

using namespace rho;
using namespace rho::common;
String getSyncTypeName( RHOM_SYNC_TYPE sync_type );
void parseSyncNotify(const char* msg, RHO_SYNC_NOTIFY* pNotify);
String rhom_generate_id();

extern "C" 
{

void rho_syncclient_initmodel(RHOM_MODEL* model)
{
    memset( model, 0, sizeof(RHOM_MODEL) );
    model->type = RMT_PROPERTY_BAG;
    model->sync_type = RST_NONE;
    model->sync_priority = 1000;
    model->partition = "user";
}

void rho_syncclient_free_syncnotify(RHO_SYNC_NOTIFY* pNotify)
{
    if (!pNotify)
        return;

    if ( pNotify->source_name != null )
        free(pNotify->source_name);

    if ( pNotify->status != null )
        free(pNotify->status);

    if ( pNotify->error_message != null )
        free(pNotify->error_message);

    if ( pNotify->callback_params != null )
        free(pNotify->callback_params);

    memset( pNotify, 0, sizeof(RHO_SYNC_NOTIFY) );
}

void rho_syncclient_processmodels(RHOM_MODEL* pModels, int nModels)
{
    db::CDBAdapter& oUserDB = db::CDBAdapter::getUserDB();
    int nStartModelID = 1;
    {
        DBResult( res, oUserDB.executeSQL("SELECT MAX(source_id) AS maxid FROM sources"));
        if ( !res.isEnd() )
            nStartModelID = res.getIntByIdx(0)+2;
    }

    for ( int i = 0; i < nModels; i++ )
    { 
        RHOM_MODEL& model = pModels[i];
        DBResult( res, oUserDB.executeSQL("SELECT sync_priority,source_id,partition, sync_type, schema_version, associations, blob_attribs FROM sources WHERE name=?",
            model.name) );

        if ( !res.isEnd() )
        {
            oUserDB.executeSQL("UPDATE sources SET sync_priority=?, sync_type=?, partition=?, schema_version=?, associations=?, blob_attribs=?",
                model.sync_priority, getSyncTypeName(model.sync_type), model.partition, "", "", "" );
                
        }else //new model
        {
            oUserDB.executeSQL("INSERT INTO sources (source_id,name,sync_priority, sync_type, partition, schema_version, associations, blob_attribs) values (?,?,?,?,?,?,?,?) ",
                nStartModelID, model.name, model.sync_priority, getSyncTypeName(model.sync_type), model.partition, "", "", "" );

            nStartModelID++;
        }
    }
}

void rho_syncclient_init(RHOM_MODEL* pModels, int nModels)
{
    rho_logconf_Init(rho_native_rhopath());
    CRhodesAppBase::Create( rho_native_rhopath() );

    String strDbPath = rho_native_rhopath();

    //create db and db-files folder
    CRhoFile::createFolder( (strDbPath + "db/db-files").c_str());

    for( int i = 0; i < nModels; i++ )
    {
        RHOM_MODEL& model = pModels[i];

        String strDbPartition = strDbPath + "db/syncdb";
        strDbPartition += model.partition;
        strDbPartition += ".sqlite";

        void* pDB = 0;
        rho_db_open( strDbPartition.c_str(), model.partition, &pDB);
    }

    //process models
    rho_syncclient_processmodels(pModels, nModels);

    rho_db_init_attr_manager();

    LOG(INFO) + "Starting sync engine...";
    sync::CSyncThread::Create(common::rho_impl_createClassFactory());

}

void rho_syncclient_database_full_reset_and_logout()
{
    rho_sync_logout();

    db::CDBAdapter& oUserDB = db::CDBAdapter::getUserDB();
    oUserDB.executeSQL("UPDATE client_info SET reset=1");

    if ( rho_conf_is_property_exists("bulksync_state") )
        rho_conf_setInt("bulksync_state", 0 );

    oUserDB.executeSQL("UPDATE sources SET token=0");

    Vector<String> arExclude;
    arExclude.addElement("sources");
    arExclude.addElement("client_info");
    db::CDBAdapter::destroy_tables_allpartitions(Vector<String>(), arExclude);

    //TODO: scema_sources
    //hash_migrate = {}
    //::Rho::RHO.init_schema_sources(hash_migrate) 
}

void rho_syncclient_destroy()
{
    sync::CSyncThread::Destroy();
}

bool rhom_method_name_isreserved(const String& strName)
{
    static Hashtable<String,int> reserved_names;
    if ( reserved_names.size() == 0 )
    {
        reserved_names.put("object",1);
        reserved_names.put("source_id",1);
        reserved_names.put("update_type",1);
        reserved_names.put("attrib_type",1);
        reserved_names.put("set_notification",1);
        reserved_names.put("clear_notification",1);
    }

    return reserved_names.get(strName) != 0;
}

void db_insert_into_table( db::CDBAdapter& db, const String& table, Hashtable<String, String>& hashObject, const char* excludes = null)
{
    String cols = "";
    String quests = "";
    Vector<String> vals;

    for ( Hashtable<String,String>::iterator it = hashObject.begin();  it != hashObject.end(); ++it )
    {
        String key = it->first;
        String val  = it->second;

        if ( excludes && key.compare(excludes) == 0 )
            continue;

        if (cols.length() > 0)
        {
            cols += ',';
            quests += ',';
        }
    
        cols += key;
        quests += '?';
        vals.addElement( val );
    }

    String query = "insert into " + table + "(" + cols + ") values (" + quests + ")";

    db.executeSQLEx(query.c_str(), vals);
}

unsigned long rhom_make_object(rho::db::CDBResult& res1, int nSrcID, bool isSchemaSrc)
{
    unsigned long item = 0;
    if ( res1.isEnd() )
        return item;

    item = rho_syncclient_hash_create();
    rho_syncclient_hash_put(item, "source_id", convertToStringA(nSrcID).c_str() );

    if (!isSchemaSrc)
    {
        for ( ; !res1.isEnd(); res1.next() )
            rho_syncclient_hash_put(item, res1.getStringByIdx(0).c_str(), res1.getStringByIdx(1).c_str() );
    }else
    {
        for (int i = 0; i < res1.getColCount(); i++ )
            rho_syncclient_hash_put(item, res1.getColName(i).c_str(), res1.getStringByIdx(i).c_str() );
    }

    return item;
}

unsigned long rhom_load_item_by_object(db::CDBAdapter& db, const String& src_name, int nSrcID, const String& szObject, bool isSchemaSrc )
{
    unsigned long item = 0;

    if (!isSchemaSrc)
    {
        String sql = "SELECT attrib,value FROM object_values WHERE object=? AND source_id=?";
        DBResult( res1, db.executeSQL(sql.c_str(), szObject, nSrcID) );
        item = rhom_make_object(res1, nSrcID, isSchemaSrc);
        if (item)
            rho_syncclient_hash_put(item, "object", szObject.c_str() );
    }else
    {
        String sql = "SELECT * FROM " + src_name + " WHERE object=? OFFSET 0 LIMIT 1";
        DBResult( res1, db.executeSQL(sql.c_str(), szObject) );
        item = rhom_make_object(res1, nSrcID, isSchemaSrc);
    }

    return item;
}

unsigned long rho_syncclient_find(const char* szModel,const char* szObject )
{
    String src_name = szModel;

    DBResult( res, db::CDBAdapter::getUserDB().executeSQL("SELECT source_id, partition, schema, sync_type from sources WHERE name=?", src_name) );
    if ( res.isEnd())
    {
        //TODO: report error - unknown source
        return 0;
    }

    int nSrcID = res.getIntByIdx(0);
    String db_partition = res.getStringByIdx(1);
    bool isSchemaSrc = res.getStringByIdx(2).length() > 0;
    //String tableName = isSchemaSrc ? src_name : "object_values";
    db::CDBAdapter& db = db::CDBAdapter::getDB(db_partition.c_str());

    return rhom_load_item_by_object( db, src_name, nSrcID, szObject, isSchemaSrc);
}

unsigned long rhom_find(const char* szModel, unsigned long hash, int nCount )
{
    String src_name = szModel;

    DBResult( res, db::CDBAdapter::getUserDB().executeSQL("SELECT source_id, partition, schema, sync_type from sources WHERE name=?", src_name) );
    if ( res.isEnd())
    {
        //TODO: report error - unknown source
        return 0;
    }

    int nSrcID = res.getIntByIdx(0);
    String strSrcID = convertToStringA(nSrcID);
    String db_partition = res.getStringByIdx(1);
    bool isSchemaSrc = res.getStringByIdx(2).length() > 0;
    //String tableName = isSchemaSrc ? src_name : "object_values";
    db::CDBAdapter& db = db::CDBAdapter::getDB(db_partition.c_str());

    Hashtable<String, String>& hashCond = *((Hashtable<String, String>*)hash);
    String sql = "";
    Vector<String> arValues;

    if (!isSchemaSrc)
    {
        for ( Hashtable<String,String>::iterator it = hashCond.begin();  it != hashCond.end(); ++it )
        {
            if ( sql.length() > 0 )
                sql += "\nINTERSECT\n";

            sql += "SELECT object FROM object_values WHERE attrib=? AND source_id=? AND value=?";
            arValues.addElement(it->first); 
            arValues.addElement(strSrcID);
            arValues.addElement(it->second);
        }
    }else
    {
        for ( Hashtable<String,String>::iterator it = hashCond.begin();  it != hashCond.end(); ++it )
        {
            if (it != hashCond.begin())
                sql += " AND ";
            sql += it->first + "=?" ;
            arValues.addElement(it->second);
        }
    }

    DBResult( res1, db.executeSQLEx(sql.c_str(), arValues ) );

    if ( nCount == 1 )
    {
        return isSchemaSrc ? rhom_make_object(res1, nSrcID, isSchemaSrc) :
                 rhom_load_item_by_object(db, src_name, nSrcID, res1.getStringByIdx(0), isSchemaSrc);
    }

    unsigned long items = rho_syncclient_strhasharray_create();
    for ( ; !res1.isEnd(); res1.next() )
    {
        rho_syncclient_strhasharray_add(items, 
            isSchemaSrc ? rhom_make_object(res1, nSrcID, isSchemaSrc) :
                rhom_load_item_by_object(db, src_name, nSrcID, res1.getStringByIdx(0), isSchemaSrc) );
    }

    return items;
}

unsigned long rho_syncclient_find_all(const char* szModel, unsigned long hash )
{
    return rhom_find( szModel, hash, -1 );
}

unsigned long rho_syncclient_find_first(const char* szModel, unsigned long hash )
{
    return rhom_find( szModel, hash, 1 );
}

void rho_syncclient_itemdestroy( const char* szModel, unsigned long hash )
{
    Hashtable<String, String>& hashObject = *((Hashtable<String, String>*)hash);
    String src_name = szModel;

    String obj = hashObject.get("object");
    String update_type="delete";

    DBResult( res, db::CDBAdapter::getUserDB().executeSQL("SELECT source_id, partition, schema, sync_type from sources WHERE name=?", src_name) );
    if ( res.isEnd())
    {
        //TODO: report error - unknown source
        return;
    }

    int nSrcID = res.getIntByIdx(0);
    String db_partition = res.getStringByIdx(1);
    bool isSchemaSrc = res.getStringByIdx(2).length() > 0;
    bool isSyncSrc = res.getStringByIdx(3).compare("none") != 0;
    String tableName = isSchemaSrc ? src_name : "object_values";
    db::CDBAdapter& db = db::CDBAdapter::getDB(db_partition.c_str());

    db.startTransaction();

    //save list of attrs
    unsigned long item = 0;
    
    if ( isSchemaSrc )
    {
        DBResult( attrsList, db.executeSQL( ("SELECT * FROM " + tableName + " WHERE object=?").c_str(), obj) );
        if ( !attrsList.isEnd() )
            item = rhom_make_object(attrsList,nSrcID,isSchemaSrc);
    }else
    {
        DBResult( attrsList, db.executeSQL( ("SELECT attrib, value FROM " + tableName + " WHERE object=? and source_id=?").c_str(), obj, nSrcID) );
        if ( !attrsList.isEnd() )
            item = rhom_make_object(attrsList,nSrcID,isSchemaSrc);
    }

    //first delete the record from viewable list
    db.executeSQL( ("DELETE FROM " + tableName + " WHERE object=?").c_str(), obj );

    if ( isSyncSrc )
    {
        DBResult( resCreateType, db.executeSQL("SELECT update_type FROM changed_values WHERE object=? and update_type=? and sent=?",
            obj, "create", 0) );

        db.executeSQL("DELETE FROM changed_values WHERE object=? and sent=?", obj, 0);

        if ( resCreateType.isEnd() && item != 0 )
        {
            Hashtable<String,String>& hashItem = *((Hashtable<String,String>*)item);
            for ( Hashtable<String,String>::iterator it = hashItem.begin();  it != hashItem.end(); ++it )
            {
                String key = it->first;
                String val  = it->second;

                if ( rhom_method_name_isreserved(key) )
                    continue;

                Hashtable<String,String> fields;
                fields.put("source_id", convertToStringA(nSrcID));
                fields.put("object", obj);
                fields.put("attrib", key);
                fields.put("value", val);
                fields.put("update_type", update_type);

                db_insert_into_table(db, "changed_values", fields );
            }

            rho_syncclient_hash_delete(item);
        }
    }

    db.endTransaction();
}

void rho_syncclient_save( const char* szModel, unsigned long hash )
{
    Hashtable<String, String>& hashObject = *((Hashtable<String, String>*)hash);
    String src_name = szModel;

    DBResult( res, db::CDBAdapter::getUserDB().executeSQL("SELECT source_id, partition, schema, sync_type from sources WHERE name=?", src_name) );
    if ( res.isEnd())
    {
        //TODO: report error - unknown source
        return;
    }

    int nSrcID = res.getIntByIdx(0);
    String obj = hashObject.get("object");

    String db_partition = res.getStringByIdx(1);
    bool isSchemaSrc = res.getStringByIdx(2).length() > 0;
    bool isSyncSrc = res.getStringByIdx(3).compare("none") != 0;
    String tableName = isSchemaSrc ? src_name : "object_values";
    db::CDBAdapter& db = db::CDBAdapter::getDB(db_partition.c_str());

    db.startTransaction();

    String update_type = "";
    String sql;
    Vector<String> arValues;
    if (isSchemaSrc)
    {
        sql = "SELECT object FROM " + tableName + " WHERE object=? LIMIT 1 OFFSET 0";
        arValues.addElement(obj);
    }
    else
    {
        sql = "SELECT object FROM " + tableName + " WHERE object=? AND source_id=? LIMIT 1 OFFSET 0";
        arValues.addElement(obj);
        arValues.addElement(convertToStringA(nSrcID));
    }
    DBResult( res1, db.executeSQLEx(sql.c_str(), arValues ) );
    if (!res1.isEnd())
    {
        if (isSyncSrc)
        {
            DBResult( resUpdateType, db.executeSQL( "SELECT update_type FROM changed_values WHERE object=? and source_id=? and sent=?", 
                obj, nSrcID, 0 ) );
            if (!resUpdateType.isEnd()) 
                update_type = resUpdateType.getStringByIdx(0);
            else
	            update_type = "update";
        }else
            update_type = "update";
    }
    else
    {
        rho_syncclient_create_object(szModel, hash);
    }

    if (!res.isEnd())
    {
        unsigned long item = rhom_load_item_by_object( db, src_name, nSrcID, obj, isSchemaSrc);
        Hashtable<String, String>& hashItem = *((Hashtable<String, String>*)item);

        for ( Hashtable<String,String>::iterator it = hashObject.begin();  it != hashObject.end(); ++it )
        {
            String key = it->first;
            String val  = it->second;

            if ( rhom_method_name_isreserved(key) )
                continue;

            // add rows excluding object, source_id and update_type
            Hashtable<String,String> fields;
            fields.put("source_id", convertToStringA(nSrcID));
            fields.put("object", obj);
            fields.put("attrib", key);
            fields.put("value", val);
            fields.put("update_type", update_type);
            if ( db::CDBAdapter::getDB(db_partition.c_str()).getAttrMgr().isBlobAttr(nSrcID, key.c_str()) )
                fields.put( "attrib_type", "blob.file");

            if ( hashItem.containsKey(key) )
            {
                bool isModified = hashItem.get(key) != val;
                if (isModified)
                {
                    if (isSyncSrc)
                    {
                        DBResult( resUpdateType, db.executeSQL( "SELECT update_type FROM changed_values WHERE object=? and attrib=? and source_id=? and sent=?", 
                            obj, key, nSrcID, 0 ) );
                        if (!resUpdateType.isEnd()) 
                        {
                            fields.put("update_type", resUpdateType.getStringByIdx(0) );
                            db.executeSQL( "DELETE FROM changed_values WHERE object=? and attrib=? and source_id=? and sent=?", 
                                obj, key, nSrcID, 0 );
                        }

                        db_insert_into_table(db, "changed_values", fields);
                    }
                        
                    if ( isSchemaSrc )
                        db.executeSQL( (String("UPDATE ") + tableName + " SET " + key + "=? WHERE object=?").c_str(), val, obj );
                    else
                        db.executeSQL( "UPDATE object_values SET value=? WHERE object=? and attrib=? and source_id=?", val, obj, key, nSrcID );
                }

            }else
            {
                if (isSyncSrc)
                    db_insert_into_table(db, "changed_values", fields);

                fields.remove("update_type");
                fields.remove("attrib_type");
                
                if (isSchemaSrc)
                    db.executeSQL( (String("UPDATE ") + tableName + " SET " + key + "=? WHERE object=?").c_str(), val, obj );
                else
                    db_insert_into_table(db, tableName, fields);
            }
        }
    }

    db.endTransaction();
}

void rho_syncclient_create_object(const char* szModel, unsigned long hash)
{
    Hashtable<String, String>& hashObject = *((Hashtable<String, String>*)hash);
    String src_name = szModel;

    DBResult( res, db::CDBAdapter::getUserDB().executeSQL("SELECT source_id, partition, schema, sync_type from sources WHERE name=?", src_name) );
    if ( res.isEnd())
    {
        //TODO: report error - unknown source
        return;
    }

    String update_type = "create";
    int nSrcID = res.getIntByIdx(0);
    String obj = rhom_generate_id();

    String db_partition = res.getStringByIdx(1);
    bool isSchemaSrc = res.getStringByIdx(2).length() > 0;
    bool isSyncSrc = res.getStringByIdx(3).compare("none") != 0;
    String tableName = isSchemaSrc ? src_name : "object_values";
    db::CDBAdapter& db = db::CDBAdapter::getDB(db_partition.c_str());

    hashObject.put("source_id", convertToStringA(nSrcID));
    hashObject.put("object", obj);

    db.startTransaction();
    if ( isSchemaSrc )
        db_insert_into_table(db, tableName, hashObject, "source_id");
                        
    if ( isSyncSrc || !isSchemaSrc )
    {
        for ( Hashtable<String,String>::iterator it = hashObject.begin();  it != hashObject.end(); ++it )
        {
            String key = it->first;
            String val  = it->second;

            if ( rhom_method_name_isreserved(key) )
                continue;

            // add rows excluding object, source_id and update_type
            Hashtable<String,String> fields;
            fields.put("source_id", convertToStringA(nSrcID));
            fields.put("object", obj);
            fields.put("attrib", key);
            fields.put("value", val);
            fields.put("update_type", update_type);

            if ( db::CDBAdapter::getDB(db_partition.c_str()).getAttrMgr().isBlobAttr(nSrcID, key.c_str()) )
                fields.put( "attrib_type", "blob.file");

            if ( isSyncSrc )
                db_insert_into_table(db, "changed_values", fields);

            fields.remove("update_type");
            fields.remove("attrib_type");

            if ( !isSchemaSrc )
                db_insert_into_table(db, tableName, fields);
        }
    }
                        
    db.endTransaction();
}

void rho_syncclient_parsenotify(const char* msg, RHO_SYNC_NOTIFY* pNotify)
{
    CTokenizer oTokenizer( msg, "&" );
    int nLastPos = 0;
    while (oTokenizer.hasMoreTokens()) 
    {
        nLastPos = oTokenizer.getCurPos();

	    String tok = oTokenizer.nextToken();
	    if (tok.length() == 0)
		    continue;

        CTokenizer oValueTok( tok, "=" );
        String name = oValueTok.nextToken();
        String value = oValueTok.nextToken();

        if ( name.compare("total_count") == 0)
            convertFromStringA( value.c_str(), pNotify->total_count );
        else if ( name.compare("processed_count") == 0)
            convertFromStringA( value.c_str(), pNotify->processed_count );
        else if ( name.compare("cumulative_count") == 0)
            convertFromStringA( value.c_str(), pNotify->cumulative_count );
        else if ( name.compare("source_id") == 0)
            convertFromStringA( value.c_str(), pNotify->source_id );
        else if ( name.compare("error_code") == 0)
            convertFromStringA( value.c_str(), pNotify->error_code );
        else if ( name.compare("source_name") == 0)
            pNotify->source_name = strdup(value.c_str());
        else if ( name.compare("sync_type") == 0)
            pNotify->sync_type = strdup(value.c_str());
        else if ( name.compare("status") == 0)
            pNotify->status = strdup(value.c_str());
        else if ( name.compare("error_message") == 0)
            pNotify->error_message = strdup(value.c_str());
        else if ( name.compare("rho_callback") != 0)
            break;

        nLastPos = oTokenizer.getCurPos();
    }

    if ( nLastPos < (int)strlen(msg) )
        pNotify->callback_params = strdup(msg+nLastPos);
}

unsigned long rho_syncclient_strarray_create()
{
    return (unsigned long)(new rho::Vector<rho::String>());
}

void rho_syncclient_strarray_add(unsigned long ar, const char* szStr)
{
    rho::Vector<rho::String>& arThis = *((rho::Vector<rho::String>*)ar);
    arThis.addElement(szStr);
}

void rho_syncclient_strarray_delete(unsigned long ar)
{
    if (ar)
        delete ((rho::Vector<rho::String>*)ar);
}

unsigned long rho_syncclient_strhasharray_create()
{
    return (unsigned long)(new VectorPtr<Hashtable<String, String>* >());
}

void rho_syncclient_strhasharray_add(unsigned long ar, unsigned long hash)
{
    VectorPtr<Hashtable<String, String>* >& arThis = *((VectorPtr<Hashtable<String, String>* >*)ar);
    arThis.addElement( (Hashtable<String, String>*)hash );
}

void rho_syncclient_strhasharray_delete(unsigned long ar)
{
    if (ar)
        delete ((VectorPtr<Hashtable<String, String>* >*)ar);
}

int rho_syncclient_strhasharray_size(unsigned long ar)
{
    VectorPtr<Hashtable<String, String>* >& arThis = *((VectorPtr<Hashtable<String, String>* >*)ar);
    return arThis.size();
}

unsigned long rho_syncclient_strhasharray_get(unsigned long ar, int nIndex)
{
    VectorPtr<Hashtable<String, String>* >& arThis = *((VectorPtr<Hashtable<String, String>* >*)ar);
    return (unsigned long) arThis.elementAt(nIndex);
}

unsigned long rho_syncclient_hash_create()
{
    return (unsigned long)(new rho::Hashtable<rho::String, rho::String>());
}

void rho_syncclient_hash_put(unsigned long hash, const char* szKey, const char* szValue)
{
    Hashtable<String, String>& hashThis = *((Hashtable<String, String>*)hash);
    hashThis.put(szKey, szValue);
}

void rho_syncclient_hash_delete(unsigned long hash)
{
    if (hash)
        delete ((rho::Hashtable<rho::String, rho::String>*)hash);
}

const char* rho_syncclient_hash_get(unsigned long hash, const char* szKey)
{
    Hashtable<String, String>& hashThis = *((Hashtable<String, String>*)hash);

    if ( hashThis.containsKey(szKey) )
        return hashThis[szKey].c_str();

    return null;
}

int rho_syncclient_hash_equal(unsigned long hash1, unsigned long hash2)
{
    Hashtable<String, String>& hashThis1 = *((Hashtable<String, String>*)hash1);    
    Hashtable<String, String>& hashThis2 = *((Hashtable<String, String>*)hash2);    

    return hashThis1 == hashThis2 ? 1 : 0;
}

}

String rhom_generate_id()
{
    static uint64 g_base_temp_id = 0;
    if (  g_base_temp_id  == 0 )
        g_base_temp_id = CLocalTime().toULong();

    g_base_temp_id ++;
    return convertToStringA(g_base_temp_id);
}

String getSyncTypeName( RHOM_SYNC_TYPE sync_type )
{
    switch( sync_type)
    {
    case RST_INCREMENTAL:
        return "incremental";
    case RST_BULK_ONLY:
        return "bulk_only";
    }

    return "none";
}

extern "C" 
{
extern "C" void alert_show_popup(const char* message)
{

}

void rho_ruby_loadserversources(const char* szData)
{
    //TODO: loadserversources     : see rho.rb
}

const char* rho_ruby_getMessageText(const char* szName)
{
    return szName;
}

const char* rho_ruby_getErrorText(int nError)
{
    return "";
}

char* RhoRuby_getRhoDBVersion()
{
    return "1.0";
}

}
