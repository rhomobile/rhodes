package com.rho.db;

import com.rho.db.DBException;
import com.rho.db.IDBResult;
import com.rho.db.IDBStorage;
import org.hsqldb.*;
import org.hsqldb.persist.*;

public class HsqlDBStorage implements IDBStorage, Session.IDBCallback{

	private Session m_dbSess;
	private FileUtilBB m_fs;
	private IDBCallback m_dbCallback;
	private HsqlDBRowResult m_rowResult = new HsqlDBRowResult();
	private boolean m_bPendingNoAutoCommit = false;
	private String m_strSqlScript;
	
	public HsqlDBStorage(){
		m_fs = FileUtilBB.getDefaultInstance(); 
	}
	
	public void deleteAllFiles(String strPath)throws Exception
	{
		String strDbName = getNameNoExt(strPath);
		m_fs.delete(strDbName + ".data");
		m_fs.delete(strDbName + ".script");
		m_fs.delete(strDbName + ".script.new");
		m_fs.delete(strDbName + ".journal");
	}
	
	private String getNameNoExt(String strPath){
		int nDot = strPath.lastIndexOf('.');
		String strDbName = "";
		if ( nDot > 0 )
			strDbName = strPath.substring(0, nDot);
		else
			strDbName = strPath;
		
		return strDbName;
	}
	
	public boolean isDbFileExists(String strPath)
	{
		String strDbName = getNameNoExt(strPath);
		return m_fs.exists(strDbName + ".data");
	}
	
	public void open(String strPath, String strSqlScript) throws DBException 
	{
		try{
			m_strSqlScript = strSqlScript;
			String strDbName = getNameNoExt(strPath);
			
			HsqlProperties props = new HsqlProperties();
			props.setProperty(HsqlDatabaseProperties.hsqldb_default_table_type, "cached");

			if ( !m_fs.exists(strDbName + ".script") && m_fs.exists(strDbName + ".script.new") )
				m_fs.renameElement(strDbName + ".script.new", strDbName + ".script");
				
			m_dbSess = DatabaseManager.newSession(DatabaseURL.S_FILE, strDbName, "SA", "", props);
			m_dbSess.setDBCallback(this);
			
			if ( !m_fs.exists(strDbName + ".data") )
			{
				m_dbSess.sqlExecuteDirectNoPreChecks(strSqlScript);
				m_dbSess.commitSchema();
			}
			
			if ( m_bPendingNoAutoCommit )
				m_dbSess.setAutoCommit(false);
			m_bPendingNoAutoCommit = false;
		}catch(HsqlException exc ){
			throw new DBException(exc);
		}
	}

	public void executeBatchSQL(String strSqlScript)throws DBException
	{
		if ( m_dbSess == null )
			throw new RuntimeException("executeSQL: m_dbSess == null");
		
		Result res = m_dbSess.sqlExecuteDirectNoPreChecks(strSqlScript);
		if ( res.getException() != null )
			throw new DBException(res.getException());
		
		m_dbSess.commitSchema();
	}
	
/*    static class HsqlDeleteTrigger implements org.hsqldb.Trigger {

        public void fire(int i, String name, String table, Object[] row1,
                         Object[] row2) {
            throw new RuntimeException("Missing Trigger class!");
        }
    }
*/ 
	//IDBCallback
	public void onBeforeDelete(Table table, Row row) {
		if ( m_dbCallback == null )
			return;
		
		m_rowResult.init(table, row);
		m_dbCallback.onBeforeDelete(table.getName().name, m_rowResult );
	}

	public void onAfterInsert(Table table, Row row) {
		if ( m_dbCallback == null )
			return;
		
		m_rowResult.init(table, row);
		m_dbCallback.onAfterInsert(table.getName().name, m_rowResult );
	}
	
	public void onBeforeUpdate(Table table, Row row, int[] cols) {
		if ( m_dbCallback == null )
			return;
		
		m_rowResult.init(table, row);
		m_dbCallback.onBeforeUpdate(table.getName().name, m_rowResult, cols );
	}
	
	//IDBCallback
	
	public void setDbCallback(IDBCallback callback)
	{
		m_dbCallback = callback;
		if ( m_dbSess != null )
			m_dbSess.setDBCallback(this);
		
//		m_dbSess.sqlExecuteDirectNoPreChecks(
//			"CREATE TRIGGER rhodeleteTrigger BEFORE DELETE ON object_values FOR EACH ROW QUEUE 0 CALL \"com.rho.HsqlDBStorage.HsqlDeleteTrigger\"");
//		m_dbSess.commitSchema();
	}
	
