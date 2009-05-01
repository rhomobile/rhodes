package com.rho.db;

import java.io.IOException;
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
	}
	
	public IDBResult executeSQL(String strStatement, Object[] values)throws DBException{
		LOG.TRACE("executeSQL: " + strStatement);
		
		return m_dbStorage.executeSQL(strStatement,values);
	}

	public static IDBResult createResult(){
		return getInstance().m_dbStorage.createResult();
	}
	
	RubyString[] getColNames(IDBResult rows)
	{
		RubyString[] colNames = new RubyString[rows.getColCount()];
		for ( int nCol = 0; nCol < rows.getColCount(); nCol ++ )
			colNames[nCol] = ObjectFactory.createString(rows.getColName(nCol));
		
		return colNames;
	}
	
    public RubyValue rb_execute(RubyValue v) 
    {
    	RubyArray res = new RubyArray(); 
    	try{
    		IDBResult rows = executeSQL(v.toStr(), null);
    		RubyString[] colNames = getColNames(rows);
    		
    		for( int i = 0; i < rows.getCount(); i++ )
    		{
    			RubyHash row = ObjectFactory.createHash();
    			for ( int nCol = 0; nCol < rows.getColCount(); nCol ++ )
    				row.add( colNames[nCol], rows.getRubyValueByIdx(i, nCol) );
    			
    			res.add( row );
    		}
    	}catch(DBException exc){
    		LOG.ERROR("executeSQL failed.", exc);
    	}
    	
        return res;
    }
	
    //@RubyAllocMethod
    private static RubyValue alloc(RubyValue receiver) {
    	return getInstance();
    }
    
    private RubyValue rb_initialize(RubyValue v) {
    	openDB(v !=null && v != RubyConstant.QNIL ? v.toString() : "");
    	
        return this;
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
		"token varchar(30) default NULL,"+
		"source_url VARCHAR(255) default NULL,"+
		"session VARCHAR(255) default NULL,"+
		"last_updated int default 0,"+
		"last_inserted_size int default 0,"+
		"last_deleted_size int default 0,"+
		"last_sync_duration int default 0,"+
		"last_sync_success int default 0);"+
		"CREATE INDEX by_src_type ON object_values (source_id, attrib_type, object);"+
		"CREATE INDEX by_type ON object_values (attrib_type)";
    }
    
    public void startTransaction()throws DBException
    {
    	m_dbStorage.startTransaction();
    }
    
    public void commit()throws DBException
    {
    	m_dbStorage.commit();
    }
    
    private void openDB(String strDBName)
    {
    	if ( m_bIsOpen )
    		return;
    	
    	try{
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
    	}catch( Exception exc ){
    		LOG.ERROR("Open database failed.", exc);
    	}
    }
    
    private RubyValue rb_close() {
    	try{
    		if ( m_dbStorage != null ){
		    	m_dbStorage.close();
		    	m_dbStorage = null;
    		}
    		
	    	m_dbCallback = null;
    	}catch( Exception exc ){
    		LOG.ERROR("Close database failed.", exc);
    	}
    	
        return ObjectFactory.createInteger(0);
    }
    
	String readDBVersion()throws Exception
	{
        SimpleFile file = RhoClassFactory.createFile();
        file.open(m_strDBVerPath, true, true);
        byte buf[] = new byte[20];
		int len = file.read(0, buf);
		file.close();
		return new String(buf,0,len);
	}
	
	void writeDBVersion(String ver)throws Exception
	{
        SimpleFile file = RhoClassFactory.createFile();
        file.open(m_strDBVerPath, false, false);
        file.write(0, ver.getBytes());
        file.close();
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
		
	}
}
