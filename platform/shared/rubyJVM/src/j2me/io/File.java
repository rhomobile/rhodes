/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.io;

import java.io.IOException;

import com.rho.FilePath;
import com.rho.RhoClassFactory;
import com.rho.file.IFileAccess;
import com.rho.file.IRAFile;

import j2me.lang.UnsupportedOperationException;

/**
 *  Class provided for the sole purpose of compiling the FileFilter and 
 *  FilenameFilter interfaces.
 */
public class File {

    public static final String separator = System.getProperty("file.separator");

    public static final char separatorChar = separator.charAt(0);

    public static final String pathSeparator = System.getProperty("path.separator") != null ? 
                System.getProperty("path.separator") : "/";

    public static final char pathSeparatorChar = pathSeparator.charAt(0);
	
    private String _path;
    private transient int prefixLength = 0;
    private IRAFile m_raFile;
    private String m_strFullPath;
    
    public File(String path) 
    {
        _path = path;
        
        try
        {
            m_strFullPath = _path;
            if (!m_strFullPath.startsWith("file:")) 
            	m_strFullPath = FilePath.join(RhoClassFactory.createFile().getDirPath(""), m_strFullPath);
        	
        	m_raFile = RhoClassFactory.createFSRAFile();
            m_raFile.open(m_strFullPath);
        }catch(Exception exc)
        {
        }
    }

    public String getPath() {
        return _path;
    }
   
    public String getParent() {
        int index = _path.lastIndexOf('/');
        if ( index < 0 )
        	index = _path.lastIndexOf('\\');
        
        if (index < prefixLength) {
            if ((prefixLength > 0) && (_path.length() > prefixLength))
                return _path.substring(0, prefixLength);
            return null;
        }
        return _path.substring(0, index);
    }

    public String getName() 
    {
    	if ( _path == null || _path.length() == 0)
    		return "";
    	
        int index = _path.lastIndexOf('/');
        if ( index < 0 )
        	index = _path.lastIndexOf('\\');

        if ( index < 0 )
        	return _path;
        
        return _path.substring(index+1, _path.length());
    }
    
    public boolean exists() 
    {
    	return m_raFile != null && m_raFile.exists();
    }

    public boolean isDirectory() 
    {
    	return m_raFile != null && m_raFile.isDirectory();    	
    }

    public File[] listFiles() {
        throw new UnsupportedOperationException(
                "File operations not supported for J2ME build");
    }
    
    public String[] list() {
        //TODO: list
        throw new RuntimeException("Not Implemented");
        //return new String[0]; 
    }
    public String getAbsolutePath() 
    {
    	return _path;
        //TODO: getAbsolutePath
        //throw new RuntimeException("Not Implemented");
        //return _path;
    }
    
    public boolean mkdir() {
        //TODO: mkdir
        throw new RuntimeException("Not Implemented");
        //return false;
    }
    public boolean delete() 
    {
    	try
    	{
	    	if ( m_raFile != null )
	    	{
	    		m_raFile.close();
	    		m_raFile.open(m_strFullPath, "w");
	    		m_raFile.delete();
	    		m_raFile.close();
	    		m_raFile = null;
	    		return true;
	    	}
    	}catch(IOException exc)
    	{
    		
    	}
    	
    	return false;
    }
    
    public boolean isFile() {
    	return m_raFile != null && m_raFile.isFile();    	
    }
    
    public boolean canWrite() {
        //TODO: canWrite
        throw new RuntimeException("Not Implemented");
        //return false;
    }
    public boolean canRead() {
        //TODO: canRead
        throw new RuntimeException("Not Implemented");
        //return false;
    }
    public long lastModified() {
        //TODO: lastModified
        throw new RuntimeException("Not Implemented");
        //return 0;
    }
    public long length() 
    {
    	try{
	    	if ( m_raFile != null )
	    		return m_raFile.size();
    	}catch(IOException exc)
    	{
    	}
    	return 0;
    }
    
    public boolean renameTo(File dest) {
        //TODO: renameTo
        throw new RuntimeException("Not Implemented");
        //return false;
    }
    public boolean setLastModified(long time) {
        //TODO: setLastModified
        throw new RuntimeException("Not Implemented");
        //return false;
    }
    
    public String[] list(FilenameFilter filter) {
        //TODO: list
        throw new RuntimeException("Not Implemented");
        //return new String[0];
    }
}
