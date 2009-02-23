package org.garret.perst.impl;

import org.garret.perst.*;

public class FileFactory {
	public static SimpleFile createFile() {
		Class systemClass;
		Class wrapperClass;
		// RHO COMMENT
		try { //for blackberry
			systemClass = Class.forName("javax.microedition.io.file.FileConnection");
			wrapperClass = Class.forName("org.garret.perst.impl.Jsr75File");
		} catch (ClassNotFoundException x2) {

			try { //for android
				systemClass = Class.forName("java.io.RandomAccessFile");
				wrapperClass = Class.forName("com.rhomobile.rhodes.AndroidFile");
			} catch (ClassNotFoundException x) {

				try { //rms
					systemClass = Class
							.forName("javax.microedition.rms.RecordStore");
					wrapperClass = Class
							.forName("org.garret.perst.impl.RmsFile");
				} catch (ClassNotFoundException x3) {
					throw new StorageError(StorageError.FILE_ACCESS_ERROR, x3);
				}
			}
		}
		try {
			return (SimpleFile) wrapperClass.newInstance();
		} catch (Exception x) {
			throw new StorageError(StorageError.FILE_ACCESS_ERROR, x);
		}
	}
}
