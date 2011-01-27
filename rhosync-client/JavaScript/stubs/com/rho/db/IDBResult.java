package com.rho.db;

public interface IDBResult {

	String getStringByIdx(int i);

	void close();

	int getColCount();

	String getColName(int i);

	int getIntByIdx(int i);

	long getLongByIdx(int i);

	boolean isEnd();

	boolean isNonUnique();

	boolean isNullByIdx(int i);

	boolean isOneEnd();

	void next();

}
