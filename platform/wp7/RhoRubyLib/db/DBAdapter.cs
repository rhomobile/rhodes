using System;
using rho.common;
using rho;
using Community.CsharpSqlite;

namespace rho.db
{
    public class DBAdapter
    {
    private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("DbAdapter");
    private static CRhodesApp RHODESAPP(){return CRhodesApp.Instance;}
    public CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }

    private IDBStorage m_dbStorage;
	private boolean m_bIsOpen = false;
	private String  m_strDBPath, m_strDbVerPath;
	private String  m_strDbPartition;
	private DBAttrManager m_attrMgr = new DBAttrManager();
	static Hashtable<String,DBAdapter> m_mapDBPartitions = new Hashtable<String,DBAdapter>();
	
    Mutex m_mxDB = new Mutex();
    int m_nTransactionCounter=0;
    boolean m_bUIWaitDB = false;
	
    static String USER_PARTITION_NAME(){return "user";}
    static Hashtable<String,DBAdapter> getDBPartitions(){ return  m_mapDBPartitions; }
    String m_strClientInfoInsert = "";
    Object[] m_dataClientInfo = null;
    
	public DBAdapter() 
    {
		try{
			m_dbStorage = RhoClassFactory.createDBStorage();
		}catch(Exception exc){
    		LOG.ERROR("createDBStorage failed.", exc);
		}
	}

    private static DBAdapter getDBByHandle(Sqlite3.sqlite3 db)
    {
        Hashtable<String, DBAdapter>.Enumerator hashEnum = m_mapDBPartitions.GetEnumerator();
		while( hashEnum.MoveNext() )
		{
            if(hashEnum.Current.Value.m_dbStorage == db)
			    return hashEnum.Current.Value;
		}

        return getDBPartitions().get(USER_PARTITION_NAME());
    }

    public static void SyncBlob_DeleteCallback(Sqlite3.sqlite3_context dbContext, int nArgs, Sqlite3.Mem[] Args)
    {
        if (nArgs < 3)
            return;

        DBAttrManager attrMgr = getDBByHandle(Sqlite3.sqlite3_context_db_handle(dbContext)).getAttrMgr();

        String szAttrName = Sqlite3.sqlite3_value_text(Args[2]);
        int nSrcID = Sqlite3.sqlite3_value_int(Args[1]);
        if (attrMgr.isBlobAttr(nSrcID, szAttrName))
        {
            String strFilePath = RHODESAPP().resolveDBFilesPath(Sqlite3.sqlite3_value_text(Args[0]));
            CRhoFile.deleteFile(strFilePath);
        }
    }

    public static void SyncBlob_UpdateCallback(Sqlite3.sqlite3_context dbContext, int nArgs, Sqlite3.Mem[] Args)
    {
        if (nArgs < 3)
            return;

        DBAttrManager attrMgr = getDBByHandle(Sqlite3.sqlite3_context_db_handle(dbContext)).getAttrMgr();

        String szAttrName = Sqlite3.sqlite3_value_text(Args[2]);
        int nSrcID = Sqlite3.sqlite3_value_int(Args[1]);
        if (attrMgr.isBlobAttr(nSrcID, szAttrName))
        {
            String strFilePath = RHODESAPP().resolveDBFilesPath(Sqlite3.sqlite3_value_text(Args[0]));
            CRhoFile.deleteFile(strFilePath);
        }
    }

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

	public void executeBatchSQL(String strStatement)
    {
		LOG.TRACE("executeBatchSQL: " + strStatement);

        Lock();
		try{		
		    m_dbStorage.executeBatchSQL(strStatement);
        }
        finally
        {
            Unlock();
        }
	}

    public IDBResult executeSQL(String strStatement, Object[] values)
    {
        return executeSQL(strStatement, values, false);
    }
	
	public IDBResult executeSQL(String strStatement, Object[] values, boolean bNoCopy)
    {
		LOG.TRACE("executeSQL: " + strStatement + ";" + values);
		IDBResult res = null;
		Lock();
		try{
            res = m_dbStorage.executeSQL(strStatement, values, false, bNoCopy);
		}finally
		{
			Unlock();
		}
		return res;
	}

