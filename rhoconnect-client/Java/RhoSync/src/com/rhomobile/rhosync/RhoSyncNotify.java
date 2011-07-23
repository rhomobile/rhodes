package com.rhomobile.rhosync;

public class RhoSyncNotify {
	int mTotalCount;
	int mProcessedCount;
	int mCumulativeCount;
	int mSourceId;
	int mErrorCode;
	String mSourceName;
	String mStatus;
	String mSyncType;
	String mParams;
	
	int getTotalCount() { return mTotalCount; }
	void setTotalCount(int count) { mTotalCount = count; }

	int getProcessedCount() { return mProcessedCount; }
	void setProcessedCount(int count) { mProcessedCount = count; }

	int getCumulativeCount() { return mCumulativeCount; }
	void setCumulativeCount(int count) { mCumulativeCount = count; }

	int getSourceId() { return mSourceId; }
	void setSourceId(int id) { mSourceId = id; }

	int getErrorCode() { return mErrorCode; }
	void setErrorCode(int code) { mErrorCode = code; }

	String getSourceName() { return mSourceName; }
	String getStatus() { return mStatus; }
	String getSyncType() { return mSyncType; }
	String getErrorMessage() { return new String(); /*TODO: make implementation */}
	String getCallbackParams() { return mParams; }

}
