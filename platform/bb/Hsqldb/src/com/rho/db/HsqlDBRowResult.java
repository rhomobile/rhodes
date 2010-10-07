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
