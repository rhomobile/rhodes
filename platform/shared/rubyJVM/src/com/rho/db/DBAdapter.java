package com.rho.db;

import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;
import com.rho.*;
import com.rho.file.*;

import java.util.Enumeration;
import java.util.Vector;
import java.util.Hashtable;

public class DBAdapter extends RubyBasic 
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("DbAdapter");

	//private static DBAdapter m_Instance;
	
	private IDBStorage m_dbStorage;
	private boolean m_bIsOpen = false;
	private String  m_strDBPath, m_strDBVerPath;
	private String  m_strDbPartition;
	private DBAttrManager m_attrMgr = new DBAttrManager();
	static Hashtable/*Ptr<String,CDBAdapter*>*/ m_mapDBPartitions = new Hashtable();
	
    Mutex m_mxDB = new Mutex();
    int m_nTransactionCounter=0;
    boolean m_bUIWaitDB = false;
	
    static String USER_PARTITION_NAME(){return "user";}
    static Hashtable/*Ptr<String,CDBAdapter*>&*/ getDBPartitions(){ return  m_mapDBPartitions; }
    
	DBAdapter(RubyClass c) {
		super(c);
		
		try{
			m_dbStorage = RhoClassFactory.createDBStorage();
		}catch(Exception exc){
    		LOG.ERROR("createDBStorage failed.", exc);
		}
	}
