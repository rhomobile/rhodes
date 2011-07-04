package com.rhomobile.rhosync_client_test;

import java.io.File;

import android.content.pm.ApplicationInfo;
import android.test.AndroidTestCase;
import android.util.Log;

import com.rhomobile.rhosync.*;

public class TestRhoSyncClient extends AndroidTestCase {
    private static final String TAG = TestRhoSyncClient.class.getSimpleName();

	private final String SYNC_URL = "http://rhodes-store-server.heroku.com/application";

    RhoSyncClient mClient;

    @Override
    protected void setUp()
    {
		ApplicationInfo appInfo = this.getContext().getApplicationInfo();
		String dataDir = appInfo.dataDir;
		
		String rootPath = dataDir + "/rhodata/";
		Log.d(TAG, "Root path: " + rootPath);
		
		String sqliteJournals = dataDir + "/sqlite_stmt_journals/";
		Log.d(TAG, "Sqlite journals path: " + sqliteJournals);
		
		File f = new File(rootPath);
		f.mkdirs();
		f = new File(f, "db/db-files");
		f.mkdirs();
		f = new File(sqliteJournals);
		f.mkdirs();
		RhoSyncClient.nativeInit(rootPath, sqliteJournals);
    	
    	mClient = new RhoSyncClient();
//        mClient.setThreadedMode(false);
//        mClient.setSyncServer(SYNC_URL);
//        mClient.loginWithUser("", "");
    }

    public void testLogin()
    {
        assertTrue(mClient.isLoggedIn());
    }

}
