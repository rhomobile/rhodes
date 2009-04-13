package com.rho;

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
    void open(String path, boolean readOnly, boolean noFlush);
    
    /**
     * Delete file
     * @param path path to the file
     */
    void delete(String path);
    
    boolean isOpened();
    String readString();
    void truncate(int nSize)throws IOException;
    InputStream getInputStream()throws IOException;
    OutputStream getOutStream();
    
    /**
     * 
     * @param path
     * @return
     */
    String getDirPath(String path) throws IOException;

}
