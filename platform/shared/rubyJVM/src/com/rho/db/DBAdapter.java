package com.rho.db;

import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;
import com.rho.*;

public class DBAdapter extends RubyBasic {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("DbAdapter");

	private static DBAdapter m_Instance;
	
	private IDBCallback m_dbCallback;
	private IDBStorage m_dbStorage;
	private boolean m_bIsOpen = false;
	private String  m_strDBPath;
	private String  m_strDBVerPath;
	
    Mutex m_mxDB = new Mutex();
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
	
	public void setDbCallback(IDBCallback callback){
		m_dbCallback = callback;
		
		m_dbStorage.setDbCallback(callback);
	}
	
	public IDBResult executeSQL(String strStatement, Object[] values)throws DBException{
		LOG.TRACE("executeSQL: " + strStatement);
		
		return m_dbStorage.executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement)throws DBException{
		LOG.TRACE("executeSQL: " + strStatement);
		
		return executeSQL(strStatement,null);
	}
	public IDBResult executeSQL(String strStatement, Object arg1)throws DBException{
		LOG.TRACE("executeSQL: " + strStatement);
		
		Object[] values = {arg1};
		return executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement, int arg1)throws DBException{
		LOG.TRACE("executeSQL: " + strStatement);
		
		Object[] values = { new Integer(arg1)};
		return executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement, long arg1)throws DBException{
		LOG.TRACE("executeSQL: " + strStatement);
		
		Object[] values = { new Long(arg1)};
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2)throws DBException{
		LOG.TRACE("executeSQL: " + strStatement);
		
		Object[] values = {arg1,arg2};
		return executeSQL(strStatement,values);
	}
	
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3)throws DBException{
		LOG.TRACE("executeSQL: " + strStatement);
		
		Object[] values = {arg1,arg2,arg3};
		return executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5, Object arg6)throws DBException{
		LOG.TRACE("executeSQL: " + strStatement);
		
		Object[] values = {arg1,arg2,arg3,arg4,arg5,arg6};
		return executeSQL(strStatement,values);
	}
	public IDBResult executeSQL(String strStatement, Object arg1, Object arg2, Object arg3, Object arg4, Object arg5, Object arg6, Object arg7)throws DBException{
		LOG.TRACE("executeSQL: " + strStatement);
		
		Object[] values = {arg1,arg2,arg3,arg4,arg5,arg6,arg7};
		return executeSQL(strStatement,values);
	}

	public boolean isUnlockDB(){ return m_bUnlockDB; }
	public void setUnlockDB(boolean b){ m_bUnlockDB = b; }
	public void Lock(){ m_mxDB.Lock(); }
	public void Unlock(){ setUnlockDB(false); m_mxDB.Unlock(); }
	
	public static IDBResult createResult(){
		return getInstance().m_dbStorage.createResult();
	}
	
	public static String makeBlobFolderName()throws Exception{
		String fName = RhoClassFactory.createFile().getDirPath("blobs");
		
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
		" id INTEGER default NULL,"+
		" token varchar(30) default NULL,"+
		" source_id int default NULL,"+
		" attrib varchar(255) default NULL,"+
		" object varchar(255) default NULL,"+
		" value varchar default NULL,"+
		" update_type varchar(255) default NULL,"+
		" attrib_type varchar(255) default NULL);"+
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
		"last_sync_success int default 0);"+
		//"CREATE INDEX by_attrib_obj_utype on object_values (attrib,object,update_type);"+
		"CREATE INDEX by_attrib_utype on object_values (attrib,update_type);"+
		"CREATE INDEX by_src_type ON object_values (source_id, attrib_type, object);"+
		"CREATE INDEX by_type ON object_values (attrib_type)";
    }
    
    public void startTransaction()throws DBException
    {
    	Lock();
    	m_dbStorage.startTransaction();
    }
    
    public void commit()throws DBException
    {
    	m_dbStorage.commit();
    	Unlock();
    }
    public void endTransaction()throws DBException
    {
    	commit();
    }

	String readDBVersion()throws Exception
	{
        SimpleFile file = RhoClassFactory.createFile();
        file.open(m_strDBVerPath, true, true);
        byte buf[] = new byte[20];
		int len = file.read(0, buf);
		file.close();
		if ( len > 0 )
			return new String(buf,0,len);
		else
			return null;
	}
	
	void writeDBVersion(String ver)throws Exception
	{
        SimpleFile file = RhoClassFactory.createFile();
        file.open(m_strDBVerPath, false, false);
        file.write(0, ver.getBytes());
        file.close();
	}
    
    private void openDB(String strDBName)throws Exception
    {
    	if ( m_bIsOpen )
    		return;
    	
		String strVer = RhoSupport.getRhoDBVersion();
		initFilePaths(strDBName);
		
    	//Check version
		if ( strVer != null && strVer.length() > 0 ){
        	String dbVer = readDBVersion();
			if ( dbVer == null || !dbVer.equalsIgnoreCase(strVer) )
			{
				m_dbStorage.deleteAllFiles(m_strDBPath);
				if ( m_dbCallback != null )
					m_dbCallback.OnDeleteAll();
				
	            writeDBVersion(strVer);
			}
        }
		
		m_dbStorage.open(m_strDBPath, getSqlScript() );
		
		m_bIsOpen = true;
    }
    
    public void rollback()throws DBException
    {
    	Unlock();
    	throw new DBException("Not implemented");
    }
    
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
    		
	    	m_dbCallback = null;
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
		klass.defineMethod( "execute", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((DBAdapter)receiver).rb_execute(arg);}
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
		
	}
}
