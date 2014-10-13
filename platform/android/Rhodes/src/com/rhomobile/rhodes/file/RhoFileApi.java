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

package com.rhomobile.rhodes.file;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.util.Utils;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.os.ParcelFileDescriptor;
import android.util.Log;

public class RhoFileApi {
	
	private static final String TAG = RhoFileApi.class.getSimpleName();
	
	private static final int MAX_SIZE = 2*1024*1024;
	
	private static final String STAT_TABLE_FILENAME = "rho.dat";
	
	private static AssetManager am;
	private static String root;
    private static String sharedPath;
    private static final String DB_FILES_FOLDER = "db/db-files";
    private static final String TMP_FOLDER = "tmp";

	private static native void nativeInitPath(String rootPath, String sqliteJournalsPath, String apkPath, String sharedPath);
	private static native void nativeInitLogPath(String path);
	private static native void nativeInit();
    private static native void nativeInitAssetManager(AssetManager assetMgr);
	
	/**
	 * Replaces '\' (back slashes) with '/' slashes. Useful for changing Windows paths to more URI friendly strings   
	 * @param path The path to convert
	 * @return The converted path
	 */
	public static native String normalizePath(String path);
    public static native String absolutePath(String path);
	
	private static native boolean needEmulate(String path);
	private static native String makeRelativePath(String path);
	
    public static native void setFsModeTransparrent(boolean transparrent);
	public static native void removeBundleUpgrade();

    private static native void processStatTable(boolean emulateFileTree, boolean resetFileTree) throws IOException;

	private static void copyAssets(String assets[])
	{
		for(String asset: assets)
		{
			Log.i(TAG, "Forsing file: " + asset);
			forceFile(asset);
		}
	}

	public static String initRootPath(String dataDir, String sourceDir, String sharedDir) {
		
		root = dataDir + "/rhodata/";
		String sqliteJournals = dataDir + "/sqlite_stmt_journals/";
		
		File f = new File(getRootPath());
		f.mkdirs();
		f = new File(getDbFilesPath());
		f.mkdirs();
		f = new File(sqliteJournals);
		f.mkdirs();
		f = new File(getTempPath());
		f.mkdirs();
		
		String apkPath = sourceDir;
		
		if (sharedDir == null || sharedDir.length() == 0) {
		    sharedPath = root;
		} else {
		    sharedPath = sharedDir;
		}
		
        Log.d(TAG, "App root path: " + root);
        Log.d(TAG, "Sqlite journals path: " + sqliteJournals);
        Log.d(TAG, "Shared path: " + sharedDir);

        nativeInitPath(root, sqliteJournals, apkPath, sharedPath);
        return root;
    }

    public static void initLogPath(String path) {
        nativeInitLogPath(path);
    }

    public static String getRootPath() { return root; }
    public static String getDbFilesUriPath() { return DB_FILES_FOLDER; }
    public static String getDbFilesPath() { return new File(getRootPath(), DB_FILES_FOLDER).getAbsolutePath(); }
    public static String getTempPath() { return new File(getRootPath(), TMP_FOLDER).getAbsolutePath(); }

    public static void basicSetup(Context ctx, boolean resetFileTree)
    {
        am = ctx.getAssets();
        nativeInitAssetManager(am);
        nativeInit();

        if (resetFileTree) {
            Log.i(TAG, "Application hash was changed");
            
            File rho_dat = new File(getRootPath(), "rho.dat");
            if (rho_dat.exists()) {
                Log.i(TAG, "Removing rho.dat from file system");
                rho_dat.delete();
            }
            
            RhoFileApi.initialCopy(ctx, new String[] {"hash", "apps/rhoconfig.txt"});
        }
    }
    
    public static void finalSetup(Context ctx, boolean resetFileTree, boolean emulateFileTree) throws IOException
    {

        if (resetFileTree) {
            Logger.T(TAG, "Removing bundle upgrade");
            
            removeBundleUpgrade();
            copy("apps/rhoconfig.txt");

            File libDir = new File(getRootPath(), "lib");
            File testLib = new File(libDir.getPath(), "rhoframework.iseq");
            if (libDir.isDirectory() && testLib.isFile())
            {
                Logger.W(TAG, "Updating from very old rhodes version, clean filesystem.");
                Utils.deleteChildrenIgnoreFirstLevel(new File(getRootPath(), "apps"), "rhoconfig.txt");
                Utils.deleteRecursively(libDir);
            }

            ApplicationInfo appInfo = ctx.getApplicationInfo();
            Logger.T(TAG, "Reinitializing root path");
            initRootPath(appInfo.dataDir, appInfo.sourceDir, sharedPath);
        }

        processStatTable(emulateFileTree, resetFileTree && !emulateFileTree);
    }