	public void close() throws DBException {
		try{
			if ( m_dbSess != null )
				m_dbSess.getDatabase().close(Database.CLOSEMODE_NORMAL);
		}catch(HsqlException exc ){
			throw new DBException(exc);
		}
	}

	public IDBResult createResult() {
		return new HsqlDBResult();
	}

	public IDBResult executeSQL(String strStatement, Object[] values, boolean bReportNonUnique)
			throws DBException {
		
		try {
			if ( m_dbSess == null )
				throw new RuntimeException("executeSQL: m_dbSess == null");
			
			CompiledStatement st = m_dbSess.compiledStatementManager.compile(m_dbSess, strStatement);
			Result res = m_dbSess.sqlExecuteCompiledNoPreChecksSafe(st, values, bReportNonUnique);
			if ( m_dbSess.isAutoCommit() )
				m_dbSess.commit();
			
			return new HsqlDBResult(res);
		}catch(HsqlException exc ){
			throw new DBException(exc);
		}
	}
	
	public void startTransaction()throws DBException
	{
		try{
			if ( m_dbSess == null )
				m_bPendingNoAutoCommit = true;
			else
				m_dbSess.setAutoCommit(false);
		}catch(HsqlException exc ){
			throw new DBException(exc);
		}
	}
	
	public void commit()throws DBException
	{
		try{
			if ( m_dbSess!= null )
				m_dbSess.setAutoCommit(true);
		}catch(HsqlException exc ){
			throw new DBException(exc);
		}
			
	}

	public void rollback()throws DBException
	{
		if ( m_dbSess!= null )
			m_dbSess.rollback();
	}
	
	public String[] getAllTableNames()throws DBException
	{
		org.hsqldb.lib.HsqlArrayList arTables = m_dbSess.getDatabase().schemaManager.getAllTables();
		
		String[] vecTables = new String[arTables.size()]; 
		
		for ( int i = 0; i< arTables.size(); i++ )
			vecTables[i] = ((Table)arTables.get(i)).getName().name;
		
		return vecTables;
	}
/*	
	private String createInsertStatement(HsqlDBResult res)
	{
		String strInsert = "INSERT INTO ";
		
		strInsert += res.getResult().metaData.tableNames[0];
		strInsert += "(";
		String strQuest = ") VALUES(";
		for (int i = 0; i < res.getColCount(); i++ )
		{
			if ( i > 0 )
			{
				strInsert += ",";
				strQuest += ",";
			}
			
			strInsert += res.getColName(i);
			strQuest += "?";
		}
		
		strInsert += strQuest + ")"; 
		return strInsert;
	}
	
    private void destroy_table(String strTable)throws DBException 
    {
		if ( m_dbSess == null )
			return;
		
		String dbName = getNameNoExt(m_dbSess.getDatabase().getPath());
		String dbNewName  = dbName + "new";
		
	    m_fs.delete(dbNewName + ".data");
	    m_fs.delete(dbNewName + ".script");
		
		HsqlDBStorage db = new HsqlDBStorage();
		db.open(dbNewName, m_strSqlScript);
		
		org.hsqldb.lib.HsqlArrayList arTables = m_dbSess.getDatabase().schemaManager.getAllTables();
		//String[] tables = {"sources", "client_info"};
		//Copy all other tables
		HsqlDBResult res;

	    db.startTransaction();
		
		for ( int i = 0; i< arTables.size(); i++ )
		{
			String tableName = ((Table)arTables.get(i)).getName().name;
			if ( tableName.equalsIgnoreCase(strTable) )
				continue;
			
			res = (HsqlDBResult)executeSQL("SELECT * from " + tableName, null);
			String strInsert = "";
		    for ( ; !res.isEnd(); res.next() )
		    {
		    	if ( strInsert.length() == 0 )
		    		strInsert = createInsertStatement(res);
		    	
		    	db.executeSQL(strInsert, res.getCurData() );
		    }
		}
		
	    db.commit();
	    db.close();
	    close();
	    
	    m_fs.renameElement(dbNewName + ".data", dbName+".data");
	    m_fs.renameElement(dbNewName + ".script", dbName+".script");
	    open(dbName, m_strSqlScript );
    }*/
	
}
