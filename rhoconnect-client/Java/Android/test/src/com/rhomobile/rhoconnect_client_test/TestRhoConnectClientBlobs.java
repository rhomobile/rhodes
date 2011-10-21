package com.rhomobile.rhoconnect_client_test;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.text.SimpleDateFormat;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import com.rhomobile.rhoconnect.RhoConnectClient;
import com.rhomobile.rhoconnect.RhoConnectNotify;
import com.rhomobile.rhoconnect.RhomModel;
import com.rhomobile.rhodes.RhoLogConf;
import com.rhomobile.rhodes.file.RhoFileApi;

import android.content.pm.ApplicationInfo;
import android.test.AndroidTestCase;

public class TestRhoConnectClientBlobs extends AndroidTestCase
{
    private final String SYNC_URL = "http://rhodes-samples-server.heroku.com/application";
    private final String TEST_FILENAME = "blobtest.txt";
    private final String TEST_BLOB_CONTENT = "Blob tets at" + new SimpleDateFormat("yyyy/MM/dd HH:mm:ss").format(new Date());

    RhoConnectClient mClient;
    RhomModel mModels[];
    RhomModel mBlobTest;

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
                new RhomModel("BlobTest", RhomModel.SYNC_TYPE_INCREMENTAL)
            };
        mBlobTest = mModels[0];
        mBlobTest.getBlobAttribs().put("image_uri", false);

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

    public void test1Login()
    {
        RhoConnectNotify notify = mClient.loginWithUserSync("", "");
        assertEquals(notify.getErrorCode(), 0);
        assertTrue(mClient.isLoggedIn());
    }

    public void test2SupportBlobType()
    {
        String blobUri = new File(RhoFileApi.getDbFilesUriPath(), TEST_FILENAME).getPath();
        File testFile = new File(RhoFileApi.getDbFilesPath(), TEST_FILENAME);
        if (testFile.exists()) {
            testFile.delete();
            if (testFile.exists()) {
                fail("Cannot remove test file: " + testFile.getAbsolutePath());
            }
        }

        try {
            testFile.createNewFile();
            FileWriter writer = new FileWriter(testFile);
            writer.write(TEST_BLOB_CONTENT);
            writer.close();
        } catch(Exception e)
        {
            fail("Cannot write test blob file: " + testFile.getAbsolutePath());
        }
        assertTrue(testFile.exists());

        Map<String, String> item = new HashMap<String, String>();
        item.put("name", new Long(new Date().getTime()).toString());
        item.put("image_uri", blobUri);

        mBlobTest.create(item);
        assertTrue(item.containsKey("object"));
        assertEquals(blobUri, item.get("image_uri"));
        assertTrue(new File(RhoFileApi.getDbFilesPath(), TEST_FILENAME).exists());

        mBlobTest.destroy(item);

        Map<String, String> cond = new HashMap<String, String>();
        cond.put("name", item.get("name"));
        Map<String, String> itemFound = mBlobTest.findFirst(cond);

        assertNull(itemFound);
        assertFalse(testFile.exists());
    }
    public void test3SyncBlobTest()
    {
        RhoConnectNotify notify = mClient.loginWithUserSync("", "");
        assertEquals(notify.getErrorCode(), 0);
        assertTrue(mClient.isLoggedIn());

        notify = mBlobTest.sync();
        assertEquals(notify.getErrorMessage(), 0, notify.getErrorCode());
    }

    public void test4DeleteAllBlobs()
    {
        RhoConnectNotify notify = mClient.loginWithUserSync("", "");
        assertEquals(notify.getErrorCode(), 0);
        assertTrue(mClient.isLoggedIn());

        notify = mBlobTest.sync();
        assertEquals(notify.getErrorMessage(), 0, notify.getErrorCode());

        Collection<Map<String, String> > items = mBlobTest.findAll(null);
        if (items != null && items.size() > 0) {
            for (Map<String, String> item : items) {
                mBlobTest.destroy(item);
            }
        }

        notify = mBlobTest.sync();
        assertEquals(notify.getErrorMessage(), 0, notify.getErrorCode());

        items = mBlobTest.findAll(null);
        assertTrue(items == null || items.size() == 0);
    }

    public void test5CreateNewBlobObject()
    {
        String fileName = "androidtest.png";
        String filePath = new File("apps", fileName).getPath();

        RhoConnectNotify notify = mClient.loginWithUserSync("", "");
        assertEquals(notify.getErrorCode(), 0);
        assertTrue(mClient.isLoggedIn());

        Map<String, String> item = new HashMap<String, String>();
        item.put("image_uri", "apps/" + fileName);

        mBlobTest.create(item);
        mBlobTest.save(item);

        notify = mBlobTest.sync();
        assertEquals(notify.getErrorMessage(), 0, notify.getErrorCode());

        Collection<Map<String, String> > items = mBlobTest.findAll(null);
        assertNotNull(items);
        assertEquals(1, items.size());

        Map<String, String> newItem = items.iterator().next();
        assertFalse(filePath.equals(newItem.get("image_uri")));

        String files[] = new String[] {"apps/" + fileName };
        RhoFileApi.initCopy(this.getContext(), files);

        File file = new File(RhoFileApi.absolutePath(filePath));
        File newFile = new File(RhoFileApi.absolutePath(newItem.get("image_uri")));

        assertTrue(file.isFile());
        assertEquals(file.length(), newFile.length());

        FileInputStream fileStream = null;
        FileInputStream newFileStream = null;
        int b1 = 0;
        int b2 = 0;
        try {
            fileStream = new FileInputStream(file);
            newFileStream = new FileInputStream(newFile);

            do {
                b1 = fileStream.read();
                b2 = newFileStream.read();
            } while (b1 == b2 && (b1 != -1 && b2 != -1));

        } catch (Throwable e){
            try {
                if (fileStream != null)
                    fileStream.close();
            } catch (Throwable e1){}
            try {
                if (newFileStream != null)
                    newFileStream.close();
            } catch (Throwable e1){}

            fail(e.getMessage());
        }

        // Check both streams reach end
        assertTrue(b1 == -1 && b2 == -1);

        file.delete();
    }

}
