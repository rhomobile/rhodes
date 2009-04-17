package com.rho;

import java.io.IOException;

public class FileFactory {
	public static SimpleFile createFile() throws IOException {
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
					throw new IOException(x3.getMessage());
				}
			}
		}
		try {
			return (SimpleFile) wrapperClass.newInstance();
		} catch (Exception x) {
			throw new IOException(x.getMessage());
		}
	}
}
