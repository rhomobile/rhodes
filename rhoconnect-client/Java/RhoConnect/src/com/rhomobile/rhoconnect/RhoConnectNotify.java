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
