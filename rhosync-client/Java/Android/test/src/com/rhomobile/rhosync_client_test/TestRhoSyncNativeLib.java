package com.rhomobile.rhosync_client_test;

import java.io.File;

import com.rhomobile.rhosync.RhoSyncClient;

import android.content.pm.ApplicationInfo;
import android.test.AndroidTestCase;
import android.util.Log;

public class TestRhoSyncNativeLib extends AndroidTestCase {
    private static final String TAG = TestRhoSyncNativeLib.class.getSimpleName();
	RhoSyncClient mClient;
	
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
		mClient.setThreadedMode(false);
	}
	
	public void testLibraryLoaded()
	{
		//mClient.setPollInterval(60);
		//assertEquals(mClient.getPollInterval(), 60);

		//mClient.setPollInterval(0);
		//assertEquals(mClient.getPollInterval(), 0);
	}
	
	
	
}
