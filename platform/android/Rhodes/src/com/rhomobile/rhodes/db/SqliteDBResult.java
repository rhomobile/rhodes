package com.rhomobile.rhodes.db;

import java.util.ArrayList;
import java.util.List;

import android.database.Cursor;

import com.rho.db.IDBResult;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.lang.RubyValue;

public class SqliteDBResult implements IDBResult {

	//private static RWLock lock = new RWLock();
	private Cursor cursor;
	private int curIndex = 0;
	private boolean useLocalCursor = false;
	private List< List<String> > localDataCopy = new ArrayList< List<String> >();
	private List<String> cashedColumnsNames =  new ArrayList< String > ();
	private boolean nonUnique = false;
	
	public SqliteDBResult() {
		//lock.getWriteLock();
	}
	
	public SqliteDBResult(boolean bNonUnique) {
		nonUnique = bNonUnique;
	}

    public boolean isNonUnique()
    {
        return nonUnique;
    }

	public void assign(Cursor cursor) {
		this.cursor = cursor;
		localDataCopy = null;
		cashedColumnsNames = null;
		useLocalCursor = false;
		
		reset();
		
		//if ( this.cursor == null ) //nothing to lock
		//	lock.releaseLock();
	}
	
	public void copy(Cursor cursor) {
		//copy data
		this.cursor = cursor;
		reset();
		
		if ( this.cursor != null )
		{
			try { 
				localDataCopy = new ArrayList< List<String> >();
				cashedColumnsNames = new ArrayList<String>();
				
				for ( int j = 0; j < getColCount(); j++ )
				{
					cashedColumnsNames.add(getColName(j));
				}
				
				for ( int i = 0; i < getCount(); i++ )
				{
					List<String> row = new ArrayList<String>();
					for ( int j = 0; j < getColCount(); j++ )
					{
						row.add(getStringByIdx(i, j));
					}
					
					localDataCopy.add(row);
				}
			} finally {
				this.cursor.close();
			}
			
			this.cursor = null;
			curIndex = 0;
			useLocalCursor = true;
		}
		
		//if ( this.cursor == null ) //nothing to lock
		//	lock.releaseLock();
	}

	/*public void close() {

		if (this.cursor != null)
			this.cursor.close();

		//lock.releaseLock();
	}*/

	public int getColCount() {
		if (this.cursor != null) {
			return this.cursor.getColumnCount();
		} else if ( useLocalCursor ) {
			return cashedColumnsNames.size();
		}
		return 0;
	}

	public String getColName(int col) {

		if ( useLocalCursor ) {
			if ( col >= 0 && col < cashedColumnsNames.size() )
				return cashedColumnsNames.get(col);
			else
				return null;
		}else if (col >= 0 && col < getColCount()) {
			return this.cursor.getColumnName(col);
		}
		
		return null;
	}

	public int getCount() {

		if ( useLocalCursor ) {
			return localDataCopy.size();
		} else 	if (this.cursor != null) {
			return this.cursor.getCount();
		}
		
		return 0;
	}

	public int getInt(int item, String colname) {
		return getIntByIdx(item, findColIndex(colname) );
	}

	public int getIntByIdx(int item, int col) {
		String val = getValueByIdx(item,col);
		return val != null ? Integer.parseInt(val) : 0; 
	}

	public long getLong(int item, String colname) {
		return getLongByIdx(item, findColIndex(colname) );
	}

	public long getLongByIdx(int item, int col) {
		String val = getValueByIdx(item,col);
		return val != null ? Long.parseLong(val) : 0;
	}

	public RubyValue getRubyValue(int item, String colname) {
		return getRubyValueByIdx( item, findColIndex(colname));
	}

	public RubyValue getRubyValueByIdx(int item, int col) {
		String val = getValueByIdx(item,col);
		return ObjectFactory.createString( val != null ? val : "");
	}

	public String getString(int item, String colname) {
		return getStringByIdx(item, findColIndex(colname));
	}

	public String getStringByIdx(int item, int col) {
		String val = getValueByIdx(item,col);
		return val != null ? val : "";
	}

	private String getValueByIdx(int item, int col) {
		if (col < 0 || col >= getColCount())
			throw new IndexOutOfBoundsException(
					"SqliteDBResult.getStringByIdx : " + col + ". Count : "
							+ getColCount());

		if (moveToPosition(item))
		{
			if ( cursor != null )
				return cursor.getString(col);
			else if ( useLocalCursor )
				return this.localDataCopy.get(item).get(col);
		}
		return null;
	}

	private void reset() {
		if (this.cursor != null) {
			cursor.moveToFirst();
			curIndex = 0;
		} else {
			curIndex = 0;
		}
	}

	private boolean moveToPosition(int nItem) {
		if (curIndex == -1 || nItem < 0)
			return false;

		if (curIndex != nItem) {
			curIndex = nItem;
			
			if ( cursor != null )
				return cursor.moveToPosition(nItem);
			else 
				return useLocalCursor;
		}

		return true;
	}
	
	private int findColIndex(String colname )
	{
		if ( cursor != null)
			return cursor.getColumnIndex(colname);
		else if ( useLocalCursor ) {
			
			for ( int i = 0; i < getColCount(); i++ ){
				if ( cashedColumnsNames.get(i).equalsIgnoreCase(colname) )
					return i;
			}
		}
		
		return -1;
	}
	
	//New
    public boolean isEnd()
    {
    	return curIndex >= getCount();
    }
    
    public void next()
    {
    	moveToPosition(curIndex+1);
    }

	public String getStringByIdx(int col) {
		String val = getValueByIdx(curIndex,col);
		return val != null ? val : "";
	}

	public int getIntByIdx(int col) {
		String val = getValueByIdx(curIndex,col);
		return val != null ? Integer.parseInt(val) : 0; 
	}
	
	public long getLongByIdx(int col) {
		String val = getValueByIdx(curIndex,col);
		return val != null ? Long.parseLong(val) : 0;
	}
	
	public String getUInt64ByIdx(int nCol)
	{
		return getStringByIdx(nCol);
	}

	public RubyValue getRubyValueByIdx(int col) {
		return getRubyValueByIdx(curIndex, col);
	}
	
	public RubyValue getRubyValue(String colname){ return getRubyValueByIdx( findColIndex(colname) ); }
	public int getInt(String colname){ return getIntByIdx( findColIndex(colname) ); }
	public String getString(String colname){ return getStringByIdx(findColIndex(colname) ); }
	
	public Object[] getCurData()
	{
		List<String> row = localDataCopy.get(curIndex);
		Object[] res = new Object[row.size()];
		for( int i = 0; i < row.size(); i++ )
			res[i] = row.get(i);
		
		return res;
	}
}
