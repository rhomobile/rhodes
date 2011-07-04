package com.rhomobile.rhosync_client_test;

import java.io.File;

import com.rhomobile.rhosync.RhoSyncClient;
import com.rhomobile.rhosync.RhomModel;

import android.content.pm.ApplicationInfo;
import android.test.AndroidTestCase;
import android.util.Log;

public class TestRhoSyncNativeLib extends AndroidTestCase {
    private static final String TAG = TestRhoSyncNativeLib.class.getSimpleName();
	
	public void testLibraryLoaded()
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

		RhoSyncClient client = new RhoSyncClient();
		RhomModel models[] = {
				new RhomModel("Customer", RhomModel.SYNC_TYPE_INCREMENTAL),
				new RhomModel("Product", RhomModel.SYNC_TYPE_INCREMENTAL)
			};

		client.initialize(models);
		client.setThreadedMode(false);
		client.setPollInterval(60);
		assertEquals(client.getPollInterval(), 60);

		client.setPollInterval(0);
		assertEquals(client.getPollInterval(), 0);
	}
	
	
	
}
