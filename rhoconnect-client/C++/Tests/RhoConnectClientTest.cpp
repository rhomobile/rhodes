/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_NON_CONFORMING_SWPRINTFS 1

#ifdef WIN32
#include <stdio.h>
#include <tchar.h>
#endif //WIN32

#include "RhoConnectClient/RhoConnectClient.h"
#include "sync/SyncThread.h"
#include "common/RhoConf.h"
#include "RhoConnectClient/RhoError.h"
#include "common/RhoTime.h"

#include "gtest/gtest.h"
#include "test/test_helper.h"

using namespace rho;
using namespace rho::common;
const char* g_szProduct = "Product";
const char* g_szCustomer = "Customer";
String g_product_test_name;
int m_nProductSrcID = 0;

#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
{
#else
extern "C" int runSyncClientTests()
{
    int argc = 0;
	char** argv = 0;
#endif

    RHOM_MODEL models[2] = {0};
    rho_connectclient_initmodel(&models[0]);
    models[0].name = g_szProduct;
    //models[0].type = RMT_PROPERTY_FIXEDSCHEMA;
    //For schema model - add this create to rhoconnect-client\C++\Tests\win32\bin\Win32\RhoConnectClientTest\Debug\rho\db\syncdb.schema:
    /*
    CREATE TABLE "Product_s" ( 
        "brand" varchar default null,
        "created_at" varchar default null,
        "name" varchar default null,
        "price" varchar default null,
        "quantity" varchar default null,
        "sku" varchar default null,
        "updated_at" varchar default null,
        "object" varchar(255) PRIMARY KEY );
    */
    models[0].sync_type = RST_INCREMENTAL;

    models[0].associations = rho_connectclient_hash_create();
    rho_connectclient_hash_put( models[0].associations, "quantity", g_szCustomer );
    rho_connectclient_hash_put( models[0].associations, "sku", g_szCustomer );

    rho_connectclient_initmodel(&models[1]);
    models[1].name = g_szCustomer;
    models[1].sync_type = RST_INCREMENTAL;

    rho_connectclient_init(models, 2);

    rho_sync_set_threaded_mode(0);
    rho_sync_set_pollinterval(0);
    rho_logconf_setSeverity(1);

    m_nProductSrcID = models[0].source_id;
//run tests
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
//

    rho_connectclient_destroy();

	return 0;
}

TEST(SyncClient, shouldFindProductBySql) 
{
    unsigned long items = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM sources", 0 );
    EXPECT_NE( items, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(items), 2 );
    rho_connectclient_strhasharray_delete(items);


    unsigned long params = rho_connectclient_strarray_create();
    rho_connectclient_strarray_add(params, g_szProduct);

    unsigned long items2 = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM sources WHERE name=?", params );
    EXPECT_NE( items2, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(items2), 1 );

    rho_connectclient_strarray_delete(params);
    rho_connectclient_strhasharray_delete(items2);

}

TEST(SyncClient, ResetAndLogout) 
{
    rho_sync_set_syncserver("http://rhodes-store-server.heroku.com/application");

    rho_connectclient_database_full_reset_and_logout();

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

    char* szRes = (char*)rho_sync_doSyncSourceByName(g_szProduct);
    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "error");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_CLIENTISNOTLOGGEDIN);

    rho_connectclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);
}

TEST(SyncClient, shouldLogin) 
{
    EXPECT_EQ(rho_sync_logged_in(), 0);

    char* szRes = (char*)rho_sync_login("lars", "larspass", "");
    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);
    EXPECT_EQ(rho_sync_logged_in(), 1);

    rho_connectclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);

}

TEST(SyncClient, shouldSyncProductByName) 
{
    EXPECT_EQ(rho_sync_logged_in(), 1);

    char* szRes = (char*)rho_sync_doSyncSourceByName(g_szProduct);
    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "ok");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);

    unsigned long cond = rho_connectclient_hash_create();
    unsigned long items = rho_connectclient_find_all(g_szProduct, cond );
    EXPECT_NE( rho_connectclient_strhasharray_size(items), 0 );

    rho_connectclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);

}

