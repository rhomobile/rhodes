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


package com.rhomobile.rhoconnect;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

public class RhomModel {
	public final static int MODEL_TYPE_PROPERTY_BAG = 0;
	public final static int MODEL_TYPE_FIXED_SCHEMA = 1;

	public final static int SYNC_TYPE_NONE = 0;
	public final static int SYNC_TYPE_INCREMENTAL = 1;
	public final static int SYNC_TYPE_BULK_ONLY = 2;
	
    private String mName;
    private int mModelType;

    private int mSyncType;
    private int mSyncPriority;

    private String mPartition;
    private Map<String, Boolean> mBlobAttribs = new TreeMap<String, Boolean>();
    private Map<String, String> mAssociations = new TreeMap<String, String>();

    private native void init();
    private static native RhoConnectNotify syncByName(String modelName);
    private static native Map<String, String> findByName(String modelName, String objectId);
    private static native Map<String, String> findFirstByName(String modelName, String condKeys[], String condVals[]);
    private static native Collection<Map<String, String> > findAllByName(String modelName, String condKeys[], String condVals[]);
    private static native Collection<Map<String, String> > findBySqlByName(String modelName, String query, String params[]);
    private static native Map<String, String> createByName(String modelName, String keys[], String values[]);
    private static native void saveByName(String modelName, String keys[], String values[]);
    private static native void destroyByName(String modelName, String keys[], String values[]);
    private static native void startBulkUpdateByName(String modelName);
    private static native void stopBulkUpdateByName(String modelName);
    
    public RhomModel(String name, int syncType) {
    	init();
    	mName = name;
    	mSyncType = syncType;
    }
    
    public String getName() { return mName; }
    public int getModelType() { return mModelType; }
    public void setModelType(int type) { mModelType = type; }
    public int getSyncType() { return mSyncType; } 
    public void setSyncType(int type) { mSyncType = type; }
    public int getSyncPriority() { return mSyncPriority; }
    public void setSyncPriority(int prio) { mSyncPriority = prio; }
    public String getPartition() { return mPartition; }
    public void setPartition(String part) { mPartition = part; }
    public Map<String, Boolean> getBlobAttribs() { return mBlobAttribs; }
    public String getBlobAttribsAsString()
    {
        Set<Map.Entry<String, Boolean> > entries = mBlobAttribs.entrySet();
        Iterator<Map.Entry<String, Boolean> > entryIt = entries.iterator();
        StringBuilder blobAttribs = new StringBuilder();
    
        while(entryIt.hasNext()) {
            Map.Entry<String, Boolean> entry = entryIt.next();
    
            if(blobAttribs.length() > 0)
                blobAttribs.append(',');

            blobAttribs.append(entry.getKey()).append(',').append(entry.getValue()?'1':'0');
        }
        return blobAttribs.toString();
    }
    public Map<String, String> getAssociations() { return mAssociations; }

	public RhoConnectNotify sync() { return syncByName(mName); }

	private void mapToArrays(Map<String, String> item,
						String[] keys, String[] vals)
	{
		int n = 0;
		for (String key : item.keySet()) {
			keys[n] = key;
			vals[n] = item.get(key);
			++n;
		}
	}
	
	public Map<String, String> find(String objectId) { return findByName(mName, objectId); }
    public Map<String, String> findFirst(Map<String, String> condition)
    {
		String keys[] = new String[condition.size()];
		String vals[] = new String[condition.size()];
		mapToArrays(condition, keys, vals);
		return findFirstByName(mName, keys, vals);
    }
    public Collection<Map<String, String> > findAll(Map<String, String> condition)
    {
		String keys[];
		String vals[];
    	if (condition != null) {
			keys = new String[condition.size()];
			vals = new String[condition.size()];
			mapToArrays(condition, keys, vals);
    	} else {
    		keys = vals = new String[0];
    	}
		return findAllByName(mName, keys, vals);
    }
    
    public Collection<Map<String, String> > findBySql(String query, List<String> params)
    {
        String arrayParams[] = null;
        if (params != null) {
            arrayParams = new String[params.size()];
            params.toArray(arrayParams);
        }
        return findBySqlByName(mName, query, arrayParams);
    }
	
	public void create(Map<String, String> item) {
		String keys[] = new String[item.size()];
		String vals[] = new String[item.size()];
		mapToArrays(item, keys, vals);
		Map<String, String> created = createByName(mName, keys, vals);
		
		if (created != null) {
			item.clear();
			item.putAll(created);
		}
	}
	public void save(Map<String, String> item) {
		String keys[] = new String[item.size()];
		String vals[] = new String[item.size()];
		mapToArrays(item, keys, vals);
		saveByName(mName, keys, vals);
	}
	public void destroy(Map<String, String> item) {
		String keys[] = new String[item.size()];
		String vals[] = new String[item.size()];
		mapToArrays(item, keys, vals);
		destroyByName(mName, keys, vals);
	}

	public void startBulkUpdate() { startBulkUpdateByName(mName); }
	public void stopBulkUpdate() { stopBulkUpdateByName(mName); }

}
