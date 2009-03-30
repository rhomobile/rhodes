package com.rho.db;

public interface IDbCallback {

	public abstract void OnDeleteAllFromTable(String tableName);
	public abstract void OnDeleteFromTable(String tableName);
}
