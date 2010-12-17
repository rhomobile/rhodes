package com.rho.db;

import j2me.math.Number;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyValue;
import net.rim.device.api.database.*;

public class SqliteResult implements IDBResult 
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("SqliteResult");
	
	private Statement m_st;
	private Cursor m_cursor;
	private Row m_row;
	private SqliteCopyResult m_resCopy;
	private boolean m_bNonUnique = false;
	SqliteResult()
	{
	}

	SqliteResult(boolean bNonUnique)
	{
		m_bNonUnique = bNonUnique;
	}
	
	public boolean isNonUnique() {
		return m_bNonUnique;
	}
	
	SqliteResult( Statement st )throws DatabaseException
	{
		m_st = st;
		if ( m_st != null )
		{
			m_cursor = m_st.getCursor();
			if ( m_cursor == null )
				close();
			else
				reset();
		}
	}
	
	public void close()
	{
		try
		{
			if ( m_cursor != null )
				m_cursor.close();
		}catch(DatabaseException exc)
		{
			LOG.ERROR("cursor close failed.", exc);
		}

		try
		{
			
			if ( m_st != null )
				m_st.close();
		}catch(DatabaseException exc)
		{
			LOG.ERROR("statement close failed.", exc);
		}
		m_row = null;
		m_cursor = null;
		m_st = null;
	}
	
	void reset()throws DatabaseException
	{
		if ( m_cursor != null && m_cursor.first() )
			m_row = m_cursor.getRow();
		else
			m_row = null;
	}
	
	public boolean isEnd() 
	{
		if ( m_row == null )
		{
			close();
			return true;
		}
		
		return m_row == null;
	}

	public boolean isOneEnd() throws DBException 
	{
		if ( m_row == null )
		{
			close();
			return true;
		}
		
		if (m_resCopy == null )
			m_resCopy = new SqliteCopyResult(this);
		
		return false;
	}
	
	public void next() throws DBException
	{
		if ( m_resCopy != null )
			m_resCopy.next();
		
		try
		{
			if ( m_cursor != null && m_cursor.next() )
				m_row = m_cursor.getRow();
			else
				m_row = null;
		}catch(DatabaseException exc)
		{
			throw new DBException(exc);
		}
	}
	
	public int getColCount() 
	{
		if ( m_resCopy != null )
			return m_resCopy.getColCount();
		
		return m_row != null ? m_row.getColumnNames().length : 0;
	}

	public String getColName(int nCol) 
	{
		if ( m_resCopy != null )
			return m_resCopy.getColName(nCol);
		
		return m_row.getColumnNames()[nCol];
	}

	public String getOrigColName(int nCol) 
	{
		if ( m_resCopy != null )
			return m_resCopy.getOrigColName(nCol);
		
		return m_row.getColumnNames()[nCol];
	}
	
	public Object[] getCurData() throws DBException 
	{
		if ( m_resCopy != null )
			return m_resCopy.getCurData();
		
		try
		{
			String[] cols = m_row.getColumnNames();
			Object[] res = new Object[cols.length]; 
			for ( int i = 0; i < cols.length; i++ )
				res[i] = m_row.getObject(i);
			
			return res;
		}catch(DataTypeException e)
		{
			throw new DBException(e);
		}
	}

    protected Object getCurValue(int nCol)
    {
		if ( m_resCopy != null )
			return m_resCopy.getCurValue(nCol);
    	
    	try
    	{
    		return m_row.getObject(nCol);
    	}catch(DataTypeException exc)
    	{
    		//throw new DBException(exc);
    	}
    	
    	return null;
    }
	
	int findColIndex(String colname )
	{
		if ( m_resCopy != null )
			return m_resCopy.findColIndex(colname);
		
		String[] cols = m_row.getColumnNames();
		for( int i = 0; i < cols.length; i++ )
		{
			if ( cols[i].equalsIgnoreCase(colname) )
				return i;
		}
		
		return -1;
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
		else if ( val instanceof String)
			return ObjectFactory.createString((String)val);
		else if ( val instanceof Integer)
			return ObjectFactory.createInteger(Number.intValue(val));
		else if ( val instanceof Short)
			return ObjectFactory.createInteger(Number.intValue(val));
		else if ( val instanceof Long)
			return ObjectFactory.createInteger(Number.longValue(val));
		else if ( val instanceof Float)
			return ObjectFactory.createFloat(Number.doubleValue(val));
		else if ( val instanceof Double)
			return ObjectFactory.createFloat(Number.doubleValue(val));
		
		
		return ObjectFactory.createString(val.toString());
	}

	public RubyValue getRubyValue(String colname){ return getRubyValueByIdx( findColIndex(colname) ); }
	public int getInt(String colname){ return getIntByIdx( findColIndex(colname) ); }
	public String getString(String colname){return getStringByIdx(findColIndex(colname) );}

}
