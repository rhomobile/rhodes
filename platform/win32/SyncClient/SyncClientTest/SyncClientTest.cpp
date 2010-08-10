#include "stdafx.h"

#include "SyncClient/SyncClient.h"
#include "sync/SyncThread.h"
#include "common/RhoConf.h"
#include "common/RhoError.h"

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

TEST(SyncClient, shouldSyncProduct) 
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

