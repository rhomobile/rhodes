package com.rhomobile.android_store;

import com.rhomobile.rhoconnect.RhoConnectClient;
import com.rhomobile.rhoconnect.RhoConnectNotify;
import com.rhomobile.rhoconnect.RhomModel;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoLogConf;
import com.rhomobile.rhodes.file.RhoFileApi;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.os.Bundle;
import android.util.Log;

public class StoreActivity extends Activity {

	private static final String TAG = StoreActivity.class.getSimpleName();
	private RhoConnectClient mSyncClient;
	private RhomModel mModels[];

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

		ApplicationInfo appInfo = getApplicationInfo();

		try {
			RhoFileApi.initRootPath(appInfo.dataDir, appInfo.sourceDir);
			RhoFileApi.init(this.getApplicationContext());

			RhoLogConf.setMinSeverity(0);
			RhoLogConf.setEnabledCategories("*");

			RhoConnectClient.nativeInit();
		} catch (Exception e) {
			Logger.E(TAG, e.getMessage());
		}

        mSyncClient = new RhoConnectClient();

    	mModels = new RhomModel[]{
				new RhomModel("Customer", RhomModel.SYNC_TYPE_INCREMENTAL),
				new RhomModel("Product", RhomModel.SYNC_TYPE_INCREMENTAL)
			};

    	mSyncClient.initialize(mModels);
    	mSyncClient.setPollInterval(0);
    	mSyncClient.setSyncServer("http://rhodes-store-server.heroku.com/application");
    	mSyncClient.setBulkSyncState(1);

    	mSyncClient.loginWithUserAsync("", "", new RhoConnectNotify.IDelegate()
    		{
    			public void call(RhoConnectNotify notify) { onLogin(notify); }
    		});
    }

    @Override
    public void onDestroy() {
        mSyncClient.databaseFullResetAndLogout();
    	mSyncClient.close();
    }

    private void onLogin(RhoConnectNotify notify) {
    	Log.i(TAG, "onLogin");
    }

}