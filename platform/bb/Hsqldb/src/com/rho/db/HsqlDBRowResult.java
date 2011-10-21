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

import org.hsqldb.Row;
import org.hsqldb.Table;
import org.hsqldb.Column;

public class HsqlDBRowResult extends HsqlDBResult {

	Table m_table;
	Row   m_row;

	HsqlDBRowResult(){}

	void init( Table table, Row row ){
		m_table = table;
		m_row = row;
	}

	public int getCount(){
		return m_row != null ? 1 : 0;
	}

	public int getColCount(){
		return m_table != null ? m_table.getColumnCount() : 0;
	}

	public String getColName(int nCol){
		Object col = m_table.columnList.get(nCol);
		return ((Column)col).columnName.name;//.toLowerCase();
	}

	//Object[] getItem(int nItem){
	//	if ( m_row == null || nItem < 0 || nItem >= getCount() )
	//		return null;

	//	return m_row.getData();
	//}

    public boolean isEnd()
    {
    	return m_row == null;
    }

    public void next()
    {
    	m_row = null;
    }

    public Object[] getCurData()
    {
    	return m_row.getData();
    }

    protected Object getCurValue(int nCol)
    {
    	return m_row.getData()[nCol];
    }

	int findColIndex(String colname )
	{
		for( int i = 0; i < getColCount(); i++ )
		{
			Object col = m_table.columnList.get(i);
			if ( ((Column)col).columnName.name.equalsIgnoreCase(colname) )
				return i;
		}

		return -1;
	}


}
