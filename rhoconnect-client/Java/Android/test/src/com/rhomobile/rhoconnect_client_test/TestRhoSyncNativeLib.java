package com.rhomobile.rhoconnect_client_test;

import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhosync.RhoConnectClient;
import com.rhomobile.rhosync.RhomModel;

import android.content.pm.ApplicationInfo;
import android.test.AndroidTestCase;

public class TestRhoSyncNativeLib/* extends AndroidTestCase */{
    private static final String TAG = TestRhoSyncNativeLib.class.getSimpleName();
	
	public void testLibraryLoaded()
	{
//		System.loadLibrary("rhosyncclient");
//		String resources[] = {"apps/rhoconfig.txt", "db/syncdb.schema", "db/synchdb.triggers", "syncdb_java.triggers"};
//		ApplicationInfo appInfo = this.getContext().getApplicationInfo();
//		try {
//			RhoFileApi.initRootPath(appInfo.dataDir, appInfo.sourceDir);
//			RhoFileApi.initCopy(this.getContext(), resources);
//			RhoSyncClient.nativeInit();
//		} catch (Exception e) {
//			fail(e.getMessage());
//		}
//
//		RhoSyncClient client = new RhoSyncClient();
//		RhomModel models[] = {
//				new RhomModel("Customer", RhomModel.SYNC_TYPE_INCREMENTAL),
//				new RhomModel("Product", RhomModel.SYNC_TYPE_INCREMENTAL)
//			};
//
//		client.initialize(models);
//		client.setThreadedMode(false);
//		client.setPollInterval(60);
//		assertEquals(client.getPollInterval(), 60);
//
//		client.setPollInterval(0);
//		assertEquals(client.getPollInterval(), 0);
	}
	
	
	
}
