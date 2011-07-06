package com.rhomobile.rhosync_client_test;

import java.io.File;

import android.content.pm.ApplicationInfo;
import android.test.AndroidTestCase;

import com.rhomobile.rhodes.RhoLogConf;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhosync.RhoSyncClient;
import com.rhomobile.rhosync.RhomModel;

public class TestRhoSyncClient extends AndroidTestCase {
    private static final String TAG = TestRhoSyncClient.class.getSimpleName();

	private final String SYNC_URL = "http://rhodes-store-server.heroku.com/application";

    RhoSyncClient mClient;
    RhomModel mModels[];
    RhoLogConf mLogConf;

    @Override
    protected void setUp()
    {
		System.loadLibrary("rhosyncclient");
		mLogConf = new RhoLogConf();
		
		//String resources[] = {"db/syncdb.schema", "db/syncdb.triggers", "db/syncdb_java.triggers"};

    	ApplicationInfo appInfo = this.getContext().getApplicationInfo();
		try {
			RhoFileApi.initRootPath(appInfo.dataDir, appInfo.sourceDir);
			RhoFileApi.init(this.getContext());
			
			//mLogConf.setMinSeverity(0);
			//mLogConf.setEnabledCategories("*");
			
			RhoSyncClient.nativeInit();
		} catch (Exception e) {
			fail(e.getMessage());
		}
		
    	mClient = new RhoSyncClient();

    	mModels = new RhomModel[]{
				new RhomModel("Customer", RhomModel.SYNC_TYPE_INCREMENTAL),
				new RhomModel("Product", RhomModel.SYNC_TYPE_INCREMENTAL)
			};

		mClient.initialize(mModels);
        mClient.setThreadedMode(false);
        mClient.setPollInterval(0);
        mClient.setSyncServer(SYNC_URL);
        mClient.loginWithUser("", "");
    }
    
    public void testLogin()
    {
        assertTrue(mClient.isLoggedIn());
    }

}
