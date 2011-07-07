/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
package com.rhomobile.rhodes;

public class RhoLogConf {

	public static native String getEnabledCategories();
	public static native void setEnabledCategories(String c);
	
	public static native String getDisabledCategories();
	public static native void setDisabledCategories(String c);
	
	public static native int getMinSeverity();
	public static native void setMinSeverity(int s);
	
	public static native void saveToFile();
	public static native void clearLog();
	
	public static native String getLogText();
	public static native int getLogTextPos();
	
	public static native void sendLog();
}
