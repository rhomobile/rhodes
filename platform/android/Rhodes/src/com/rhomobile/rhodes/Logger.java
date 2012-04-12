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

import java.io.CharArrayWriter;
import java.io.PrintWriter;
import java.io.Writer;
import android.app.ActivityManager;
import android.content.Context;

import java.io.*;

public class Logger {

    private static boolean mMemoryInfoDumpEnabled = false;
    private static long mMemoryInfoDumpInterval = 0;
    private static long mLastTimeMemoryInfoDumped = 0;

	// Errors
	public static void E(String tag, String msg) {
        eNative(tag, appendMemoryDumpIfNeeded(msg));
    }
	// Warnings
	public static void W(String tag, String msg) {
        wNative(tag, appendMemoryDumpIfNeeded(msg));
    }
	// Info
	public static void I(String tag, String msg) {
        iNative(tag, appendMemoryDumpIfNeeded(msg));
    }
	// Debug
	public static void D(String tag, String msg) {
        dNative(tag, appendMemoryDumpIfNeeded(msg));
    }
	// Trace
	public static void T(String tag, String msg) {
        tNative(tag, appendMemoryDumpIfNeeded(msg));
    }
	
	public static void E(String tag, Throwable e) {
	    Writer bufWriter = new CharArrayWriter(256);
	    PrintWriter writer = new PrintWriter(bufWriter);
	    e.printStackTrace(writer);

	    if (writer.checkError()) {
	        E(tag, e.getMessage());
	        E("Logger", "Cannot print stack trace");
	    }

		E(tag, bufWriter.toString());
	}
	
	public static void W(String tag, Throwable e) {
		W(tag, e.getMessage());
	}
	
	public static void I(String tag, Throwable e) {
		I(tag, e.getMessage());
	}
	
	public static void D(String tag, Throwable e) {
		D(tag, e.getMessage());
	}
	
	public static void T(String tag, Throwable e) {
		T(tag, e.getMessage());
	}

    public static void enableMemoryInfoDump(long interval) {
        mMemoryInfoDumpEnabled = true;
        mMemoryInfoDumpInterval = interval;
    }

    public static void disableMemoryInfoDump() {
        mMemoryInfoDumpEnabled = false;
    }

    private static String appendMemoryDumpIfNeeded( String msg ) {
        long now = System.currentTimeMillis();
        if ( mMemoryInfoDumpEnabled && ( ( now - mLastTimeMemoryInfoDumped) > mMemoryInfoDumpInterval) ) {
            mLastTimeMemoryInfoDumped = now;
            return msg + "\n" + getMemoryDump();
        }
        return msg;
    }

    private static String getMemoryDump() {
        StringBuilder builder = new StringBuilder();
        builder.append("It's time to dump memory info:\n");

        {
            builder.append("Contents of /proc/meminfo:\n");
            builder.append(getProcMeminfoContents());
            builder.append("\n");
        }

        {
            builder.append("MemoryInfo from ActivityService:\n");
            builder.append( getActivitiManagerMemInfo() );
            builder.append("\n");
        }

        return builder.toString();
    }

    private static String getProcMeminfoContents() {
        try {
            BufferedReader reader = new BufferedReader( new FileReader ("/proc/meminfo"));
            String line  = null;
            StringBuilder stringBuilder = new StringBuilder();
            String ls = System.getProperty("line.separator");
            int nLines = 0;
            //get only first 5 lines
            while( (( line = reader.readLine() ) != null) && ( nLines++ < 5 ) ) {

                stringBuilder.append( line );
                stringBuilder.append( ls );
            }
            return stringBuilder.toString();
        } catch (IOException e) {
            return "Can't get contents of /proc/meminfo";
        }
    }

    private static String getActivitiManagerMemInfo() {
		try {
			Context context = RhodesService.getContext();
			ActivityManager activityManager = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
			ActivityManager.MemoryInfo memoryInfo = new ActivityManager.MemoryInfo();
			activityManager.getMemoryInfo(memoryInfo);

			StringBuilder builder = new StringBuilder();

			builder.append("memoryInfo.availMem ").append(memoryInfo.availMem).append("\n");
			builder.append("memoryInfo.lowMemory ").append(memoryInfo.lowMemory).append("\n");
			builder.append("memoryInfo.threshold ").append(memoryInfo.threshold).append("\n");

			return builder.toString();
		} catch ( IllegalStateException e ) {
			return "Can't get memory info from ActivityManager";
		}


    }

    // Errors
    private native static void eNative(String tag, String msg);
    // Warnings
    private native static void wNative(String tag, String msg);
    // Info
    private native static void iNative(String tag, String msg);
    // Debug
    private native static void dNative(String tag, String msg);
    // Trace
    private native static void tNative(String tag, String msg);
}
