package com.rho.db;

import java.io.*;
import j2me.io.FileNotFoundException;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;

import j2me.nio.channels.*;

import j2me.io.File;

public class RandomAccessFile  
{
    private boolean        m_bWriteAccess;
    private FileConnection m_file;
    private long           m_nSeekPos;
    private InputStream    m_in;
    private OutputStream   m_out;
    private long           m_outPos;
    private long           m_inPos;    
    private long           m_fileSize;
    private static final int ZERO_BUF_SIZE = 4096;
    
    public RandomAccessFile(String name, String mode)
        throws FileNotFoundException
    {
        this(name != null ? new File(name) : null, mode);
    }
        
    public RandomAccessFile(File file, String mode)
        throws FileNotFoundException
    {
    	String name = (file != null ? file.getPath() : null);
    	int imode = -1;
    	if (mode.equals("r"))
    	    imode = Connector.READ;
    	else if (mode.startsWith("rw")) {
    	    imode = Connector.READ_WRITE;
    	    m_bWriteAccess = true;
    	}
    	
    	if (imode < 0)
    	    throw new IllegalArgumentException("Illegal mode \"" + mode
    					       + "\" must be one of "
    					       + "\"r\", \" or rw\"");
        if (name == null) {
            throw new NullPointerException();
        }
    	open(name, imode);
    }
    
    private void open(String name, int mode) throws FileNotFoundException{
    	try{
    		m_file = (FileConnection)Connector.open(name, mode);
    		
            if (!m_file.exists() && mode == Connector.READ_WRITE ) { 
            	m_file.create();  // create the file if it doesn't exist
            }
    		
            m_fileSize = m_file.fileSize();
    	}catch(IOException exc){
    		System.out.println("RandomAccessFile:open Exception: " + exc.getMessage());
    		throw new FileNotFoundException(exc.getMessage());
    	}
    		
    }
    
    public long length() throws IOException
    {
   		return m_file != null ? m_file.fileSize() : 0;
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
    		m_file.close();
    	
    	m_file = null;
    }
    
    public void seek(long pos) throws IOException
    {
    	m_nSeekPos = pos;
    }
    
    public void write(byte b[]) throws IOException {
    	writeBytes(b, 0, b.length); 
    }
    public void write(byte b[], int off, int len) throws IOException {
    	writeBytes(b, off, len);
    }
    public final void readFully(byte b[]) throws IOException {
    	readFully(b, 0, b.length);
    }
    
    public final void writeInt(int v) throws IOException {
    	write((v >>> 24) & 0xFF);
    	write((v >>> 16) & 0xFF);
    	write((v >>>  8) & 0xFF);
    	write((v >>>  0) & 0xFF);
    	//written += 4;
    }

