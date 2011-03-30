using System;
using rho.common;

namespace rho.db
{
    public interface IDBResult : IDisposable
    {
    int getColCount();
	String getColName(int nCol);
	String getOrigColName(int nCol);
	
	//New
    boolean isEnd();
    void next();
	String getStringByIdx(int nCol);
	int getIntByIdx(int nCol);
	long getLongByIdx(int nCol);
	String getUInt64ByIdx(int nCol);

	Object /*RubyValue*/ getRubyValueByIdx(int nCol);
	boolean isNullByIdx(int nCol);
	
	Object /*RubyValue*/ getRubyValue(String colname);
	int getInt(String colname);
	String getString(String colname);
	
	Object[] getCurData();
	
    boolean isNonUnique();
    
    void close();
    }
}
