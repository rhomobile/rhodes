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
	private String  m_strDBPath, m_strDbVerPath;
	private String  m_strDbPartition;
	private DBAttrManager m_attrMgr = new DBAttrManager();
	static Hashtable/*Ptr<String,CDBAdapter*>*/ m_mapDBPartitions = new Hashtable();
	
    Mutex m_mxDB = new Mutex();
    int m_nTransactionCounter=0;
    boolean m_bUIWaitDB = false;
	
    static String USER_PARTITION_NAME(){return "user";}
    static Hashtable/*Ptr<String,CDBAdapter*>&*/ getDBPartitions(){ return  m_mapDBPartitions; }
    String m_strClientInfoInsert = "";
    Object[] m_dataClientInfo = null;
    
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

		Lock();
		try{		
			m_dbStorage.executeBatchSQL(strStatement);
		}finally
		{
			Unlock();
		}			
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
	public IDBResult executeSQL(String strStatement, int arg1, int arg2)throws DBException{
		Object[] values = { new Integer(arg1), new Integer(arg2)};
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
		//LOG.TRACE("executeSQLReportNonUnique: " + strStatement);
		
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

	public IDBResult executeSQLReportNonUnique(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5, Object arg6, Object arg7)throws DBException{
		//LOG.TRACE("executeSQLReportNonUnique: " + strStatement);
		
		Object[] values = {arg1,arg2,arg3,arg4, arg5, arg6, arg7};
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
		//LOG.TRACE("executeSQLReportNonUnique: " + strStatement);
		
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
    	m_strDbVerPath = strPath + getNameNoExt(strDBName.substring(nSlash+1)) +  ".version";
    }
    
    private String getSqlScript()
    {
    	return RhoFile.readStringFromJarFile("apps/db/syncdb.schema", this);
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
    		m_dbStorage.onBeforeCommit();
	    	//getAttrMgr().save(this);
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
    	boolean m_bEncrypted = false;
    	boolean m_bSqlite = false;
    	
    	boolean isRhoVerChanged(DBVersion dbNewVer)
    	{
    		return m_strRhoVer.compareTo(dbNewVer.m_strRhoVer) != 0;
    	}
    	boolean isAppVerChanged(DBVersion dbNewVer)
    	{
    		return m_strAppVer.compareTo(dbNewVer.m_strAppVer) != 0;
    	}
    	
    	boolean isDbFormatChanged(DBVersion dbNewVer)
    	{
    		return m_bEncrypted != dbNewVer.m_bEncrypted || 
    			m_bSqlite != dbNewVer.m_bSqlite;
    	}
    	
	    void fromFile(String strFilePath)throws Exception
		{
	        String strData = RhoFile.readStringFromFile(strFilePath);
	        
	        Tokenizer oTokenizer = new Tokenizer( strData, ";" );
	        int nPos = 0;
			while (oTokenizer.hasMoreTokens()) 
	        {
				String tok = oTokenizer.nextToken().trim();
				
				switch(nPos)
				{
				case 0:
					m_strRhoVer = tok;
					break;
				case 1:
					m_strAppVer = tok;
					break;
				case 2:
					m_bEncrypted = tok.compareTo("encrypted") == 0;
					break;
				case 3:
					m_bSqlite = tok.compareTo("sqlite") == 0;
					break;				
				}
				nPos++;
	        }
		}
		
		void toFile(String strFilePath)throws Exception
		{
			String strFullVer = m_strRhoVer + ";" + m_strAppVer + 
        		";" + (m_bEncrypted ? "encrypted":"") + 
        		";" + (m_bSqlite ? "sqlite" : "");
			
			try{
				RhoClassFactory.createFile().delete(strFilePath);
				RhoFile.writeStringToFile(strFilePath, strFullVer);
			}catch (Exception e) {
		    	LOG.ERROR("writeDBVersion failed.", e);
		    	throw e;
		    }
		}
    };
    
	boolean migrateDB(DBVersion dbVer, DBVersion dbNewVer )
	{
	    LOG.INFO( "Try migrate database from " + (dbVer != null ? dbVer.m_strRhoVer:"") + " to " + (dbNewVer.m_strRhoVer !=null ? dbNewVer.m_strRhoVer:"") );
	    if ( dbVer != null && dbNewVer.m_strRhoVer != null &&
	    	 (dbVer.m_strRhoVer.startsWith("1.4")||dbVer.m_strRhoVer.startsWith("1.4")) && (dbNewVer.m_strRhoVer.startsWith("1.5")||dbNewVer.m_strRhoVer.startsWith("1.4")) )
		{
            LOG.INFO( "No migration required from " + (dbVer != null ? dbVer.m_strRhoVer:"") + " to " + (dbNewVer.m_strRhoVer !=null ? dbNewVer.m_strRhoVer:"") );
            
            try{
            	dbNewVer.toFile(m_strDbVerPath);
			}catch(Exception e)
			{
	    		LOG.ERROR("migrateDB failed.", e);
			}
			
            
		    return true;
		}
		
	    //1.2.x -> 1.5.x,1.4.x
	    if ( dbVer != null && dbNewVer.m_strRhoVer != null &&
	    	 (dbVer.m_strRhoVer.startsWith("1.2")||dbVer.m_strRhoVer.startsWith("1.4")) && (dbNewVer.m_strRhoVer.startsWith("1.5")||dbNewVer.m_strRhoVer.startsWith("1.4")) )
	    {
	    //sources
	    //priority INTEGER, ADD
	    //backend_refresh_time int default 0, ADD
	    	LOG.INFO("Migrate database from " + dbVer.m_strRhoVer + " to " + dbNewVer.m_strRhoVer);
	    	
			IDBStorage db = null;
			try{
			    db = RhoClassFactory.createDBStorage();	    
				db.open( m_strDBPath, getSqlScript(), getEncryptionInfo() );
	
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
		        
		        dbNewVer.toFile(m_strDbVerPath);
	
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
	
	String getEncryptionInfo()
	{
		boolean bEncrypted =  AppBuildConfig.getItem("encrypt_database") != null &&
			AppBuildConfig.getItem("encrypt_database").compareTo("1") == 0;
		
		String strRes = "";
		
		if (bEncrypted)
		{
			String strAppName = "rhodes";
			try{
				strAppName = RhoClassFactory.createRhoRubyHelper().getModuleName();
			}catch(Exception e){}
			
			strRes = m_strDbPartition + "_" + strAppName; 
		}
		return strRes;
	}
	
	void checkDBVersion()throws Exception
	{
		DBVersion dbNewVer = new DBVersion();
		dbNewVer.m_strRhoVer = RhoSupport.getRhoDBVersion(); 
		dbNewVer.m_strAppVer = RhoConf.getInstance().getString("app_db_version");
		String strEncryptionInfo = getEncryptionInfo();
		dbNewVer.m_bEncrypted = strEncryptionInfo != null && strEncryptionInfo.length() > 0;
		dbNewVer.m_bSqlite = Capabilities.USE_SQLITE;
			
		DBVersion dbVer = new DBVersion();  
		dbVer.fromFile(m_strDbVerPath);

		if (dbVer.m_strRhoVer.length() == 0 )
		{
			dbNewVer.toFile(m_strDbVerPath);
			return;
		}
		
		boolean bRhoReset = dbVer.isRhoVerChanged(dbNewVer);
	    boolean bAppReset = dbVer.isAppVerChanged(dbNewVer);
		
		boolean bDbFormatChanged = dbVer.isDbFormatChanged(dbNewVer);
		if ( !bDbFormatChanged && dbVer.m_bEncrypted )
		{
			if (!com.rho.RhoCrypto.isKeyExist(strEncryptionInfo) )
				bDbFormatChanged = true;
		}
		
		if ( bDbFormatChanged )
			LOG.INFO("Reset Database( format changed ):" + m_strDBPath);
		
	    if ( bRhoReset && !bAppReset && !bDbFormatChanged )
	        bRhoReset = !migrateDB(dbVer, dbNewVer);
	    
		if ( bRhoReset || bAppReset || bDbFormatChanged)
		{
			if ( !bDbFormatChanged )
			{
				IDBStorage db = null;
				try
				{
				    db = RhoClassFactory.createDBStorage();
					if ( db.isDbFileExists(m_strDBPath) )
					{
						db.open( m_strDBPath, "", strEncryptionInfo );
				    	IDBResult res = db.executeSQL("SELECT * FROM client_info", null, false);
				    	if ( !res.isOneEnd() )
				    	{
				    		m_strClientInfoInsert = createInsertStatement(res, "client_info");
				    		m_dataClientInfo = res.getCurData();
				    	}
					}
				}catch(Exception exc)
				{
					LOG.ERROR("Copy client_info table failed.", exc);
				}catch(Throwable e)
				{
					LOG.ERROR("Copy client_info table crashed.", e);
				}finally
				{
					if (db != null )
						try{ db.close(); }catch(Exception e){}
				}
			}
			
			m_dbStorage.deleteAllFiles(m_strDBPath);
			
			if ( this.m_strDbPartition.compareTo("user") == 0 ) //do it only once
			{
				String fName = makeBlobFolderName();
				RhoClassFactory.createFile().delete(fName);
				DBAdapter.makeBlobFolderName(); //Create folder back
			}
			
			dbNewVer.toFile(m_strDbVerPath);
            
            if ( RhoConf.getInstance().isExist("bulksync_state") && RhoConf.getInstance().getInt("bulksync_state") != 0)
            	RhoConf.getInstance().setInt("bulksync_state", 0, true);            
		}
		
	}
/*	
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
*/	
    private void openDB(String strDBName, boolean bTemp)throws Exception
    {
    	if ( m_bIsOpen )
    		return;
    	
		initFilePaths(strDBName);
	    if ( !bTemp )
	    	checkDBVersion();
	    
		m_dbStorage.open(m_strDBPath, getSqlScript(), getEncryptionInfo() );
		
		//executeSQL("CREATE INDEX by_src ON object_values (source_id)", null);
		m_bIsOpen = true;
		
		//getAttrMgr().load(this);
		
		m_dbStorage.setDbCallback(new DBCallback(this));
		
		//m_dbAdapters.addElement(this);

	    //copy client_info table
        if ( !bTemp && m_strClientInfoInsert != null && m_strClientInfoInsert.length() > 0 &&
        	 m_dataClientInfo != null )
        {
            LOG.INFO("Copy client_info table from old database");
    		
        	m_dbStorage.executeSQL(m_strClientInfoInsert, m_dataClientInfo, false );
        	
            IDBResult res = executeSQL( "SELECT client_id FROM client_info" );
            if ( !res.isOneEnd() &&  res.getStringByIdx(0).length() > 0 )
            {
                LOG.INFO("Set reset=1 in client_info");
                executeSQL( "UPDATE client_info SET reset=1" );
            }
        	
        }
		
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
		return m_dbStorage.isTableExists(strTableName);
	}
	
    private RubyValue rb_destroy_tables(RubyValue vInclude, RubyValue vExclude) 
    {
		if ( !m_bIsOpen )
			return RubyConstant.QNIL;
		
		IDBStorage db = null;
		try{
		    //getAttrMgr().reset(this);
			
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
		    String dbNameJournal2 = dbName + ".data-journal";
		    String dbNewNameJournal = dbNewName + ".journal";
		    String dbNewNameJournal2 = dbNewName + ".data-journal";
		    String dbNewNameProps = dbNewName + ".properties";
		    
			//LOG.TRACE("DBAdapter: " + dbNewNameDate + ": " + (fs.exists(dbNewNameData) ? "" : "not ") + "exists");
		    fs.delete(dbNewNameData);
		    //LOG.TRACE("DBAdapter: " + dbNewNameScript + ": " + (fs.exists(dbNewNameScript) ? "" : "not ") + "exists");
		    fs.delete(dbNewNameScript);
		    //LOG.TRACE("DBAdapter: " + dbNewNameJournal + ": " + (fs.exists(dbNewNameJournal) ? "" : "not ") + "exists");
		    fs.delete(dbNewNameJournal);
		    fs.delete(dbNewNameJournal2);
		    fs.delete(dbNewNameProps);
		    
		    LOG.TRACE("1. Size of " + dbNameData + ": " + fs.size(dbNameData));
		    
		    db = RhoClassFactory.createDBStorage();	    
			db.open( dbNewName, getSqlScript(), getEncryptionInfo() );
			
			String[] vecTables = m_dbStorage.getAllTableNames();
			//IDBResult res;
	
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
		    fs.delete(dbNameJournal2);
		    
			String fName = makeBlobFolderName();
			RhoClassFactory.createFile().delete(fName);
			DBAdapter.makeBlobFolderName(); //Create folder back
		    
		    fs.renameOverwrite(dbNewNameData, dbNameData);
		    if ( !Capabilities.USE_SQLITE )
		    	fs.renameOverwrite(dbNewNameScript, dbNameScript);
		    
		    LOG.TRACE("3. Size of " + dbNameData + ": " + fs.size(dbNameData));
		    
		    m_dbStorage = RhoClassFactory.createDBStorage();
			m_dbStorage.open(m_strDBPath, getSqlScript(), getEncryptionInfo() );
			m_bIsOpen = true;
			
			//getAttrMgr().load(this);
			
			m_dbStorage.setDbCallback(new DBCallback(this));
			
		}catch(Exception e)
		{
    		LOG.ERROR("execute failed.", e);
    		
			if ( !m_bIsOpen )
			{
				LOG.ERROR("destroy_table error.Try to open old DB.");
				try{
					m_dbStorage.open(m_strDBPath, getSqlScript(), getEncryptionInfo() );
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
    
    public void updateAllAttribChanges()throws DBException
    {
	    //Check for attrib = object
	    IDBResult res = executeSQL("SELECT object, source_id, update_type " +
	        "FROM changed_values where attrib = 'object' and sent=0" );

	    if ( res.isEnd() )
	        return;

	    startTransaction();

	    Vector/*<String>*/ arObj = new Vector(), arUpdateType = new Vector();
	    Vector/*<int>*/ arSrcID = new Vector();
	    for( ; !res.isEnd(); res.next() )
	    {
	        arObj.addElement(res.getStringByIdx(0));
	        arSrcID.addElement(new Integer(res.getIntByIdx(1)));
	        arUpdateType.addElement(res.getStringByIdx(2));
	    }
        
        for( int i = 0; i < (int)arObj.size(); i++ )
        {
            IDBResult resSrc = executeSQL("SELECT name, schema FROM sources where source_id=?", arSrcID.elementAt(i) );
            boolean bSchemaSource = false;
            String strTableName = "object_values";
            if ( !resSrc.isOneEnd() )
            {
                bSchemaSource = resSrc.getStringByIdx(1).length() > 0;
                if ( bSchemaSource )
                    strTableName = resSrc.getStringByIdx(0);
            }

            if (bSchemaSource)
            {
                IDBResult res2 = executeSQL( "SELECT * FROM " + strTableName + " where object=?", arObj.elementAt(i) );
                for( int j = 0; j < res2.getColCount(); j ++)
                {
                    String strAttrib = res2.getColName(j);
                    String value = res2.getStringByIdx(j);
                    String attribType = getAttrMgr().isBlobAttr((Integer)arSrcID.elementAt(i), strAttrib) ? "blob.file" : "";

    	            executeSQLReportNonUnique("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type,sent) VALUES(?,?,?,?,?,?,?)", 
        	                arSrcID.elementAt(i), arObj.elementAt(i), strAttrib, value, arUpdateType.elementAt(i), attribType, new Integer(0) );
                }
            }else
            {
                IDBResult res2 = executeSQL( "SELECT attrib, value FROM " + strTableName + " where object=? and source_id=?", 
                    arObj.elementAt(i), arSrcID.elementAt(i) );

    	        for( ; !res2.isEnd(); res2.next() )
    	        {
    	            String strAttrib = res2.getStringByIdx(0);
    	            String value = res2.getStringByIdx(1);
    	            String attribType = getAttrMgr().isBlobAttr((Integer)arSrcID.elementAt(i), strAttrib) ? "blob.file" : "";

    	            executeSQLReportNonUnique("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type,sent) VALUES(?,?,?,?,?,?,?)", 
    	                arSrcID.elementAt(i), arObj.elementAt(i), strAttrib, value, arUpdateType.elementAt(i), attribType, new Integer(0) );
    	        }
            }
        }

        executeSQL("DELETE FROM changed_values WHERE attrib='object'"); 

        endTransaction();
    }
    
    void copyChangedValues(DBAdapter db)throws DBException
    {
    	updateAllAttribChanges();    	
        copyTable("changed_values", m_dbStorage, db.m_dbStorage );
        {
            Vector/*<int>*/ arOldSrcs = new Vector();
            {
                IDBResult resSrc = db.executeSQL( "SELECT DISTINCT(source_id) FROM changed_values" );
                for ( ; !resSrc.isEnd(); resSrc.next() )
                    arOldSrcs.addElement( new Integer(resSrc.getIntByIdx(0)) );
            }
            for( int i = 0; i < arOldSrcs.size(); i++)
            {
                int nOldSrcID = ((Integer)arOldSrcs.elementAt(i)).intValue();

                IDBResult res = executeSQL("SELECT name from sources WHERE source_id=?", nOldSrcID);
                if ( !res.isOneEnd() )
                {
                    String strSrcName = res.getStringByIdx(0);
                    IDBResult res2 = db.executeSQL("SELECT source_id from sources WHERE name=?", strSrcName );
                    if ( !res2.isOneEnd() )
                    {
                        if ( nOldSrcID != res2.getIntByIdx(0) )
                        {
                            db.executeSQL("UPDATE changed_values SET source_id=? WHERE source_id=?", res2.getIntByIdx(0), nOldSrcID);
                        }
                        continue;
                    }
                }

                //source not exist in new partition, remove this changes
                db.executeSQL("DELETE FROM changed_values WHERE source_id=?", nOldSrcID);
            }
        }
    }
    
    public void setBulkSyncDB(String fDbName, String fScriptName)
    {
		DBAdapter db = null;
		try{
			db = (DBAdapter)alloc(null);
			db.setDbPartition(m_strDbPartition);			
    		db.openDB(fDbName, true);
    		db.m_dbStorage.createTriggers();
			
		    db.startTransaction();
			
		    copyTable("client_info", m_dbStorage, db.m_dbStorage );
		    copyChangedValues(db);
/*		    
		    //update User partition
		    if ( m_strDbPartition.compareTo(USER_PARTITION_NAME()) == 0 )
		    {
		        //copy all NOT user sources from current db to bulk db
		    	startTransaction();
		        executeSQL("DELETE FROM sources WHERE partition=?", m_strDbPartition);
		        copyTable("sources", m_dbStorage, db.m_dbStorage );
		        rollback();
		    }else
		    {
		        //remove all m_strDbPartition sources from user db
		        //copy all sources from bulk db to user db
		        DBAdapter dbUser  = getDB(USER_PARTITION_NAME());
		        dbUser.startTransaction();
		        dbUser.executeSQL("DELETE FROM sources WHERE partition=?", m_strDbPartition);
		        copyTable("sources", db.m_dbStorage, dbUser.m_dbStorage );
		        dbUser.endTransaction();
		    }*/
		    
		    getDBPartitions().put(m_strDbPartition, db);
		    com.rho.sync.SyncThread.getSyncEngine().applyChangedValues(db);
		    getDBPartitions().put(m_strDbPartition, this);
		    
		    db.endTransaction();
		    db.close();

		    m_dbStorage.close();
		    m_dbStorage = null;
		    m_bIsOpen = false;

			String dbName = getNameNoExt(m_strDBPath);
			IFileAccess fs = RhoClassFactory.createFileAccess();
			
			String dbNameData = dbName + ".data";
		    String dbNameScript = dbName + ".script";
		    String dbNameJournal = dbName + ".journal";
		    String dbNameJournal2 = dbName + ".data-journal";		    
		    String dbNewNameProps = getNameNoExt(fDbName) + ".properties";
		    
		    fs.delete(dbNameJournal);
		    fs.delete(dbNameJournal2);
		    fs.delete(dbNewNameProps);
		    
			String fName = makeBlobFolderName();
			RhoClassFactory.createFile().delete(fName);
			DBAdapter.makeBlobFolderName(); //Create folder back
		    
		    fs.renameOverwrite(fDbName, dbNameData);
		    if ( !Capabilities.USE_SQLITE )
		    	fs.renameOverwrite(fScriptName, dbNameScript);
		    
		    m_dbStorage = RhoClassFactory.createDBStorage();
			m_dbStorage.open(m_strDBPath, getSqlScript(), getEncryptionInfo() );
			m_bIsOpen = true;
			
			//getAttrMgr().load(this);
			
			m_dbStorage.setDbCallback(new DBCallback(this));
			
		}catch(Exception e)
		{
    		LOG.ERROR("execute failed.", e);
    		
			if ( !m_bIsOpen )
			{
				LOG.ERROR("destroy_table error.Try to open old DB.");
				try{
					m_dbStorage.open(m_strDBPath, getSqlScript(), getEncryptionInfo() );
					m_bIsOpen = true;
				}catch(Exception exc)
				{
					LOG.ERROR("destroy_table open old table failed.", exc);
				}
			}
			
			if ( db != null)
				db.close();
    		
			throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
		}
    	
    }

    public RubyValue rb_execute(RubyValue v, RubyValue batch, RubyValue arg) 
    {
    	RubyArray res = new RubyArray(); 
    	try{
    		String strSql = v.toStr();
    		if ( batch == RubyConstant.QTRUE )
    		{
    			//LOG.INFO("batch execute:" + strSql);
    			
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
	    		RubyString[] colNames = null;
	    		
	    		for( ; !rows.isEnd(); rows.next() )
	    		{
	    			RubyHash row = ObjectFactory.createHash();
	    			for ( int nCol = 0; nCol < rows.getColCount(); nCol ++ )
	    			{
	    				if ( colNames == null )
	    					colNames = getOrigColNames(rows);
	    				
	    				row.add( colNames[nCol], rows.getRubyValueByIdx(nCol) );
	    			}
	    			
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
    	return new DBAdapter((RubyClass) receiver);
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
			if ( !db.getAttrMgr().hasBlobAttrs() )
				db.m_dbStorage.setDbCallback(null);
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

    public static Vector/*<String>*/ getDBAllPartitionNames()
    {
        Vector/*<String>*/ vecNames = new Vector();
        Enumeration enumDBs = m_mapDBPartitions.keys();
		while (enumDBs.hasMoreElements()) 
		{
			vecNames.addElement(enumDBs.nextElement());
		}
		
        return vecNames;
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
		    		((DBAdapter)receiver).setDbPartition(szDbPartition);		    		
		    		((DBAdapter)receiver).openDB(szDbName, false);
		    		
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
/*
		public void onAfterInsert(String tableName, IDBResult rows2Insert)
		{
			try
			{
				if ( !tableName.equalsIgnoreCase("object_values") )
					return;
	
				for( ; !rows2Insert.isEnd(); rows2Insert.next() )
				{
					//Object[] data = rows2Insert.getCurData();
					Integer nSrcID = new Integer(rows2Insert.getIntByIdx(0));
					String attrib = rows2Insert.getStringByIdx(1);
					m_db.getAttrMgr().add(nSrcID, attrib);
				}
			}catch(DBException exc)
			{
				LOG.ERROR("onAfterInsert failed.", exc);
			}
		}*/
		
		public void onBeforeUpdate(String tableName, IDBResult rows2Delete, int[] cols)
		{
			try
			{
				processDelete(tableName, rows2Delete, cols);
			}catch(DBException exc)
			{
				LOG.ERROR("onAfterInsert failed.", exc);
			}
		}
		
		public void onBeforeDelete(String tableName, IDBResult rows2Delete) 
		{
			try
			{
				processDelete(tableName, rows2Delete, null);
			}catch(DBException exc)
			{
				LOG.ERROR("onAfterInsert failed.", exc);
			}
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
		
		private void processDelete(String tableName, IDBResult rows2Delete, int[] cols) throws DBException
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
				if ( !bSchemaSrc )
				{
					nSrcID = new Integer(rows2Delete.getIntByIdx(0));
					
					String attrib = rows2Delete.getStringByIdx(1);
					String value = rows2Delete.getStringByIdx(3);

					//if (cols == null) //delete
					//	m_db.getAttrMgr().remove(nSrcID, attrib);
					
				    if ( m_db.getAttrMgr().isBlobAttr(nSrcID, attrib) )
				    	processBlobDelete(nSrcID, attrib, value);
				}else
				{
					Object[] data = rows2Delete.getCurData();
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