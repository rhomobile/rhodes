package com.rhomobile.rhosync_client_test;

import com.rhomobile.rhosync.RhoSyncClient;

import android.test.AndroidTestCase;

public class TestRhoSyncNativeLib extends AndroidTestCase {
	RhoSyncClient mClient;
	
	protected void setUp()
	{
		mClient = new RhoSyncClient();
	}
	
	public void testLibraryLoaded()
	{
		mClient.setThreadedMode(false);
		assertTrue(!mClient.getThreadedMode());
		
		mClient.setThreadedMode(true);
		assertTrue(mClient.getThreadedMode());
	}
	
	
	
}
