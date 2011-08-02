package com.rhomobile.rhoconnect;

public class RhoConnectObjectNotify {
	String[] mDeletedObjects;
	String[] mUpdatedObjects;
	String[] mCreatedObjects;
	
	int[] mDeletedSrcIds;
	int[] mUpdatedSrcIds;
	int[] mCreatedSrcIds;

	public String[] getDeletedObjects() { return mDeletedObjects; }
	public String[] getUpdatedObjects() { return mUpdatedObjects; }
	public String[] getCreatedObjects() { return mCreatedObjects; }

	public int[] getDeletedSourceIds() { return mDeletedSrcIds; }
	public int[] getUpdatedSourceIds() { return mUpdatedSrcIds; }
	public int[] getCreatedSourceIds() { return mCreatedSrcIds; }

	public static interface IDelegate {
		public void call(RhoConnectObjectNotify notify); 
	}
}