TEST(SyncClient, shouldSearchProduct) 
{
    EXPECT_EQ(rho_sync_logged_in(), 1);

    unsigned long ar_sources = rho_connectclient_strarray_create();
    rho_connectclient_strarray_add(ar_sources, g_szProduct);

    String strParams = "offset=0&max_results=10&filterData=Test&search_id=" + 
        convertToStringA( CLocalTime().toULong() );
    char* szRes = (char*)rho_sync_doSearchByNames( ar_sources, "search", 
        strParams.c_str(), false, 0, "", "" );

    rho_connectclient_strarray_delete(ar_sources);

    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "complete");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);

    rho_connectclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);

}

TEST(SyncClient, shouldCreateNewProduct) 
{
    EXPECT_EQ(rho_sync_logged_in(), 1);

    unsigned long item = rho_connectclient_hash_create();
    rho_connectclient_hash_put(item, "name", "Test");
    rho_connectclient_create_object(g_szProduct, item);

    unsigned long item2 = rho_connectclient_find(g_szProduct, rho_connectclient_hash_get(item, "object") );
    EXPECT_EQ( rho_connectclient_hash_equal(item2, item), 1 );

    EXPECT_EQ( rho_connectclient_is_changed(g_szProduct), 1 );

    char* szRes = (char*)rho_sync_doSyncSourceByName(g_szProduct);
    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "ok");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);

    unsigned long item3 = rho_connectclient_find(g_szProduct, rho_connectclient_hash_get(item, "object") );
    EXPECT_EQ( item3, 0 );

    EXPECT_EQ( rho_connectclient_is_changed(g_szProduct), 0 );

    rho_connectclient_hash_delete(item);
    rho_connectclient_hash_delete(item2);
    rho_connectclient_hash_delete(item3);

    rho_connectclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);
}

TEST(SyncClient, shouldCreateNewProductWithCustomers) 
{
    EXPECT_EQ(rho_sync_logged_in(), 1);

    unsigned long cust1 = rho_connectclient_hash_create();
    rho_connectclient_hash_put(cust1, "first", "CustTest1");
    rho_connectclient_create_object(g_szCustomer, cust1);

    unsigned long cust2 = rho_connectclient_hash_create();
    rho_connectclient_hash_put(cust2, "first", "CustTest2");
    rho_connectclient_create_object(g_szCustomer, cust2);

    g_product_test_name = CLocalTime().toString(true, true);
    unsigned long item = rho_connectclient_hash_create();
    rho_connectclient_hash_put(item, "name", g_product_test_name.c_str());
    rho_connectclient_hash_put(item, "quantity", rho_connectclient_hash_get(cust1, "object"));
    rho_connectclient_hash_put(item, "sku", rho_connectclient_hash_get(cust2, "object"));
    rho_connectclient_create_object(g_szProduct, item);

    char* szRes = (char*)rho_sync_doSyncAllSources(0,0);
    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "complete");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);
    rho_connectclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);

    unsigned long prodCond = rho_connectclient_hash_create();
    rho_connectclient_hash_put(prodCond, "name", g_product_test_name.c_str());
    unsigned long prod = rho_connectclient_find_first(g_szProduct, prodCond );
    rho_connectclient_hash_delete(prodCond);

    EXPECT_NE( prod, 0 );
    EXPECT_NE( String(rho_connectclient_hash_get(prod, "object")), rho_connectclient_hash_get(item, "object") );
    EXPECT_NE( String(rho_connectclient_hash_get(prod, "quantity")), rho_connectclient_hash_get(cust1, "object") );
    EXPECT_NE( String(rho_connectclient_hash_get(prod, "sku")), rho_connectclient_hash_get(cust2, "object") );

    unsigned long cust11 = rho_connectclient_find(g_szCustomer, rho_connectclient_hash_get(prod, "quantity") );
    EXPECT_NE( cust11, 0 );
    EXPECT_EQ( String(rho_connectclient_hash_get(cust11, "first")), rho_connectclient_hash_get(cust1, "first") );

    unsigned long cust22 = rho_connectclient_find(g_szCustomer, rho_connectclient_hash_get(prod, "sku") );
    EXPECT_NE( cust22, 0 );
    EXPECT_EQ( String(rho_connectclient_hash_get(cust22, "first")), rho_connectclient_hash_get(cust2, "first") );

    rho_connectclient_hash_delete(cust22);
    rho_connectclient_hash_delete(cust11);
    rho_connectclient_hash_delete(cust1);
    rho_connectclient_hash_delete(cust2);
    rho_connectclient_hash_delete(item);
    rho_connectclient_hash_delete(prod);
}

