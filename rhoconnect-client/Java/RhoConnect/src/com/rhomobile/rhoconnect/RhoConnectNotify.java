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
	
	public int getTotalCount() { return mTotalCount; }
	void setTotalCount(int count) { mTotalCount = count; }

	public int getProcessedCount() { return mProcessedCount; }
	void setProcessedCount(int count) { mProcessedCount = count; }

	public int getCumulativeCount() { return mCumulativeCount; }
	void setCumulativeCount(int count) { mCumulativeCount = count; }

	public int getSourceId() { return mSourceId; }
	void setSourceId(int id) { mSourceId = id; }

	public int getErrorCode() { return mErrorCode; }
	void setErrorCode(int code) { mErrorCode = code; }

	public String getSourceName() { return mSourceName; }
	public String getStatus() { return mStatus; }
	public String getSyncType() { return mSyncType; }
	public String getErrorMessage() { return new String(); /*TODO: make implementation */}
	public String getCallbackParams() { return mParams; }
	
	public static interface IDelegate {
		public void call(RhoConnectNotify notify); 
	}

}
