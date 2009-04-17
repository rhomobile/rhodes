package com.rhomobile.rhodes;

import java.io.File;

import java.io.FileWriter;
import java.io.RandomAccessFile;
import java.io.IOException;

import com.rho.SimpleFile;

import android.content.res.Resources;
import android.util.Log;

public class AndroidFile implements SimpleFile {
	final public static String RHODES_ROOT_DIR = "/sdcard/rhomobile/";

	static private String rootDir = null;

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
			file.close();
		} catch (IOException e) {
			Log.e(getClass().getSimpleName(), e.getMessage());
		}
	}

	public void open(String filePath, boolean readOnly, boolean noFlush) {
		this.noFlush = noFlush;
		try {

			file = new RandomAccessFile(rootDir + filePath, readOnly ? "r"
					: "rw");

		} catch (IOException x) {

			try {
				// try to create file
				FileWriter f = new FileWriter(rootDir + filePath);
				f.close();

				file = new RandomAccessFile(rootDir + filePath, readOnly ? "r"
						: "rw");

			} catch (IOException e) {
				Log.e(getClass().getSimpleName(), e.getMessage());
			}
		}
	}

	public long length() {
		try {
			return file.length();
		} catch (IOException x) {
			return -1;
		}
	}

	public void delete(String path) {
		try {
			File file = new File(rootDir + path);
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

	protected RandomAccessFile file;
	protected boolean noFlush;
}
