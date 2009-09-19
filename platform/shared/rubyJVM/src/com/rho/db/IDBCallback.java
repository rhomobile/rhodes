package com.rho.db;

public interface IDBCallback 
{
	//public abstract void OnDeleteAll();
	//public abstract void OnDeleteAllFromTable(String tableName);
	public abstract void OnDeleteFromTable(String tableName, IDBResult rows2Delete);
	public abstract void OnInsertIntoTable(String tableName, IDBResult rows2Insert);

}
