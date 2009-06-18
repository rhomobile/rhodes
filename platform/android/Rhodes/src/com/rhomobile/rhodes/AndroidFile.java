package com.rhomobile.rhodes;

import java.io.File;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.RandomAccessFile;
import java.io.IOException;

import com.rho.SimpleFile;

import android.content.res.Resources;
import android.util.Log;

public class AndroidFile implements SimpleFile {
	final public static String RHODES_ROOT_DIR = "/sdcard/rhomobile/";

	static private String rootDir = null;

	private String currentFile;
	private FileInputStream fis = null;
	private FileOutputStream fos = null;
	private boolean readOnly = false;
	
	protected RandomAccessFile file;
	protected boolean noFlush;

	public AndroidFile() {
		try {
			if (rootDir == null) {
				Resources appR = RhodesInstance.getInstance()
						.getApplicationContext().getResources();
				CharSequence app_name = appR.getText(appR.getIdentifier(
						"app_name", "string", RhodesInstance.getInstance()
								.getApplicationContext().getPackageName()));

				rootDir = RHODES_ROOT_DIR + app_name + "/";
				File sddir = new File(rootDir);
				sddir.mkdirs();
			}
		} catch (Exception e) {
			Log.e(getClass().getSimpleName(), e.getMessage());
		}
	}

	public void write(long pos, byte[] buf) {
		try {
			file.seek(pos);
			file.write(buf, 0, buf.length);
		} catch (Exception e) {
			Log.e(getClass().getSimpleName(), e.getMessage());
		}
	}

	public int read(long pos, byte[] buf) {
		try {
			file.seek(pos);
			return file.read(buf, 0, buf.length);
		} catch (Exception e) {
			Log.e(getClass().getSimpleName(), e.getMessage());
		}
		return 0;
	}

	public void sync() {
		if (!noFlush) {
			try {
				file.getFD().sync();
			} catch (IOException e) {
				Log.e(getClass().getSimpleName(), e.getMessage());
			}
		}
	}

	public void close() {
		try {
			if ( file != null )
				file.close();
			
			if ( fis != null )
				fis.close();
			
			if ( fos != null )
				fos.close();
			
			currentFile = "";
		} catch (IOException e) {
			Log.e(getClass().getSimpleName(), e.getMessage());
		}
	}

	public void open(String filePath, boolean readOnly, boolean noFlush) {
		this.noFlush = noFlush;
		this.readOnly = readOnly;
		try {

			file = new RandomAccessFile(filePath, readOnly ? "r"
					: "rw");

		} catch (IOException x) {

			try {
				// try to create file
				FileWriter f = new FileWriter(filePath);
				f.close();

				file = new RandomAccessFile(filePath, readOnly ? "r"
						: "rw");

			} catch (IOException e) {
				Log.e(getClass().getSimpleName(), e.getMessage());
			}
		}

		currentFile = filePath;
	}

	public long length() {
		try {
			if ( file != null )
				return file.length();
			if ( fis != null )
				return fis.getChannel().size();
			if ( fos != null )
				return fos.getChannel().size();
		} catch (IOException x) {
			return -1;
		}
		
		return -1;
	}

	public void delete(String path) {
		try {
			File file = new File(path);
			file.delete();
		} catch (Exception e) {
		}
	}

	public String getDirPath(String path) {
		try {
			File sddir = new File(rootDir + path);
			sddir.mkdirs();
		} catch (Exception e) {
		}

		return rootDir + path;
	}

	public InputStream getInputStream() throws IOException {
		fis = new FileInputStream(this.currentFile);
		return fis;
	}

	public OutputStream getOutStream() {
		try
		{
			fos = new FileOutputStream( this.currentFile );
			return fos;
		}
		catch ( Exception e ){
			return null;
		}
	}

	public boolean isOpened() {
		return ( file != null || fis != null || fos != null );
	}

	public String readString() throws IOException {
		byte[] data = new byte[(int)length()];
	    int len = read(0,data);
	    if ( len == 0 )
	        return new String("");

	    return new String(data,0,len);
	}

	public void truncate(int size) throws IOException {
		
		if ( file != null )
		{
			if ( readOnly )
				getInputStream();
			else
				getOutStream();
		}
		
		if ( fis != null )
			fis.getChannel().truncate(size);
		
		if ( fos != null )
			fos.getChannel().truncate(size);
	}
	
	public InputStream getResourceAsStream(Class fromClass, String path) {

		// load resource from assets
		try {
			if (path.startsWith("/")) {
				return RhodesInstance.getInstance().getAssets().open(path.substring(1));
			} else {
				return RhodesInstance.getInstance().getAssets().open(path);
			}
		} catch (IOException e) {
			Log.w("AndroidFile", e.getMessage());
		}

		return null;
	}


}
