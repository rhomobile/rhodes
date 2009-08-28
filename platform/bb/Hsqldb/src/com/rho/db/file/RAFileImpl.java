package com.rho.db.file;

import java.io.IOException;

import j2me.io.FileNotFoundException;

public interface RAFileImpl {
	public void open(String name, int mode) throws FileNotFoundException;
	public void close() throws IOException;
	
	public long size() throws IOException;
	public void setSize(long newSize) throws IOException;
	
	public void seek(long pos) throws IOException;
	public long seekPos() throws IOException;
	
	public void write(int b) throws IOException;
	public void write(byte[] b, int off, int len) throws IOException;
	
	public int read() throws IOException;
	public int read(byte[] b, int off, int len) throws IOException;
	
	public void sync() throws IOException;
}
