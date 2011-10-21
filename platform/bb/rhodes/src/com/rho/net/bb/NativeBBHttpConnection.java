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

package com.rho.net.bb;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.microedition.io.HttpConnection;
import com.rho.net.IHttpConnection;

public class NativeBBHttpConnection implements HttpConnection {

	IHttpConnection m_conn;

	public NativeBBHttpConnection(IHttpConnection conn){
		m_conn = conn;
	}

	public Object getNativeConnection(){
		return m_conn;
	}

	public long getDate() throws IOException {
		return m_conn.getDate();
	}

	public long getExpiration() throws IOException {
		return m_conn.getExpiration();
	}

	public String getFile() {
		return m_conn.getFile();
	}

	public String getHeaderField(String name) throws IOException {
		return m_conn.getHeaderField(name);
	}

	public String getHeaderField(int n) throws IOException {
		return m_conn.getHeaderField(n);
	}

	public long getHeaderFieldDate(String name, long def) throws IOException {
		return m_conn.getHeaderFieldDate(name, def);
	}

	public int getHeaderFieldInt(String name, int def) throws IOException {
		return m_conn.getHeaderFieldInt(name, def);
	}

	public String getHeaderFieldKey(int n) throws IOException {
		return m_conn.getHeaderFieldKey(n);
	}

	public String getHost() {
		return m_conn.getHost();
	}

	public long getLastModified() throws IOException {
		return m_conn.getLastModified();
	}

	public int getPort() {
		return m_conn.getPort();
	}

	public String getProtocol() {
		return m_conn.getProtocol();
	}

	public String getQuery() {
		return m_conn.getQuery();
	}

	public String getRef() {
		return m_conn.getRef();
	}

	public String getRequestMethod() {
		return m_conn.getRequestMethod();
	}

	public String getRequestProperty(String key) {
		return m_conn.getRequestProperty(key);
	}

	public int getResponseCode() throws IOException {
		return m_conn.getResponseCode();
	}

	public String getResponseMessage() throws IOException {
		return m_conn.getResponseMessage();
	}

	public String getURL() {
		return m_conn.getURL();
	}

	public void setRequestMethod(String method) throws IOException {
		m_conn.setRequestMethod(method);
	}

	public void setRequestProperty(String key, String value) throws IOException {
		m_conn.setRequestProperty(key, value);
	}

	public String getEncoding() {
		return m_conn.getEncoding();
	}

	public long getLength() {
		return m_conn.getLength();
	}

	public String getType() {
		return m_conn.getType();
	}

	public DataInputStream openDataInputStream() throws IOException {
		return m_conn.openDataInputStream();
	}

	public InputStream openInputStream() throws IOException {
		return m_conn.openInputStream();
	}

	public void close() throws IOException {
		m_conn.close();
	}

	public DataOutputStream openDataOutputStream() throws IOException {
		return m_conn.openDataOutputStream();
	}

	public OutputStream openOutputStream() throws IOException {
		return m_conn.openOutputStream();
	}

}