TEST(SyncClient, shouldModifyProduct) 
{
    EXPECT_EQ(rho_sync_logged_in(), 1);

    unsigned long cond = rho_connectclient_hash_create();
    rho_connectclient_hash_put(cond, "name", "Test");

    unsigned long item = rho_connectclient_find_first(g_szProduct, cond );
    rho_connectclient_hash_delete(cond);

    EXPECT_NE( item, 0 );
    String saved_obj = rho_connectclient_hash_get(item, "object");

    const char* szSku = rho_connectclient_hash_get(item, "sku");
    String new_sku = szSku ? szSku : "";
    new_sku += "_TEST";

    rho_connectclient_hash_put(item, "sku", new_sku.c_str());
    rho_connectclient_save( g_szProduct, item );

    char* szRes = (char*)rho_sync_doSyncSourceByName(g_szProduct);
    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "ok");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);

    unsigned long item3 = rho_connectclient_find(g_szProduct, saved_obj.c_str() );
    //EXPECT_EQ( rho_connectclient_hash_equal(item3, item), 1 );

    rho_connectclient_hash_delete(item);
    rho_connectclient_hash_delete(item3);

    rho_connectclient_free_syncnotify(&oNotify);
    rho_sync_free_string(szRes);

}

unsigned long beforeProcessCreateError1(RHO_CONNECT_NOTIFY& oNotify)
{
    unsigned long item = rho_connectclient_hash_create();
    rho_connectclient_hash_put(item, "name", "Test");
    rho_connectclient_hash_put(item, "brand", "Rho");
    rho_connectclient_create_object(g_szProduct, item);

    unsigned long records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='create'", 0 );
    EXPECT_NE( records, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 1 );
    unsigned long rec0 = rho_connectclient_strhasharray_get(records, 0);
    EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "attrib")), "object");
    rho_connectclient_strhasharray_delete(records);

    rho_connectclient_hash_put(item, "price", "123");
    rho_connectclient_save( g_szProduct, item );

    records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='create'", 0 );
    EXPECT_NE( records, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 1 );
    rec0 = rho_connectclient_strhasharray_get(records, 0);
    EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "attrib")), "object");
    rho_connectclient_strhasharray_delete(records);

    String err_resp = String("[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"create-error\":{\"") + rho_connectclient_hash_get(item,"object") + "\":{\"name\":\"wrongname\",\"an_attribute\":\"error create\"},\"" + rho_connectclient_hash_get(item,"object") + "-error\":{\"message\":\"error create\"}}}]";
    int nSrcID = 0;
    convertFromStringA(rho_connectclient_hash_get(item,"source_id"), nSrcID );
    rho_sync_set_source_property( nSrcID, "rho_server_response", err_resp.c_str() );

    char* szRes = (char*)rho_sync_doSyncSourceByName(g_szProduct);
    
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "error");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_CUSTOMSYNCSERVER);
    EXPECT_NE(oNotify.create_errors, 0);
    EXPECT_NE( rho_connectclient_hash_get(oNotify.create_errors, rho_connectclient_hash_get(item,"object")), (char*)0);

    records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='create'", 0 );
    EXPECT_NE( records, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 3 );
    rec0 = rho_connectclient_strhasharray_get(records, 0);
    EXPECT_NE( String(rho_connectclient_hash_get(rec0, "attrib")), "object");
    EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "sent")), "2");
    rho_connectclient_strhasharray_delete(records);

    return item;
}

unsigned long beforeProcessCreateError(RHO_CONNECT_NOTIFY& oNotify)
{
    unsigned long item = beforeProcessCreateError1(oNotify);

    rho_connectclient_hash_put(item, "price", "456");
    rho_connectclient_save( g_szProduct, item );

    unsigned long records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='create'", 0 );
    EXPECT_NE( records, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 3 );
    unsigned long rec0 = rho_connectclient_strhasharray_get(records, 0);
    EXPECT_NE( String(rho_connectclient_hash_get(rec0, "attrib")), "object");
    EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "sent")), "2");
    rho_connectclient_strhasharray_delete(records);

    records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='update'", 0 );
    EXPECT_NE( records, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 1 );
    rec0 = rho_connectclient_strhasharray_get(records, 0);
    EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "attrib")), "price");
    EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "sent")), "0");
    rho_connectclient_strhasharray_delete(records);

    return item;
}

