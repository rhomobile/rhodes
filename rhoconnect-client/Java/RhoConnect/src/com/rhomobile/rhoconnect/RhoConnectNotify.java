package com.rhomobile.rhoconnect;

public class RhoConnectNotify {
	int mTotalCount;
	int mProcessedCount;
	int mCumulativeCount;
	int mSourceId;
	int mErrorCode;
	String mSourceName;
	String mStatus;
	String mSyncType;
	String mParams;
	String mErrorMessage;
	
	public int getTotalCount() { return mTotalCount; }
	public int getProcessedCount() { return mProcessedCount; }
	public int getCumulativeCount() { return mCumulativeCount; }
	public int getSourceId() { return mSourceId; }
	public int getErrorCode() { return mErrorCode; }
	public String getSourceName() { return mSourceName; }
	public String getStatus() { return mStatus; }
	public String getSyncType() { return mSyncType; }
	public String getErrorMessage() { return mErrorMessage; }
	public String getCallbackParams() { return mParams; }
	
	public static interface IDelegate {
		public void call(RhoConnectNotify notify); 
	}

}
