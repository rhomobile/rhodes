package com.rhomobile.rhodes;

import android.app.ActivityManager;
import android.content.Context;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class MemoryInfoCollector {
	public static String collect() {
		StringBuilder builder = new StringBuilder();
		builder.append("Memory usage:\n");

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
			BufferedReader reader = new BufferedReader( new FileReader("/proc/meminfo"));
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
}
