package com.rhomobile.rhodes.db;

import java.io.File;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.db.DBException;
import com.rho.db.IDBCallback;
import com.rho.db.IDBResult;
import com.rho.db.IDBStorage;
import com.rhomobile.rhodes.AndroidFile;
import com.rhomobile.rhodes.RhodesInstance;

public class DBStorage implements IDBStorage {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("DBStorage");
	
	private IDBCallback callback;
	private String dbPath;
	private String dbName;
	private RhoDB rhoDB;

	synchronized public void close() throws DBException {
		if (rhoDB != null && rhoDB.isOpen()) {
			rhoDB.close();
			rhoDB = null;
		}
	}

	synchronized public void commit() throws DBException {
		if ( rhoDB != null && rhoDB.isOpen())
		    rhoDB.endTransaction();
	}

    synchronized public void rollback() throws DBException
    {
        if (rhoDB != null && rhoDB.isOpen())
            rhoDB.rollbackTransaction();
    }

	synchronized public IDBResult createResult() {
		return new SqliteDBResult();
	}

	synchronized public void deleteAllFiles(String strPath) throws Exception {
		AndroidFile andFile = new AndroidFile();
		andFile.delete(strPath);
	}

    synchronized public IDBResult executeSQL(String strStatement, Object[] values, boolean bReportNonUnique)
			throws DBException {
		if (rhoDB != null && rhoDB.isOpen()) {
            IDBResult result = executeSQLNoCallbacks(strStatement, values, bReportNonUnique);
			
			onInsertRecords(strStatement);
			onDeleteRecords(strStatement);
			
			return result;
		}
		return null;
	}
    
    synchronized private IDBResult executeSQLNoCallbacks(String strStatement, Object[] values,
    		boolean bReportNonUnique) throws DBException
    {
    	return rhoDB.executeSQL(strStatement, values, bReportNonUnique);
    }
	
	void onInsertRecords(String strStatement) {
		IDBResult rows2Insert = null;
		try {
			if (this.callback == null)
				return;
			
			if (!strStatement.toLowerCase().startsWith("insert "))
				return;
			
			rows2Insert = executeSQLNoCallbacks("SELECT * FROM object_attribs_to_load", null, false);
			this.callback.OnInsertIntoTable("object_values", rows2Insert);
			executeSQLNoCallbacks("DELETE FROM object_attribs_to_load", null, false);
		}
		catch (Exception e) {
			LOG.ERROR(e.getMessage());
		}
	}
	
	void onDeleteRecords(String strStatement){
		IDBResult rows2Delete = null;
		try
		{
			if (this.callback == null)
				return;
			
			if (!strStatement.toLowerCase().startsWith("delete "))
				return;
		
			rows2Delete = executeSQLNoCallbacks("SELECT * FROM object_values_to_delete", null,false);
			this.callback.OnDeleteFromTable("object_values", rows2Delete);
			executeSQLNoCallbacks("DELETE FROM object_values_to_delete", null, false);
		}
		catch( Exception e ){
			LOG.ERROR(e.getMessage());
		}finally{
			//if ( rows2Delete != null)
			//	rows2Delete.close();
		}
	}

	synchronized public void open(String strPath, String strSqlScript) throws DBException {
		parsePath(strPath);

		close();

		rhoDB = new RhoDB(RhodesInstance.getInstance(), this.dbPath,
				this.dbName);

		boolean loadSchema = false;
		if (!(new File(strPath).exists()))
			loadSchema = true;
			
		rhoDB.open();
		
		if ( loadSchema )
			rhoDB.loadSchema(strSqlScript);
	}

	synchronized public void setDbCallback(IDBCallback callback) {
		this.callback = callback;
	}

	synchronized public void startTransaction() throws DBException {
		 if ( rhoDB != null && rhoDB.isOpen())
		    rhoDB.beginTransaction();
	}

	private void parsePath(String strPath) {
		int lastSep = java.lang.Math.max(strPath.lastIndexOf("/"), strPath
				.lastIndexOf("\\"));
		dbPath = strPath.substring(0, lastSep+1);
		dbName = strPath.substring(lastSep+1, strPath.length());
	}

	public String[] getAllTableNames()throws DBException
	{
		IDBResult res = rhoDB.executeSQL("SELECT name FROM sqlite_master WHERE type='table' ", null);
		java.util.Vector<String> arTables = new java.util.Vector<String>();
	    for ( ; !res.isEnd(); res.next() )
	    	arTables.add(res.getStringByIdx(0));
	    
		String[] resTables = new String[arTables.size()];
		arTables.copyInto(resTables);
		
		return resTables;
	}
}
