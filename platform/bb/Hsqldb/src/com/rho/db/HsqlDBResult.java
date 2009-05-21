package com.rho.db;

import org.hsqldb.HsqlDateTime;
import org.hsqldb.Library;
import org.hsqldb.Record;
import org.hsqldb.Result;
import org.hsqldb.Types;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.lang.*;
import org.hsqldb.*;
import org.hsqldb.persist.*;
import org.hsqldb.types.Binary;

import j2me.math.BigDecimal;
import j2me.math.Number;
import j2me.sql.Date;
import j2me.sql.Time;
import j2me.sql.Timestamp;

public class HsqlDBResult implements IDBResult 
{
	private Result m_result;
	private Record m_current;
	private int m_nCurIndex;
	
	public HsqlDBResult(Result res){
		m_result = res;
		reset();
	}
	
	public HsqlDBResult(){
	}
	
	public int getCount(){ 
		return m_result != null ? m_result.getSize() : 0; 
	}
	
	public int getColCount(){ 
		return m_result != null ? m_result.getColumnCount() : 0; 
	}
	
	public String getColName(int nCol){
		Result.ResultMetaData md = m_result.metaData;
		return md.colNames[nCol].toLowerCase();
	}
	
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
	}
	
	private void reset(){
		m_current = m_result.rRoot;
		m_nCurIndex = 0;
	}
	
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
	}

	public void close(){
	  //TODO: Add lock to this class
        //FIXME: Here this lock must be released
	}
	
}
