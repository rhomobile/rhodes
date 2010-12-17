package com.rho.db;

import j2me.lang.CharacterMe;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.file.*;

import net.rim.device.api.io.URI;
import net.rim.device.api.database.*;
import java.util.Vector;

public class SqliteStorage implements IDBStorage 
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("SqliteStorage");
	
	private IFileAccess m_fs;
	private boolean m_bPendingTransaction = false;
	private Database m_db;
	
	public SqliteStorage()
	{
		m_fs = new FileAccessBB();
	}
	
	public void deleteAllFiles(String strPath) throws Exception 
	{
		String strDbName = getNameNoExt(strPath);
		m_fs.delete(strDbName + ".data");
		m_fs.delete(strDbName + ".data-journal");
	}

	public IDBResult createResult() {
		return new SqliteResult();
	}

	void bindObject(Statement st, int i, Object obj )throws DatabaseException
	{
		if ( obj instanceof String)
			st.bind(i, (String)obj);
		else if ( obj instanceof Integer)
			st.bind(i, ((Integer)obj).intValue());
		else if ( obj instanceof Short)
			st.bind(i, ((Short)obj).shortValue());
		else if ( obj instanceof Long)
			st.bind(i, ((Long)obj).longValue());
		else if ( obj instanceof Float)
			st.bind(i, ((Float)obj).floatValue());
		else if ( obj instanceof Double)
			st.bind(i, ((Double)obj).doubleValue());
		else if ( obj instanceof Byte)
			st.bind(i, ((Byte)obj).byteValue());
		else if ( obj instanceof byte[])
			st.bind(i, (byte[])obj );
		
	}

	public IDBResult executeSQL(String strStatement, Object[] values,
			boolean bReportNonUnique) throws DBException 
	{
		if ( m_db == null )
			throw new RuntimeException("executeSQL: m_db == null");
		
		IDBResult res = null;
		try
		{
			while( strStatement != null && strStatement.length()> 0)
			{
				//LOG.INFO(strStatement);
	            int start = 0;
	            while ( start < strStatement.length() && (strStatement.charAt(start) == '\n' ||
	            		strStatement.charAt(start) == '\r' ||
	            		strStatement.charAt(start) == ';' ||
	            		CharacterMe.isWhitespace(strStatement.charAt(start) ) ) )
	            		start++;
	            if (start > 0 )
	            	strStatement = strStatement.substring(start);
	            	
				if ( strStatement == null || start >= strStatement.length() )
					break;
				
				String strSelect = strStatement.length() > 6 ? strStatement.substring(0, 6) : ""; 
				
				Statement st = m_db.createStatement(strStatement);
				boolean bDontCloseStatement = false;
				try
				{
					st.prepare();
                	strStatement = st.getTail();
					
					for ( int i = 0; values != null && i < values.length; i++ )
					{
						bindObject(st, i+1, values[i] );
					}
					
					if ( strSelect.equalsIgnoreCase("SELECT"))
					{
		                if ( res == null )
		                {
		                	res = new SqliteResult(st);
		                	bDontCloseStatement = true;
		                }
					}else
					{
						try
						{
							st.execute();
						}catch(DatabaseException exc)
						{
							if ( res == null && bReportNonUnique && exc.getMessage().indexOf("constraint failed") >= 0)
								res = new SqliteResult(true);
							else
								throw exc;
						}
						
	                	if ( res == null )
	                		res = new SqliteResult(null);
					}
					
				}finally
				{
					if ( !bDontCloseStatement )
						st.close();
					
					st = null;
				}
			}
		}catch(DatabaseException exc )
		{
			throw new DBException(exc);
		}
		return res;
	}

	public boolean isTableExists(String strName)throws DBException
	{
		Object vals[] = {strName};
		IDBResult res = executeSQL("SELECT name FROM sqlite_master WHERE type='table' AND name=?", vals, false );
		boolean bRes = !res.isEnd();
		res.close();
		
		return bRes;
	}
	
	public String[] getAllTableNames() throws DBException 
	{
		IDBResult res = executeSQL("SELECT name FROM sqlite_master WHERE type='table'", null ,false );
		
		Vector arTables = new Vector();
	    for ( ; !res.isEnd(); res.next() )
	    {
	    	arTables.addElement(res.getCurData());
	    }
	    
		String[] vecTables = new String[arTables.size()]; 
		for ( int i = 0; i<arTables.size();i++)
			vecTables[i] = (String)arTables.elementAt(i);
		
	    return vecTables;
	}

	public boolean isDbFileExists(String strPath) 
	{
		String strDbName = getNameNoExt(strPath);
		return m_fs.exists(strDbName + ".data");
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
	
	public void executeBatchSQL(String strSqlScript)throws DBException
	{
		executeSQL(strSqlScript, null, false);
	}
	
	public void open(String strPath, String strSqlScript) throws DBException 
	{
		try{
			String strDbName = getNameNoExt(strPath) + ".data";
			
			//m_dbSess.setDBCallback(this);

			URI myURI = URI.create(strDbName);
			
			if ( !m_fs.exists(strDbName) )
			{
				m_db = DatabaseFactory.create(myURI);
				
				m_db.beginTransaction();
				try
				{
					executeBatchSQL( strSqlScript );
				}catch(DBException exc)
				{
					m_db.rollbackTransaction();
					throw exc;
				}
				m_db.commitTransaction();
			}else
				m_db = DatabaseFactory.open(myURI);
				
			if ( m_bPendingTransaction )
				m_db.beginTransaction();
			
			m_bPendingTransaction = false;
		}catch(Exception exc ){
			throw new DBException(exc);
		}
	}

	public void startTransaction() throws DBException {
		try{
			if ( m_db == null )
				m_bPendingTransaction = true;
			else
				m_db.beginTransaction();
		}catch(DatabaseException exc ){
			throw new DBException(exc);
		}
	}
	
	public void commit() throws DBException {
		try{
			if ( m_db!= null )
				m_db.commitTransaction();
		}catch(DatabaseException exc ){
			throw new DBException(exc);
		}
	}
	
	public void rollback() throws DBException 
	{
		try{
			if ( m_db!= null )
				m_db.rollbackTransaction();
		}catch(DatabaseException exc ){
			throw new DBException(exc);
		}
	}

	public void close() throws DBException 
	{
		try{
			if ( m_db!= null )
				m_db.close();
		}catch(DatabaseException exc ){
			throw new DBException(exc);
		}
	}
	
	public void setDbCallback(IDBCallback callback) {
		// TODO Auto-generated method stub

	}

}
