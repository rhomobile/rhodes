package com.rho.sync;
import org.json.me.RhoJSONException;
import org.json.me.RhoJSONObject;

class JSONEntry
{
    RhoJSONObject m_object;
	
	JSONEntry(RhoJSONObject obj)
	{
	    m_object = obj;
	}

	JSONEntry(String szData)throws RhoJSONException
	{
	    m_object = new RhoJSONObject(szData);
	}

	boolean isEmpty()
	{
	    return m_object == null;
	}

	boolean hasName(String name)
	{
		return m_object.has(name);
	}
	
	static String quoteValue(String str)
	{
		return RhoJSONObject.quote(str);
	}
	
	String getString(String name)throws RhoJSONException
	{
	    String szRes = null;
	    if ( m_object.has(name))
	    	szRes = m_object.getString(name);	    	
	
	    return szRes;
	}

	String getString()
	{
		return m_object.toString();
	}
	
	int getInt(String name)throws RhoJSONException
	{
	    int nRes = 0;
	    if ( m_object.has(name))
	    	nRes = m_object.getInt(name);	    	
	
	    return nRes;
	}

	long getLong(String name)throws RhoJSONException
	{
	    long nRes = 0;
	    if ( m_object.has(name))
	    	nRes = m_object.getLong(name);	    	
	
	    return nRes;
	}
	
	long getUInt64(String name)throws RhoJSONException
	{
	    return getLong(name);
	}

	JSONEntry getEntry(String name)throws RhoJSONException
	{
	    return new JSONEntry((RhoJSONObject) m_object.get(name));
	}
}