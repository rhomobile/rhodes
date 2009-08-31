package com.rho.db.file;

import j2me.io.FileNotFoundException;
import j2me.lang.AssertMe;

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
	
	private static final String version = "2.0";
	
	private static final int PAGE_SIZE = 256;
	
	private static Hashtable m_shared = new Hashtable();
	
	// Only for debug purposes
	private static int id = 0;
	private final int m_id = ++id;
	private String m_name;
	private int m_mode;
	
	private long m_nSeekPos;
	
	private String getObjName() {
		return kprefix + "." + version + ":" + m_name + ";pagesize=" + PAGE_SIZE;
	}
	
	private class Page {
		public byte[] content;
		
		public Page() {
			this(null);
		}
		
		public Page(byte[] c) {
			content = c;
		}
	};
	
	private class FileInfo {
		//private String m_name;
		private long m_size;
		private Page[] m_pages;
		private int m_use_count;
		
		private long getSizeKey() {
			return StringUtilities.stringHashToLong(getObjName() + ";size");
		}
		
		private long getPageKey(int n) {
			return StringUtilities.stringHashToLong(getObjName() + ";page" + Integer.toString(n));
		}
		
		public FileInfo() {
			long key = getSizeKey();
			PersistentObject persInfo = PersistentStore.getPersistentObject(key);
			Long psize = (Long)persInfo.getContents();
			if (psize == null)
				psize = new Long(0);
			
			m_size = psize.longValue();
			int n = (int)(m_size/PAGE_SIZE) + 1;
			m_pages = new Page[n];
			for (int i = 0; i != n; ++i)
				m_pages[i] = new Page();
			
			m_use_count = 0;
		}
		
		public byte[] getPage(int n) {
			Page page = m_pages[n];
			if (page.content == null) {
				long key = getPageKey(n);
				PersistentObject persPage = PersistentStore.getPersistentObject(key);
				byte[] content = (byte[])persPage.getContents();
				if (content == null)
					content = new byte[PAGE_SIZE];
				page.content = content;
			}
			return page.content;
		}
		
		public long getSize() {
			return m_size;
		}
		
		public void setSize(long newSize) {
			int n = (int)(newSize/PAGE_SIZE + 1);
			if (n != m_pages.length) {
				Page[] newPages = new Page[n];
				System.arraycopy(m_pages, 0, newPages, 0, n >= m_pages.length ? m_pages.length : n);
				for (int i = m_pages.length; i != n; ++i)
					newPages[i] = new Page();
				m_pages = newPages;
			}
			m_size = newSize;
		}
		
		public void sync() {
			synchronized (PersistentStore.getSynchObject()) {
				for(int i = 0; i != m_pages.length; ++i) {
					Page page = m_pages[i];
					if (page.content != null) {
						long key = getPageKey(i);
						PersistentObject persPage = PersistentStore.getPersistentObject(key);
						persPage.setContents(page.content);
						persPage.commit();
					}
				}
				
				long key = getSizeKey();
				PersistentObject persInfo = PersistentStore.getPersistentObject(key);
				persInfo.setContents(new Long(m_size));
				persInfo.commit();
			}
		}
		
		public void addRef() {
			++m_use_count;
		}
		
		public void release() {
			if (--m_use_count == 0) {
				sync();
			}
		}
	};
	
	private FileInfo m_info = null;
	
	public void open(String name, int mode) throws FileNotFoundException {
		m_name = name;
		m_mode = mode;
		
		synchronized (m_shared) {
			FileInfo info = (FileInfo)m_shared.get(m_name);
			if (info == null) {
				info = new FileInfo();
				m_shared.put(m_name, info);
			}
			info.addRef();
			m_info = info;
		}
		
		m_nSeekPos = 0;
		
		LOG.TRACE("Open file: " + m_name);
	}
	
	public void close() throws IOException {
		LOG.TRACE("Close file: " + m_name);
		
		m_info.release();
		m_info = null;
		
		m_nSeekPos = 0;
	}
	
	public void seek(long pos) throws IOException {
		m_nSeekPos = pos;
	}

	public long seekPos() throws IOException {
		return m_nSeekPos;
	}
	
	private int getPageNumber(long pos) {
		int n = (int)(pos/PAGE_SIZE);
		return n;
	}

	public void setSize(long newSize) throws IOException {
		if (newSize < 0)
			throw new IllegalArgumentException();
		
		synchronized (m_info) {
			long size = m_info.getSize();
			if (size == newSize)
				return;
			
			m_info.setSize(newSize);
		}
	}
	
	public long size() throws IOException {
		synchronized (m_info) {
			return m_info.getSize();
		}
	}

	public void sync() throws IOException {
		if (m_mode != RandomAccessFile.WRITE && m_mode != RandomAccessFile.READ_WRITE)
			throw new IOException("File is not open in write mode");
		
		synchronized (m_info) {
			m_info.sync();
		}
	}
	
	public int read() throws IOException {
		synchronized (m_info) {
			if (m_nSeekPos >= m_info.getSize())
				return -1;
			
			int n = (int)(m_nSeekPos/PAGE_SIZE);
			byte[] content = m_info.getPage(n);
			int pos = (int)(m_nSeekPos % PAGE_SIZE);
			++m_nSeekPos;
			return content[pos];
		}
	}

	public int read(byte[] b, int off, int len) throws IOException {
		if (b == null)
			throw new NullPointerException();
		if (off < 0 || len < 0 || off + len > b.length)
			throw new IndexOutOfBoundsException();
		if (len == 0)
			return 0;
		
		synchronized (m_info) {
			long size = m_info.getSize();
			if (m_nSeekPos >= size)
				return -1;
			
			if (m_nSeekPos + len > size)
				len = (int)(size - m_nSeekPos);
			
			int startPage = getPageNumber(m_nSeekPos);
			int endPage = getPageNumber(m_nSeekPos + len);
			
			int n = startPage == endPage ? len : PAGE_SIZE - (int)m_nSeekPos%PAGE_SIZE;
			
			byte[] content = m_info.getPage(startPage);
			System.arraycopy(content, (int)(m_nSeekPos%PAGE_SIZE), b, off, n);
			for (int i = startPage + 1; i <= endPage; ++i) {
				content = m_info.getPage(i);
				int howmuch = i == endPage ? (int)((m_nSeekPos + len)%PAGE_SIZE) : PAGE_SIZE;
				System.arraycopy(content, 0, b, n, howmuch);
				n += howmuch;
			}
			m_nSeekPos += n;
			return n;
		}
	}

	public void write(int b) throws IOException {
		synchronized (m_info) {
			if (m_nSeekPos >= m_info.getSize())
				m_info.setSize(m_nSeekPos + 1);
			
			int n = getPageNumber(m_nSeekPos);
			byte[] content = m_info.getPage(n);
			content[(int)(m_nSeekPos%PAGE_SIZE)] = (byte)b;
			++m_nSeekPos;
		}
	}

	public void write(byte[] b, int off, int len) throws IOException {
		if (b == null)
			throw new NullPointerException();
		if (off < 0 || len < 0 || off + len > b.length)
			throw new IndexOutOfBoundsException();
		if (len == 0)
			return;
		
		synchronized (m_info) {
			if (m_nSeekPos + len >= m_info.getSize())
				m_info.setSize(m_nSeekPos + len);
			
			int startPage = getPageNumber(m_nSeekPos);
			int endPage = getPageNumber(m_nSeekPos + len);
			
			int n = startPage == endPage ? len : PAGE_SIZE - (int)m_nSeekPos%PAGE_SIZE;
			
			byte[] content = m_info.getPage(startPage);
			System.arraycopy(b, off, content, (int)(m_nSeekPos%PAGE_SIZE), n);
			for (int i = startPage + 1; i <= endPage; ++i) {
				content = m_info.getPage(i);
				int howmuch = i == endPage ? (int)((m_nSeekPos + len)%PAGE_SIZE) : PAGE_SIZE;
				System.arraycopy(b, off + n, content, 0, howmuch);
				n += howmuch;
			}
			m_nSeekPos += n;
		}
	}

}