TEST(SyncClient, shouldProcessCreateError_Delete) 
{
    RHO_CONNECT_NOTIFY oNotify = {0};
    unsigned long item = beforeProcessCreateError(oNotify);

    rho_connectclient_on_sync_create_error(g_szProduct, &oNotify, "delete" );

    unsigned long params = rho_connectclient_strarray_create();
    rho_connectclient_strarray_add(params, rho_connectclient_hash_get(item,"object"));
    unsigned long records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE object=?", params );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 0 );

    records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='create'", 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 0 );

    item = rho_connectclient_find(g_szProduct, rho_connectclient_hash_get(item,"object") );
    EXPECT_EQ(item, 0 );

    rho_connectclient_findbysql(g_szProduct, "DELETE FROM changed_values", 0 );
}

TEST(SyncClient, shouldProcessCreateError_Recreate) 
{
    RHO_CONNECT_NOTIFY oNotify = {0};
    unsigned long item = beforeProcessCreateError(oNotify);

    rho_connectclient_on_sync_create_error(g_szProduct, &oNotify, "recreate" );

    unsigned long params = rho_connectclient_strarray_create();
    rho_connectclient_strarray_add(params, rho_connectclient_hash_get(item,"object"));
    unsigned long records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE object=?", params );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 1 );
    unsigned long rec0 = rho_connectclient_strhasharray_get(records, 0);
    EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "attrib")), "object");
    rho_connectclient_strhasharray_delete(records);

    records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='update'", 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 0 );

    item = rho_connectclient_find(g_szProduct, rho_connectclient_hash_get(item,"object") );
    EXPECT_NE(item, 0 );

    rho_connectclient_findbysql(g_szProduct, "DELETE FROM changed_values", 0 );
}

TEST(SyncClient, shouldProcessCreateError_RecreateDeleted) 
{
    RHO_CONNECT_NOTIFY oNotify = {0};
    unsigned long item = beforeProcessCreateError1(oNotify);

    rho_connectclient_itemdestroy(g_szProduct, item);

    unsigned long records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='create'", 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 3 );
    unsigned long rec0 = rho_connectclient_strhasharray_get(records, 0);
    EXPECT_NE( String(rho_connectclient_hash_get(rec0, "attrib")), "object");
    EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "sent")), "2");
    rho_connectclient_strhasharray_delete(records);

    records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='delete'", 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 3 ); //7 for schema
    rec0 = rho_connectclient_strhasharray_get(records, 0);
    EXPECT_NE( String(rho_connectclient_hash_get(rec0, "attrib")), "object");
    EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "sent")), "0");
    rho_connectclient_strhasharray_delete(records);

    rho_connectclient_on_sync_create_error(g_szProduct, &oNotify, "recreate" );

    unsigned long params = rho_connectclient_strarray_create();
    rho_connectclient_strarray_add(params, rho_connectclient_hash_get(item,"object"));
    records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE object=?", params );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 0 );
    rho_connectclient_strhasharray_delete(records);

    records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='update'", 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 0 );

    item = rho_connectclient_find(g_szProduct, rho_connectclient_hash_get(item,"object") );
    EXPECT_EQ(item, 0 );

    rho_connectclient_findbysql(g_szProduct, "DELETE FROM changed_values", 0 );
}

