package com.rho.file;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Interface of simple file. Simple file can be automaticaslly opened by database given file path and access mode.
 */
public interface SimpleFile extends IFile { 
    /**
     * Open the file
     * @param path path to the file
     * @param readOnly if file is readonly
     * @param noFlush whther synchronous flush is needed
     */
    void open(String path, boolean readOnly, boolean noFlush)throws IOException;
    
    /**
     * Delete file
     * @param path path to the file
     */
    void delete(String path);
    
    boolean isOpened();
    String readString()throws IOException;
    void truncate(int nSize)throws IOException;
    InputStream getInputStream()throws IOException;
    OutputStream getOutStream();
    OutputStream getOutStreamEx(long pos)throws IOException;    
    long length();
    
    /**
     * 
     * @param path
     * @return
     */
    String getDirPath(String path) throws IOException;

    /**
     * @param fromClass
     * @param path
     * @return
     */
    public abstract InputStream getResourceAsStream(Class fromClass, String path);
    
    public abstract void renameOverwrite(String oldName, String newName);
    
    public abstract void copyJarFileToMemory(String strFileName, InputStream jarStream)throws IOException;
}
