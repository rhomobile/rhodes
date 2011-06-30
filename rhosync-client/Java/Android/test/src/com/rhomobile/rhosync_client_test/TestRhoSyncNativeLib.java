package com.rhomobile.rhosync_client_test;

import com.rhomobile.rhosync.RhoSyncClient;

import android.test.AndroidTestCase;

public class TestRhoSyncNativeLib extends AndroidTestCase {
    RhoSyncClient mClient;

    @Override
    protected void setUp()
    {
        mClient = new RhoSyncClient();
    }

    public void testLibraryLoaded()
    {
        mClient.setThreadedMode(true);
        assertTrue(!mClient.getThreadedMode());

        mClient.setThreadedMode(false);
        assertTrue(!mClient.getThreadedMode());
    }

}
