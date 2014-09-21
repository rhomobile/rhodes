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

import com.rhomobile.rhodes.extmanager.IRhoConfig;

public class RhoConf {
    public static native void setPath(String path);

	public static native String getString(String name);
	public static native int getInt(String name);
	public static native boolean getBool(String name);
	
	public static native void setString(String name, String value);
	public static native void setInt(String name, int value);
	public static native void setBoolean(String name, boolean value);
	
	public static native boolean isExist(String name);

    public static class RhoConfig implements IRhoConfig {

        @Override
        public boolean isExist(String name) {
            return RhoConf.isExist(name);
        }

        @Override
        public String getString(String name) {
            return RhoConf.getString(name);
        }

        @Override
        public boolean getBool(String name, boolean defValue) {
            return isExist(name) ? RhoConf.getBool(name) : defValue;
        }

        @Override
        public int getInt(String name, int defValue) {
            return isExist(name) ? RhoConf.getInt(name) : defValue;
        }

        @Override
        public double getDouble(String name, double defValue) {
            return isExist(name) ? Double.parseDouble(RhoConf.getString(name)) : defValue;
        }
	    
	}
}