    public static void initialCopy(Context ctx, String assets[])
    {
        am = ctx.getAssets();
        for(String asset: assets)
        {
            copy(makeRelativePath(getRootPath() + asset));
        }
    }

	public static boolean copy(String path)
	{
		Log.d(TAG, "Copy " + path + " to FS");
		InputStream is = null;
		OutputStream os = null;
		try {
			is = am.open(path);
			
			File dst = new File(root, path);
			File parent = dst.getParentFile();
			if (parent == null)
				return false;
			parent.mkdirs();
			
			os = new FileOutputStream(dst);
			byte[] buf = new byte[4096];
			int len;
			while((len = is.read(buf)) > 0)
				os.write(buf, 0, len);
			
			Log.d(TAG, "File " + path + " copied");
			return true;
		}
		catch (Exception e) {
			Log.e(TAG, "Can not copy " + path + " to FS");
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
		if (needEmulate(path)) {
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

    private static void forceFile(String path)
    {
        String relPath = makeRelativePath(path);
        if(needEmulate(relPath)) {
            if(copy(relPath)) {
                Logger.D(TAG, "File extracted from package to file system: " + path);
            }
        }
    }

    public static FileDescriptor openFd(String path)
    {
        forceFile(path);
        try {
            Logger.D(TAG, "Opening file from file system: " + absolutePath(path));

            FileInputStream fs = new FileInputStream(absolutePath(path));
            return fs.getFD();
        } catch (Exception e) {
            Logger.E(TAG, "Can not open file descriptor: " + e.getMessage());
            return null;
        }
    }

    public static ParcelFileDescriptor openParcelFd(String path)
    {
        try {
            if(needEmulate(path)) {
                return null;
            } else {
                String absPath = absolutePath(path);

                Logger.D(TAG, "Opening file from file system: " + absPath);
    
                File file = new File(absPath);
                return ParcelFileDescriptor.open(file, ParcelFileDescriptor.MODE_READ_ONLY);
            }
        } catch (FileNotFoundException e) {
            Logger.E(TAG, "Can not open ParcelFileDescriptor" + e.getMessage());
            return null;
        }
    }
    
    public static AssetFileDescriptor openAssetFd(String path) {
        try {
            if(needEmulate(path)) {
                String relPath = makeRelativePath(path);

                Logger.D(TAG, "Opening file from assets: " + relPath);

                return am.openFd(relPath);
                
            } else {
                String absPath = absolutePath(path);

                Logger.D(TAG, "Opening file from file system: " + absPath);

                return am.openNonAssetFd(absPath);
            }
        } catch (IOException e) {
            Logger.E(TAG, "Can not open AssetFileDescriptor: " + path);
            Logger.E(TAG, e);
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
		catch (IOException e) {
			Log.e(TAG, e.getMessage());
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
	
	public static String[] getChildren(String path) {
		try {
			// Merge children from both packed assets and file system 
			String[] list1 = am.list(path);
			File f = new File(root, path);
			String[] list2 = f.list();
			
			List<String> list = new ArrayList<String>();
			if (list1 != null)
				for (String child : list1)
					list.add(child);
			if (list2 != null)
				for (String child : list2) {
					if (list.contains(child))
						continue;
					list.add(child);
				}
			
			return list.toArray(new String[] {});
		}
		catch (IOException e) {
			return null;
		}
	}

    public static int copyRecursively(String srcPath, String trgPath) {
        try {
            File source = new File(srcPath);
            File target = new File(trgPath);
            Utils.copyRecursively(new Utils.FileSource(), source, target, true);
            return 0;
        } catch (Throwable ex) {
            Logger.E(TAG, ex.getMessage());
            return -1;
        }
    }

    public static int deleteRecursively(String path) {
        try {
            Utils.deleteRecursively(new File(path));
            return 0;
        } catch (Throwable ex) {
            Logger.E(TAG, ex.getMessage());
            return -1;
        }
    }
    
}
