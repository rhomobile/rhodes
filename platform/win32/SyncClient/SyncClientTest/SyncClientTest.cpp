#include "stdafx.h"

#include "SyncClient/SyncClient.h"
#include "sync/SyncThread.h"
#include "common/RhoConf.h"
#include "common/RhoError.h"
#include "common/RhoTime.h"

#include "gtest/gtest.h"
#include "test/test_helper.h"

using namespace rho;
using namespace rho::common;

int _tmain(int argc, _TCHAR* argv[])
{
    RHOM_MODEL models[2] = {0};
    rho_syncclient_initmodel(&models[0]);
    models[0].name = "Customer";
    models[0].sync_type = RST_INCREMENTAL;

    rho_syncclient_initmodel(&models[1]);
    models[1].name = "Product";
    models[1].sync_type = RST_INCREMENTAL;

    rho_syncclient_init(models, 2);

    rho_sync_set_threaded_mode(0);
    rho_sync_set_pollinterval(0);
//run tests
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
//

    rho_syncclient_destroy();

	return 0;
}

TEST(SyncClient, ResetAndLogout) 
{
    rho_sync_set_syncserver("http://rhodes-store-server.heroku.com/application");

    rho_syncclient_database_full_reset_and_logout();

    EXPECT_EQ(rho_sync_logged_in(), 0);
}

TEST(SyncClient, UpdateSyncServer) 
{
    char* saveSrv = rho_conf_getString("syncserver");
    rho_sync_set_syncserver("http://example.com/sources/");
    char* newSrv = rho_conf_getString("syncserver");
    EXPECT_EQ(String(newSrv), "http://example.com/sources/");
    rho_conf_freeString(newSrv);

    rho_sync_set_syncserver(saveSrv);
    newSrv = rho_conf_getString("syncserver");
    EXPECT_EQ(String(newSrv), saveSrv);
    rho_conf_freeString(saveSrv);
}

TEST(SyncClient, shouldNotSyncWithoutLogin) 
{
    EXPECT_EQ(rho_sync_logged_in(), 0);

    char* szRes = (char*)rho_sync_doSyncSourceByName("Product");
    RHO_SYNC_NOTIFY oNotify = {0};
    rho_syncclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "error");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_CLIENTISNOTLOGGEDIN);

    rho_syncclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);
}

TEST(SyncClient, shouldLogin) 
{
    EXPECT_EQ(rho_sync_logged_in(), 0);

    char* szRes = (char*)rho_sync_login("lars", "larspass", "");
    RHO_SYNC_NOTIFY oNotify = {0};
    rho_syncclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);
    EXPECT_EQ(rho_sync_logged_in(), 1);

    rho_syncclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);

}

TEST(SyncClient, shouldSyncProductByName) 
{
    EXPECT_EQ(rho_sync_logged_in(), 1);

    char* szRes = (char*)rho_sync_doSyncSourceByName("Product");
    RHO_SYNC_NOTIFY oNotify = {0};
    rho_syncclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "ok");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);

    rho_syncclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);

}

TEST(SyncClient, shouldSearchProduct) 
{
    EXPECT_EQ(rho_sync_logged_in(), 1);

    unsigned long ar_sources = rho_syncclient_strarray_create();
    rho_syncclient_strarray_add(ar_sources, "Product");

    String strParams = "offset=0&max_results=10&filterData=Test&search_id=" + 
        convertToStringA( CLocalTime().toULong() );
    char* szRes = (char*)rho_sync_doSearchByNames( ar_sources, "search", 
        strParams.c_str(), false, 0, "", "" );

    rho_syncclient_strarray_delete(ar_sources);

    RHO_SYNC_NOTIFY oNotify = {0};
    rho_syncclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "complete");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);

    rho_syncclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);

}

TEST(SyncClient, shouldCreateNewProduct) 
{
    EXPECT_EQ(rho_sync_logged_in(), 1);

    unsigned long item = rho_syncclient_hash_create();
    rho_syncclient_hash_put(item, "name", "Test");
    rho_syncclient_create_object("Product", item);

    unsigned long item2 = rho_syncclient_find("Product", rho_syncclient_hash_get(item, "object") );
    EXPECT_EQ( rho_syncclient_hash_equal(item2, item), 1 );

    char* szRes = (char*)rho_sync_doSyncSourceByName("Product");
    RHO_SYNC_NOTIFY oNotify = {0};
    rho_syncclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "ok");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);

    unsigned long item3 = rho_syncclient_find("Product", rho_syncclient_hash_get(item, "object") );
    EXPECT_EQ( item3, 0 );

    rho_syncclient_hash_delete(item);
    rho_syncclient_hash_delete(item2);

    rho_syncclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);
}

TEST(SyncClient, shouldModifyProduct) 
{
    EXPECT_EQ(rho_sync_logged_in(), 1);

    unsigned long cond = rho_syncclient_hash_create();
    rho_syncclient_hash_put(cond, "name", "Test");

    unsigned long item = rho_syncclient_find_first("Product", cond );
    rho_syncclient_hash_delete(cond);

    EXPECT_NE( item, 0 );
    String saved_obj = rho_syncclient_hash_get(item, "object");

    const char* szSku = rho_syncclient_hash_get(item, "sku");
    String new_sku = szSku ? szSku : "";
    new_sku += "_TEST";

    rho_syncclient_hash_put(item, "sku", new_sku.c_str());
    rho_syncclient_save( "Product", item );

    char* szRes = (char*)rho_sync_doSyncSourceByName("Product");
    RHO_SYNC_NOTIFY oNotify = {0};
    rho_syncclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "ok");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);

    unsigned long item3 = rho_syncclient_find("Product", saved_obj.c_str() );
    //EXPECT_EQ( rho_syncclient_hash_equal(item3, item), 1 );

    rho_syncclient_hash_delete(item);
    rho_syncclient_hash_delete(item3);

    rho_syncclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);

}

TEST(SyncClient, shouldDeleteAllTestProduct) 
{
    EXPECT_EQ(rho_sync_logged_in(), 1);

    unsigned long cond = rho_syncclient_hash_create();
    rho_syncclient_hash_put(cond, "name", "Test");

    unsigned long items = rho_syncclient_find_all("Product", cond );
    EXPECT_NE( items, 0 );
  
    for ( int i = 0; i < rho_syncclient_strhasharray_size(items); i++ )
    {
        rho_syncclient_itemdestroy("Product", rho_syncclient_strhasharray_get(items, i));
    }

    char* szRes = (char*)rho_sync_doSyncSourceByName("Product");
    RHO_SYNC_NOTIFY oNotify = {0};
    rho_syncclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "ok");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);

    unsigned long item2 = rho_syncclient_find_first("Product", cond );
    EXPECT_EQ(item2, 0);

    rho_syncclient_hash_delete(cond);
    rho_syncclient_strhasharray_delete(items);
}
