package com.rhomobile.rhodes.db;

import java.io.File;

import android.os.Environment;

import com.rho.db.DBException;
import com.rho.db.IDBCallback;
import com.rho.db.IDBResult;
import com.rho.db.IDBStorage;
import com.rhomobile.rhodes.AndroidFile;
import com.rhomobile.rhodes.RhodesInstance;

public class DBStorage implements IDBStorage {

	private IDBCallback callback;
	private String dbPath;
	private String dbName;
	private RhoDB rhoDB;

	public void close() throws DBException {
		if (rhoDB != null && rhoDB.isOpen()) {
			rhoDB.close();
			rhoDB = null;
		}
	}

	public void commit() throws DBException {
		// if ( rhoDB != null && rhoDB.isOpen())
		// rhoDB.endTransaction();
	}

	public IDBResult createResult() {
		return new SqliteDBResult();
	}

	public void deleteAllFiles(String strPath) throws Exception {
		AndroidFile andFile = new AndroidFile();
		andFile.delete(strPath);
	}

	public IDBResult executeSQL(String strStatement, Object[] values)
			throws DBException {
		if (rhoDB != null && rhoDB.isOpen()) {
			return rhoDB.executeSQL(strStatement, values);
		}
		return null;
	}

	public void open(String strPath, String strSqlScript) throws DBException {
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

	public void setDbCallback(IDBCallback callback) {
		this.callback = callback;
	}

	public void startTransaction() throws DBException {
		// if ( rhoDB != null && rhoDB.isOpen())
		// rhoDB.beginTransaction();
	}

	private void parsePath(String strPath) {
		int lastSep = java.lang.Math.max(strPath.lastIndexOf("/"), strPath
				.lastIndexOf("\\"));
		dbPath = strPath.substring(0, lastSep+1);
		dbName = strPath.substring(lastSep+1, strPath.length());
	}

}