	public IDBResult executeSQL(String strStatement){
		return executeSQL(strStatement,null);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1){
		Object[] values = {arg1};
		return executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement, int arg1){
		Object[] values = { arg1 };
		return executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement, int arg1, int arg2){
		Object[] values = { arg1, arg2};
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQL(String strStatement, long arg1){
		Object[] values = { arg1};
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2){
		Object[] values = {arg1,arg2};
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3){
		Object[] values = {arg1,arg2,arg3};
		return executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4){
		Object[] values = {arg1,arg2,arg3,arg4};
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5){
		Object[] values = {arg1,arg2,arg3,arg4,arg5};
		return executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5, Object arg6){
		Object[] values = {arg1,arg2,arg3,arg4,arg5,arg6};
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQLReportNonUnique(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4){
		//LOG.TRACE("executeSQLReportNonUnique: " + strStatement);
		
		Object[] values = {arg1,arg2,arg3,arg4};
		IDBResult res = null;
		Lock();
		try{
			res = m_dbStorage.executeSQL(strStatement,values, true, false);
		}finally
		{
			Unlock();
		}
		
		return res; 
	}

	public IDBResult executeSQLReportNonUnique(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5, Object arg6, Object arg7){
		//LOG.TRACE("executeSQLReportNonUnique: " + strStatement);
		
		Object[] values = {arg1,arg2,arg3,arg4, arg5, arg6, arg7};
		IDBResult res = null;
		Lock();
		try{
			res = m_dbStorage.executeSQL(strStatement,values, true, false);
		}finally
		{
			Unlock();
		}
		
		return res; 
	}
	
	public IDBResult executeSQLReportNonUniqueEx(String strStatement, Vector<Object> vecValues){
		//LOG.TRACE("executeSQLReportNonUnique: " + strStatement);
		
		Object[] values = new Object[vecValues.size()];
		for (int i = 0; i < vecValues.size(); i++ )
			values[i] = vecValues.elementAt(i);
		
		IDBResult res = null;
		Lock();
		try{
			res = m_dbStorage.executeSQL(strStatement,values, true, false);
		}finally
		{
			Unlock();
		}
		
		return res; 
	}
	
	public IDBResult executeSQLEx(String strStatement, Vector<Object> vecValues){
		Object[] values = new Object[vecValues.size()];
		for (int i = 0; i < vecValues.size(); i++ )
			values[i] = vecValues.elementAt(i);
		
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5, Object arg6, Object arg7){
		Object[] values = {arg1,arg2,arg3,arg4,arg5,arg6,arg7};
		return executeSQL(strStatement,values);
	}

	public boolean isUIWaitDB(){ return m_bUIWaitDB; }
	public void Lock()
	{
	    if ( RHODESAPP().isMainRubyThread() )
	        m_bUIWaitDB = true;
		
		m_mxDB.Lock();

        if (RHODESAPP().isMainRubyThread())
	        m_bUIWaitDB = false;
	}
	
	public void Unlock(){ m_mxDB.Unlock(); }
    public boolean isInsideTransaction(){ return m_nTransactionCounter>0; }
	
	//public static IDBResult createResult(){
	//	return getInstance().m_dbStorage.createResult();
	//}
	
	public String makeBlobFolderName()
    {
		String fName = CFilePath.join(CRhodesApp.getRhoRootPath(), "db/db-files");
        CRhoFile.recursiveCreateDir(fName);
	
		return fName;
	}
	/*
	RubyString[] getOrigColNames(IDBResult rows)
	{
		RubyString[] colNames = new RubyString[rows.getColCount()];
		for ( int nCol = 0; nCol < rows.getColCount(); nCol ++ )
			colNames[nCol] = ObjectFactory.createString(rows.getOrigColName(nCol));
		
		return colNames;
	} */
    
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
	
    private void initFilePaths(String strDBName)
    {
        CRhoFile.recursiveCreateDir(strDBName);
    	m_strDBPath = strDBName;
    	m_strDbVerPath = m_strDBPath+".version";
    }
    
    private String getSqlScript()
    {
        return CRhoFile.readStringFromResourceFile("db/syncdb.schema");
    }
    
    public void startTransaction()
    {
    	Lock();
    	m_nTransactionCounter++;
    	if (m_nTransactionCounter == 1)
    		m_dbStorage.startTransaction();    	
    }
    
    public void commit()
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

    public void rollback()
    {
    	m_nTransactionCounter--;
    	if (m_nTransactionCounter == 0)     	
    		m_dbStorage.rollback();
    	
    	Unlock();
    }
    
    public void endTransaction()
    {
    	commit();
    }

    class DBVersion
    {
    	public String m_strRhoVer = "";
    	public String m_strAppVer = "";
    	public boolean m_bEncrypted = false;
    	public boolean m_bSqlite = false;
    	
    	public boolean isRhoVerChanged(DBVersion dbNewVer)
    	{
    		return m_strRhoVer.compareTo(dbNewVer.m_strRhoVer) != 0;
    	}
    	public boolean isAppVerChanged(DBVersion dbNewVer)
    	{
    		return m_strAppVer.compareTo(dbNewVer.m_strAppVer) != 0;
    	}
    	
    	public boolean isDbFormatChanged(DBVersion dbNewVer)
    	{
    		return m_bEncrypted != dbNewVer.m_bEncrypted || 
    			m_bSqlite != dbNewVer.m_bSqlite;
    	}
    	
	    public void fromFile(String strFilePath)
		{
	        String strData = CRhoFile.readStringFromFile(strFilePath);
	        
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
		
		public void toFile(String strFilePath)
		{
			String strFullVer = m_strRhoVer + ";" + m_strAppVer + 
        		";" + (m_bEncrypted ? "encrypted":"") + 
        		";" + (m_bSqlite ? "sqlite" : "");
			
			try{
				CRhoFile.writeStringToFile(strFilePath, strFullVer);
			}catch (Exception e) {
		    	LOG.ERROR("writeDBVersion failed.", e);
		    	throw e;
		    }
		}
    };
    
	boolean migrateDB(DBVersion dbVer, DBVersion dbNewVer )
	{
	    LOG.INFO( "Try migrate database from " + (dbVer != null ? dbVer.m_strRhoVer:"") + " to " + (dbNewVer.m_strRhoVer !=null ? dbNewVer.m_strRhoVer:"") );
	    return false;
	}
	
	String getEncryptionInfo()
	{
        /*
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
		return strRes;*/
        //TODO: getEncryptionInfo
        return "";
	}
	
	void checkDBVersion()
	{
		DBVersion dbNewVer = new DBVersion();
		dbNewVer.m_strRhoVer = RhoRuby.getRhoDBVersion(); 
		dbNewVer.m_strAppVer = RhoConf.getInstance().getString("app_db_version");
		String strEncryptionInfo = getEncryptionInfo();
		dbNewVer.m_bEncrypted = strEncryptionInfo != null && strEncryptionInfo.length() > 0;
		dbNewVer.m_bSqlite = true;
			
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
            //TODO: check encryption key
			//if (!com.rho.RhoCrypto.isKeyExist(strEncryptionInfo) )
			//	bDbFormatChanged = true;
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
				    	IDBResult res = db.executeSQL("SELECT * FROM client_info", null, false, false);
				    	if ( !res.isEnd() )
				    	{
				    		m_strClientInfoInsert = createInsertStatement(res, "client_info");
				    		m_dataClientInfo = res.getCurData();
				    	}
					}
				}catch(Exception exc)
				{
					LOG.ERROR("Copy client_info table failed.", exc);
				}finally
				{
					if (db != null )
                        try { db.close(); } catch (Exception ) {  }
				}
			}
			
			m_dbStorage.deleteAllFiles(m_strDBPath);
			
			if ( this.m_strDbPartition.compareTo("user") == 0 ) //do it only once
			{
				String fName = makeBlobFolderName();
				CRhoFile.deleteDirectory(fName);
				makeBlobFolderName(); //Create folder back
			}
			
			dbNewVer.toFile(m_strDbVerPath);
            
            if ( RhoConf.getInstance().isExist("bulksync_state") && RhoConf.getInstance().getInt("bulksync_state") != 0)
            	RhoConf.getInstance().setInt("bulksync_state", 0, true);            
		}
		
	}

    public void rb_open(String szDbName, String szDbPartition)
    {
        setDbPartition(szDbPartition);
        openDB(szDbName, false);

        DBAdapter.getDBPartitions().put(szDbPartition, this);
    }

    private void openDB(String strDBName, boolean bTemp)
    {
    	if ( m_bIsOpen )
    		return;
    	
		initFilePaths(strDBName);
	    if ( !bTemp )
	    	checkDBVersion();
	    
		m_dbStorage.open(m_strDBPath, getSqlScript(), getEncryptionInfo() );
		
		m_bIsOpen = true;
		
		//getAttrMgr().load(this);
		
		m_dbStorage.setDbCallback(new DBCallback(this));
		
		//m_dbAdapters.addElement(this);

	    //copy client_info table
        if ( !bTemp && m_strClientInfoInsert != null && m_strClientInfoInsert.length() > 0 &&
        	 m_dataClientInfo != null )
        {
            LOG.INFO("Copy client_info table from old database");
    		
        	m_dbStorage.executeSQL(m_strClientInfoInsert, m_dataClientInfo, false, false );
        	
            IDBResult res = executeSQL( "SELECT client_id FROM client_info" );
            if ( !res.isEnd() &&  res.getStringByIdx(0).length() > 0 )
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
    
	private boolean destroyTableName(String tableName, Vector<String> arIncludeTables, Vector<String> arExcludeTables )
	{
	    int i;
	    for (i = 0; i < (int)arExcludeTables.size(); i++ )
	    {
	        if ( arExcludeTables.elementAt(i).equalsIgnoreCase(tableName) )
	            return false;
	    }

	    for (i = 0; i < (int)arIncludeTables.size(); i++ )
	    {
	        if ( arIncludeTables.elementAt(i).equalsIgnoreCase(tableName) )
	            return true;
	    }

	    return arIncludeTables.size()==0;
	}
	
	public boolean isTableExist(String strTableName)
	{
		return m_dbStorage.isTableExists(strTableName);
	}
	
    public void rb_destroy_tables(Vector<String> vecIncludes, Vector<String> vecExcludes) 
    {
		if ( !m_bIsOpen )
			return;
		
		IDBStorage db = null;
		try{
            String dbNewName  = CFilePath.changeBaseName(m_strDBPath, "resetdbtemp.sqlite");

            CRhoFile.deleteFile(dbNewName);
            CRhoFile.deleteFile(dbNewName+"-journal");
            CRhoFile.deleteFile(dbNewName+".version");

		    db = RhoClassFactory.createDBStorage();	    
			db.open( dbNewName, getSqlScript(), getEncryptionInfo() );
			
			String[] vecTables = m_dbStorage.getAllTableNames();
			//IDBResult res;
	
		    db.startTransaction();
			
			for ( int i = 0; i< vecTables.Length; i++ )
			{
				String tableName = vecTables[i];
				if ( destroyTableName( tableName, vecIncludes, vecExcludes ) )
					continue;
				
				copyTable(tableName, this.m_dbStorage, db );
			}
			
		    db.commit();
		    db.close();

		    String dbOldName = m_strDBPath;

		    m_dbStorage.close();
		    m_dbStorage = null;
		    m_bIsOpen = false;

            CRhoFile.deleteFilesInFolder(RHODESAPP().getBlobsDirPath());

            string[] ar1 = CRhoFile.enumDirectory("db");

            CRhoFile.deleteFile(dbOldName);
            CRhoFile.deleteFile(dbOldName + "-journal");
            CRhoFile.renameFile(dbNewName,dbOldName);
            CRhoFile.renameFile(dbNewName + "-journal", dbOldName + "-journal");

            string[] ar2 = CRhoFile.enumDirectory("db");

		    m_dbStorage = RhoClassFactory.createDBStorage();
			m_dbStorage.open(m_strDBPath, getSqlScript(), getEncryptionInfo() );
			m_bIsOpen = true;

            string[] ar3 = CRhoFile.enumDirectory("db");
			m_dbStorage.setDbCallback(new DBCallback(this));
			
		}catch(Exception e)
		{
            LOG.ERROR("destroy_table failed.", e);
    		
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
				LOG.ERROR("closing of DB caused exception: " + e1.Message);
			}

            throw e;
		}
    }
    
    private void copyTable(String tableName, IDBStorage dbFrom, IDBStorage dbTo)
    {
    	IDBResult res = dbFrom.executeSQL("SELECT * from " + tableName, null, false, false);
		String strInsert = "";
	    for ( ; !res.isEnd(); res.next() )
	    {
	    	if ( strInsert.length() == 0 )
	    		strInsert = createInsertStatement(res, tableName);
	    	
	    	dbTo.executeSQL(strInsert, res.getCurData(), false, false );
	    }
    }
    
    public void updateAllAttribChanges()
    {
	    //Check for attrib = object
	    IDBResult res = executeSQL("SELECT object, source_id, update_type " +
	        "FROM changed_values where attrib = 'object' and sent=0" );

	    if ( res.isEnd() )
	        return;

	    startTransaction();

	    Vector<String> arObj = new Vector<String>(), arUpdateType = new Vector<String>();
	    Vector<int> arSrcID = new Vector<int>();
	    for( ; !res.isEnd(); res.next() )
	    {
	        arObj.addElement(res.getStringByIdx(0));
	        arSrcID.addElement(res.getIntByIdx(1));
	        arUpdateType.addElement(res.getStringByIdx(2));
	    }
        
        for( int i = 0; i < (int)arObj.size(); i++ )
        {
            IDBResult resSrc = executeSQL("SELECT name, schema FROM sources where source_id=?", arSrcID.elementAt(i) );
            boolean bSchemaSource = false;
            String strTableName = "object_values";
            if ( !resSrc.isEnd() )
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
                    String attribType = getAttrMgr().isBlobAttr(arSrcID.elementAt(i), strAttrib) ? "blob.file" : "";

    	            executeSQLReportNonUnique("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type,sent) VALUES(?,?,?,?,?,?,?)", 
        	                arSrcID.elementAt(i), arObj.elementAt(i), strAttrib, value, arUpdateType.elementAt(i), attribType, 0 );
                }
            }else
            {
                IDBResult res2 = executeSQL( "SELECT attrib, value FROM " + strTableName + " where object=? and source_id=?", 
                    arObj.elementAt(i), arSrcID.elementAt(i) );

    	        for( ; !res2.isEnd(); res2.next() )
    	        {
    	            String strAttrib = res2.getStringByIdx(0);
    	            String value = res2.getStringByIdx(1);
    	            String attribType = getAttrMgr().isBlobAttr(arSrcID.elementAt(i), strAttrib) ? "blob.file" : "";

    	            executeSQLReportNonUnique("INSERT INTO changed_values (source_id,object,attrib,value,update_type,attrib_type,sent) VALUES(?,?,?,?,?,?,?)", 
    	                arSrcID.elementAt(i), arObj.elementAt(i), strAttrib, value, arUpdateType.elementAt(i), attribType, 0 );
    	        }
            }
        }

        executeSQL("DELETE FROM changed_values WHERE attrib='object'"); 

        endTransaction();
    }
    
    void copyChangedValues(DBAdapter db)
    {
    	updateAllAttribChanges();    	
        copyTable("changed_values", m_dbStorage, db.m_dbStorage );
        {
            Vector<int> arOldSrcs = new Vector<int>();
            {
                IDBResult resSrc = db.executeSQL( "SELECT DISTINCT(source_id) FROM changed_values" );
                for ( ; !resSrc.isEnd(); resSrc.next() )
                    arOldSrcs.addElement( resSrc.getIntByIdx(0) );
            }
            for( int i = 0; i < arOldSrcs.size(); i++)
            {
                int nOldSrcID = arOldSrcs.elementAt(i);

                IDBResult res = executeSQL("SELECT name from sources WHERE source_id=?", nOldSrcID);
                if ( !res.isEnd() )
                {
                    String strSrcName = res.getStringByIdx(0);
                    IDBResult res2 = db.executeSQL("SELECT source_id from sources WHERE name=?", strSrcName );
                    if ( !res2.isEnd() )
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
        //TODO: setBulkSyncDB
/*
		DBAdapter db = null;
		try{
			db = (DBAdapter)alloc(null);
			db.setDbPartition(m_strDbPartition);			
    		db.openDB(fDbName, true);
    		db.m_dbStorage.createTriggers();
			
		    db.startTransaction();
			
		    copyTable("client_info", m_dbStorage, db.m_dbStorage );
		    copyChangedValues(db);
		    
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
*/    	
    }
/*
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
    }*/
    
    public static void closeAll()
    {
        Hashtable<String, DBAdapter>.Enumerator hashEnum = m_mapDBPartitions.GetEnumerator();
		while( hashEnum.MoveNext() )
		{
            DBAdapter db = hashEnum.Current.Value;
			db.close();
		}
    }

    public static void initAttrManager()
    {
        Hashtable<String, DBAdapter>.Enumerator hashEnum = m_mapDBPartitions.GetEnumerator();
        while (hashEnum.MoveNext())
        {
            DBAdapter db = hashEnum.Current.Value; 
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

    public static Vector<String> getDBAllPartitionNames()
    {
        Vector<String> vecNames = new Vector<String>();
        Hashtable<String, DBAdapter>.Enumerator hashEnum = m_mapDBPartitions.GetEnumerator();
        while (hashEnum.MoveNext())
        {
            vecNames.addElement(hashEnum.Current.Key);
		}
		
        return vecNames;
    }
        
    public static boolean isAnyInsideTransaction()
    {
        Hashtable<String, DBAdapter>.Enumerator hashEnum = m_mapDBPartitions.GetEnumerator();
        while (hashEnum.MoveNext())
        {
            DBAdapter db = hashEnum.Current.Value;			
			if ( db.isInsideTransaction() )
				return true;
		}

        return false;
    }

    public class DBCallback : IDBCallback
	{
		private DBAdapter m_db;
		public DBCallback(DBAdapter db){ m_db = db; }
		
		public void onBeforeUpdate(String tableName, IDBResult rows2Delete, int[] cols)
		{
			try
			{
				processDelete(tableName, rows2Delete, cols);
			}catch(Exception exc)
			{
				LOG.ERROR("onAfterInsert failed.", exc);
			}
		}
		
		public void onBeforeDelete(String tableName, IDBResult rows2Delete) 
		{
			try
			{
				processDelete(tableName, rows2Delete, null);
			}catch(Exception exc)
			{
				LOG.ERROR("onAfterInsert failed.", exc);
			}
		}
		
		private boolean isChangedCol(int[] cols, int iCol)
		{
			if (cols==null)
				return true;
			
			for ( int i = 0; i < cols.Length; i++ )
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
			int nSrcID = 0;
			if ( !bProcessTable )
			{
				nSrcID = m_db.getAttrMgr().getSrcIDHasBlobsByName(tableName);
				bProcessTable = nSrcID != 0; 
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
					nSrcID = rows2Delete.getIntByIdx(0);
					
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
						if ( !(data[i] is String ) )
							continue;
						
						String value = (String)data[i];
					    if ( m_db.getAttrMgr().isBlobAttr(nSrcID, attrib) )
					    	processBlobDelete(nSrcID, attrib, value);
					}
				}
			}
		}
		
		private void processBlobDelete(int nSrcID, String attrib, String value)
		{
			if ( value == null || value.length() == 0 )
				return;
			
			try{
		        String strFilePath = RHODESAPP().resolveDBFilesPath(value);
                CRhoFile.deleteFile(strFilePath);
			}catch(Exception exc){
				LOG.ERROR("DBCallback.OnDeleteFromTable: Error delete file: " + value, exc);				
			}
		}
		
	}
    }
}
