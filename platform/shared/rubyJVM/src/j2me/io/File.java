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
    
    public File(String path) {
        _path = path;
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
    	try
    	{
    		String strPath = _path;
	        if (!strPath.startsWith("file:")) 
           		strPath = FilePath.join(RhoClassFactory.createFile().getDirPath(""), strPath);
    		
	        com.rho.file.IRAFile fs = RhoClassFactory.createFSRAFile();
	        fs.open(strPath);
	    	return fs.exists();
    	}catch(Exception exc)
    	{
    		return false;
    	}
    }

    public boolean isDirectory() {
        throw new UnsupportedOperationException(
                "File operations not supported for J2ME build");
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
    public boolean delete() {
        //TODO: delete
        throw new RuntimeException("Not Implemented");
        //return false;
    }
    
    public boolean isFile() {
        //TODO: isFile
        throw new RuntimeException("Not Implemented");
        //return false;
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
    public long length() {
        //TODO: length
        throw new RuntimeException("Not Implemented");
        //return 0;
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
