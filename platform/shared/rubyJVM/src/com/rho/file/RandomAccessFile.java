package com.rho.file;

import java.io.*;

import j2me.io.FileNotFoundException;

import javax.microedition.io.Connector;

import com.rho.RhoClassFactory;
//import com.rho.RhoEmptyLogger;
//import com.rho.RhoLogger;

import j2me.nio.channels.*;

import j2me.io.File;

public class RandomAccessFile  
{
	//private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
	//	new RhoLogger("RAFile");
	
	private IRAFile m_impl = null;
	
    private boolean        m_bWriteAccess;
    
    public RandomAccessFile(String name, String mode)
        throws FileNotFoundException
    {
        this(name != null ? new File(name) : null, mode);
    }
        
    public RandomAccessFile(File file, String mode)
        throws FileNotFoundException
    {
    	String name = (file != null ? file.getAbsolutePath() : null);
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
    	
        try {
			m_impl = RhoClassFactory.createRAFile();
		} catch (Exception e) {
			throw new FileNotFoundException(e.getMessage());
		}
        m_impl.open(name, mode);
    }
    
    public long length() throws IOException
    {
   		return m_impl.size();
    }
    
    public void close() throws IOException {
        m_impl.close();
    }
    
    public void seek(long pos) throws IOException
    {
    	m_impl.seek(pos);
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

    public void write(int b) throws IOException
    {
    	if ( !this.m_bWriteAccess )
			throw new IOException("Illegal mode");
    	
    	m_impl.write(b);
    }
    
    private void writeBytes(byte b[], int off, int len) throws IOException
    {
    	if ( !this.m_bWriteAccess )
			throw new IOException("Illegal mode");
    	m_impl.write(b, off, len);
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
    
    private int readBytes(byte b[], int off, int len) throws IOException{
        return m_impl.read(b, off, len);
    }

    public int read() throws IOException
    {
        return m_impl.read();
    }
    
    public void sync() throws IOException 
    {
        m_impl.sync();
    }

    public long getFilePointer() throws IOException
    {
        return m_impl.seekPos();
    }
    
    public void setLength(long newLength) throws IOException
    {
        m_impl.setSize(newLength);
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
    
    public void listenForSync(String name) throws IOException {
    	m_impl.listenForSync(name);
    }
    
    public void stopListenForSync(String name) throws IOException {
    	m_impl.stopListenForSync(name);
    }
}
