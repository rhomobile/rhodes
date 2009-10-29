package com.rho.db;

import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;
import com.rho.*;

public class DBAdapter extends RubyBasic {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("DbAdapter");

	private static DBAdapter m_Instance;
	
	private IDBStorage m_dbStorage;
	private boolean m_bIsOpen = false;
	private String  m_strDBPath;
	private String  m_strDBVerPath;
	private DBAttrManager m_attrMgr = new DBAttrManager();
	
    Mutex m_mxDB = new Mutex();
    boolean m_bInsideTransaction=false;
    boolean m_bUnlockDB = false;
	
	DBAdapter(RubyClass c) {
		super(c);
		
		try{
			m_dbStorage = RhoClassFactory.createDBStorage();
		}catch(Exception exc){
    		LOG.ERROR("createDBStorage failed.", exc);
		}
	}
	
	public static DBAdapter getInstance(){
		if ( m_Instance == null )
			m_Instance = new DBAdapter(RubyRuntime.DatabaseClass); 
		
		return m_Instance;
	}
	
	public DBAttrManager getAttrMgr()
	{ 
		return m_attrMgr; 
	}
	
	public IDBResult executeSQL(String strStatement, Object[] values)throws DBException{
		LOG.TRACE("executeSQL: " + strStatement);
		
		return m_dbStorage.executeSQL(strStatement,values,false);
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
	
	public IDBResult executeSQLReportNonUnique(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5, Object arg6)throws DBException{
		LOG.TRACE("executeSQLReportNonUnique: " + strStatement);
		
		Object[] values = {arg1,arg2,arg3,arg4,arg5,arg6};
		return m_dbStorage.executeSQL(strStatement,values, true);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5, Object arg6, Object arg7)throws DBException{
		Object[] values = {arg1,arg2,arg3,arg4,arg5,arg6,arg7};
		return executeSQL(strStatement,values);
	}

	public boolean isUnlockDB(){ return m_bUnlockDB; }
	public void setUnlockDB(boolean b){ m_bUnlockDB = b; }
	public void Lock(){ m_mxDB.Lock(); }
	public void Unlock(){ setUnlockDB(false); m_mxDB.Unlock(); }
    public boolean isInsideTransaction(){ return m_bInsideTransaction; }
	
	public static IDBResult createResult(){
		return getInstance().m_dbStorage.createResult();
	}
	
	public static String makeBlobFolderName()throws Exception{
		String fName = RhoClassFactory.createFile().getDirPath("apps/public/db-files");
		
		return fName;
	}
	
	RubyString[] getColNames(IDBResult rows)
	{
		RubyString[] colNames = new RubyString[rows.getColCount()];
		for ( int nCol = 0; nCol < rows.getColCount(); nCol ++ )
			colNames[nCol] = ObjectFactory.createString(rows.getColName(nCol));
		
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
    
    private String getSqlScript(){
    	//TODO: read script from jar
		return "CREATE TABLE client_info ("+
		"client_id VARCHAR(255) PRIMARY KEY,"+
		"token VARCHAR(255) default NULL,"+
		"token_sent int default 0,"+
		"reset int default 0,"+
		"port VARCHAR(10) default NULL,"+
		"last_sync_success VARCHAR(100) default NULL);"+
		"CREATE TABLE object_values ("+
		" id INTEGER PRIMARY KEY,"+
		" source_id int default NULL,"+
		" attrib varchar(255) default NULL,"+
		" object varchar(255) default NULL,"+
		" value varchar default NULL,"+
		" attrib_type varchar(255) default NULL);"+
		"CREATE TABLE changed_values ("+
		" id INTEGER default NULL,"+
		" source_id int default NULL,"+
		" attrib varchar(255) default NULL,"+
		" object varchar(255) default NULL,"+
		" value varchar default NULL,"+
		" attrib_type varchar(255) default NULL," +
		" update_type varchar(255) default NULL, " +
		" sent int default 0," +
		" main_id INTEGER default 0 );"+
		"CREATE TABLE sources ("+
		//"id INTEGER PRIMARY KEY,"+
		"source_id int PRIMARY KEY,"+
		"name varchar(255) default NULL,"+
		"token varchar(30) default NULL,"+
		"source_url VARCHAR(255) default NULL,"+
		"session VARCHAR(255) default NULL,"+
		"last_updated int default 0,"+
		"last_inserted_size int default 0,"+
		"last_deleted_size int default 0,"+
		"last_sync_duration int default 0,"+
		"last_sync_success int default 0," +
		"source_attribs varchar default NULL);"+
		//"CREATE INDEX by_attrib_obj_utype on object_values (attrib,object,update_type);"+
		//"CREATE INDEX by_attrib_utype on object_values (attrib,update_type);"+
		//"CREATE INDEX by_src_type ON object_values (source_id, attrib_type, object);"+
		"CREATE INDEX by_src_id ON object_values (source_id);"+
		"CREATE UNIQUE INDEX by_src_object ON object_values (object, attrib, source_id);"+
		"CREATE INDEX by_src_value ON object_values (attrib, source_id, value);";//+
		//"CREATE INDEX by_id ON object_values (id);"; //for delete operation
		//"CREATE INDEX by_src_object ON object_values (source_id, object);"+
		//"CREATE INDEX by_src_up_value ON object_values (source_id, update_type, value);";
		//"CREATE INDEX by_type ON object_values (attrib_type)";
    }

    /*private String getSqlScript(){
    	//TODO: read script from jar
		return "CREATE TABLE client_info ("+
		"client_id VARCHAR(255) PRIMARY KEY,"+
		"token VARCHAR(255) default NULL,"+
		"token_sent int default 0,"+
		"reset int default 0,"+
		"port VARCHAR(10) default NULL,"+
		"last_sync_success VARCHAR(100) default NULL);"+
		"CREATE TABLE object_values ("+
		" id INTEGER PRIMARY KEY,"+
		" source_id int default NULL,"+
		" attrib varchar(255) default NULL,"+
		" object varchar(255) default NULL,"+
		" value varchar default NULL,"+
		" attrib_type varchar(32) default NULL);"+
		"CREATE TABLE changed_values ("+
		" id INTEGER PRIMARY KEY,"+
		" update_type varchar(255) default NULL);"+
		"CREATE TABLE sources ("+
		//"id INTEGER PRIMARY KEY,"+
		"source_id int PRIMARY KEY,"+
		"name varchar(255) default NULL,"+
		"token varchar(32) default NULL,"+
		"source_url VARCHAR(255) default NULL,"+
		"session VARCHAR(255) default NULL,"+
		"last_updated int default 0,"+
		"last_inserted_size int default 0,"+
		"last_deleted_size int default 0,"+
		"last_sync_duration int default 0,"+
		"last_sync_success int default 0," +
		"source_attribs varchar default NULL);"+
		//"CREATE INDEX by_attrib_obj_utype on object_values (attrib,object,update_type);"+
		//"CREATE INDEX by_attrib_utype on object_values (attrib,update_type);"+
		//"CREATE INDEX by_src_type ON object_values (source_id, attrib_type, object);"+
		"CREATE INDEX by_src_update ON object_values (source_id, update_type);"+
		"CREATE INDEX by_id ON object_values (id);"; //for delete operation
		//"CREATE INDEX by_src_object ON object_values (source_id, object);"+
		//"CREATE INDEX by_src_up_value ON object_values (source_id, update_type, value);";
		//"CREATE INDEX by_type ON object_values (attrib_type)";
    }*/
    
    public void startTransaction()throws DBException
    {
    	Lock();
    	m_bInsideTransaction=true;    	
    	m_dbStorage.startTransaction();
    }
    
    public void commit()throws DBException
    {
    	getAttrMgr().save(this);
    	m_dbStorage.commit();
    	m_bInsideTransaction=false;
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
    
    DBVersion readDBVersion()throws Exception
	{
    	String strFullVer = "";
    	try {
	    	IRAFile file = RhoClassFactory.createRAFile();
	    	file.open(m_strDBVerPath);
	        byte buf[] = new byte[20];
	        int len = file.read(buf, 0, buf.length);
			file.close();
			if ( len > 0 )
				strFullVer = new String(buf,0,len);
    	}
    	catch (Exception e) {
    		LOG.TRACE("readDBVersion: Exception: " + e.getMessage());
    	}
    	
		if ( strFullVer.length() == 0 )
			return new DBVersion();
		
		int nSep = strFullVer.indexOf(';');
		if ( nSep == -1 )
			return new DBVersion(strFullVer, "");
		
		return new DBVersion(strFullVer.substring(0,nSep), strFullVer.substring(nSep+1) );
	}
	
	void writeDBVersion(DBVersion ver)throws Exception
	{
		IRAFile file = RhoClassFactory.createRAFile();
		file.open(m_strDBVerPath, "rw");
        String strFullVer = ver.m_strRhoVer + ";" + ver.m_strAppVer;
        byte[] buf = strFullVer.getBytes();
        file.write(buf, 0, buf.length);
        file.close();
	}
    
	void checkDBVersion()throws Exception
	{
		String strRhoDBVer = RhoSupport.getRhoDBVersion();
		String strAppDBVer = RhoConf.getInstance().getString("app_db_version");
		
		DBVersion dbVer = readDBVersion();

		boolean bReset = false;
		
		if ( strRhoDBVer != null && strRhoDBVer.length() > 0 )
		{
			if ( dbVer == null || dbVer.m_strRhoVer == null || !dbVer.m_strRhoVer.equalsIgnoreCase(strRhoDBVer) )
				bReset = true;
		}
		if ( strAppDBVer != null && strAppDBVer.length() > 0 )
		{
			if ( dbVer == null || dbVer.m_strAppVer == null || !dbVer.m_strAppVer.equalsIgnoreCase(strAppDBVer) )
				bReset = true;
		}
		
		if ( bReset )
		{
			m_dbStorage.deleteAllFiles(m_strDBPath);
			
			String fName = makeBlobFolderName();
			RhoClassFactory.createFile().delete(fName);
			DBAdapter.makeBlobFolderName(); //Create folder back
			
            writeDBVersion(new DBVersion(strRhoDBVer, strAppDBVer) );
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
    
    private RubyValue rb_destroy_table(RubyValue v) 
    {
		if ( !m_bIsOpen )
			return RubyConstant.QNIL;
		
		IDBStorage db = null;
		try{
		    getAttrMgr().reset(this);
			
			String strTable = v.toStr();
			
			String dbName = getNameNoExt(m_strDBPath);
			String dbNewName  = dbName + "new";
			
			IFileAccess fs = RhoClassFactory.createFileAccess();
			
			String dbNameData = dbName + ".data";
		    String dbNewNameData = dbNewName + ".data";
		    String dbNameScript = dbName + ".script";
		    String dbNewNameScript = dbNewName + ".script";
		    String dbNameJournal = dbName + ".journal";
		    String dbNewNameJournal = dbNewName + ".journal";
		    
			//LOG.TRACE("DBAdapter: " + dbNewNameDate + ": " + (fs.exists(dbNewNameData) ? "" : "not ") + "exists");
		    fs.delete(dbNewNameData);
		    //LOG.TRACE("DBAdapter: " + dbNewNameScript + ": " + (fs.exists(dbNewNameScript) ? "" : "not ") + "exists");
		    fs.delete(dbNewNameScript);
		    //LOG.TRACE("DBAdapter: " + dbNewNameJournal + ": " + (fs.exists(dbNewNameJournal) ? "" : "not ") + "exists");
		    fs.delete(dbNewNameJournal);
		    
		    LOG.TRACE("1. Size of " + dbNameData + ": " + fs.size(dbNameData));
		    
		    db = RhoClassFactory.createDBStorage();	    
			db.open( dbNewName, getSqlScript() );
			
			String[] vecTables = m_dbStorage.getAllTableNames();
			IDBResult res;
	
		    db.startTransaction();
			
			for ( int i = 0; i< vecTables.length; i++ )
			{
				String tableName = vecTables[i];
				if ( tableName.equalsIgnoreCase(strTable) )
					continue;
				
				res = executeSQL("SELECT * from " + tableName, null);
				String strInsert = "";
			    for ( ; !res.isEnd(); res.next() )
			    {
			    	if ( strInsert.length() == 0 )
			    		strInsert = createInsertStatement(res, tableName);
			    	
			    	db.executeSQL(strInsert, res.getCurData(), false );
			    }
			}
			
		    db.commit();
		    db.close();
		    
		    m_dbStorage.close();
		    m_dbStorage = null;
		    m_bIsOpen = false;
		    
		    LOG.TRACE("2. Size of " + dbNewNameData + ": " + fs.size(dbNewNameData));
		    
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
    
    public void rollback()throws DBException
    {
    	m_dbStorage.rollback();
    	m_bInsideTransaction=false;
    	
    	Unlock();
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

    public RubyValue rb_execute(RubyValue v, RubyValue arg) 
    {
    	RubyArray res = new RubyArray(); 
    	try{
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
	    			if ( val instanceof RubyFixnum )
	    				values[i] = new Long( ((RubyFixnum)val).toLong() );
	    			else
	    				values[i] = val.toString();
	    		}
    		}
    		
    		IDBResult rows = executeSQL(v.toStr(), values);
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
    }
    
    //@RubyAllocMethod
    private static RubyValue alloc(RubyValue receiver) {
    	return getInstance();
    }
    
    private RubyValue rb_initialize(RubyValue v) 
    {
    	try{
    		openDB(v !=null && v != RubyConstant.QNIL ? v.toString() : "");
		}catch(Exception e)
		{
			LOG.ERROR("initialize failed.", e);
			throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
		}
    	
        return this;
    }
    
    private RubyValue rb_start_transaction() {
    	try{
    		setUnlockDB(true);
    		startTransaction();
    	}catch( Exception e ){
    		LOG.ERROR("start_transaction failed.", e);
			throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
    	}
    	
        return ObjectFactory.createInteger(0);
    }
    
    private RubyValue rb_commit() {
    	try{
    		commit();
    	}catch( Exception e ){
    		LOG.ERROR("commit failed.", e);
    		throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
    	}
    	
        return ObjectFactory.createInteger(0);
    }
    
    private RubyValue rb_rollback() {
    	try{
    		rollback();
    	}catch( Exception e ){
    		LOG.ERROR("rollback failed.", e);
    		throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
    	}
    	
        return ObjectFactory.createInteger(0);
    }
    
    private RubyValue rb_close() {
    	try{
    		if ( m_dbStorage != null ){
		    	m_dbStorage.close();
		    	m_dbStorage = null;
    		}
    		
    	}catch( Exception e ){
    		LOG.ERROR("close failed.", e);
    		throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
    	}
    	
        return ObjectFactory.createInteger(0);
    }
    
	public static void initMethods(RubyClass klass) {
		
		klass.defineAllocMethod(new RubyNoArgMethod(){
			protected RubyValue run(RubyValue receiver, RubyBlock block )	{
				return DBAdapter.alloc(receiver);}
		});
		
		klass.defineMethod( "initialize", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((DBAdapter)receiver).rb_initialize(arg);}
		});
		klass.defineMethod( "close", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return ((DBAdapter)receiver).rb_close();}
		});
		klass.defineMethod( "execute", new RubyVarArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
				return ((DBAdapter)receiver).rb_execute(args.get(0), 
						(args.size() > 1 ? args.get(1):null));}
		});
		
		klass.defineMethod( "start_transaction", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return ((DBAdapter)receiver).rb_start_transaction();}
		});
		klass.defineMethod( "commit", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return ((DBAdapter)receiver).rb_commit();}
		});
		klass.defineMethod( "rollback", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return ((DBAdapter)receiver).rb_rollback();}
		});
		klass.defineMethod( "destroy_table", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((DBAdapter)receiver).rb_destroy_table(arg);}
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

		public void OnInsertIntoTable(String tableName, IDBResult rows2Insert)
		{
			if ( !tableName.equalsIgnoreCase("object_values") )
				return;

			for( ; !rows2Insert.isEnd(); rows2Insert.next() )
			{
				int nSrcID = rows2Insert.getIntByIdx(1);
				String name = rows2Insert.getStringByIdx(2);
				m_db.getAttrMgr().add(nSrcID, name);
			}
			
		}
		
		public void OnDeleteFromTable(String tableName, IDBResult rows2Delete) 
		{
			if ( !tableName.equalsIgnoreCase("object_values") )
				return;
			
			for( ; !rows2Delete.isEnd(); rows2Delete.next() )
			{
				int nSrcID = rows2Delete.getIntByIdx(1);
				String attrib = rows2Delete.getStringByIdx(2);
				m_db.getAttrMgr().remove(nSrcID, attrib);
				
				String attrib_type = rows2Delete.getStringByIdx(5);
				if ( !attrib_type.equals("blob.file") )
					continue;

				String url = rows2Delete.getStringByIdx(4);
				if ( url == null || url.length() == 0 )
					continue;
				
				try{
				    SimpleFile oFile = RhoClassFactory.createFile();
				    
			        String strFilePath = oFile.getDirPath("");
			        strFilePath += url;
				    
				    oFile.delete(strFilePath);
				}catch(Exception exc){
					LOG.ERROR("DBCallback.OnDeleteFromTable: Error delete file: " + url, exc);				
				}
				
			}
		}
		
	}
	
}