TEST(SyncClient, shouldProcessUpdateError_Retry) 
{
    String err_resp = "[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"update-error\":{\"broken_object_id\":{\"name\":\"wrongname\",\"an_attribute\":\"error update\"},\"broken_object_id-error\":{\"message\":\"error update\"}}}]";
    rho_sync_set_source_property( m_nProductSrcID, "rho_server_response", err_resp.c_str() );

    char* szRes = (char*)rho_sync_doSyncSourceByName(g_szProduct);

    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "error");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_CUSTOMSYNCSERVER);
    EXPECT_NE(oNotify.update_errors_obj, 0);
    EXPECT_NE( rho_connectclient_strarray_find(oNotify.update_errors_obj, "broken_object_id"), -1);

    unsigned long records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='update'", 0 );
    EXPECT_NE( records, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 0 );

    rho_connectclient_on_sync_update_error(g_szProduct, &oNotify, "retry" );

    records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='update'", 0 );
    EXPECT_NE( records, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 2 );
    unsigned long rec0 = rho_connectclient_strhasharray_get(records, 0);
    unsigned long rec1 = rho_connectclient_strhasharray_get(records, 1);
    if ( strcmp(rho_connectclient_hash_get(rec0, "attrib"), "name") == 0)
    {
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "object")), "broken_object_id");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "attrib")), "name");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "value")), "wrongname");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "update_type")), "update");
    }else
    {
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "object")), "broken_object_id");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "attrib")), "name");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "value")), "wrongname");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "update_type")), "update");
    }
    if ( strcmp(rho_connectclient_hash_get(rec1, "attrib"), "an_attribute") == 0)
    {
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "object")), "broken_object_id");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "attrib")), "an_attribute");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "value")), "error update");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "update_type")), "update");
    }else
    {
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "object")), "broken_object_id");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "attrib")), "an_attribute");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "value")), "error update");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "update_type")), "update");
    }

    rho_connectclient_strhasharray_delete(records);

    rho_connectclient_findbysql(g_szProduct, "DELETE FROM changed_values", 0 );
}

TEST(SyncClient, shouldProcessUpdateError_Rollback) 
{
    unsigned long cond = rho_connectclient_hash_create();
    unsigned long item = rho_connectclient_find_first( g_szProduct, cond );
    EXPECT_NE(item, 0 );
    String obj_id = rho_connectclient_hash_get(item,"object");
    String prod_name = rho_connectclient_hash_get(item,"name");

    String err_resp = String("[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"update-rollback\": {\"") + obj_id + "\": {\"name\": \"OLD_NAME\"}},\"update-error\":{\"" + obj_id + "\":{\"name\":\"wrongname\",\"an_attribute\":\"error update\"},\"" + obj_id + "-error\":{\"message\":\"error update\"}}}]";
    rho_sync_set_source_property( m_nProductSrcID, "rho_server_response", err_resp.c_str() );

    char* szRes = (char*)rho_sync_doSyncSourceByName(g_szProduct);

    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "error");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_CUSTOMSYNCSERVER);
    EXPECT_NE(oNotify.update_errors_obj, 0);
    EXPECT_NE( rho_connectclient_strarray_find(oNotify.update_errors_obj, obj_id.c_str()), -1);

    EXPECT_NE(oNotify.update_rollback_obj, 0);

    unsigned long records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='update'", 0 );
    EXPECT_NE( records, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 0 );
    rho_connectclient_strhasharray_delete(records);

    item = rho_connectclient_find(g_szProduct, obj_id.c_str() );
    EXPECT_NE(item, 0 );
    EXPECT_EQ(rho_connectclient_hash_get(item,"name"), prod_name );

    rho_connectclient_on_sync_update_error(g_szProduct, &oNotify, "rollback" );

    records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='update'", 0 );
    EXPECT_NE( records, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 0 );
    rho_connectclient_strhasharray_delete(records);

    item = rho_connectclient_find(g_szProduct, obj_id.c_str() );
    EXPECT_NE(item, 0 );
    EXPECT_EQ( String(rho_connectclient_hash_get(item,"name")), "OLD_NAME" );

    rho_connectclient_findbysql(g_szProduct, "DELETE FROM changed_values", 0 );
}

