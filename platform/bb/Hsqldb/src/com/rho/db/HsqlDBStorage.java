package com.rho.db;

import com.rho.db.DBException;
import com.rho.db.IDBResult;
import com.rho.db.IDBStorage;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.lang.RubyValue;
import org.hsqldb.*;
import org.hsqldb.persist.*;

public class HsqlDBStorage implements IDBStorage {

	private Session m_dbSess;
	private FileUtilBB m_fs;
	
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
	
	public void open(String strPath, String strSqlScript) throws DBException 
	{
		try{
			String strDbName = getNameNoExt(strPath);
			
			HsqlProperties props = new HsqlProperties();
			props.setProperty(HsqlDatabaseProperties.hsqldb_default_table_type, "cached");
	
			m_dbSess = DatabaseManager.newSession(DatabaseURL.S_FILE, strDbName, "SA", "", props);
			
			if ( !m_fs.exists(strDbName + ".data") )
			{
				m_dbSess.sqlExecuteDirectNoPreChecks(strSqlScript);
				m_dbSess.commitSchema();
			}
		}catch(HsqlException exc ){
			throw new DBException(exc);
		}
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

	public IDBResult executeSQL(String strStatement, Object[] values)
			throws DBException {
		
		try {
			if ( m_dbSess == null )
				throw new RuntimeException("executeSQL: m_dbSess == null");
			
			CompiledStatement st = m_dbSess.compiledStatementManager.compile(m_dbSess, strStatement);
			Result res = m_dbSess.sqlExecuteCompiledNoPreChecksSafe(st, values);
			
			if ( m_dbSess.isAutoCommit() )
				m_dbSess.commit();
			
			return new HsqlDBResult(res);
		}catch(Throwable exc ){
			throw new DBException(exc);
		}
	}
	
}
