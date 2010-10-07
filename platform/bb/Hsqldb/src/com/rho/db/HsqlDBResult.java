package com.rho.db;

import org.hsqldb.Record;
import org.hsqldb.Result;
import org.hsqldb.Trace;
import org.hsqldb.Types;
import org.hsqldb.HsqlException;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.lang.*;
import j2me.math.Number;

public class HsqlDBResult implements IDBResult 
{
	private Result m_result;
	private Record m_current;
//	private int m_nCurIndex;
	
	public HsqlDBResult(Result res){
		m_result = res;
		reset();
	}
	
	public HsqlDBResult(){
	}
/*	
	public int getCount(){ 
		return m_result != null ? m_result.getSize() : 0; 
	}*/

	public boolean isNonUnique() {
		if ( m_result != null && m_result.getException() != null &&
			 m_result.getException() instanceof HsqlException )
		{
			HsqlException e = (HsqlException)m_result.getException();
			return e.getErrorCode() == -Trace.VIOLATION_OF_UNIQUE_INDEX ||
				e.getErrorCode() == -Trace.VIOLATION_OF_UNIQUE_CONSTRAINT;
		}
		
		return false;
	}
	
	public int getColCount(){ 
		return m_result != null ? m_result.getColumnCount() : 0; 
	}
	
	public String getColName(int nCol){
		Result.ResultMetaData md = m_result.metaData;
		if ( md.tableNames[nCol] != null && md.tableNames[nCol].length() > 0 )
			return md.colNames[nCol];//.toLowerCase();

		//AS alias
		return md.colNames[nCol];
	}

	public String getOrigColName(int nCol){
		Result.ResultMetaData md = m_result.metaData;
		if ( md.tableNames[nCol] != null && md.tableNames[nCol].length() > 0 )
		{
			if ( md.colOrigNames[nCol] != null )
				return md.colOrigNames[nCol];
			
			return md.colNames[nCol];//.toLowerCase();
		}
		
		//AS alias
		return md.colNames[nCol];
	}
	
	public Result getResult(){
		return m_result;
	}
	
	/*
	public RubyValue getRubyValueByIdx(int nItem, int nCol)
	{ 
		Object val = getColvalueByIdx(nItem,nCol);
		if (val == null)
			return RubyConstant.QNIL;
		
		switch( m_result.metaData.colTypes[nCol] )
		{
        case Types.NULL :
            return RubyConstant.QNIL;

        case Types.VARCHAR :
        case Types.LONGVARCHAR :
        case Types.CHAR :
        case Types.VARCHAR_IGNORECASE :
        	return ObjectFactory.createString((String)val);
        	
        case Types.TINYINT :
        case Types.SMALLINT :
        case Types.INTEGER :
        	return ObjectFactory.createInteger(Number.intValue(val));
        	
        case Types.BIGINT :
        	return ObjectFactory.createInteger(Number.longValue(val));
        case Types.REAL :
        case Types.FLOAT :
        case Types.DOUBLE :
        	return ObjectFactory.createFloat(Number.doubleValue(val));

        case Types.BOOLEAN : 
        	return ObjectFactory.createBoolean(((Boolean) val).booleanValue());
        	
        case Types.NUMERIC :
        case Types.DECIMAL :
        case Types.DATE :
        case Types.TIME :
        case Types.TIMESTAMP :
        case Types.BINARY :
        case Types.VARBINARY :
        case Types.LONGVARBINARY :
        case Types.OTHER :
        	return ObjectFactory.createString(val.toString());
		}
		
		throw new java.lang.RuntimeException("HsqlDBResult: unknown type :" + m_result.metaData.colTypes[nCol] );

//		return ObjectFactory.createString(); 
	}
	
	public long getLongByIdx(int nItem, int nCol)
	{ 
		Object val = getColvalueByIdx(nItem,nCol);
		return val != null ? Number.longValue(val) : 0; 
	}
	
	public int getIntByIdx(int nItem, int nCol)
	{
		Object val = getColvalueByIdx(nItem,nCol);
		return val != null ? Number.intValue(val) : 0; 
	}
	
	public String getStringByIdx(int nItem, int nCol)
	{
		Object val = getColvalueByIdx(nItem,nCol);
		return val != null ? val.toString() : ""; 
	}
	
	public RubyValue getRubyValue(int nItem, String colname){ return getRubyValueByIdx(nItem, findColIndex(colname) ); }
	public long getLong(int nItem, String colname){ return getLongByIdx(nItem, findColIndex(colname) ); }
	public int getInt(int nItem, String colname){ return getIntByIdx(nItem, findColIndex(colname) ); }
	public String getString(int nItem, String colname){ return getStringByIdx(nItem, findColIndex(colname) ); }

	Object getColvalueByIdx(int nItem, int nCol)
	{
		if ( nCol < 0 || nCol >= getColCount() )
			throw new IndexOutOfBoundsException("HsqlDBResult.getStringByIdx : " + nCol + ". Count : " + getColCount() );
		
		Object[] item = getItem(nItem);
		if ( item != null )
			return item[nCol];
		
		return null; 
	}*/
	
