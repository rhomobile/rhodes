package com.rhomobile.rhodes;

import j2me.io.File;

import java.io.FileWriter;
import java.io.RandomAccessFile;
import java.io.IOException;

import org.garret.perst.SimpleFile;
import org.garret.perst.StorageError;

public class AndroidFile implements SimpleFile {
	final public static String RHODES_ROOT_DIR = "/sdcard/";

	public void write(long pos, byte[] buf) {
		try {
			file.seek(pos);
			file.write(buf, 0, buf.length);
		} catch (IOException x) {
			throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
		}
	}

	public int read(long pos, byte[] buf) {
		try {
			file.seek(pos);
			return file.read(buf, 0, buf.length);
		} catch (IOException x) {
			throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
		}
	}

	public void sync() {
		if (!noFlush) {
			try {
				file.getFD().sync();
			} catch (IOException x) {
				throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
			}
		}
	}

	public void close() {
		try {
			file.close();
		} catch (IOException x) {
			throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
		}
	}

	public void open(String filePath, boolean readOnly, boolean noFlush) {
		this.noFlush = noFlush;
		try {
			file = new RandomAccessFile(RHODES_ROOT_DIR + filePath,
					readOnly ? "r" : "rw");

		} catch (IOException x) {

			try {
				// try to create file
				FileWriter f = new FileWriter(RHODES_ROOT_DIR + filePath);
				f.close();

				file = new RandomAccessFile(RHODES_ROOT_DIR + filePath,
						readOnly ? "r" : "rw");

			} catch (IOException e) {
				throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
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
			File file = new File(RHODES_ROOT_DIR + path);
			file.delete();
		} catch (Exception e) {
		}
	}

	protected RandomAccessFile file;
	protected boolean noFlush;
}
