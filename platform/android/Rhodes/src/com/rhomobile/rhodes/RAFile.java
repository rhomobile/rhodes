package com.rhomobile.rhodes;

import j2me.io.FileNotFoundException;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;

import com.rho.IRAFile;

public class RAFile implements IRAFile {

	private RandomAccessFile m_file = null;
	private String m_name = null;
	
	public void open(String name) throws FileNotFoundException {
		open(name, "r");
	}

	public void open(String name, String mode) throws FileNotFoundException {
		try {
			if (mode.equals("rw"))
				mode = "rws";
			m_file = new RandomAccessFile(name, mode);
			m_name = name;
		} catch (java.io.FileNotFoundException e) {
			throw new FileNotFoundException(e.getMessage());
		}
	}
	
	public void close() throws IOException {
		if (m_file != null) {
			m_file.close();
			m_file = null;
			m_name = null;
		}
	}

	public int read() throws IOException {
		return m_file.read();
	}

	public int read(byte[] b, int off, int len) throws IOException {
		return m_file.read(b, off, len);
	}

	public void seek(long pos) throws IOException {
		m_file.seek(pos);
	}

	public long seekPos() throws IOException {
		return m_file.getFilePointer();
	}

	public void setSize(long newSize) throws IOException {
		m_file.setLength(newSize);
	}

	public long size() throws IOException {
		return m_file.length();
	}

	public void sync() throws IOException {
		// Nothing
	}

	public void write(int b) throws IOException {
		m_file.write(b);
	}

	public void write(byte[] b, int off, int len) throws IOException {
		m_file.write(b, off, len);
	}
	
	public void delete() throws IOException {
		String name = m_name;
		close();
		File file = new File(name);
		file.delete();
	}

	public boolean exists() {
		File file = new File(m_name);
		return file.exists();
	}

	public void rename(String newName) throws IOException {
		String name = m_name;
		close();
		File file = new File(name);
		if (!file.renameTo(new File(newName)))
			throw new RuntimeException("Can not rename file");
	}

	public void listenForSync(String name) throws IOException {
		// TODO: implement
	}

	public void stopListenForSync(String name) throws IOException {
		// TODO: implement
	}
}
