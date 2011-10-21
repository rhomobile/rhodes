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

import j2me.io.FileNotFoundException;
import java.util.Enumeration;

public interface IRAFile {
	public void open(String name) throws FileNotFoundException;
	public void open(String name, String mode) throws FileNotFoundException;
	public void close() throws IOException;

	public long size() throws IOException;
	public void setSize(long newSize) throws IOException;

	public void seek(long pos) throws IOException;
	public long seekPos() throws IOException;

	public void write(int b) throws IOException;
	public void write(byte[] b, int off, int len) throws IOException;

	public int read() throws IOException;
	public int read(byte[] b, int off, int len) throws IOException;

	public void sync() throws IOException;

	public void listenForSync(String name) throws IOException;
	public void stopListenForSync(String name) throws IOException;

	public boolean exists();
	public void delete() throws IOException;
	public void rename(String newName) throws IOException;

	public boolean isDirectory();
	public boolean isFile();

	public boolean mkdir();

	public Enumeration list()throws IOException;
}
