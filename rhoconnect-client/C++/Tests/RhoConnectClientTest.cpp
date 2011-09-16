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
//run tests
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
//

    rho_connectclient_destroy();

	return 0;
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

    char* szRes = (char*)rho_sync_doSyncSourceByName(g_szProduct);
    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szRes, &oNotify);

    EXPECT_EQ(String(oNotify.status), "ok");
    EXPECT_EQ(oNotify.error_code, RHO_ERR_NONE);

    unsigned long item3 = rho_connectclient_find(g_szProduct, rho_connectclient_hash_get(item, "object") );
    EXPECT_EQ( item3, 0 );

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