/*	
	public static DBAdapter getInstance(){
		if ( m_Instance == null )
			m_Instance = new DBAdapter(RubyRuntime.DatabaseClass); 
		
		return m_Instance;
	}*/

	private void setDbPartition(String strPartition)
	{
		m_strDbPartition = strPartition;
	}
	 
	public void close()
	{ 
		try{
			m_dbStorage.close();
			m_dbStorage = null;
		}catch(Exception exc)
		{
    		LOG.ERROR("DB close failed.", exc);
		}
	}
	
	public DBAttrManager getAttrMgr()
	{ 
		return m_attrMgr; 
	}

	public void executeBatchSQL(String strStatement)throws DBException{
		LOG.TRACE("executeBatchSQL: " + strStatement);
		
		m_dbStorage.executeBatchSQL(strStatement);
	}
	
	public IDBResult executeSQL(String strStatement, Object[] values)throws DBException{
		LOG.TRACE("executeSQL: " + strStatement);
		IDBResult res = null;
		Lock();
		try{
			res = m_dbStorage.executeSQL(strStatement,values,false);
		}finally
		{
			Unlock();
		}
		return res;
	}
	public IDBResult executeSQL(String strStatement)throws DBException{
		return executeSQL(strStatement,null);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1)throws DBException{
		Object[] values = {arg1};
		return executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement, int arg1)throws DBException{
		Object[] values = { new Integer(arg1)};
		return executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement, long arg1)throws DBException{
		Object[] values = { new Long(arg1)};
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2)throws DBException{
		Object[] values = {arg1,arg2};
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3)throws DBException{
		Object[] values = {arg1,arg2,arg3};
		return executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4)throws DBException{
		Object[] values = {arg1,arg2,arg3,arg4};
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5)throws DBException{
		Object[] values = {arg1,arg2,arg3,arg4,arg5};
		return executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5, Object arg6)throws DBException{
		Object[] values = {arg1,arg2,arg3,arg4,arg5,arg6};
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQLReportNonUnique(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4)throws DBException{
		LOG.TRACE("executeSQLReportNonUnique: " + strStatement);
		
		Object[] values = {arg1,arg2,arg3,arg4};
		IDBResult res = null;
		Lock();
		try{
			res = m_dbStorage.executeSQL(strStatement,values, true);
		}finally
		{
			Unlock();
		}
		
		return res; 
	}

	public IDBResult executeSQLReportNonUniqueEx(String strStatement, Vector vecValues)throws DBException{
		LOG.TRACE("executeSQLReportNonUnique: " + strStatement);
		
		Object[] values = new Object[vecValues.size()];
		for (int i = 0; i < vecValues.size(); i++ )
			values[i] = vecValues.elementAt(i);
		
		IDBResult res = null;
		Lock();
		try{
			res = m_dbStorage.executeSQL(strStatement,values, true);
		}finally
		{
			Unlock();
		}
		
		return res; 
	}
	
	public IDBResult executeSQLEx(String strStatement, Vector vecValues)throws DBException{
		Object[] values = new Object[vecValues.size()];
		for (int i = 0; i < vecValues.size(); i++ )
			values[i] = vecValues.elementAt(i);
		
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5, Object arg6, Object arg7)throws DBException{
		Object[] values = {arg1,arg2,arg3,arg4,arg5,arg6,arg7};
		return executeSQL(strStatement,values);
	}

	public boolean isUIWaitDB(){ return m_bUIWaitDB; }
	public void Lock()
	{
	    if ( RhoRuby.isMainRubyThread() )
	        m_bUIWaitDB = true;
		
		m_mxDB.Lock();
		
	    if ( RhoRuby.isMainRubyThread() )
	        m_bUIWaitDB = false;
	}
	
	public void Unlock(){ m_mxDB.Unlock(); }
    public boolean isInsideTransaction(){ return m_nTransactionCounter>0; }
	
	//public static IDBResult createResult(){
	//	return getInstance().m_dbStorage.createResult();
	//}
	
	public static String makeBlobFolderName()throws Exception{
		String fName = RhoClassFactory.createFile().getDirPath("db/db-files");
		
		return fName;
	}
	
	RubyString[] getOrigColNames(IDBResult rows)
	{
		RubyString[] colNames = new RubyString[rows.getColCount()];
		for ( int nCol = 0; nCol < rows.getColCount(); nCol ++ )
			colNames[nCol] = ObjectFactory.createString(rows.getOrigColName(nCol));
		
		return colNames;
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
    
	public String getDBPath(){ return getNameNoExt(m_strDBPath); }
	
    private void initFilePaths(String strDBName)throws Exception
    {
    	if ( strDBName.charAt(0) == '/' || strDBName.charAt(0) == '\\' )
    		strDBName = strDBName.substring(1);
    	
    	int nSlash = strDBName.lastIndexOf('/');
    	if ( nSlash < 0 )
    		nSlash = strDBName.lastIndexOf('\\');
    	
    	String strDBDir = "";
    	if ( nSlash > 0 )
    		strDBDir = strDBName.substring(0, nSlash);
    	
    	String strPath = RhoClassFactory.createFile().getDirPath(strDBDir);
    	m_strDBPath = strPath + strDBName.substring(nSlash+1);
    	m_strDBVerPath = strPath + getNameNoExt(strDBName.substring(nSlash+1)) +  ".version";
    }
    
    private String getSqlScript()
    {
    	return RhoConf.getInstance().loadFileFromJar("apps/db/syncdb.schema");
    }
    
    public void startTransaction()throws DBException
    {
    	Lock();
    	m_nTransactionCounter++;
    	if (m_nTransactionCounter == 1)
    		m_dbStorage.startTransaction();    	
    }
    
    public void commit()throws DBException
    {
    	m_nTransactionCounter--;
    	if (m_nTransactionCounter == 0)
    	{
	    	getAttrMgr().save(this);
	    	m_dbStorage.commit();
    	}
    	
    	Unlock();
    }

    public void rollback()throws DBException
    {
    	m_nTransactionCounter--;
    	if (m_nTransactionCounter == 0)     	
    		m_dbStorage.rollback();
    	
    	Unlock();
    }
    
    public void endTransaction()throws DBException
    {
    	commit();
    }

    class DBVersion
    {
    	String m_strRhoVer = "";
    	String m_strAppVer = "";
    	
    	DBVersion(){}
    	
    	DBVersion( String strRhoVer, String strAppVer )
    	{
    		m_strRhoVer = strRhoVer;
    		m_strAppVer = strAppVer;
    	}
    };
    
    /*private void testError()throws Exception
    {
    	throw new net.rim.device.api.io.file.FileIOException(net.rim.device.api.io.file.FileIOException.FILESYSTEM_FULL);
    }*/
    DBVersion readDBVersion()throws Exception
	{
    	String strFullVer = "";
    	IRAFile file = null;
    	try {
	    	file = RhoClassFactory.createRAFile();
	    	try{
	    		file.open(m_strDBVerPath);
	    	}catch(j2me.io.FileNotFoundException exc)
	    	{
	    		//file not exist
	    		return new DBVersion();
	    	}
	    	
	        byte buf[] = new byte[20];
//	        testError();
	        int len = file.read(buf, 0, buf.length);
			if ( len > 0 )
				strFullVer = new String(buf,0,len);
			
			if ( strFullVer.length() == 0 )
				return new DBVersion();
			
			int nSep = strFullVer.indexOf(';');
			if ( nSep == -1 )
				return new DBVersion(strFullVer, "");
			
			return new DBVersion(strFullVer.substring(0,nSep), strFullVer.substring(nSep+1) );
    	}
    	catch (Exception e) {
    		LOG.ERROR("readDBVersion failed.", e);
    		throw e;
    	}finally
    	{
    		if (file!=null)
    			try{ file.close(); }catch(Exception exc){}
    	}
	}
	
	void writeDBVersion(DBVersion ver)throws Exception
	{
		IRAFile file = null;
		try{
			file = RhoClassFactory.createRAFile();
			file.open(m_strDBVerPath, "rw");
	        String strFullVer = ver.m_strRhoVer + ";" + ver.m_strAppVer;
	        byte[] buf = strFullVer.getBytes();
	        file.write(buf, 0, buf.length);
		}catch (Exception e) {
	    	LOG.ERROR("writeDBVersion failed.", e);
	    	throw e;
	    }finally
	    {
	    	if (file!=null)
	    		try{ file.close(); }catch(Exception exc){}
	    }
	}
    
	boolean migrateDB(DBVersion dbVer, String strRhoDBVer, String strAppDBVer )
	{
	    LOG.INFO( "Try migrate database from " + (dbVer != null ? dbVer.m_strRhoVer:"") + " to " + (strRhoDBVer !=null ? strRhoDBVer:"") );
	    if ( dbVer != null && strRhoDBVer != null &&
	    	 (dbVer.m_strRhoVer.startsWith("1.4")||dbVer.m_strRhoVer.startsWith("1.4")) && (strRhoDBVer.startsWith("1.5")||strRhoDBVer.startsWith("1.4")) )
		{
            LOG.INFO( "No migration required from " + (dbVer != null ? dbVer.m_strRhoVer:"") + " to " + (strRhoDBVer !=null ? strRhoDBVer:"") );
            
            try{
	        	writeDBVersion( new DBVersion(strRhoDBVer, strAppDBVer) );
			}catch(Exception e)
			{
	    		LOG.ERROR("migrateDB failed.", e);
			}
			
            
		    return true;
		}
		
	    //1.2.x -> 1.5.x,1.4.x
	    if ( dbVer != null && strRhoDBVer != null &&
	    	 (dbVer.m_strRhoVer.startsWith("1.2")||dbVer.m_strRhoVer.startsWith("1.4")) && (strRhoDBVer.startsWith("1.5")||strRhoDBVer.startsWith("1.4")) )
	    {
	    //sources
	    //priority INTEGER, ADD
	    //backend_refresh_time int default 0, ADD
	    	LOG.INFO("Migrate database from " + dbVer.m_strRhoVer + " to " + strRhoDBVer);
	    	
			IDBStorage db = null;
			try{
			    db = RhoClassFactory.createDBStorage();	    
				db.open( m_strDBPath, getSqlScript() );
	
		        db.executeSQL( "ALTER TABLE sources ADD priority INTEGER", null, false);
		        db.executeSQL( "ALTER TABLE sources ADD backend_refresh_time int default 0", null, false);
	
		        {
		            Vector/*<int>*/ vecSrcIds = new Vector();
		            IDBResult res2 = db.executeSQL( "SELECT source_id FROM sources", null, false );
		            for ( ; !res2.isEnd(); res2.next() )
		                vecSrcIds.addElement( new Integer(res2.getIntByIdx(0)) );
	
		            for( int i = 0; i < vecSrcIds.size(); i++)
		            {
		            	Object[] values = {vecSrcIds.elementAt(i), vecSrcIds.elementAt(i)};
		                db.executeSQL( "UPDATE sources SET priority=? where source_id=?", values, false );
		            }
		        }
		        db.close();
		        db = null;
		        
		        writeDBVersion( new DBVersion(strRhoDBVer, strAppDBVer) );
	
		        return true;
			}catch(Exception e)
			{
	    		LOG.ERROR("migrateDB failed.", e);
	    		
				if (db!=null)
				{
					try{db.close();}catch(DBException exc)
					{
						LOG.ERROR("migrateDB : close db after error failed.", e);
					}
					db = null;
				}
			}
		}


	    return false;
	}
	
	void checkDBVersion()throws Exception
	{
		String strRhoDBVer = RhoSupport.getRhoDBVersion();
		String strAppDBVer = RhoConf.getInstance().getString("app_db_version");
		
		DBVersion dbVer = readDBVersion();

		boolean bRhoReset = false;
	    boolean bAppReset = false;
		
		if ( strRhoDBVer != null && strRhoDBVer.length() > 0 )
		{
			if ( dbVer == null || dbVer.m_strRhoVer == null || !dbVer.m_strRhoVer.equalsIgnoreCase(strRhoDBVer) )
				bRhoReset = true;
		}
		if ( strAppDBVer != null && strAppDBVer.length() > 0 )
		{
			if ( dbVer == null || dbVer.m_strAppVer == null || !dbVer.m_strAppVer.equalsIgnoreCase(strAppDBVer) )
				bAppReset = true;
		}
		
	    if ( bRhoReset && !bAppReset )
	        bRhoReset = !migrateDB(dbVer, strRhoDBVer, strAppDBVer);

		if ( bRhoReset || bAppReset )
		{
			m_dbStorage.deleteAllFiles(m_strDBPath);
			
			String fName = makeBlobFolderName();
			RhoClassFactory.createFile().delete(fName);
			DBAdapter.makeBlobFolderName(); //Create folder back
			
            writeDBVersion(new DBVersion(strRhoDBVer, strAppDBVer) );
		}
		
	}
	
	static Vector m_dbAdapters = new Vector();
	
	public static DBAdapter findDBAdapterByBaseName(String strBaseName)
	{
		for ( int i = 0; i < m_dbAdapters.size(); i++ )
		{
			DBAdapter item = (DBAdapter)m_dbAdapters.elementAt(i);
			FilePath oPath = new FilePath(item.getDBPath());
			
			if ( oPath.getBaseName().compareTo(strBaseName) == 0 )
				return item;
		}
		return null;
	}
	
	public static void startAllDBTransaction()throws DBException
	{
		for ( int i = 0; i < m_dbAdapters.size(); i++ )
		{
			DBAdapter item = (DBAdapter)m_dbAdapters.elementAt(i);
			item.startTransaction();
		}
	}

	public static void commitAllDBTransaction()throws DBException
	{
		for ( int i = 0; i < m_dbAdapters.size(); i++ )
		{
			DBAdapter item = (DBAdapter)m_dbAdapters.elementAt(i);
			item.commit();
		}
	}
	
    private void openDB(String strDBName)throws Exception
    {
    	if ( m_bIsOpen )
    		return;
    	
		initFilePaths(strDBName);
		checkDBVersion();
		m_dbStorage.open(m_strDBPath, getSqlScript() );
		
		//executeSQL("CREATE INDEX by_src ON object_values (source_id)", null);
		m_bIsOpen = true;
		
		getAttrMgr().load(this);
		
		m_dbStorage.setDbCallback(new DBCallback(this));
		
		m_dbAdapters.addElement(this);
    }
    
	private String createInsertStatement(IDBResult res, String tableName)
	{
		String strInsert = "INSERT INTO ";
		
		strInsert += tableName;
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
    
	private boolean destroyTableName(String tableName, Vector arIncludeTables, Vector arExcludeTables )
	{
	    int i;
	    for (i = 0; i < (int)arExcludeTables.size(); i++ )
	    {
	        if ( ((String)arExcludeTables.elementAt(i)).equalsIgnoreCase(tableName) )
	            return false;
	    }

	    for (i = 0; i < (int)arIncludeTables.size(); i++ )
	    {
	        if ( ((String)arIncludeTables.elementAt(i)).equalsIgnoreCase(tableName) )
	            return true;
	    }

	    return arIncludeTables.size()==0;
	}
	
	public boolean isTableExist(String strTableName)throws DBException
	{
		String[] vecTables = m_dbStorage.getAllTableNames();
		for ( int i = 0; i< vecTables.length; i++ )
		{
			String tableName = vecTables[i];
			if ( tableName.equalsIgnoreCase(strTableName) )
				return true;
		}
		
		return false;
	}
	
    private RubyValue rb_destroy_tables(RubyValue vInclude, RubyValue vExclude) 
    {
		if ( !m_bIsOpen )
			return RubyConstant.QNIL;
		
		IDBStorage db = null;
		try{
		    getAttrMgr().reset(this);
			
			Vector vecIncludes = RhoRuby.makeVectorStringFromArray(vInclude);
			Vector vecExcludes = RhoRuby.makeVectorStringFromArray(vExclude);
			
			String dbName = getNameNoExt(m_strDBPath);
			String dbNewName  = dbName + "new";
			
			IFileAccess fs = RhoClassFactory.createFileAccess();
			
			String dbNameData = dbName + ".data";
		    String dbNewNameData = dbNewName + ".data";
		    String dbNameScript = dbName + ".script";
		    String dbNewNameScript = dbNewName + ".script";
		    String dbNameJournal = dbName + ".journal";
		    String dbNewNameJournal = dbNewName + ".journal";
		    String dbNewNameProps = dbNewName + ".properties";
		    
			//LOG.TRACE("DBAdapter: " + dbNewNameDate + ": " + (fs.exists(dbNewNameData) ? "" : "not ") + "exists");
		    fs.delete(dbNewNameData);
		    //LOG.TRACE("DBAdapter: " + dbNewNameScript + ": " + (fs.exists(dbNewNameScript) ? "" : "not ") + "exists");
		    fs.delete(dbNewNameScript);
		    //LOG.TRACE("DBAdapter: " + dbNewNameJournal + ": " + (fs.exists(dbNewNameJournal) ? "" : "not ") + "exists");
		    fs.delete(dbNewNameJournal);
		    fs.delete(dbNewNameProps);
		    
		    LOG.TRACE("1. Size of " + dbNameData + ": " + fs.size(dbNameData));
		    
		    db = RhoClassFactory.createDBStorage();	    
			db.open( dbNewName, getSqlScript() );
			
			String[] vecTables = m_dbStorage.getAllTableNames();
			IDBResult res;
	
		    db.startTransaction();
			
			for ( int i = 0; i< vecTables.length; i++ )
			{
				String tableName = vecTables[i];
				if ( destroyTableName( tableName, vecIncludes, vecExcludes ) )
					continue;
				
				copyTable(tableName, this.m_dbStorage, db );
			}
			
		    db.commit();
		    db.close();
		    
		    m_dbStorage.close();
		    m_dbStorage = null;
		    m_bIsOpen = false;
		    
		    LOG.TRACE("2. Size of " + dbNewNameData + ": " + fs.size(dbNewNameData));
		    
		    fs.delete(dbNewNameProps);
		    fs.delete(dbNameJournal);
		    
			String fName = makeBlobFolderName();
			RhoClassFactory.createFile().delete(fName);
			DBAdapter.makeBlobFolderName(); //Create folder back
		    
		    fs.renameOverwrite(dbNewNameData, dbNameData);
		    fs.renameOverwrite(dbNewNameScript, dbNameScript);
		    
		    LOG.TRACE("3. Size of " + dbNameData + ": " + fs.size(dbNameData));
		    
		    m_dbStorage = RhoClassFactory.createDBStorage();
			m_dbStorage.open(m_strDBPath, getSqlScript() );
			m_bIsOpen = true;
			
			getAttrMgr().load(this);
			
			m_dbStorage.setDbCallback(new DBCallback(this));
			
		}catch(Exception e)
		{
    		LOG.ERROR("execute failed.", e);
    		
			if ( !m_bIsOpen )
			{
				LOG.ERROR("destroy_table error.Try to open old DB.");
				try{
					m_dbStorage.open(m_strDBPath, getSqlScript() );
					m_bIsOpen = true;
				}catch(Exception exc)
				{
					LOG.ERROR("destroy_table open old table failed.", exc);
				}
			}
			
			try {
				if ( db != null)
					db.close();
			} catch (DBException e1) {
				LOG.ERROR("closing of DB caused exception: " + e1.getMessage());
			}
    		
			throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
		}
		
		return RubyConstant.QNIL;
    }
    
    private void copyTable(String tableName, IDBStorage dbFrom, IDBStorage dbTo)throws DBException
    {
    	IDBResult res = dbFrom.executeSQL("SELECT * from " + tableName, null, false);
		String strInsert = "";
	    for ( ; !res.isEnd(); res.next() )
	    {
	    	if ( strInsert.length() == 0 )
	    		strInsert = createInsertStatement(res, tableName);
	    	
	    	dbTo.executeSQL(strInsert, res.getCurData(), false );
	    }
    }
    
    public void setBulkSyncDB(String fDbName, String fScriptName)
    {
		IDBStorage db = null;
		try{
		    db = RhoClassFactory.createDBStorage();	    
			db.open( fDbName, "" );

		    db.startTransaction();
			
		    copyTable("client_info", this.m_dbStorage, db );
		    
		    //update User partition
		    if ( m_strDbPartition.compareTo(USER_PARTITION_NAME()) == 0 )
		    {
		        //copy all NOT user sources from current db to bulk db
		        executeSQL("DELETE FROM sources WHERE partition=?", m_strDbPartition);
		        copyTable("sources", this.m_dbStorage, db );
		    }else
		    {
		        //remove all m_strDbPartition sources from user db
		        //copy all sources from bulk db to user db
		        DBAdapter dbUser  = getDB(USER_PARTITION_NAME());
		        dbUser.executeSQL("DELETE FROM sources WHERE partition=?", m_strDbPartition);

		        copyTable("sources", db, dbUser.m_dbStorage );
		    }
			
		    db.commit();
		    db.close();

		    m_dbStorage.close();
		    m_dbStorage = null;
		    m_bIsOpen = false;

			String dbName = getNameNoExt(m_strDBPath);
			IFileAccess fs = RhoClassFactory.createFileAccess();
			
			String dbNameData = dbName + ".data";
		    String dbNameScript = dbName + ".script";
		    String dbNameJournal = dbName + ".journal";
		    String dbNewNameProps = getNameNoExt(fDbName) + ".properties";
		    
		    fs.delete(dbNameJournal);
		    fs.delete(dbNewNameProps);
		    
			String fName = makeBlobFolderName();
			RhoClassFactory.createFile().delete(fName);
			DBAdapter.makeBlobFolderName(); //Create folder back
		    
		    fs.renameOverwrite(fDbName, dbNameData);
		    fs.renameOverwrite(fScriptName, dbNameScript);
		    
		    m_dbStorage = RhoClassFactory.createDBStorage();
			m_dbStorage.open(m_strDBPath, getSqlScript() );
			m_bIsOpen = true;
			
			getAttrMgr().load(this);
			
			m_dbStorage.setDbCallback(new DBCallback(this));
			
		}catch(Exception e)
		{
    		LOG.ERROR("execute failed.", e);
    		
			if ( !m_bIsOpen )
			{
				LOG.ERROR("destroy_table error.Try to open old DB.");
				try{
					m_dbStorage.open(m_strDBPath, getSqlScript() );
					m_bIsOpen = true;
				}catch(Exception exc)
				{
					LOG.ERROR("destroy_table open old table failed.", exc);
				}
			}
			
			try {
				if ( db != null)
					db.close();
			} catch (DBException e1) {
				LOG.ERROR("closing of DB caused exception: " + e1.getMessage());
			}
    		
			throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
		}
    	
    }
/*    
    public RubyValue rb_execute(RubyValue v) 
    {
    	RubyArray res = new RubyArray(); 
    	try{
    		IDBResult rows = executeSQL(v.toStr(), null);
    		RubyString[] colNames = getColNames(rows);
    		
    		for( ; !rows.isEnd(); rows.next() )
    		{
    			RubyHash row = ObjectFactory.createHash();
    			for ( int nCol = 0; nCol < rows.getColCount(); nCol ++ )
    				row.add( colNames[nCol], rows.getRubyValueByIdx(nCol) );
    			
    			res.add( row );
    		}
		}catch(Exception e)
		{
    		LOG.ERROR("execute failed.", e);
			throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
		}
    	
        return res;
    }*/

    public RubyValue rb_execute(RubyValue v, RubyValue batch, RubyValue arg) 
    {
    	RubyArray res = new RubyArray(); 
    	try{
    		String strSql = v.toStr();
    		if ( batch == RubyConstant.QTRUE )
    		{
    			LOG.INFO("batch execute:" + strSql);
    			
    			executeBatchSQL( strSql );
    		}
    		else
    		{
	    		Object[] values = null;
	    		if ( arg != null )
	    		{
		    		RubyArray args1 = arg.toAry();
		    		RubyArray args = args1;
		    		if ( args.size() > 0 && args.get(0) instanceof RubyArray )
		    			args = (RubyArray)args.get(0);
		    		
		    		values = new Object[args.size()];
		    		for ( int i = 0; i < args.size(); i++ )
		    		{
		    			RubyValue val = args.get(i);
		    			if ( val == RubyConstant.QNIL )
		    				values[i] = null;
		    			else if ( val instanceof RubyInteger )
		    				values[i] = new Long( ((RubyInteger)val).toLong() );
		    			else if ( val instanceof RubyFloat )
		    				values[i] = new Double( ((RubyFloat)val).toFloat() );
		    			else if ( val instanceof RubyString )
		    				values[i] = new String( ((RubyString)val).toStr() );
		    			else if  ( val == RubyConstant.QTRUE )
		    				values[i] = new String( "true" );
		    			else if  ( val == RubyConstant.QFALSE )
		    				values[i] = new String( "false" );
		    			else
		    				values[i] = val.toStr();
		    		}
	    		}
	    		
	    		IDBResult rows = executeSQL( strSql, values);
	    		RubyString[] colNames = getOrigColNames(rows);
	    		
	    		for( ; !rows.isEnd(); rows.next() )
	    		{
	    			RubyHash row = ObjectFactory.createHash();
	    			for ( int nCol = 0; nCol < rows.getColCount(); nCol ++ )
	    				row.add( colNames[nCol], rows.getRubyValueByIdx(nCol) );
	    			
	    			res.add( row );
	    		}
    		}
		}catch(Exception e)
		{
    		LOG.ERROR("execute failed.", e);
			throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
		}
    	
        return res;
    }
    
    //@RubyAllocMethod
    private static RubyValue alloc(RubyValue receiver) {
    	return new DBAdapter(RubyRuntime.DatabaseClass);
    }
    
    public static void closeAll()
    {
    	Enumeration enumDBs = m_mapDBPartitions.elements();
		while (enumDBs.hasMoreElements()) 
		{
			DBAdapter db = (DBAdapter)enumDBs.nextElement();
			db.close();
		}
    }

    public static void initAttrManager()throws DBException
    {
    	Enumeration enumDBs = m_mapDBPartitions.elements();
		while (enumDBs.hasMoreElements()) 
		{
			DBAdapter db = (DBAdapter)enumDBs.nextElement();
			db.getAttrMgr().loadBlobAttrs(db);
		}
    }
    
    public static DBAdapter getUserDB()
    {
        return (DBAdapter)getDBPartitions().get(USER_PARTITION_NAME());
    }

    public static DBAdapter getDB(String szPartition)
    {
        return (DBAdapter)getDBPartitions().get(szPartition);
    }

    public static boolean isAnyInsideTransaction()
    {
    	Enumeration enumDBs = m_mapDBPartitions.elements();
		while (enumDBs.hasMoreElements()) 
		{
			DBAdapter db = (DBAdapter)enumDBs.nextElement();
			
			if ( db.isInsideTransaction() )
				return true;
		}

        return false;
    }
    
	public static void initMethods(RubyClass klass) {
		
		klass.defineAllocMethod(new RubyNoArgMethod(){
			protected RubyValue run(RubyValue receiver, RubyBlock block )	{
				return DBAdapter.alloc(receiver);}
		});
		
		klass.defineMethod( "initialize", new RubyTwoArgMethod()
		{ 
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block )
			{
		    	try
		    	{
		    		String szDbName = arg1.toStr();
		    		String szDbPartition = arg2.toStr();
		    		((DBAdapter)receiver).openDB(szDbName);
		    		((DBAdapter)receiver).setDbPartition(szDbPartition);
		    		
		    		DBAdapter.getDBPartitions().put(szDbPartition, receiver);
		    		
			        return receiver;
				}catch(Exception e)
				{
					LOG.ERROR("initialize failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
		    	
			}
		});
		/*klass.defineMethod( "close", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return ((DBAdapter)receiver).rb_close();}
		});*/
		klass.defineMethod( "execute", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
				return ((DBAdapter)receiver).rb_execute(args.get(0), args.get(1), 
						(args.size() > 2 ? args.get(2):null));}
		});
		
		klass.defineMethod( "start_transaction", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
		    	try{
		    		((DBAdapter)receiver).startTransaction();
			        return ObjectFactory.createInteger(0);
		    	}catch( Exception e ){
		    		LOG.ERROR("start_transaction failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
		    	}
			}
		});
		klass.defineMethod( "commit", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
		    	try{
		    		((DBAdapter)receiver).commit();
			        return ObjectFactory.createInteger(0);
		    	}catch( Exception e ){
		    		LOG.ERROR("commit failed.", e);
		    		throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
		    	}
			}
		});
		klass.defineMethod( "rollback", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
		    	try{
		    		((DBAdapter)receiver).rollback();
			        return ObjectFactory.createInteger(0);
		    	}catch( Exception e ){
		    		LOG.ERROR("rollback failed.", e);
		    		throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
		    	}
			}
		});
		klass.defineMethod( "destroy_tables", new RubyTwoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyValue arg1, RubyBlock block ){
				return ((DBAdapter)receiver).rb_destroy_tables(arg, arg1);}
		});
		
		klass.defineMethod("is_ui_waitfordb", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) 
			{
				try{
				    boolean bRes = ((DBAdapter)receiver).isUIWaitDB();
				    return ObjectFactory.createBoolean(bRes);				    
				}catch(Exception e)
				{
					LOG.ERROR("is_ui_waitfordb failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
		
		klass.defineMethod("lock_db",
			new RubyNoArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyBlock block) {
					try{
					    DBAdapter db = (DBAdapter)receiver;
					    //db.setUnlockDB(true);
					    db.Lock();
					}catch(Exception e)
					{
						LOG.ERROR("lock_db failed", e);
						throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
					}
				    
				    return RubyConstant.QNIL;
				}
			});
		klass.defineMethod("unlock_db",
			new RubyNoArgMethod() {
				protected RubyValue run(RubyValue receiver, RubyBlock block) {
					try{
						DBAdapter db = (DBAdapter)receiver;
					    db.Unlock();
					}catch(Exception e)
					{
						LOG.ERROR("unlock_db failed", e);
						throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
					}
					
				    return RubyConstant.QNIL;
				}
			});
		
		klass.defineMethod( "table_exist?", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
				try{
					DBAdapter db = (DBAdapter)receiver;
				    boolean bExists = db.isTableExist(arg.toStr());
				    
				    return ObjectFactory.createBoolean(bExists);
				}catch(Exception e)
				{
					LOG.ERROR("unlock_db failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
	}
	
	public static class DBCallback implements IDBCallback
	{
		private DBAdapter m_db;
		DBCallback(DBAdapter db){ m_db = db; }
		/*public void OnDeleteAll() {
			OnDeleteAllFromTable("object_values");
		}
		
		public void OnDeleteAllFromTable(String tableName) {
			if ( !tableName.equals("object_values") )
				return;

			try{
				String fName = DBAdapter.makeBlobFolderName();
				RhoClassFactory.createFile().delete(fName);
				DBAdapter.makeBlobFolderName(); //Create folder back
			}catch(Exception exc){
				LOG.ERROR("DBCallback.OnDeleteAllFromTable: Error delete files from table: " + tableName, exc);				
			}
		}*/

		public void onAfterInsert(String tableName, IDBResult rows2Insert)
		{
			if ( !tableName.equalsIgnoreCase("object_values") )
				return;

			for( ; !rows2Insert.isEnd(); rows2Insert.next() )
			{
				Object[] data = rows2Insert.getCurData();
				Integer nSrcID = new Integer(rows2Insert.getIntByIdx(0));
				String attrib = (String)data[1];
				m_db.getAttrMgr().add(nSrcID, attrib);
			}
			
		}
		
		public void onBeforeUpdate(String tableName, IDBResult rows2Delete, int[] cols)
		{
			processDelete(tableName, rows2Delete, cols);
		}
		
		public void onBeforeDelete(String tableName, IDBResult rows2Delete) 
		{
			processDelete(tableName, rows2Delete, null);
		}
		
		private boolean isChangedCol(int[] cols, int iCol)
		{
			if (cols==null)
				return true;
			
			for ( int i = 0; i < cols.length; i++ )
			{
				if ( cols[i] == iCol )
					return true;
			}
			return false;
		}
		
		private void processDelete(String tableName, IDBResult rows2Delete, int[] cols)
		{
			if ( tableName.equalsIgnoreCase("changed_values") || tableName.equalsIgnoreCase("sources") ||
			     tableName.equalsIgnoreCase("client_info"))
				return;
			
			boolean bProcessTable = tableName.equalsIgnoreCase("object_values");
			boolean bSchemaSrc = false;
			Integer nSrcID = null;
			if ( !bProcessTable )
			{
				nSrcID = m_db.getAttrMgr().getSrcIDHasBlobsByName(tableName);
				bProcessTable = nSrcID != null; 
				bSchemaSrc = bProcessTable;
			}
			
			if ( !bProcessTable)
				return;
		
			if ( !bSchemaSrc && !isChangedCol(cols, 3))//value
				return;
			
			for( ; !rows2Delete.isEnd(); rows2Delete.next() )
			{
				Object[] data = rows2Delete.getCurData();
				
				if ( !bSchemaSrc )
				{
					if ( nSrcID == null )
						nSrcID = new Integer(rows2Delete.getIntByIdx(0));
					
					String attrib = (String)data[1];
					String value = (String)data[3];

					if (cols == null) //delete
						m_db.getAttrMgr().remove(nSrcID, attrib);
					
				    if ( m_db.getAttrMgr().isBlobAttr(nSrcID, attrib) )
				    	processBlobDelete(nSrcID, attrib, value);
				}else
				{
					for ( int i = 0; i < rows2Delete.getColCount(); i++ )
					{
						if (!isChangedCol(cols, i))
							continue;
						
						String attrib = rows2Delete.getColName(i);
						if ( !(data[i] instanceof String) )
							continue;
						
						String value = (String)data[i];
					    if ( m_db.getAttrMgr().isBlobAttr(nSrcID, attrib) )
					    	processBlobDelete(nSrcID, attrib, value);
					}
				}
			}
		}
		
		private void processBlobDelete(Integer nSrcID, String attrib, String value)
		{
			if ( value == null || value.length() == 0 )
				return;
			
			try{
		        String strFilePath = RhodesApp.getInstance().resolveDBFilesPath(value);
		        
			    SimpleFile oFile = RhoClassFactory.createFile();
			    oFile.delete(strFilePath);
			}catch(Exception exc){
				LOG.ERROR("DBCallback.OnDeleteFromTable: Error delete file: " + value, exc);				
			}
		}
		
	}
}