	private void reset(){
		m_current = m_result.rRoot;
	//	m_nCurIndex = 0;
	}
	/*
	Object[] getItem(int nItem){
		if ( m_current == null || nItem < 0 || nItem >= getCount() )
			return null;

		if ( m_nCurIndex > nItem  )
			reset();
		
		if ( m_nCurIndex < nItem  ){
			for ( ; m_nCurIndex < nItem; m_nCurIndex++ )
				m_current = m_current.next;
		}

		return m_current.data;  
	}
	
	int findColIndex(String colname )
	{
		for( int i = 0; i < getColCount(); i++ )
		{
			if ( m_result.metaData.colNames[i].equalsIgnoreCase(colname) )
				return i;
		}
		
		return -1;
	}*/

	//New
    public boolean isEnd()
    {
    	return m_current == null;
    }
    
    public void next()
    {
    	if ( m_current != null )
    		m_current = m_current.next;
    }
    
    protected Object getCurValue(int nCol)
    {
    	return m_current.data[nCol];
    }

    public Object[] getCurData()
    {
    	return m_current.data;
    }

	public boolean isNullByIdx(int nCol)
	{
		Object val = getCurValue(nCol);
		return val == null; 
	}
    
	public String getStringByIdx(int nCol)
	{
		Object val = getCurValue(nCol);
		return val != null ? val.toString() : ""; 
	}
	
	public int getIntByIdx(int nCol)
	{
		Object val = getCurValue(nCol);
		return val != null ? Number.intValue(val) : 0; 
	}
	
	public long getLongByIdx(int nCol)
	{
		Object val = getCurValue(nCol);
		return val != null ? Number.longValue(val) : 0; 
	}
	
	public String getUInt64ByIdx(int nCol)
	{
		return getStringByIdx(nCol);
	}

	public RubyValue getRubyValueByIdx(int nCol)
	{ 
		Object val = getCurValue(nCol);
		if (val == null)
			return RubyConstant.QNIL;
		
		switch( m_result.metaData.colTypes[nCol] )
		{
        case Types.NULL :
            return RubyConstant.QNIL;

        case Types.VARCHAR :
        case Types.LONGVARCHAR :
        case Types.CHAR :
        case Types.VARCHAR_IGNORECASE :
        	return ObjectFactory.createString((String)val);
        	
        case Types.TINYINT :
        case Types.SMALLINT :
        case Types.INTEGER :
        	return ObjectFactory.createInteger(Number.intValue(val));
        	
        case Types.BIGINT :
        	return ObjectFactory.createInteger(Number.longValue(val));
        	
        case Types.REAL :
        case Types.FLOAT :
        case Types.DOUBLE :
        	return ObjectFactory.createFloat(Number.doubleValue(val));

        case Types.BOOLEAN : 
        	//return ObjectFactory.createBoolean(((Boolean) val).booleanValue());
        	
        case Types.NUMERIC :
        case Types.DECIMAL :
        case Types.DATE :
        case Types.TIME :
        case Types.TIMESTAMP :
        case Types.BINARY :
        case Types.VARBINARY :
        case Types.LONGVARBINARY :
        case Types.OTHER :
        	return ObjectFactory.createString(val.toString());
		}
		
		throw new java.lang.RuntimeException("HsqlDBResult: unknown type :" + m_result.metaData.colTypes[nCol] );

//		return ObjectFactory.createString(); 
	}

	public RubyValue getRubyValue(String colname){ return getRubyValueByIdx( findColIndex(colname) ); }
	public int getInt(String colname){ return getIntByIdx( findColIndex(colname) ); }
	public String getString(String colname){ return getStringByIdx(findColIndex(colname) ); }

	int findColIndex(String colname )
	{
		for( int i = 0; i < getColCount(); i++ )
		{
			if ( m_result.metaData.colNames[i].equalsIgnoreCase(colname) )
				return i;
		}
		
		return -1;
	}
	
}
