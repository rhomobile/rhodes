/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rho.db;

//import com.rho.RhoEmptyLogger;
//import com.rho.RhoLogger;
import java.util.Vector;

public class SqliteCopyResult// implements IDBResult
{
//	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
//		new RhoLogger("SqliteCopyResult");

	Vector m_arRows = new Vector();
	int m_nCurPos = 0;
	Vector m_arColumns = new Vector();

	public SqliteCopyResult( IDBResult res)throws DBException
	{
		for( int i = 0; i < res.getColCount(); i++)
			m_arColumns.addElement(res.getColName(i));

		for( ; !res.isEnd(); res.next() )
		{
			m_arRows.addElement(res.getCurData());
		}
	}

	public void close()
	{
		m_arRows = new Vector();
		m_nCurPos = 0;
	}

	public boolean isEnd()
	{
		return m_nCurPos >= m_arRows.size();
	}

	public void next() throws DBException
	{
		m_nCurPos++;
	}

	public int getColCount()
	{
		return m_arColumns.size();
	}

	public String getColName(int nCol)
	{
		return (String)m_arColumns.elementAt(nCol);
	}

	public String getOrigColName(int nCol)
	{
		return (String)m_arColumns.elementAt(nCol);
	}

	public Object[] getCurData() throws DBException
	{
		return (Object[])m_arRows.elementAt(m_nCurPos);
	}

	public boolean isNonUnique() {
		// TODO Auto-generated method stub
		return false;
	}

    protected Object getCurValue(int nCol)
    {
    	return ((Object[])m_arRows.elementAt(m_nCurPos))[nCol];
    }
/*
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
*/
	int findColIndex(String colname )
	{
		for( int i = 0; i < m_arColumns.size(); i++ )
		{
			if ( ((String)m_arColumns.elementAt(i)).equalsIgnoreCase(colname) )
				return i;
		}

		return -1;
	}

}
