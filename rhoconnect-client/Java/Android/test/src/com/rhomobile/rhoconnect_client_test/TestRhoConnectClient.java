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

import java.text.SimpleDateFormat;
import java.util.Date;
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
    //private static final String TAG = TestRhoConnectClient.class.getSimpleName();

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
    RhomModel mCustomer;
    
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
        mCustomer = mModels[1];
        mProduct  = mModels[2];

        mProduct.getAssociations().put("quantity", "Customer");
        mProduct.getAssociations().put("sku", "Customer");

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
        RhoConnectNotify notify = mClient.loginWithUserSync("", "");
        assertEquals(notify.getErrorCode(), 0);
        assertTrue(mClient.isLoggedIn());

        notify = mCustomer.sync();
        assertEquals(0, notify.getErrorCode());
    }
    
    public void testSyncAll()
    {
        RhoConnectNotify notify = mClient.loginWithUserSync("", "");
        assertEquals(notify.getErrorCode(), 0);
        assertTrue(mClient.isLoggedIn());

        notify = mClient.syncAll();
        assertEquals(notify.getErrorCode(), 0);
    }

    public void testCreateNewProduct()
    {
        Map<String, String> item = new HashMap<String, String>();
        item.put("name", "AndroidTest");

        mProduct.create(item);

        assertTrue(item.containsKey("object"));
        assertTrue(item.containsKey("source_id"));

        Map<String, String> item2 = mProduct.find(item.get("object"));
        assertTrue(item2 != null);
        assertEquals(item.get("name"), item2.get("name"));

        mProduct.sync();

        Map<String, String> cond = new HashMap<String, String>();
        cond.put("name", "AndroidTest");
        Map<String, String> item3 = mProduct.findFirst(cond);
        assertNotNull(item3);
        assertFalse(item3.isEmpty());
        assertEquals(item.get("name"), item3.get("name"));
    }

    public void testCreateNewProductWithCustomer()
    {
        RhoConnectNotify notify = mClient.loginWithUserSync("", "");
        assertEquals(notify.getErrorCode(), 0);
        assertTrue(mClient.isLoggedIn());

        Map<String, String> cust1 = new HashMap<String, String>();
        cust1.put("first", "CustTest1");
        mCustomer.create(cust1);

        Map<String, String> cust2 = new HashMap<String, String>();
        cust2.put("first", "CustTest2");
        mCustomer.create(cust2);

        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
        String prodName = dateFormat.format(new Date());

        Map<String, String> item = new HashMap<String, String>();
        item.put("name", prodName);
        item.put("quantity", cust1.get("object"));
        item.put("sku", cust2.get("object"));
        mProduct.create(item);

        notify = mClient.syncAll();
        assertEquals(notify.getErrorMessage(), 0, notify.getErrorCode());

        Map<String, String> prodCondition = new HashMap<String, String>();
        prodCondition.put("name", prodName);

        Map<String, String> prod = mProduct.findFirst(prodCondition);

        assertFalse(prod.isEmpty());
        assertFalse(item.get("object").equals(prod.get("object")));
        assertFalse(prod.get("quantity").equals(cust1.get("object")));
        assertFalse(prod.get("sku").equals(cust2.get("object")));

        Map<String, String> cust11 = mCustomer.find(prod.get("quantity"));
        assertFalse(cust11.isEmpty());
        assertEquals(cust11.get("first"), cust1.get("first"));

        Map<String, String> cust22 = mCustomer.find(prod.get("sku"));
        assertFalse(cust22.isEmpty());
        assertEquals(cust22.get("first"), cust2.get("first"));

    }
    
    public void testCreateObjectNotify()
    {
        RhoConnectNotify notify = mClient.loginWithUserSync("", "");
        assertEquals(notify.getErrorCode(), 0);
        assertTrue(mClient.isLoggedIn());

    	Map<String, String> item = new HashMap<String, String>();
    	item.put("name", "AndroidTest2");
    	
    	mProduct.create(item);
    	
    	assertTrue(item.containsKey("object"));
    	assertTrue(item.containsKey("source_id"));
    	
    	Map<String, String> item2 = mProduct.find(item.get("object"));
    	assertNotNull(item2);
    	assertEquals(item.get("name"), item2.get("name"));
    	
    	ObjectNotifyDelegate objectCallback = new ObjectNotifyDelegate();
    	mClient.setObjectNotification(objectCallback);
    	mClient.addObjectNotify(Integer.parseInt(item.get("source_id")), item.get("object"));

        notify = mProduct.sync();
    	assertEquals(notify.getErrorMessage(), 0, notify.getErrorCode());
    	
    	assertNotNull(objectCallback.mNotify);
    	
    	String[] createdObjects = objectCallback.mNotify.getCreatedObjects();
    	assertNotNull(createdObjects);
    	
    	int[] createdSourceIds = objectCallback.mNotify.getCreatedSourceIds();
    	assertNotNull(createdSourceIds);
    	
    	assertEquals(createdObjects[0], item.get("object"));
    }

    public void testModifyProduct()
    {
        RhoConnectNotify notify = mClient.loginWithUserSync("", "");
        assertEquals(notify.getErrorCode(), 0);
        assertTrue(mClient.isLoggedIn());

        notify = mProduct.sync();
        assertEquals(notify.getErrorMessage(), notify.getErrorCode(), 0);

        Map<String, String> cond = new HashMap<String, String>();
        cond.put("name", "AndroidTest");

        Map<String,String> item = mProduct.findFirst(cond);

        if(item == null || item.isEmpty()) {
            testCreateNewProduct();
            item = mProduct.findFirst(cond);
        }

        assertNotNull(item);
        assertTrue(item.size() > 0);

        String object = item.get("object");

        item.put("sku", item.get("sku") + "_TEST");
        mProduct.save(item);

        notify = mProduct.sync();
        assertEquals(notify.getErrorMessage(), 0, notify.getErrorCode());

        Map<String,String> foundItem = mProduct.find(object);

        assertNotNull(foundItem);
        assertFalse(foundItem.isEmpty());

        assertEquals(item.get("sku"), foundItem.get("sku"));
    }
}
