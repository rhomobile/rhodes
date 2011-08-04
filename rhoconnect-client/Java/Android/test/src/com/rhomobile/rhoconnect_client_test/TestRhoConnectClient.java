/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhoconnect_client_test;

import java.util.HashMap;
import java.util.Map;

import android.content.pm.ApplicationInfo;
import android.test.AndroidTestCase;

import com.rhomobile.rhodes.RhoLogConf;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhoconnect.RhoConnectClient;
import com.rhomobile.rhoconnect.RhoConnectObjectNotify;
import com.rhomobile.rhoconnect.RhomModel;
import com.rhomobile.rhoconnect.RhoConnectNotify;

public class TestRhoConnectClient extends AndroidTestCase {
    private static final String TAG = TestRhoConnectClient.class.getSimpleName();

	private final String SYNC_URL = "http://rhodes-store-server.heroku.com/application";
	
	class ObjectNotifyDelegate implements RhoConnectObjectNotify.IDelegate
	{
		RhoConnectObjectNotify mNotify;
		@Override
		public void call(RhoConnectObjectNotify notify) {
			mNotify = notify;
		}
	}

    RhoConnectClient mClient;
    RhomModel mModels[];
    RhomModel mProduct;
    
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
    	mProduct = mModels[2];

		mClient.initialize(mModels);
        mClient.setThreadedMode(false);
        mClient.setPollInterval(0);
        mClient.setSyncServer(SYNC_URL);
        mClient.setBulkSyncState(1);
    }
    
    @Override
    protected void tearDown()
    {
        //mClient.databaseFullResetAndLogout();
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
    	RhoConnectNotify notify = mProduct.sync();
    	assertEquals(notify.getErrorCode(), 0);
    }
    
    public void testSyncAll()
    {
    	testLogin();
    	RhoConnectNotify notify = mClient.syncAll();
    	assertEquals(notify.getErrorCode(), 0);
    }
    
    public void testCreateNewProduct()
    {
    	Map<String, String> item = new HashMap<String, String>();
    	item.put("name", "AndroidTest");
    	
    	mProduct.create(item);
    	
    	assertTrue(item.containsKey("object"));
    	assertTrue(item.containsKey("source_id"));
    	
    	Map<String, String> item2 = mModels[2].find(item.get("object"));
    	assertTrue(item2 != null);
    	assertEquals(item.get("name"), item2.get("name"));
    	
    }
    
    public void testCreateObjectNotify()
    {
    	testLogin();
    	
    	Map<String, String> item = new HashMap<String, String>();
    	item.put("name", "AndroidTest2");
    	
    	mProduct.create(item);
    	
    	assertTrue(item.containsKey("object"));
    	assertTrue(item.containsKey("source_id"));
    	
    	Map<String, String> item2 = mModels[2].find(item.get("object"));
    	assertNotNull(item2);
    	assertEquals(item.get("name"), item2.get("name"));
    	
    	ObjectNotifyDelegate objectCallback = new ObjectNotifyDelegate();
    	mClient.setObjectNotification(objectCallback);
    	mClient.addObjectNotify(Integer.parseInt(item.get("source_id")), item.get("object"));
    	
    	RhoConnectNotify notify = mProduct.sync();
    	assertEquals(notify.getErrorCode(), 0);
    	
    	assertNotNull(objectCallback.mNotify);
    	
    	String[] createdObjects = objectCallback.mNotify.getCreatedObjects();
    	assertNotNull(createdObjects);
    	
    	int[] createdSourceIds = objectCallback.mNotify.getCreatedSourceIds();
    	assertNotNull(createdSourceIds);
    	
    	assertEquals(createdObjects[0], item.get("object"));
    }

    
    public void testModifyProduct()
    {
    	testLogin();
    	
    	Map<String, String> cond = new HashMap<String, String>();
    	cond.put("name", "AndroidTest2");

    	Map<String,String> item = mProduct.findFirst(cond);
    	
    	assertNotNull(item);
    	assertTrue(item.size() > 0);
    	
    	String object = item.get("object");

    	item.put("sku", item.get("sku") + "_TEST");
    	mProduct.save(item);
    	
    	RhoConnectNotify notify = mProduct.sync();
    	assertEquals(notify.getErrorCode(), 0);
    	
    	Map<String,String> foundItem = mProduct.find(object);

    	assertNotNull(foundItem);
    	assertTrue(foundItem.size() > 0);
    	
    	assertEquals(item.get("sku"), foundItem.get("sku"));
    }
}
