package com.rhomobile.rhodes.file;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;

import com.rhomobile.rhodes.RhodesService;

import android.content.res.AssetManager;
//import android.util.Log;

public class RhoFileApi {
	
	//private static final String TAG = "RhoFileApiJava";
	
	private static final int MAX_SIZE = 2*1024*1024;
	
	private static AssetManager am;
	
	private static native void nativeInit();
	private static native void updateStatTable(String path, String type, long size, long mtime);
	
	public static native String normalizePath(String path);
	
	private static native boolean needJavaWay(String path);
	private static native String makeRelativePath(String path);
	
	private static void fillStatTable() throws IOException
	{
		InputStream is = null;
		try {
			is = am.open("rho.dat");
			BufferedReader in = new BufferedReader(new InputStreamReader(is, "UTF-8"));
			for (;;) {
				String line = in.readLine();
				if (line == null)
					break;
				
				int idx = line.indexOf('\t');
				if (idx == -1)
					continue;
				String path = line.substring(0, idx);
				line = line.substring(idx + 1);
				idx = line.indexOf('\t');
				if (idx == -1)
					continue;
				String type = line.substring(0, idx);
				line = line.substring(idx + 1);
				idx = line.indexOf('\t');
				if (idx == -1)
					continue;
				long size = Long.parseLong(line.substring(0, idx));
				long mtime = Long.parseLong(line.substring(idx + 1));
				
				updateStatTable(path, type, size, mtime);
			}
		}
		finally {
			if (is != null)
				is.close();
		}
	}
	
	public static void init() throws IOException
	{
		nativeInit();
	
		//RhodesService r = RhodesService.getInstance();
		am = RhodesService.getInstance().getContext().getAssets();

		fillStatTable();
	}
	
	public static boolean copy(String path)
	{
		//Log.d(TAG, "Copy " + path + " to FS");
		InputStream is = null;
		OutputStream os = null;
		try {
			RhodesService r = RhodesService.getInstance();
			
			is = am.open(path);
			
			File dst = new File(r.getRootPath(), path);
			File parent = dst.getParentFile();
			if (parent == null)
				return false;
			parent.mkdirs();
			
			os = new FileOutputStream(dst);
			byte[] buf = new byte[4096];
			int len;
			while((len = is.read(buf)) > 0)
				os.write(buf, 0, len);
			
			//Log.d(TAG, "File " + path + " copied");
			return true;
		}
		catch (Exception e) {
			//Log.e(TAG, "Can not copy " + path + " to FS");
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
	
	public static InputStream open(String path)
	{
		//Log.d(TAG, "open: " + path);
		if (needJavaWay(path)) {
			String relPath = makeRelativePath(path);
			//Log.d(TAG, "open: (1): " + relPath);
			return openInPackage(relPath);
		}
		
		try {
			//Log.d(TAG, "open (2): " + path);
			return new FileInputStream(path);
		} catch (FileNotFoundException e) {
			return null;
		}
	}
	
	public static InputStream openInPackage(String path)
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
