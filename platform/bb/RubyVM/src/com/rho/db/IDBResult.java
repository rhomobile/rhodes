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

package com.rho.db;

import com.xruby.runtime.lang.RubyValue;

public interface IDBResult {
	//public abstract int getCount();
	public abstract int getColCount();
	public abstract String getColName(int nCol);
	public abstract String getOrigColName(int nCol);
	/*
	public abstract RubyValue getRubyValueByIdx(int nItem, int nCol);
	public abstract long getLongByIdx(int nItem, int nCol);
	public abstract int getIntByIdx(int nItem, int nCol);
	public abstract String getStringByIdx(int nItem, int nCol);

	public abstract RubyValue getRubyValue(int nItem, String colname);
	public abstract long getLong(int nItem, String colname);
	public abstract int getInt(int nItem, String colname);
	public abstract String getString(int nItem, String colname);*/

	//public abstract void close();//close cursor and release any locks

	//New
    public abstract boolean isEnd();
    public abstract void next() throws DBException;
	public abstract String getStringByIdx(int nCol);
	public abstract int getIntByIdx(int nCol);
	public abstract long getLongByIdx(int nCol);
	public abstract String getUInt64ByIdx(int nCol);

	public abstract RubyValue getRubyValueByIdx(int nCol);
	public abstract boolean isNullByIdx(int nCol);

	public abstract RubyValue getRubyValue(String colname);
	public abstract int getInt(String colname);
	public abstract String getString(String colname);

	public abstract Object[] getCurData() throws DBException;

    public abstract boolean isNonUnique();

    public abstract void close();
}
