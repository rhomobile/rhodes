package com.rho.file;

import j2me.io.FileNotFoundException;

import java.io.IOException;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import javax.microedition.io.Connector;
import javax.microedition.io.file.ConnectionClosedException;

import net.rim.device.api.system.PersistentObject;
import net.rim.device.api.system.PersistentStore;
import net.rim.device.api.util.Arrays;
import net.rim.device.api.util.Persistable;
import net.rim.device.api.util.StringUtilities;

import com.rho.RhoEmptyLogger;
import com.rho.RhoEmptyProfiler;
import com.rho.RhoLogger;
import com.rho.RhoProfiler;

public class PersistRAFileImpl implements IRAFile {
	
	private static final boolean debug = false;
	
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("PersistFileImpl");

	//private static final RhoProfiler PROF = RhoProfiler.RHO_STRIP_PROFILER ? new RhoEmptyProfiler() : 
	//	new RhoProfiler();
	
	private static void log(String message) {
		LOG.INFO(message);
		//LOG.TRACE(message);
	}
	
	// WARNING!!! Be very carefull when modify this line! There was a case when
	// entire Hsqldb module has preverification error in case if this line was exactly as:
	//private static final String kprefix = PersistRAFileImpl.class.getName() + ":";
	// It is impossible to explain why it happened but need to be remembered
	private static final String kprefix = PersistRAFileImpl.class.getName();
	
	private static final String version = "2.6";
	//private static final String version = "debug.2.68";
	
	private static final int PAGE_SIZE = 4096;
	private static final int MAX_CLEAR_PAGES_CACHED = 2;
	
	private static Hashtable m_shared = new Hashtable();
	
	//private static int m_all_sync_times = 0;
	//private int m_sync_times = 0;
	
	// Only for debug purposes
	private static int id = 0;
	private int m_id;
	private int m_mode;
	
	private long m_nSeekPos;
	
	private static class PageWrapper implements Persistable {
		public String name;
		public int index;
		public byte[] content;
		public PageWrapper(String n, int i, byte[] c) {
			name = n;
			index = i;
			content = c;
		}
	};
	
	private static class FileInfoWrapper implements Persistable {
		public String name;
		public Long size;
		
		public FileInfoWrapper(String n, long s) {
			name = n;
			size = new Long(s);
		}
	};
	
	private static class Page {
		public byte[] content;
		public boolean dirty;
		
		public Page() {
			this(null);
		}
		
		public Page(byte[] c) {
			content = c;
			dirty = false;
		}
	};
	
	private static class FileInfo {
		public int use_count = 0;
		
		private String objName;
		
		private String m_name;
		private long m_size;
		private boolean m_exists;
		private Page[] m_pages;
		private int m_loaded;
		private int m_dirty;
		
		private int m_syncOut;
		private Hashtable m_listenForSync;
		
		private long getInfoKey() {
			String infoName = objName + ";info";
			return StringUtilities.stringHashToLong(infoName);
		}
		
		private long getPageKey(int n) {
			String pageName = objName + ";page=" + n;
			return StringUtilities.stringHashToLong(pageName);
		}
		
		public FileInfo(String name) {
			objName = kprefix + "." + version + ":" + name + ";pagesize=" + PAGE_SIZE;
			
			long key = getInfoKey();
			PersistentObject persInfo = PersistentStore.getPersistentObject(key);
			FileInfoWrapper wrapper = (FileInfoWrapper)persInfo.getContents();
			if (wrapper != null && !wrapper.name.equals(name))
				throw new RuntimeException("File name location mechanism internal error");
			
			m_name = name;
			m_exists = wrapper != null;
			m_size = m_exists ? wrapper.size.longValue() : 0;
			m_loaded = 0;
			m_dirty = 0;
			
			int n = (int)(m_size/PAGE_SIZE) + 1;
			m_pages = new Page[n];
			for (int i = 0; i != n; ++i)
				m_pages[i] = new Page();
			
			m_syncOut = 0;
			m_listenForSync = new Hashtable();
		}
		
		public String getName() {
			return m_name;
		}
		
		public boolean exists() {
			return m_exists;
		}
		
		private void unloadClearPages(int needToClear) {
			for (int i = m_pages.length - 1; needToClear > 0 && m_loaded > 0 && i >= 0; --i) {
				Page page = m_pages[i];
				if (page.dirty)
					continue;
				page.content = null;
				--m_loaded;
				--needToClear;
			}
		}
		
