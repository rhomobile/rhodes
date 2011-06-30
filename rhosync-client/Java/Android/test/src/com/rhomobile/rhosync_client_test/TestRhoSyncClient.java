package com.rhomobile.rhosync_client_test;

import android.test.AndroidTestCase;
import com.rhomobile.rhosync.*;

public class TestRhoSyncClient extends AndroidTestCase {
    private final String SYNC_URL = "http://rhodes-store-server.heroku.com/application";

    RhoSyncClient mClient;

    @Override
    protected void setUp()
    {
        mClient = new RhoSyncClient();
        mClient.setThreadedMode(false);
        mClient.setSyncServer(SYNC_URL);
        mClient.loginWithUser("", "");
    }

    public void testLogin()
    {
        assertTrue(mClient.isLoggedIn());
    }

}
