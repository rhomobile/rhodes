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
import java.util.Enumeration;
import java.util.Vector;

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
    //private IRAFile m_raFile;
    private String m_strFullPath;
    
    public File(String path) 
    {
        _path = path.replace('\\', '/');
        
        try
        {
            m_strFullPath = _path;
            
            if ( m_strFullPath != null && m_strFullPath.length() > 0 && !m_strFullPath.startsWith("file:")) 
            	m_strFullPath = FilePath.join(RhoClassFactory.createFile().getDirPath(""), m_strFullPath);
        	
        }catch(Exception exc)
        {
        }
    }

    public File(String path, String mode) 
    {
        _path = path.replace('\\', '/');
        
        try
        {
            m_strFullPath = _path;
            if ( m_strFullPath != null && m_strFullPath.length() > 0 && !m_strFullPath.startsWith("file:")) 
            	m_strFullPath = FilePath.join(RhoClassFactory.createFile().getDirPath(""), m_strFullPath);
        	
        }catch(Exception exc)
        {
        }
    }
    
    public String getPath() {
        return _path;
    }
   
    public String getParent() {
        int index = _path.lastIndexOf('/');
        //if ( index < 0 )
        //	index = _path.lastIndexOf('\\');
        
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
        //if ( index < 0 )
        //	index = _path.lastIndexOf('\\');

        if ( index < 0 )
        	return _path;
        
        return _path.substring(index+1, _path.length());
    }
    
    public boolean exists() 
    {
    	IRAFile raFile = null;
    	try
    	{
        	raFile = RhoClassFactory.createFSRAFile();
    		raFile.open(m_strFullPath, "r");
    	
    		return raFile.exists();
    	}catch(Exception exc)
    	{
    		return false;
    	}finally
    	{
    		if (raFile!= null)
    			try{ raFile.close(); } catch(Exception exc){}
    	}
    }

    public boolean isDirectory() 
    {
    	IRAFile raFile = null;
    	try
    	{
        	raFile = RhoClassFactory.createFSRAFile();
    		raFile.open(m_strFullPath, "r");
    	
    		return raFile.isDirectory();
    	}catch(Exception exc)
    	{
    		return false;
    	}finally
    	{
    		if (raFile!= null)
    			try{ raFile.close(); } catch(Exception exc){}
    	}
    }

    public File[] listFiles() {
        throw new UnsupportedOperationException(
                "File operations not supported for J2ME build");
    }
    
    public Vector list() 
    {
    	Vector res = new Vector();

    	IRAFile raFile = null;
    	try
    	{
        	raFile = RhoClassFactory.createFSRAFile();
    		raFile.open(m_strFullPath, "r");
    	
	    	Enumeration eFiles = raFile.list();
	    	while( eFiles != null && eFiles.hasMoreElements() )
	    	{
	    		res.addElement(eFiles.nextElement());
	    	}
	    	
	    	return res;
    	}catch(Exception exc)
    	{
    		return res;
    	}finally
    	{
    		if (raFile!= null)
    			try{ raFile.close(); } catch(Exception exc){}
    	}
    }
    
    public String getAbsolutePath() 
    {
    	return m_strFullPath;
    }
    
    public boolean mkdir() 
    {
    	IRAFile raFile = null;
    	try
    	{
        	raFile = RhoClassFactory.createFSRAFile();
    		raFile.open(m_strFullPath, "dw");
    	
    		return raFile.mkdir();
    	}catch(Exception exc)
    	{
    		return false;
    	}finally
    	{
    		if (raFile!= null)
    			try{ raFile.close(); } catch(Exception exc){}
    	}
    }
    
    public boolean delete() 
    {
    	IRAFile raFile = null;
    	try
    	{
        	raFile = RhoClassFactory.createFSRAFile();

    		raFile.open(m_strFullPath, "w");
    		raFile.delete();
    		
    		return true;
    	}catch(Exception exc)
    	{
    		return false;
    	}finally
    	{
    		if (raFile!= null)
    			try{ raFile.close(); } catch(Exception exc){}
    	}
    }
    
    public boolean isFile() 
    {
    	IRAFile raFile = null;
    	try
    	{
        	raFile = RhoClassFactory.createFSRAFile();
    		raFile.open(m_strFullPath, "r");
    	
    		return raFile.isFile();
    	}catch(Exception exc)
    	{
    		return false;
    	}finally
    	{
    		if (raFile!= null)
    			try{ raFile.close(); } catch(Exception exc){}
    	}
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
    	IRAFile raFile = null;
    	try
    	{
        	raFile = RhoClassFactory.createFSRAFile();
    		raFile.open(m_strFullPath, "r");
    	
    		return raFile.size();
    	}catch(Exception exc)
    	{
    		return 0;
    	}finally
    	{
    		if (raFile!= null)
    			try{ raFile.close(); } catch(Exception exc){}
    	}
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
