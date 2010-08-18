package com.rho.db;

public interface IDBCallback 
{
	public abstract void onBeforeDelete(String tableName, IDBResult rows2Delete);
	public abstract void onBeforeUpdate(String tableName, IDBResult rows2Insert, int[] cols);
	public abstract void onAfterInsert(String tableName, IDBResult rows2Insert);
}
