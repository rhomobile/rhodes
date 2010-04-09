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
    public abstract void next();
	public abstract String getStringByIdx(int nCol);
	public abstract int getIntByIdx(int nCol);
	public abstract long getLongByIdx(int nCol);
	public abstract String getUInt64ByIdx(int nCol);

	public abstract RubyValue getRubyValueByIdx(int nCol);
	public abstract boolean isNullByIdx(int nCol);
	
	public abstract RubyValue getRubyValue(String colname);
	public abstract int getInt(String colname);
	public abstract String getString(String colname);
	
	public abstract Object[] getCurData();
	
    public abstract boolean isNonUnique();

}
