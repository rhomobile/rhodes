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

package com.rhomobile.rhodes.osfunctionality;

import android.os.Build;


public class AndroidFunctionalityManager {

private static AndroidFunctionality ourFunctionality = null;
	
	public static AndroidFunctionality getAndroidFunctionality() {
		if (ourFunctionality == null) {
			int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
			if (sdkVersion >= 10) {
				ourFunctionality = new AndroidFunctionality10();
			}
			else if (sdkVersion >= 9) {
				ourFunctionality = new AndroidFunctionality09();
			}
			else if (sdkVersion >= 8) {
				ourFunctionality = new AndroidFunctionality08();
			}
			else if (sdkVersion >= 7) {
				ourFunctionality = new AndroidFunctionality07();
			}
			else if (sdkVersion >= 6) {
				ourFunctionality = new AndroidFunctionality06();
			}
			else if (sdkVersion >= 5) {
				ourFunctionality = new AndroidFunctionality05();
			}
			else if (sdkVersion >= 4) {
				ourFunctionality = new AndroidFunctionality04();
			}
			else if (sdkVersion >= 3) {
				ourFunctionality = new AndroidFunctionality03();
			}
			else if (sdkVersion >= 2) {
				ourFunctionality = new AndroidFunctionality02();
			}
			else {
				ourFunctionality = new AndroidFunctionality01();
			}
		}
		return ourFunctionality;
	}
	
}