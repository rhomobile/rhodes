package com.rho.db;

public interface IDBStorage {

	public abstract void open(String strPath, String strSqlScript, String strEncryptionInfo)throws DBException;
	public abstract void close()throws DBException;
	
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
