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

/**
 * Interface of file.
 * Programmer can provide its own impleentation of this interface, adding such features
 * as support of flash cards, encrypted files,...
 * Implentation of this interface should throw StorageError exception in case of failure
 */
public interface IFile 
{ 
    /**
     * Write data to the file
     * @param pos offset in the file
     * @param buf array with data to be writter (size is always equal to database page size)
     */
    void write(long pos, byte[] buf)throws IOException;

    /**
     * Read data from the file
     * @param pos offset in the file
     * @param buf array to receive readen data (size is always equal to database page size)
     * @return number of bytes actually readen
     */
    int read(long pos, byte[] buf)throws IOException;

    /**
     * Flush all fiels changes to the disk
     */
    void sync()throws IOException;

    /**
     * Close file
     */
    void close()throws IOException;

    /**
     * Length of the file
     */
    long length();
    
}