    private void prepareWrite() throws IOException{
        if (m_out == null){
			if ( !this.m_bWriteAccess )
				throw new IOException("Illegal mode");
        	
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
                    m_file.truncate(m_fileSize);
                } while (m_nSeekPos != m_fileSize);
            }
            m_outPos = m_nSeekPos;
        }
    }
    
    public void write(int b) throws IOException{
    	
    	prepareWrite();
        m_out.write(b);
        postWrite(1);
    }
    
    private void postWrite(int len)throws IOException{
        
        m_outPos += len;
        if (m_outPos > m_fileSize) { 
            m_fileSize = m_outPos;
        }
        //BB
        m_file.truncate(m_fileSize);
        if (m_in != null) { 
            m_in.close();
            m_in = null;
        }
        
        m_nSeekPos = m_outPos; 
    }
    
    private void writeBytes(byte b[], int off, int len) throws IOException
    {
    	prepareWrite();
        m_out.write(b, off, len);
        postWrite(len);
    }
    
    public final void writeLong(long v) throws IOException {
    	write((int)(v >>> 56) & 0xFF);
    	write((int)(v >>> 48) & 0xFF);
    	write((int)(v >>> 40) & 0xFF);
    	write((int)(v >>> 32) & 0xFF);
    	write((int)(v >>> 24) & 0xFF);
    	write((int)(v >>> 16) & 0xFF);
    	write((int)(v >>>  8) & 0xFF);
    	write((int)(v >>>  0) & 0xFF);
    	//written += 8;
    }
    
    public final void readFully(byte b[], int off, int len) throws IOException {
        int n = 0;
		do {
		    int count = this.read(b, off + n, len - n);
		    if (count < 0)
		    	throw new EOFException();
		    
		    n += count;
		} while (n < len);
    }
    
    public int read(byte b[], int off, int len) throws IOException {
    	return readBytes(b, off, len);
    }

    public final long readLong() throws IOException {
    	return ((long)(readInt()) << 32) + (readInt() & 0xFFFFFFFFL);
    }
    
    public final int readInt() throws IOException {
    	int ch1 = this.read();
    	int ch2 = this.read();
    	int ch3 = this.read();
    	int ch4 = this.read();
    	if ((ch1 | ch2 | ch3 | ch4) < 0)
    	    throw new EOFException();
    	return ((ch1 << 24) + (ch2 << 16) + (ch3 << 8) + (ch4 << 0));
    }
    
    private int readBytes(byte b[], int offData, int len) throws IOException{
        if ( !prepareRead() )
        	return -1;
        
        int off = offData;
        while (len > 0) { 
            int rc = m_in.read(b, off, len);
            if (rc > 0) 
            { 
                m_inPos += rc;
                off += rc;
                len -= rc;
            } else { 
                break;
            }
        }
        
        m_nSeekPos = m_inPos;
        return off;
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
    
    public int read() throws IOException
    {
        if ( !prepareRead() )
        	return -1;
        
        int res = m_in.read();
        if ( res >= 0 ){
	        m_inPos += 1;
	        m_nSeekPos = m_inPos;
        }
        
        return res;
    }
    
    public void sync() throws IOException 
    {
        if (m_out != null) 
        	m_out.flush();
    }

    public long getFilePointer() throws IOException
    {
        return m_nSeekPos;
    }
    
    public void setLength(long newLength) throws IOException
    {
        if (m_in != null) { 
            m_in.close();
            m_in = null;
        }
        if (m_out != null) { 
            m_out.close();
            m_out = null;
        }
    	
        if (newLength > m_fileSize) { 
            byte[] zeroBuf = new byte[ZERO_BUF_SIZE];
            do { 
                int size = newLength - m_fileSize > ZERO_BUF_SIZE ? ZERO_BUF_SIZE : (int)(newLength - m_fileSize);
                m_out.write(zeroBuf, 0, size);
                m_fileSize += size;
                
                m_file.truncate(m_fileSize);
            } while (newLength != m_fileSize);
        }else
            m_file.truncate(newLength);
        
        m_fileSize = newLength;
    }
    
    public final FileChannel getChannel() {
        //return new FileChannel();
    	throw new RuntimeException("Not Implemented");
    }
    
    public final void writeBytes(String s) throws IOException {
    	byte[] buf = s.getBytes(); 
    	writeBytes(buf, 0, buf.length);
    }
    
    public int read(byte b[]) throws IOException {
    	return readBytes(b, 0, b.length);
    }
    
    public final String readLine() throws IOException 
    {
    	StringBuffer input = new StringBuffer();
    	int c = -1;
    	boolean eol = false;

    	while (!eol) {
    	    switch (c = read()) {
    	    case -1:
    	    case '\n':
    	    	eol = true;
    		break;
    	    case '\r':
	    		eol = true;
	    		long cur = getFilePointer();
	    		if ((read()) != '\n') {
	    		    seek(cur);
	    		}
    		break;
    	    default:
    	    	input.append((char)c);
    		break;
    	    }
    	}

    	if ((c == -1) && (input.length() == 0)) {
    	    return null;
    	}
    	return input.toString();
    }
}
