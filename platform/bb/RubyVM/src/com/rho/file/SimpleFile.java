/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