TEST(SyncClient, shouldProcessDeleteError_Retry) 
{
    String err_resp = "[{\"version\":3},{\"token\":\"\"},{\"count\":0},{\"progress_count\":0},{\"total_count\":0},{\"delete-error\":{\"broken_object_id\":{\"name\":\"wrongname\",\"an_attribute\":\"error delete\"},\"broken_object_id-error\":{\"message\":\"Error delete record\"}}}]";
    rho_sync_set_source_property( m_nProductSrcID, "rho_server_response", err_resp.c_str() );

    char* szRes = (char*)rho_sync_doSyncSourceByName(g_szProduct);

    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "error");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_CUSTOMSYNCSERVER);
    EXPECT_NE(oNotify.delete_errors_obj, 0);
    EXPECT_NE( rho_connectclient_strarray_find(oNotify.delete_errors_obj, "broken_object_id"), -1);

    unsigned long records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='delete'", 0 );
    EXPECT_NE( records, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 0 );
    rho_connectclient_strhasharray_delete(records);

    rho_connectclient_on_sync_delete_error(g_szProduct, &oNotify, "retry" );

    records = rho_connectclient_findbysql(g_szProduct, "SELECT * FROM changed_values WHERE update_type='delete'", 0 );
    EXPECT_NE( records, 0 );
    EXPECT_EQ(rho_connectclient_strhasharray_size(records), 2 );
    unsigned long rec0 = rho_connectclient_strhasharray_get(records, 0);
    unsigned long rec1 = rho_connectclient_strhasharray_get(records, 1);
    if ( strcmp(rho_connectclient_hash_get(rec0, "attrib"), "name") == 0)
    {
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "object")), "broken_object_id");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "attrib")), "name");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "value")), "wrongname");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "update_type")), "delete");
    }else
    {
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "object")), "broken_object_id");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "attrib")), "name");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "value")), "wrongname");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "update_type")), "delete");
    }
    if ( strcmp(rho_connectclient_hash_get(rec1, "attrib"), "an_attribute") == 0)
    {
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "object")), "broken_object_id");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "attrib")), "an_attribute");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "value")), "error delete");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec1, "update_type")), "delete");
    }else
    {
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "object")), "broken_object_id");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "attrib")), "an_attribute");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "value")), "error delete");
        EXPECT_EQ( String(rho_connectclient_hash_get(rec0, "update_type")), "delete");
    }

    rho_connectclient_strhasharray_delete(records);
    rho_connectclient_findbysql(g_szProduct, "DELETE FROM changed_values", 0 );

    rho_sync_set_source_property( m_nProductSrcID, "rho_server_response", "" );
}

TEST(SyncClient, shouldDeleteAllTestProduct) 
{
    EXPECT_EQ(rho_sync_logged_in(), 1);

    unsigned long cond = rho_connectclient_hash_create();
    rho_connectclient_hash_put(cond, "name", "Test");

    unsigned long items = rho_connectclient_find_all(g_szProduct, cond );
    EXPECT_NE( items, 0 );
  
    for ( int i = 0; i < rho_connectclient_strhasharray_size(items); i++ )
    {
        rho_connectclient_itemdestroy(g_szProduct, rho_connectclient_strhasharray_get(items, i));
    }

    unsigned long cond2 = rho_connectclient_hash_create();
    rho_connectclient_hash_put(cond2, "name", g_product_test_name.c_str());

    unsigned long items2 = rho_connectclient_find_all(g_szProduct, cond2 );
    EXPECT_NE( items2, 0 );
  
    for ( int i = 0; i < rho_connectclient_strhasharray_size(items2); i++ )
    {
        rho_connectclient_itemdestroy(g_szProduct, rho_connectclient_strhasharray_get(items2, i));
    }

    unsigned long cond3 = rho_connectclient_hash_create();
    rho_connectclient_hash_put(cond3, "first", "CustTest1");
    rho_connectclient_itemdestroy(g_szCustomer, cond3);

    unsigned long cond4 = rho_connectclient_hash_create();
    rho_connectclient_hash_put(cond4, "first", "CustTest2");
    rho_connectclient_itemdestroy(g_szCustomer, cond4);

    char* szRes = (char*)rho_sync_doSyncSourceByName(g_szProduct);
    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "ok");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);

    unsigned long item2 = rho_connectclient_find_first(g_szProduct, cond );
    EXPECT_EQ(item2, 0);

    rho_connectclient_hash_delete(cond);
    rho_connectclient_hash_delete(cond2);
    rho_connectclient_hash_delete(cond3);
    rho_connectclient_hash_delete(cond4);
    rho_connectclient_strhasharray_delete(items);
}

extern "C" void rho_free_callbackdata(void* pData)
{
	//It is used in SyncClient.
}
