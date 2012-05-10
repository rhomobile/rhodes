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
import java.util.Hashtable;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import android.graphics.Bitmap;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.util.Utils;

public class MapBitmapManager {
	
	private static final String TAG = MapBitmapManager.class.getSimpleName();
	
	
	private static int ourBitmapGlobalID = 0;
	
	private class BitmapHolder {
		public Bitmap bitmap;
		public int refCount;
	}
	
	
	
	private Hashtable<Integer, BitmapHolder> mBitmaps = null;
	
	private static MapBitmapManager ourManager = null;
	
	public MapBitmapManager() {
		mBitmaps = new Hashtable<Integer, BitmapHolder>();
	}
	
	public static MapBitmapManager getSharedInstance() {
		if (ourManager == null) {
			ourManager = new MapBitmapManager();
		}
		return ourManager;
	}
	
	public Bitmap getBitmap(int id) {
		BitmapHolder bh = mBitmaps.get(new Integer(id));
		if (bh == null) {
			return null;
		}
		return bh.bitmap;
	}
	
	public int addBitmap(Bitmap bitmap) {
		int id = 0;
		synchronized (mBitmaps) {
			BitmapHolder bh = new BitmapHolder();
			bh.bitmap = bitmap;
			bh.refCount = 1;
			id = ++ourBitmapGlobalID;
			mBitmaps.put(new Integer(id), bh);
		}
		//Logger.I(TAG, "addBitmap() return "+String.valueOf(id));
		return id;
	}
	
	public void releaseBitmap(int id) {
		//Logger.I(TAG, "releaseBitmap("+String.valueOf(id)+")");
		synchronized (mBitmaps) {
			BitmapHolder bh = mBitmaps.get(new Integer(id));
			if (bh != null) {
				bh.refCount--;
				if (bh.refCount <= 0) {
					//Logger.I(TAG, "releaseBitmap() REAL REMOVE");
					bh.bitmap.recycle();
					mBitmaps.remove(new Integer(id));
				}
			}
		}
	}
	
	public void addRef(int id) {
		BitmapHolder bh = mBitmaps.get(new Integer(id));
		if (bh != null) {
			bh.refCount++;
		}
	}
	
	public void totalClean() {
		synchronized (mBitmaps) {
			mBitmaps.clear();
		}
	}


}