/*
 * Javolution - Java(TM) Solution for Real-Time and Embedded Systems
 * Copyright (C) 2005 - Javolution (http://javolution.org/)
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software is
 * freely granted, provided that this notice is preserved.
 */
package j2me.io;

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
        int index = _path.lastIndexOf(separatorChar);
        if (index < prefixLength) {
            if ((prefixLength > 0) && (_path.length() > prefixLength))
                return _path.substring(0, prefixLength);
            return null;
        }
        return _path.substring(0, index);
        }
    
    public boolean exists() {
        throw new UnsupportedOperationException(
                "File operations not supported for J2ME build");
    }

    public boolean isDirectory() {
        throw new UnsupportedOperationException(
                "File operations not supported for J2ME build");
    }

    public String getName() {
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
    public String getAbsolutePath() {
        //TODO: getAbsolutePath
        throw new RuntimeException("Not Implemented");
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
