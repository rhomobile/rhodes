package com.rho.file;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;

import j2me.io.FileNotFoundException;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;

import com.rho.RhoEmptyLogger;
import com.rho.RhoEmptyProfiler;
import com.rho.RhoLogger;
import com.rho.RhoProfiler;

public class Jsr75RAFileImpl implements IRAFile {
	
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Jsr75RAFileImpl");
	
	private static final RhoProfiler PROF = RhoProfiler.RHO_STRIP_PROFILER ? new RhoEmptyProfiler() : 
		new RhoProfiler();
	
	private FileConnection m_file;
	private InputStream    m_in;
    private OutputStream   m_out;
	private long           m_fileSize;
	private long           m_nSeekPos;
	private long           m_outPos;
    private long           m_inPos;
    
    private static final int ZERO_BUF_SIZE = 4096;
	
    public void open(String name) throws FileNotFoundException {
    	open(name, "r");
    }
    
	public void open(String name, String mode) throws FileNotFoundException {
		try {
			int imode = Connector.READ;
			if (mode.startsWith("rw") || mode.startsWith("w"))
				imode = Connector.READ_WRITE;
			m_file = (FileConnection)Connector.open(name, imode);
			LOG.TRACE("Open file: " + name);
			if (!m_file.exists()) {
				if (mode.startsWith("rw") || mode.startsWith("w")) {
					LOG.TRACE("Create file: " + name);
					try{
						m_file.create();  // create the file if it doesn't exist
					}catch(IOException exc)
					{
						Jsr75File.recursiveCreateDir(name);
						m_file.create();  // create the file if it doesn't exist
					}
				}
				else
					throw new FileNotFoundException("File '" + name + "' not exists");
	        }
			
	        m_fileSize = m_file.fileSize();
	        m_nSeekPos = 0;
		}
		catch (Exception exc) {
			//LOG.ERROR("Open '" + name + "' failed");
			throw new FileNotFoundException(exc.getMessage());
		}
	}
	
	public boolean mkdir()
	{
		if ( m_file == null )
			return false;
		
        if (m_file.exists())
        	return true;
        
        try{ 
        	m_file.mkdir();  // create the dir if it doesn't exist
        	return true;
        	
        }catch(IOException exc)
        {
        	LOG.ERROR("cannot create directory: " + m_file.getPath(), exc);
        }
        
        return false;
	}
	
	public Enumeration list()throws IOException
	{
		if ( m_file == null )
			return null;
		
		return m_file.list();
	}
	
	public long size() throws IOException {
		return m_fileSize;
	}

	public boolean isDirectory() {
		return m_file.isDirectory();
	}

	public boolean isFile() {
		return !m_file.isDirectory();
	}
	
	public void close() throws IOException {
		if (m_in != null) { 
            m_in.close();
            m_in = null;
        }
        if (m_out != null) { 
            m_out.close();
            m_out = null;
        }

    	if ( m_file != null )
    	{
    		LOG.TRACE("Close file: " + m_file.getName());
    		m_file.close();
    	}
    	
    	m_file = null;
    	m_fileSize = 0;
    	m_nSeekPos = 0;
	}
	
	public void seek(long pos) throws IOException {
		m_nSeekPos = pos;
	}
	
	public long seekPos() throws IOException {
		return m_nSeekPos;
	}
	
	private void prepareWrite() throws IOException {
    	if (m_out == null){
            m_out = m_file.openOutputStream(m_nSeekPos);
            m_outPos = m_nSeekPos;
        }
        
        if (m_outPos != m_nSeekPos)
        {                         
        	m_out.close();
            m_out = m_file.openOutputStream(m_nSeekPos);
            if (m_nSeekPos > m_fileSize) { 
                byte[] zeroBuf = new byte[ZERO_BUF_SIZE];
                do { 
                    int size = m_nSeekPos - m_fileSize > ZERO_BUF_SIZE ? ZERO_BUF_SIZE : (int)(m_nSeekPos - m_fileSize);
                    m_out.write(zeroBuf, 0, size);
                    m_fileSize += size;
                    
                    //BB
                    //m_file.truncate(m_fileSize);
                } while (m_nSeekPos != m_fileSize);
            }
            m_outPos = m_nSeekPos;
        }
    }
	
	private void postWrite(int len) throws IOException {
        m_outPos += len;
        if (m_outPos > m_fileSize) { 
            m_fileSize = m_outPos;
        }
        //BB
        //m_file.truncate(m_fileSize);
        if (m_in != null) { 
            m_in.close();
            m_in = null;
        }
        
        m_nSeekPos = m_outPos; 
    }

