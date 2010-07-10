package com.rhomobile.rhodes.file;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

import com.rhomobile.rhodes.Rhodes;
import com.rhomobile.rhodes.RhodesInstance;

import android.content.res.AssetManager;
//import android.util.Log;

public class RhoFileApi {
	
	//private static final String TAG = "RhoFileApiJava";
	
	private static final int MAX_SIZE = 2*1024*1024;
	
	private static AssetManager am;
	
	private static native void nativeInit();
	
	public static void init()
	{
		nativeInit();
	
		Rhodes r = RhodesInstance.getInstance();
		am = r.getAssets();
	}
	
	public static boolean copy(String path)
	{
		InputStream is = null;
		OutputStream os = null;
		try {
			is = am.open(path);
			File dst = new File(RhodesInstance.getInstance().getRootPath(), path);
			File parent = dst.getParentFile();
			if (parent == null)
				return false;
			parent.mkdirs();
			
			os = new FileOutputStream(dst);
			byte[] buf = new byte[4096];
			int len;
			while((len = is.read(buf)) > 0)
				os.write(buf, 0, len);
			
			return true;
		}
		catch (Exception e) {
			return false;
		}
		finally {
			try {
				if (is != null)
					is.close();
				if (os != null)
					os.close();
			}
			catch (Exception e1) {
				// Ignore
			}
		}
	}
	
	public static boolean exists(String path)
	{
		try {
			//Log.d(TAG, "Check " + path + "...");
			InputStream is = am.open(path, AssetManager.ACCESS_RANDOM);
			is.close();
			return true;
		}
		catch (Exception e) {
			//Log.e(TAG, "Can not open " + path);
			return false;
		}
	}
	
	public static long size(String path)
	{
		InputStream is = null;
		try {
			//Log.d(TAG, "Read size of " + path + "...");
			is = am.open(path, AssetManager.ACCESS_RANDOM);
			long total = 0;
			for (;;) {
				long ret = is.skip(4096);
				if (ret == 0)
					break;
				total += ret;
			}
			return total;
		}
		catch (Exception e) {
			//Log.e(TAG, "Can not open " + path);
			return -1;
		}
		finally {
			try {
				if (is != null)
					is.close();
			}
			catch (Exception e1) {
				// Ignore
			}
		}
	}
	
	public static InputStream open(String path)
	{
		try {
			//Log.d(TAG, "Open " + path + "...");
			InputStream is = am.open(path, AssetManager.ACCESS_RANDOM);
			is.mark(MAX_SIZE);
			return is;
		}
		catch (Exception e) {
			//Log.e(TAG, "Can not open " + path);
			return null;
		}
	}
	
	public static void close(InputStream is)
	{
		try {
			if (is == null)
				return;
			is.close();
		}
		catch (Exception e) {
			// Ignore
		}
	}
	
	public static int read(InputStream is, byte[] buf)
	{
		try {
			int n = is.read(buf);
			return n == -1 ? 0 : n;
		}
		catch (Exception e) {
			return -1;
		}
	}
	
	public static void seek(InputStream is, int offset)
	{
		try {
			is.reset();
			for (long total = 0; total < offset;) {
				long n = is.skip(offset - total);
				if (n == 0)
					break;
				total += n;
			}
		}
		catch (Exception e) {
			// Ignore
		}
	}
	
}