		public void setPageDirty(int idx) {
			Page page = m_pages[idx];
			if (!page.dirty)
				++m_dirty;
			page.dirty = true;
		}
		
		public byte[] getPage(int n) {
			Page page = m_pages[n];
			if (page.content == null) {
				int clearPages = m_loaded - m_dirty - (MAX_CLEAR_PAGES_CACHED - 1);
				if (clearPages > 0)
					unloadClearPages(clearPages);
				long key = getPageKey(n);
				PersistentObject persPage = PersistentStore.getPersistentObject(key);
				PageWrapper wrapper = (PageWrapper)persPage.getContents();
				if (wrapper != null) {
					if (wrapper.index != n || !wrapper.name.equals(m_name))
						throw new RuntimeException("Page location mechanism internal error");
				}
				page.content = wrapper == null ? new byte[PAGE_SIZE] : wrapper.content;
				++m_loaded;
			}
			return page.content;
		}
		
		public long getSize() {
			return m_size;
		}
		
		public void setSize(long newSize) {
			int n = (int)(newSize/PAGE_SIZE + 1);
			int prevSize = m_pages.length;
			if (n != prevSize) {
				if (n < prevSize)
					deletePages(n, prevSize - 1);
				Page[] newPages = new Page[n];
				System.arraycopy(m_pages, 0, newPages, 0, n < prevSize ? n : prevSize);
				for (int i = prevSize; i < n; ++i)
					newPages[i] = new Page();
				m_pages = newPages;
			}
			m_size = newSize;
		}
		
		public void sync() {
			if (debug)
				log("--- File '" + m_name + "' sync, size: " + m_size + ", dirty pages: " + m_dirty + "...");
			
			synchronized (PersistentStore.getSynchObject()) {
				Enumeration e = m_listenForSync.elements();
				while (e.hasMoreElements()) {
					FileInfo info = (FileInfo)e.nextElement();
					info.sync();
				}
				
				for (int i = m_pages.length - 1; m_dirty > 0 && i >= 0; --i) {
					Page page = m_pages[i];
					if (page.content != null && page.dirty) {
						long key = getPageKey(i);
						PersistentObject persPage = PersistentStore.getPersistentObject(key);
						persPage.setContents(new PageWrapper(m_name, i, page.content));
						persPage.commit();
						page.dirty = false;
						--m_dirty;
					}
				}
				
				int clearPages = m_loaded - m_dirty - MAX_CLEAR_PAGES_CACHED;
				if (clearPages > 0)
					unloadClearPages(clearPages);
				
				long key = getInfoKey();
				PersistentObject persInfo = PersistentStore.getPersistentObject(key);
				persInfo.setContents(new FileInfoWrapper(m_name, m_size));
				persInfo.commit();
			}
			
			//log("This is result for file '" + m_name + "', size - " + m_size + ", dirty pages: " + dirty);
			
			if (debug)
				log("+++ File '" + m_name + "' sync done");
		}
		
		private void deletePages(int start, int end) {
			//if (start > end)
			//	throw new IllegalArgumentException();
			//if (start < 0 || end >= m_pages.length)
			//	throw new IndexOutOfBoundsException();
			int rm = end - start + 1;
			//if (rm > m_pages.length)
			//	throw new IndexOutOfBoundsException();
			
			synchronized (PersistentStore.getSynchObject()) {
				for (int i = start; i <= end; ++i) {
					long key = getPageKey(i);
					PersistentStore.destroyPersistentObject(key);
				}
			}
			
			Page[] newPages = new Page[m_pages.length - rm]; 
			System.arraycopy(m_pages, 0, newPages, 0, start);
			System.arraycopy(m_pages, end + 1, newPages, start, newPages.length - start);
			m_pages = newPages;
			m_loaded -= rm;
			if (m_loaded < 0)
				m_loaded = 0;
		}
		
		public void delete() {
			synchronized (PersistentStore.getSynchObject()) {
				deletePages(0, m_pages.length - 1);
				
				long key = getInfoKey();
				PersistentStore.destroyPersistentObject(key);
				m_size = 0;
				
				m_dirty = 0;
				m_exists = false;
			}
		}
		
		public void listenForSync(String name) {
		}
		
		public void stopListenForSync(String name) {
		}
	};
	
	private FileInfo m_info = null;
	
