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

import com.rhomobile.rhodes.LocalFileProvider;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.util.PerformOnUiThread;
import com.rhomobile.rhodes.util.Utils;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.os.ParcelFileDescriptor;
import android.view.View;
import android.webkit.WebView;
import android.net.Uri;
import android.content.Intent;

public class RhoFileApi {
	
	private static final String TAG = RhoFileApi.class.getSimpleName();
	
	private static final int MAX_SIZE = 2*1024*1024;
	
	private static final String STAT_TABLE_FILENAME = "rho.dat";
	private static final String FORCE_ASSETS_FLAG_FILE = "assets_forced.txt";
	
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

    
    static void reloadStatTable() {
        Logger.I(TAG, "reloadStatTable()");
        try {
        	//setFsModeTransparrent(false);
        	processStatTable( !RhoConf.isExist("useAssetFS") || RhoConf.getBool("useAssetFS"), false);
        }
        catch (Throwable e) {
        	e.printStackTrace();
            Logger.E(TAG, "Exception during update Stat Table !!!");
        }
    }

	private static void copyAssets(String assets[])
	{
		for(String asset: assets)
		{
			Logger.I(TAG, "Forsing file: " + asset);
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
		
        Logger.D(TAG, "App root path: " + root);
        Logger.D(TAG, "Sqlite journals path: " + sqliteJournals);
        Logger.D(TAG, "Shared path: " + sharedDir);

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

        if (resetFileTree) {
            Logger.I(TAG, "Application hash was changed");
            
            File rho_dat = new File(getRootPath(), "rho.dat");
            if (rho_dat.exists()) {
                Logger.I(TAG, "Removing rho.dat from file system");
                rho_dat.delete();
            }
            
            RhoFileApi.initialCopy(ctx, new String[] {"hash", "apps/rhoconfig.txt"});
        }
		nativeInit();
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

	private static void doForceAllFilesForContext(Context ctx) {
		
		// clear cache in WebView
		PerformOnUiThread.exec(new Runnable() {
			public void run() {
				View v = RhoExtManager.getInstance().getWebView().getView();
				if (v instanceof WebView) {
					WebView wv = (WebView)v;
					wv.clearCache(true);
					Logger.D(TAG, "%% WebCache was cleaned !!!");
				}
			}
		});

		// check for already forced files
		File assets_forced_file = new File(root, FORCE_ASSETS_FLAG_FILE);
		if (assets_forced_file.exists()) {
			Logger.D(TAG, "doForceAllFilesForContext() SKIP because founded flag file %%");
			return;
		}
		try {
			File parent = assets_forced_file.getParentFile();
			if (parent != null)
				parent.mkdirs();
			OutputStream os = new FileOutputStream(assets_forced_file);
			os.write("assets_forced".getBytes());
			os.close();
		} catch (Exception e1) {
			// TODO Auto-generated catch block
			Logger.D(TAG, "doForceAllFilesForContext() ERROR with creation of flag file %%");
			e1.printStackTrace();
		}
		
        try {
        	//setFsModeTransparrent(false);
        	//processStatTable( !RhoConf.isExist("useAssetFS") || RhoConf.getBool("useAssetFS"), true);
        }
        catch (Throwable e) {
        	e.printStackTrace();
            Logger.E(TAG, "Exception during update Stat Table with copy files from bundle to filesystem !!!");
        }
		
		///*
		try {
	       InputStream is = null;
	        try {
	
	            File statFile = new File(getRootPath(), STAT_TABLE_FILENAME);
	            if (statFile.exists() && statFile.isFile()) {
	                Logger.I(TAG, "Opening stat table from FS: " + statFile.getCanonicalPath());
	                is = new FileInputStream(statFile);
	            } else {
	                Logger.I(TAG, "Opening stat table from package assets");
	                is = am.open("rho.dat");
	            }
	
	            BufferedReader in = new BufferedReader(new InputStreamReader(is, "UTF-8"));
				for (;;) {
					String line = in.readLine();
					if (line == null)
						break;
					
					int idx = line.indexOf('|');
					if (idx == -1)
						continue;
					String path = line.substring(0, idx);
					line = line.substring(idx + 1);
					idx = line.indexOf('|');
					if (idx == -1)
						continue;
					String type = line.substring(0, idx);
					line = line.substring(idx + 1);
					idx = line.indexOf('|');
					if (idx == -1)
						continue;
					
					//updateStatTable(path, type, size, mtime);
					if ("dir".equals(type)) {
						path = path + "/ttt.txt";
						File dst = new File(root, path);
						File parent = dst.getParentFile();
						if (parent != null) {
							Logger.D(TAG, "doForceAllFilesForContext() %% make dirs for = "+path);
							parent.mkdirs();
						}
					}
				}
			}
			finally {
				if (is != null)
					is.close();
			}
		}
		catch(Exception e) {
			Logger.E(TAG, "doForceAllFilesForContext() Error during make dirs = "+e.getMessage());
		}
		
		AssetManager amam = ctx.getAssets();
		
		try {
			initialCopy(ctx, amam.list(""));
		} catch (IOException e) {
			Logger.E(TAG, "%% Can not force all files !!!");
			e.printStackTrace();
		}
		//*/
	}
	
	public static void doForceAllFiles() 
	{
		Context ctx = RhodesService.getContext();
		doForceAllFilesForContext(ctx);
	}

    public static void initialCopy(Context ctx, String assets[])
    {
        am = ctx.getAssets();
        for(String asset: assets){
            //copy(makeRelativePath(getRootPath() + asset));
			copyFileOrFolder(ctx, makeRelativePath(getRootPath() + asset));
        }
    }

    public static void copyFileOrFolder(Context ctx, String path) {
		// check for file
        am = ctx.getAssets();
    	try {
			String[] l = am.list(path);
			if (l.length > 0) {
				// folder !
				for (int i = 0; i < l.length; ++i) {
					copyFileOrFolder(ctx, path + "/" + l[i]);
	            }
			}
			else {
				// file
				copy(path);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
		
		
    	
    }
    
	public static boolean copy(String path)
	{

		Logger.D(TAG, "%% Copy " + path + " to FS");
		InputStream is = null;
		OutputStream os = null;
		try {
			File dst = new File(root, path);
			File parent = dst.getParentFile();
			if (parent == null)
				return false;
			parent.mkdirs();
			
			is = am.open(path);
			//AssetFileDescriptor fs = am.openFd(path);
			
			os = new FileOutputStream(dst);
			byte[] buf = new byte[4096];
			int len;
			while((len = is.read(buf)) > 0)
				os.write(buf, 0, len);
			
			Logger.D(TAG, "File " + path + " copied");
			return true;
		}
		catch (Exception e) {
			e.printStackTrace();
			Logger.E(TAG, "Can not copy " + path + " to FS with ERROR = "+e.getMessage());
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
		//Logger.D(TAG, "open: " + path);
		if (needEmulate(path)) {
			String relPath = makeRelativePath(path);
			//Logger.D(TAG, "open: (1): " + relPath);
			return openInPackage(relPath);
		}
		
		try {
			//Logger.D(TAG, "open (2): " + path);
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
			if (needEmulate(path)) {
				Context ctx = RhodesService.getContext();
				String rel = makeRelativePath(path);
				try{
					ensureFileCopied(ctx, rel);
				} catch (IOException e) {
					Logger.D(TAG, "Can not copy asset file: " + rel + " - " + e.getMessage());
					return null;
				}
			}
			String absPath = absolutePath(path);

			Logger.D(TAG, "Opening file from file system: " + absPath);
    
			File file = new File(absPath);
			return ParcelFileDescriptor.open(file, ParcelFileDescriptor.MODE_READ_ONLY);
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

				File file = new File(absPath);
				ParcelFileDescriptor pfd = ParcelFileDescriptor.open(file, ParcelFileDescriptor.MODE_READ_ONLY);
				return new AssetFileDescriptor(pfd, 0, file.length());
            }
        } catch (IOException e) {
            Logger.E(TAG, "Can not open AssetFileDescriptor: " + path);
            Logger.E(TAG, e);
            return null;
        }
    }

	public static File ensureFileCopied(Context ctx, String assetPath) throws IOException {
		File dest = new File(
				ctx.getApplicationInfo().dataDir + "/rhodata/apps/public",
				new File(assetPath).getName()
		);

		if (!dest.exists()) {
			dest.getParentFile().mkdirs();

			try (InputStream is = ctx.getAssets().open(assetPath);
				FileOutputStream os = new FileOutputStream(dest)) {
				byte[] buf = new byte[8192];
				int n;
				while ((n = is.read(buf)) != -1) os.write(buf, 0, n);
			}
		}
		return dest;
	}

	public static InputStream openInPackage(String path)
	{
		try {
			Logger.D(TAG, "Open in package " + path + "...");
			InputStream is = am.open(path, AssetManager.ACCESS_RANDOM);
			is.mark(MAX_SIZE);
			return is;
		}
		catch (IOException e) {
			Logger.E(TAG, e.getMessage());
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