package com.rhomobile.rhoconnect_client_test;

import android.content.pm.ApplicationInfo;
import android.test.AndroidTestCase;

import com.rhomobile.rhodes.RhoLogConf;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhoconnect.RhoConnectClient;
import com.rhomobile.rhoconnect.RhomModel;
import com.rhomobile.rhoconnect.RhoConnectNotify;

public class TestRhoConnectClient extends AndroidTestCase {
    private static final String TAG = TestRhoConnectClient.class.getSimpleName();

	private final String SYNC_URL = "http://rhodes-store-server.heroku.com/application";

    RhoConnectClient mClient;
    RhomModel mModels[];
    
    @Override
    protected void setUp()
    {
		System.loadLibrary("rhoconnectclient");

		ApplicationInfo appInfo = this.getContext().getApplicationInfo();
		try {
			RhoFileApi.initRootPath(appInfo.dataDir, appInfo.sourceDir);
			RhoFileApi.init(this.getContext());
			
			RhoLogConf.setMinSeverity(0);
			RhoLogConf.setEnabledCategories("*");
			
			RhoConnectClient.nativeInit();
		} catch (Exception e) {
			fail(e.getMessage());
		}
		
    	mClient = new RhoConnectClient();

    	mModels = new RhomModel[]{
    			new RhomModel("Perftest", RhomModel.SYNC_TYPE_NONE),
				new RhomModel("Customer", RhomModel.SYNC_TYPE_INCREMENTAL),
				new RhomModel("Product", RhomModel.SYNC_TYPE_INCREMENTAL)
			};

		mClient.initialize(mModels);
        mClient.setThreadedMode(false);
        mClient.setPollInterval(0);
        mClient.setSyncServer(SYNC_URL);
        mClient.setBulkSyncState(1);
    }
    
    @Override
    protected void tearDown()
    {
        mClient.databaseFullResetAndLogout();
    	mClient.close();
    }
    
    public void testInitiallyLoggedOut()
    {
        mClient.databaseFullResetAndLogout();
    	assertFalse(mClient.isLoggedIn());
    }
    public void testLogin()
    {
        RhoConnectNotify notify = mClient.loginWithUserSync("", "");
        assertEquals(notify.getErrorCode(), 0);
        assertTrue(mClient.isLoggedIn());
    }
    public void testSyncProductByName()
    {
    	testLogin();
    	RhoConnectNotify notify = mModels[2].sync();
    	assertEquals(notify.getErrorCode(), 0);
    }
    
    public void testSyncAll()
    {
    	testLogin();
    	RhoConnectNotify notify = mClient.syncAll();
    	assertEquals(notify.getErrorCode(), 0);
    }

}
