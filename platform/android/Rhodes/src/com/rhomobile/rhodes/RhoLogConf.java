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

	public native String getEnabledCategories();
	public native void setEnabledCategories(String c);
	
	public native String getDisabledCategories();
	public native void setDisabledCategories(String c);
	
	public native int getMinSeverity();
	public native void setMinSeverity(int s);
	
	public native void saveToFile();
	public native void clearLog();
	
	public native String getLogText();
	public native int getLogTextPos();
	
	public native void sendLog();
}
