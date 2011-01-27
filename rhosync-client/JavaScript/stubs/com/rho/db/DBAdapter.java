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
/*
	static String USER_PARTITION_NAME(){return "user";}
    static Hashtable getDBPartitions(){ return  null; }
    
    String m_strClientInfoInsert = "";
    Object[] m_dataClientInfo = null;
    
	DBAdapter(RubyClass c) {
	}

	public void close()
	{ 
	}
	
	public DBAttrManager getAttrMgr()
	{ 
		return null; 
	}

	public void executeBatchSQL(String strStatement)throws DBException{
	}
	
	public IDBResult executeSQL(String strStatement, Object[] values)throws DBException{
		return null;
	}
	public IDBResult executeSQL(String strStatement)throws DBException{
		return null;
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1)throws DBException{
		return null;
	}
	public IDBResult executeSQL(String strStatement, int arg1)throws DBException{
		return null;
	}
	public IDBResult executeSQL(String strStatement, int arg1, int arg2)throws DBException{
		return null;
	}
	
	public IDBResult executeSQL(String strStatement, long arg1)throws DBException{
		return null;
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2)throws DBException{
		return null;
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3)throws DBException{
		return null;
	}
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4)throws DBException{
		return null;
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5)throws DBException{
		return null;
	}
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5, Object arg6)throws DBException{
		return null;
	}
	
	public IDBResult executeSQLReportNonUnique(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4)throws DBException{
		return null;
	}

	public IDBResult executeSQLReportNonUniqueEx(String strStatement, Vector vecValues)throws DBException{
		return null;
	}
	
	public IDBResult executeSQLEx(String strStatement, Vector vecValues)throws DBException{
		return null;
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5, Object arg6, Object arg7)throws DBException{
		return null;
	}

	public boolean isUIWaitDB(){ return false; }

	public void Lock()
	{
	}
	
	public void Unlock(){}
    public boolean isInsideTransaction(){ return false; }
	
	public static String makeBlobFolderName()throws Exception{
		return "";
	}
	
	public String getDBPath(){ 		return ""; }
	
    public void startTransaction()throws DBException
    {
    }
    
    public void commit()throws DBException
    {
    }

    public void rollback()throws DBException
    {
    }
    
    public void endTransaction()throws DBException
    {
    }

    class DBVersion
    {
    };
    
    DBVersion readDBVersion()throws Exception
	{
    	return null;
	}
	
	void writeDBVersion(DBVersion ver)throws Exception
	{
	}
    
	boolean migrateDB(DBVersion dbVer, String strRhoDBVer, String strAppDBVer )
	{
	    return false;
	}
	
	void checkDBVersion()throws Exception
	{
	}
	
	static Vector m_dbAdapters = new Vector();
	
	public static DBAdapter findDBAdapterByBaseName(String strBaseName)
	{
		return null;
	}
	
	public static void startAllDBTransaction()throws DBException
	{
	}

	public static void commitAllDBTransaction()throws DBException
	{
	}
	
	public boolean isTableExist(String strTableName)throws DBException
	{
		return true;
	}
	
    void copyChangedValues(DBAdapter db)throws DBException
    {
    }
    
    public void setBulkSyncDB(String fDbName, String fScriptName)
    {
    }

    public RubyValue rb_execute(RubyValue v, RubyValue batch, RubyValue arg) 
    {
    	return null;
    }
    
    //@RubyAllocMethod
    private static RubyValue alloc(RubyValue receiver) {
    	return null;
    }
    
    public static void closeAll()
    {
    }

    public static void initAttrManager()throws DBException
    {
    }
    
    public static DBAdapter getUserDB()
    {
    	return null;
    }

    public static DBAdapter getDB(String szPartition)
    {
    	return null;
    }

    public static Vector getDBAllPartitionNames()
    {
        Vector vecNames = new Vector();
        return vecNames;
    }
        
    public static boolean isAnyInsideTransaction()
    {
        return false;
    }
    
	public static void initMethods(RubyClass klass) {
	}
*/

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