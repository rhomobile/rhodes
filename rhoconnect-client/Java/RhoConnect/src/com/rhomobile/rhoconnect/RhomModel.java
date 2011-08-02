/*
 ============================================================================
 Author	    : Alexey Tikhvinsky
 Copyright  : Copyright (C) 2011 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */

package com.rhomobile.rhoconnect;

import java.util.Collection;
import java.util.Map;

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

    private native void init();
    private static native RhoConnectNotify syncByName(String modelName);
    private static native Map<String, String> findByName(String modelName, String objectId);
    private static native Map<String, String> findFirstByName(String modelName, String condKeys[], String condVals[]);
    private static native Collection<Map<String, String> > findAllByName(String modelName, String condKeys[], String condVals[]);
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
