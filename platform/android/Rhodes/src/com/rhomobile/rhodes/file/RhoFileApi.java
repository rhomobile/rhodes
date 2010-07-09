package com.rhomobile.rhodes.file;

import java.io.IOException;
import java.io.InputStream;

import com.rhomobile.rhodes.Rhodes;
import com.rhomobile.rhodes.RhodesInstance;

import android.content.res.AssetManager;
import android.util.Log;

public class RhoFileApi {
	
	private static final String TAG = "RhoFileApiJava";
	
	private static String rootPath;
	private static AssetManager am;

	private static native void init();
	
	private static String getRelativePath(String fullPath)
	{
		if (!fullPath.startsWith(rootPath))
			throw new IllegalArgumentException();
		return fullPath.substring(rootPath.length());
	}
	
	public static void initAll()
	{
		init();
	
		Rhodes r = RhodesInstance.getInstance();
		rootPath = r.getRootPath();
		am = r.getAssets();
	}
	
	public static int open(String fullPath)
	{
		// TODO: implement
		return -1;
	}
	
	public static int statType(String fullPath)
	{
		Log.d(TAG, "statType: " + fullPath);
		
		String relPath = getRelativePath(fullPath);
		try {
			InputStream is = am.open(relPath, AssetManager.ACCESS_RANDOM);
			is.close();
			// It is file
			return 1;
		}
		catch (Exception e) {
			try {
				am.list(relPath);
				// It is directory
				return 2;
			} catch (IOException e1) {
				return -1;
			}
		}
	}
	
	public static int statSize(String fullPath)
	{
		// TODO: implement
		return -1;
	}
	
}
