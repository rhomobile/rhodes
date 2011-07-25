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

package com.rho;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyProgram;
import com.rho.net.NetResponse;
import com.rho.file.*;

public interface IRhoRubyHelper {
	public abstract void initRubyExtensions();
	public abstract RubyProgram createMainObject() throws Exception;
	public abstract String getPlatform();
	
	public abstract IFileAccess createFileAccess();
	public abstract IRAFile createRAFile();
	public abstract IRAFile createFSRAFile();
	
	public void loadBackTrace(RubyArray backtrace);
	public String getDeviceId();
	
	public String getAppProperty(String name);
	public String getModuleName();
	public boolean isSimulator();
	
	public void showLog();
	
	public String getGeoLocationText();
	public void wakeUpGeoLocation();
	
	public NetResponse postUrl(String url, String body);
	public void postUrlNoWait(String url, String body);
	public NetResponse postUrlSync(String url, String body)throws Exception;
	
	public void navigateUrl(String url);
	public void navigateBack();
	public void app_exit();
	
	public void unzip_file(String strPath)throws Exception;
}