	public PersistRAFileImpl() {
		synchronized (m_shared) {
			m_id = ++id;
		}
	}
	
	private FileInfo getFileInfo(String name) {
		FileInfo info = (FileInfo)m_shared.get(name);
		if (info == null) {
			info = new FileInfo(name);
			m_shared.put(name, info);
		}
		if (info.use_count < 0)
			throw new RuntimeException("Internal error");
		return info;
	}
	
	public void open(String name) throws FileNotFoundException {
		open(name, "r");
	}
	
	public void open(String name, String mode) throws FileNotFoundException {
		m_mode = Connector.READ;
		if (mode.startsWith("rw") || mode.startsWith("w"))
			m_mode = Connector.READ_WRITE;
		
		if (debug)
			log("--- File '" + name + "': open in mode '" + mode + "'s...");
		FileInfo info;
		synchronized (m_shared) {
			info = getFileInfo(name);
			
			if (!info.exists()) {
				if (isWritable())
					info.m_exists = true;
				else {
					//log("+++ File '" + name + "': not found");
					throw new FileNotFoundException();
				}
			}
			
			++info.use_count;
			m_info = info;
		}
		
		m_nSeekPos = 0;
		
		if (debug)
			log("+++ File '" + name + "' opened: id " + m_id +
					(m_info.exists() ? ", size " + m_info.getSize() : ", not exists") +
					" (use_count: " + m_info.use_count + ")");
	}
	
	private void checkUseCount() {
		if (m_info.use_count <= 0)
			throw new RuntimeException("Internal error");
	}
	
	//private boolean isReadable() {
	//	return m_mode == Connector.READ || m_mode == Connector.READ_WRITE;
	//}
	
	private boolean isWritable() {
		return m_mode == Connector.WRITE || m_mode == Connector.READ_WRITE;
	}
	
