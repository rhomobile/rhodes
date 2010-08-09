#include "SyncClient.h"

#include "stdafx.h"

#include "common/RhodesAppBase.h"
#include "sync/SyncThread.h"
#include "common/RhoFile.h"
#include "common/Tokenizer.h"
#include "common/RhoConf.h"
#include "common/RhoError.h"

using namespace rho;
using namespace rho::common;
String getSyncTypeName( RHOM_SYNC_TYPE sync_type );
void parseSyncNotify(const char* msg, RHO_SYNC_NOTIFY* pNotify);

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