	public void write(int b) throws IOException {
		//PROF.START(RhoProfiler.FILE_WRITE);
		int nTry = 0;
        while (nTry <= 1){
	        try {
		    	prepareWrite();
		    	m_out.write(b);
		        postWrite(1);
		        break;
	        }catch(IOException exc){
	        	nTry++;
	        	if ( nTry > 1 )
	        		throw exc;
	        	else{
	        		m_outPos = -m_nSeekPos; //reopen out stream
	        	}
	        }
        }
        //PROF.STOP(RhoProfiler.FILE_WRITE);
	}

	public void write(byte[] b, int off, int len) throws IOException {
		//PROF.START(RhoProfiler.FILE_WRITE);
		int nTry = 0;
        while (nTry <= 1){
	        try {
		    	prepareWrite();
		        m_out.write(b, off, len);
		        postWrite(len);
		        break;
	        }catch(IOException exc){
	        	nTry++;
	        	if ( nTry > 1 )
	        		throw exc;
	        	else{
	        		m_outPos = -m_nSeekPos; //reopen out stream
	        	}
	        }
        }
        //PROF.STOP(RhoProfiler.FILE_WRITE);
	}
	
	private boolean prepareRead()throws IOException{
        if (m_in == null || m_inPos > m_nSeekPos) { 
            sync(); 
            if (m_in != null) { 
                m_in.close();
            }
            m_in = m_file.openInputStream();
            m_inPos = m_in.skip(m_nSeekPos);
        } else if (m_inPos < m_nSeekPos) { 
            m_inPos += m_in.skip(m_nSeekPos - m_inPos);
        }
        
        if (m_inPos != m_nSeekPos) 
            return false;
        
        return true;
    }
	
	public int read() throws IOException {
		//PROF.START(RhoProfiler.FILE_READ);
		try {
			if ( !prepareRead() )
	        	return -1;
	        
	        int res = m_in.read();
	        if ( res >= 0 ){
		        m_inPos += 1;
		        m_nSeekPos = m_inPos;
	        }
	        
	        return res;
		}
		finally {
			//PROF.STOP(RhoProfiler.FILE_READ);
		}
	}
	
	public int read(byte b[], int off, int len) throws IOException {
		
		//PROF.START(RhoProfiler.FILE_READ);
		try {
			if ( !prepareRead() )
	        	return -1;
	        
	        int offData = off;
	        while (len > 0) { 
	            int rc = m_in.read(b, off, len);
	            if (rc > 0) 
	            { 
	                m_inPos += rc;
	                offData += rc;
	                len -= rc;
	            } else { 
	                break;
	            }
	        }
	        
	        m_nSeekPos = m_inPos;
	        return offData;
		}
		finally {
			//PROF.STOP(RhoProfiler.FILE_READ);
		}
	}
	
	public void sync() throws IOException {
		//PROF.START(RhoProfiler.FILE_SYNC);
		if (m_out != null) 
        	m_out.flush();
		//PROF.STOP(RhoProfiler.FILE_SYNC);
	}
	
	public void setSize(long newSize) throws IOException {
		if (m_in != null) { 
            m_in.close();
            m_in = null;
        }
        if (m_out != null) { 
            m_out.close();
            m_out = null;
        }
    	
        if (newSize > m_fileSize) { 
            byte[] zeroBuf = new byte[ZERO_BUF_SIZE];
            do { 
                int size = newSize - m_fileSize > ZERO_BUF_SIZE ? ZERO_BUF_SIZE : (int)(newSize - m_fileSize);
                m_out.write(zeroBuf, 0, size);
                m_fileSize += size;
                
                //m_file.truncate(m_fileSize);
            } while (newSize != m_fileSize);
        }else
            m_file.truncate(newSize);
        
        m_fileSize = newSize;
	}

	public void delete() throws IOException {
		//PROF.START(RhoProfiler.FILE_DELETE);
		if ( m_file != null && m_file.exists() ) {
			//m_file.close();
			m_file.delete();
		}
		close();
		//PROF.STOP(RhoProfiler.FILE_DELETE);
	}

	public boolean exists() {
		return m_file.exists();
	}
	
	public void rename(String newName) throws IOException {
		//PROF.START(RhoProfiler.FILE_RENAME);
		if (m_file.exists())
			m_file.rename(newName);
		//PROF.STOP(RhoProfiler.FILE_RENAME);
	}

	public void listenForSync(String name) throws IOException {
		// TODO: implement
	}

	public void stopListenForSync(String name) throws IOException {
		// TODO: implement
	}
}
