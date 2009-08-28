package com.rho.db.file;

import j2me.io.FileNotFoundException;

import java.io.IOException;
import java.util.Hashtable;

import net.rim.device.api.system.PersistentObject;
import net.rim.device.api.system.PersistentStore;
import net.rim.device.api.util.StringUtilities;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.db.RandomAccessFile;

public class PersistRAFileImpl implements RAFileImpl {
	
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("PersistRAFileImpl");
	
	// WARNING!!! Be very carefull when modify this line! There was a case when
	// entire Hsqldb module has preverification error in case if this line was exactly as:
	//private static final String kprefix = PersistRAFileImpl.class.getName() + ":";
	// It is impossible to explain why it happened but need to be remembered
	private static final String kprefix = PersistRAFileImpl.class.getName();
	
	private static final int version = 1;
	
	private static final String NAME = "name";
	private static final String CONTENT = "content";
	private static final String SIZE = "size";
	
	private static Hashtable m_shared = new Hashtable();
	
	// Only for debug purposes
	private static int id = 0;
	private final int m_id = ++id;
	private String m_name;
	private int m_mode;
	
	private long m_key;
	private long m_nSeekPos;
	
	private PersistentObject m_persObj;
	
	private static class FileInfo {
		public byte[] content = null;
	};
	
	private FileInfo m_info = null;
	
	private FileInfo getInfo() {
		if (m_info == null) {
			synchronized (m_shared) {
				FileInfo info = (FileInfo)m_shared.get(m_name);
				if (info == null) {
					info = new FileInfo();
					m_shared.put(m_name, info);
				}
				m_info = info;
			}
		}
		return m_info;
	}
	
	private long getSize() throws IOException {
		return getContent().length;
	}
	
	private byte[] getContent() throws IOException {
		FileInfo info = getInfo();
		if (info.content == null) {
			byte[] content = (byte[])m_persObj.getContents();
			if (content == null) {
				content = new byte[0];
				m_persObj.setContents(content);
			}
			info.content = content;
		}
		return info.content;
	}
	
	private void setContent(byte[] content) {
		FileInfo info = getInfo();
		if (info.content != content)
			info.content = content;
	}

	public void open(String name, int mode) throws FileNotFoundException {
		m_name = name;
		m_mode = mode;
		
		m_key = StringUtilities.stringHashToLong(kprefix + "." + Integer.toString(version) + ":" + name);
		m_persObj = PersistentStore.getPersistentObject(m_key);
		
		m_nSeekPos = 0;
		
		LOG.TRACE("Open file: " + m_name);
	}
	
	public void close() throws IOException {
		LOG.TRACE("Close file: " + m_name);
		
		if (m_mode == RandomAccessFile.WRITE || m_mode == RandomAccessFile.READ_WRITE)
			sync();
		
		m_persObj = null;
		m_info = null;
		
		m_nSeekPos = 0;
	}
	
	public void seek(long pos) throws IOException {
		m_nSeekPos = pos;
	}

	public long seekPos() throws IOException {
		return m_nSeekPos;
	}

	public void setSize(long newSize) throws IOException {
		if (newSize < 0)
			throw new IllegalArgumentException();
		
		synchronized (getInfo()) {
			long size = getSize();
			if (size == newSize)
				return;
			
			byte[] newContent = new byte[(int)newSize];
			if (newSize < size)
				size = newSize;
			System.arraycopy(getContent(), 0, newContent, 0, (int)size);
			setContent(newContent);
		}
	}
	
	public long size() throws IOException {
		synchronized (getInfo()) {
			return getSize();
		}
	}

	public void sync() throws IOException {
		if (m_mode != RandomAccessFile.WRITE && m_mode != RandomAccessFile.READ_WRITE)
			throw new IOException("File is not open in write mode");
		
		FileInfo info = getInfo();
		synchronized (info) {
			if (m_persObj.getContents() != info.content) {
				m_persObj.setContents(info.content);
				m_persObj.commit();
			}
		}
	}
	
	public int read() throws IOException {
		synchronized (getInfo()) {
			if (m_nSeekPos >= getSize())
				return -1;
			
			byte[] content = getContent();
			int pos = (int)m_nSeekPos;
			++m_nSeekPos;
			return content[pos];
		}
	}

	public int read(byte[] b, int off, int len) throws IOException {
		synchronized (getInfo()) {
			long size = getSize();
			if (m_nSeekPos >= size)
				return -1;
			
			long n = len;
			if (n > b.length - off)
				n = b.length - off;
			if (n > size - m_nSeekPos)
				n = size - m_nSeekPos;
			System.arraycopy(getContent(), (int)m_nSeekPos, b, off, (int)n);
			m_nSeekPos += n;
			return (int)n;
		}
	}

	public void write(int b) throws IOException {
		synchronized (getInfo()) {
			if (m_nSeekPos >= getSize())
				setSize(m_nSeekPos + 1);
			
			byte[] content = getContent();
			content[(int)m_nSeekPos] = (byte)b;
			setContent(content);
			++m_nSeekPos;
		}
	}

	public void write(byte[] b, int off, int len) throws IOException {
		synchronized (getInfo()) {
			int n = b.length - off;
			if (len < n)
				n = len;
			
			if (m_nSeekPos + n >= getSize())
				setSize(m_nSeekPos + n);
			
			byte[] content = getContent();
			System.arraycopy(b, off, content, (int)m_nSeekPos, n);
			setContent(content);
			m_nSeekPos += n;
		}
	}

}