	public void close() throws IOException {
		if (m_info == null)
			return;
		
		synchronized (m_info) {
			checkUseCount();
			String name = m_info.getName();
			if (debug)
				log("--- File '" + name + "' close (use_count: " + m_info.use_count + ")...");
			
			if (--m_info.use_count == 0 && isWritable() && m_info.exists())
				m_info.sync();
			
			if (debug)
				log("+++ File '" + name + "' closed");
		}
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
			checkUseCount();
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

	public boolean isDirectory()
	{
		//TODO: isDirectory
		return false;
	}
	
	public boolean isFile()
	{
		//TODO: isFile
		return true;
	}
	
	public void sync() throws IOException {
		if (!isWritable())
			throw new IOException("File is not open in write mode");
		
		synchronized (m_info) {
			checkUseCount();
			if (m_info.exists()) {
				// Do actual sync only if m_syncOut is zero.
				// Otherwise sync would be done later by its listener.
				if (m_info.m_syncOut == 0)
					m_info.sync();
			}
		}
	}
	
	public int read() throws IOException {
		synchronized (m_info) {
			checkUseCount();
			
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
			checkUseCount();
			
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
			checkUseCount();
			
			if (m_nSeekPos >= m_info.getSize())
				m_info.setSize(m_nSeekPos + 1);
			
			int n = getPageNumber(m_nSeekPos);
			byte[] content = m_info.getPage(n);
			content[(int)(m_nSeekPos%PAGE_SIZE)] = (byte)b;
			++m_nSeekPos;
			
			m_info.setPageDirty(n);
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
			checkUseCount();
			
			if (m_nSeekPos + len >= m_info.getSize())
				m_info.setSize(m_nSeekPos + len);
			
			int startPage = getPageNumber(m_nSeekPos);
			int endPage = getPageNumber(m_nSeekPos + len);
			
			int n = startPage == endPage ? len : PAGE_SIZE - (int)m_nSeekPos%PAGE_SIZE;
			
			byte[] content = m_info.getPage(startPage);
			System.arraycopy(b, off, content, (int)(m_nSeekPos%PAGE_SIZE), n);
			m_info.setPageDirty(startPage);
			
			for (int i = startPage + 1; i <= endPage; ++i) {
				int howmuch = i == endPage ? (int)((m_nSeekPos + len)%PAGE_SIZE) : PAGE_SIZE;
				content = m_info.getPage(i);
				System.arraycopy(b, off + n, content, 0, howmuch);
				m_info.setPageDirty(i);
				
				n += howmuch;
			}
			m_nSeekPos += n;
		}
	}

	public void delete() throws IOException {
		if (m_info == null)
			return;

		synchronized (m_info) {
			checkUseCount();
			
			String name = m_info.getName();
			if (debug)
				log("--- File '" + name + "' delete...");
			m_info.delete();
			--m_info.use_count;
			if (debug)
				log("+++ File '" + name + "' deleted");
		}
		
		m_info = null;
		m_nSeekPos = 0;
	}

	public boolean exists() {
		synchronized (m_info) {
			checkUseCount();
			return m_info.exists();
		}
	}
	
	public void rename(String nname) throws IOException {
		// This function's body MUST be enclosed by common lock
		// to avoid dead-lock race conditions. Such dead-lock could happen,
		// for example, when one thread call rename A to B and other
		// thread call rename B to A at the same time.
		synchronized (m_shared) {
			synchronized (m_info) {
				checkUseCount();
				
				if (!m_info.exists())
					throw new ConnectionClosedException();
			
				String name = m_info.getName();
				
				String lastElementOld = name;
				int lastSlash = name.lastIndexOf('/');
				if (lastSlash >= 0)
					lastElementOld = name.substring(lastSlash + 1);
				
				String lastElementNew = nname;
				lastSlash = nname.lastIndexOf('/');
				if (lastSlash >= 0)
					lastElementNew = nname.substring(lastSlash + 1);
				
				if (lastElementOld == lastElementNew)
					return;
		
				String newName = name.substring(0, name.length() - lastElementOld.length()) + lastElementNew;
				
				if (debug)
					log("--- File '" + name + "' rename to '" + newName + "'...");
				
				FileInfo newInfo = getFileInfo(newName);
				
				synchronized (newInfo) {
					if (newInfo.exists())
						throw new IOException("Destination file already exists");
					
					long size = m_info.getSize();
					newInfo.setSize(size);
					
					int n = (int)(size/PAGE_SIZE + 1);
					for (int i = 0; size > 0 && i < n; ++i) {
						byte[] content = m_info.getPage(i);
						byte[] newContent = newInfo.getPage(i);
						System.arraycopy(content, 0, newContent, 0,
								(int)(size > PAGE_SIZE ? PAGE_SIZE : size % PAGE_SIZE));
						newInfo.setPageDirty(i);
						size -= PAGE_SIZE;
					}
					
					newInfo.m_exists = true;
					newInfo.sync();
					
					/*
					if (debug) {
						long oldSize = m_info.getSize();
						long newSize = newInfo.getSize();
						log("File '" + name + "': size: " + oldSize);
						log("File '" + newName + "': size: " + newSize);
						log("Sizes are " + (oldSize == newSize ? "" : "not ") + "equals");
						if (oldSize != newSize)
							throw new IOException("Internal rename error");
						for (int i = 0; i < n; ++i) {
							byte[] oldPage = m_info.getPage(i);
							byte[] newPage = newInfo.getPage(i);
							if (oldPage.length != newPage.length || newPage.length > PAGE_SIZE)
								throw new IOException("Internal rename error");
							boolean equals = Arrays.equals(oldPage, 0, newPage, 0,
									(int)(i == n - 1 ? newSize % PAGE_SIZE : PAGE_SIZE));
							log("Page " + i + ": " + (equals ? "" : "not ") + "equals");
							if (!equals)
								throw new IOException("Internal rename error");
						}
						log("Newinfo dirty pages: " + newInfo.m_dirty);
						if (newInfo.m_dirty != 0)
							throw new IOException("Internal rename error");
					}
					*/
				}
				
				m_info.delete();
				--m_info.use_count;
				
				if (debug)
					log("+++ File '" + name + "' renamed to '" + newName + "'");
			}
			
			m_info = null;
			m_nSeekPos = 0;
		}
	}

	public void listenForSync(String name) throws IOException {
		FileInfo info;
		synchronized (m_shared) {
			info = getFileInfo(name);
		}
		synchronized (info) {
			info.m_listenForSync.put(m_info.getName(), this.m_info);
		}
		synchronized (m_info) {
			++m_info.m_syncOut;
		}
	}

	public void stopListenForSync(String name) throws IOException {
		FileInfo info;
		synchronized (m_shared) {
			info = getFileInfo(name);
		}
		synchronized (info) {
			info.m_listenForSync.remove(name);
		}
		synchronized (m_info) {
			if (m_info.m_syncOut > 0)
				--m_info.m_syncOut;
		}
	}

}
