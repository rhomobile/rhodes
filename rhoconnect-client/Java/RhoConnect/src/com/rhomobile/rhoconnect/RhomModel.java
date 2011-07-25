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
    private static native RhoConnectNotify syncByName(String name);

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
	/* TODO:
	create();
	find(String text);
	findFirst();
	findAll();
	save();
	destroy();
	*/
	void startBulkUpdate() {}
	void stopBulkUpdate() {}

}
