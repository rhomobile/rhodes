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

public interface IDBStorage {

	public abstract void open(String strPath, String strSqlScript, String strEncryptionInfo)throws DBException;
	public abstract void close()throws DBException;
	
	public abstract IDBResult executeSQL(String strStatement, Object[] values, boolean bReportNonUnique, boolean bNoCopy)throws DBException;
	public abstract IDBResult executeSQL(String strStatement, Object[] values, boolean bReportNonUnique)throws DBException;
	
	public abstract void executeBatchSQL(String strStatement)throws DBException;
	
	public abstract IDBResult createResult();
	
	public abstract void deleteAllFiles(String strPath)throws Exception;
	
	public abstract void startTransaction()throws DBException;
	public abstract void commit()throws DBException;
	public abstract void rollback()throws DBException;
	public abstract void onBeforeCommit() throws DBException;
	
	public abstract void setDbCallback(IDBCallback callback);
	
	public abstract String[] getAllTableNames()throws DBException;
	public abstract boolean isTableExists(String strName)throws DBException;
	
	public abstract boolean isDbFileExists(String strPath);
	public abstract void createTriggers() throws DBException;
}
