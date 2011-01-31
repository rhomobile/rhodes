package com.rho.db;

import com.rho.db.DBAttrManager;

import java.util.Vector;

/*
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;
import com.rho.*;
import com.rho.file.*;
*/
//import java.util.Enumeration;
//import java.util.Vector;
//import java.util.Hashtable;
//
//import com.xruby.runtime.lang.RubyBasic;
//import com.xruby.runtime.lang.RubyClass;
//import com.xruby.runtime.lang.RubyValue;

public class DBAdapter /*extends RubyBasic*/ 
{

	public static DBAdapter getUserDB() {
		// TODO Auto-generated method stub
		return null;
	}

	public static DBAdapter getDB(String strPartition) {
		// TODO Auto-generated method stub
		return null;
	}

	public IDBResult executeSQL(String string) {
		// TODO Auto-generated method stub
		return null;
	}

	public IDBResult executeSQL(String string, Integer id, String string2) {
		return null;
	}

	public IDBResult executeSQL(String string, Long long1, Integer integer,
			Integer integer2, Long long2, Integer integer3, Integer integer4,
			Integer id) {
				return null;
		// TODO Auto-generated method stub
		
	}

	public IDBResult executeSQL(String string, Long long1, Integer id) {
		return null;
		// TODO Auto-generated method stub
		
	}

	public IDBResult executeSQL(String string, String strMetadata, Integer id) {
		return null;
		// TODO Auto-generated method stub
		
	}

	public IDBResult executeSQL(String strSqlUpdate, String strNewObject,
			String strOldObject) {
				return null;
		// TODO Auto-generated method stub
		
	}

	public IDBResult executeSQL(String string, String strNewObject,
			String strAttrib, Integer id, String strOldObject) {
				return null;
		// TODO Auto-generated method stub
		
	}

	public IDBResult executeSQL(String string, int nSrcID) {
		// TODO Auto-generated method stub
		return null;
	}

	public IDBResult executeSQL(String strSqlUpdate, String strObject) {
		return null;
		// TODO Auto-generated method stub
		
	}

	public IDBResult executeSQL(String string, String strObject, Object elementAt,
			Integer id) {
				return null;
		// TODO Auto-generated method stub
		
	}

	public IDBResult executeSQLEx(String strSqlUpdate, Vector vecValues) {
		return null;
		// TODO Auto-generated method stub
		
	}

	public IDBResult executeSQLReportNonUnique(String string,
			String m_strAttrib, Integer id, String strObject, String m_strValue) {
		// TODO Auto-generated method stub
		return null;
	}

	public IDBResult executeSQLReportNonUniqueEx(String strSqlInsert,
			Vector vecValues) {
		// TODO Auto-generated method stub
		return null;
	}

	public static Vector getDBAllPartitionNames() {
		// TODO Auto-generated method stub
		return null;
	}

	public void endTransaction() {
		// TODO Auto-generated method stub
		
	}

	public DBAttrManager getAttrMgr() {
		// TODO Auto-generated method stub
		return null;
	}

	public IDBResult executeSQL(String string, String m_strValue, String strObject,
			String m_strAttrib, Integer id) {
				return null;
		// TODO Auto-generated method stub
		
	}

	public String getDBPath() {
		// TODO Auto-generated method stub
		return null;
	}

	public static void initAttrManager() {
		// TODO Auto-generated method stub
		
	}

	public boolean isTableExist(String m_strName) {
		// TODO Auto-generated method stub
		return false;
	}

	public boolean isUIWaitDB() {
		// TODO Auto-generated method stub
		return false;
	}

	public void Lock() {
		// TODO Auto-generated method stub
		
	}

	public void Unlock() {
		// TODO Auto-generated method stub
		
	}

	public void setBulkSyncDB(String fDataName, String fScriptName) {
		// TODO Auto-generated method stub
		
	}

	public void startTransaction() {
		// TODO Auto-generated method stub
		
	}
}