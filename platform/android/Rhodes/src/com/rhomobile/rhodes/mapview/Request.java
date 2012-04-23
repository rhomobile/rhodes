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

package com.rhomobile.rhodes.mapview;


import java.io.IOException;
import java.io.InputStream;
import java.net.URI;
import java.util.ArrayList;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import com.rhomobile.rhodes.util.Utils;

public class Request {
	
	private static int ourRequestGlobalID = 0;
	
	public Request() {
		mID = ++ourRequestGlobalID;
		mData = null;
		mRequest = null;
	}
	
	public int getId() {
		return mID;
		
	}
	
	public void cancel() {
		HttpGet r = mRequest;
		if (r != null) {
			r.abort();
		}
		mData = null;
	}
	
	private class ArrayCollector {
		
		public ArrayCollector(int step_size) {
			mStep = step_size;
			mList = new ArrayList<byte[]>();
			mCurrentBufUsed = 0;
			mGlobalSize = 0;
			mCurrentBuf = new byte[mStep];
		}
		
		public byte[] getCurrentBuf() {
			return mCurrentBuf;
		}
		
		public boolean isCurrentBufFull() {
			return mCurrentBufUsed >= mStep;
		}
		
		public int getCurrentStartFreeIndex() {
			return mCurrentBufUsed;
		}
 		
		public int getCurrentFreeSize() {
			return mStep - mCurrentBufUsed;
		}
		
		public int getGlobalSize() {
			return mGlobalSize;
		}
		
		public void nextStep() {
			mList.add(mCurrentBuf);
			mCurrentBufUsed = 0;
			mCurrentBuf = new byte[mStep];
		}
		
		public void addUsed(int size) {
			mGlobalSize += size;
			mCurrentBufUsed += size;
		}
		
		public byte[] makeOne() {
			byte[] buf = new byte[mGlobalSize];
			int i;
			for (i = 0; i < mList.size(); i++) {
				byte[] b = mList.get(i);
				System.arraycopy(b, 0, buf, i*mStep, mStep);
			}
			System.arraycopy(mCurrentBuf, 0, buf, i*mStep, mCurrentBufUsed);
			return buf;
		}
		
		private int mGlobalSize;
		private int mStep;
		private int mCurrentBufUsed;
		private byte[] mCurrentBuf;
		private ArrayList<byte[]> mList;
	}
	
	public void requestData(String url) {
		Utils.platformLog("MapengineRequest", "requestData("+url+")");

        InputStream is = null;
        try {
            HttpClient client = new DefaultHttpClient();
            
            mRequest = new HttpGet();
            mRequest.setURI(new URI(url));
            HttpResponse response = client.execute(mRequest);
            mRequest = null;
            is = response.getEntity().getContent();
            if (is != null) {
            	
            	ArrayCollector collector = new ArrayCollector(64000); 
            	
            	int s = is.read(collector.getCurrentBuf(), collector.getCurrentStartFreeIndex(), collector.getCurrentFreeSize());
            	while (s > 0) {
            		collector.addUsed(s);
            		if (collector.isCurrentBufFull()) {
            			collector.nextStep();
            		}
            		s = is.read(collector.getCurrentBuf(), collector.getCurrentStartFreeIndex(), collector.getCurrentFreeSize());
            	}
            	if (collector.getGlobalSize() > 0) {
	            	mData = collector.makeOne();
	        		Utils.platformLog("MapengineRequest", "requestData() success received data with size2 = "+String.valueOf(collector.getGlobalSize()));
            	}
            	else {
            		Utils.platformLog("MapengineRequest", "requestData() InputStream is EMPTY !2");
            	}
            }
            else {
        		Utils.platformLog("MapengineRequest", "requestData() InputStream is NULL !");
            }
        }            
        catch (Exception e) {
    		Utils.platformLog("MapengineRequest", "requestData() EXCEPTION :"+e.getMessage());
        	e.printStackTrace();
        	mData = null;
        }
        finally {
            if (is != null) {
                try {
                    	is.close();
                    } 
                catch (IOException e) {
                		Utils.platformLog("MapengineRequest", "requestData() IO EXCEPTION :"+e.getMessage());
                    	e.printStackTrace();
                }
            }
        }		
	}
	
	public int getDataSize() {
		byte[] ar = mData;
		if (ar == null) {
			return 0;
		}
		return ar.length;
	}
	
	public byte[] getData() {
		return mData;
	}
	
	private byte[] mData; 
	private int mID;
	private HttpGet mRequest;
}