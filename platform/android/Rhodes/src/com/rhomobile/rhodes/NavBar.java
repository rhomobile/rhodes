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

package com.rhomobile.rhodes;

import java.util.Map;

import com.rhomobile.rhodes.util.PerformOnUiThread;

public class NavBar {

	private static final String TAG = "NavBar";
	
	private static boolean started = false;
	
	private static class CreateTask implements Runnable {
		
		private String title;
		private Map<Object, Object> left;
		private Map<Object, Object> right;
		
		@SuppressWarnings("unchecked")
		public CreateTask(Map<Object, Object> p) {
			Object titleObj = p.get("title");
			if (titleObj == null || !(titleObj instanceof String))
				throw new IllegalArgumentException("'title' should be String");
			title = (String)titleObj;
			
			Object leftObj = p.get("left");
			if (leftObj == null || !(leftObj instanceof Map<?,?>))
				throw new IllegalArgumentException("'left' - expected Hash");
			left = (Map<Object,Object>)leftObj;
			
			Object rightObj = p.get("right");
			if (rightObj != null && !(rightObj instanceof Map<?,?>))
				throw new IllegalArgumentException("'right' - expected Hash");
			right = (Map<Object,Object>)rightObj;
		}
		
		public void run() {
			RhodesService r = RhodesService.getInstance();
			r.getMainView().addNavBar(title, left, right);
			started = true;
		}
	};
	
	private static class RemoveTask implements Runnable {
		public void run() {
			RhodesService r = RhodesService.getInstance();
			r.getMainView().removeNavBar();
			started = false;
		}
	};
	
	private static void reportFail(String name, Exception e) {
		Logger.E(TAG, "Call of \"" + name + "\" failed: " + e.getMessage());
	}
	
	public static void create(Map<Object, Object> params) {
		try {
			PerformOnUiThread.exec(new CreateTask(params), false);
		}
		catch (Exception e) {
			reportFail("create", e);
		}
	}
	
	public static void remove() {
		try {
			PerformOnUiThread.exec(new RemoveTask(), false);
		}
		catch (Exception e) {
			reportFail("remove", e);
		}
	}
	
	public static boolean isStarted() {
		return started;
	}
}
