package com.rho.sync;
import org.json.me.JSONException;
import org.json.me.JSONObject;

class JSONEntry
{
    JSONObject m_object;
	
	JSONEntry(JSONObject obj)
	{
	    m_object = obj;
	}

	JSONEntry(String szData)throws JSONException
	{
	    m_object = new JSONObject(szData);
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
		return JSONObject.quote(str);
	}
	
	String getString(String name)throws JSONException
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
	
	int getInt(String name)throws JSONException
	{
	    int nRes = 0;
	    if ( m_object.has(name))
	    	nRes = m_object.getInt(name);	    	
	
	    return nRes;
	}

	long getLong(String name)throws JSONException
	{
	    long nRes = 0;
	    if ( m_object.has(name))
	    	nRes = m_object.getLong(name);	    	
	
	    return nRes;
	}
	
	long getUInt64(String name)throws JSONException
	{
	    return getLong(name);
	}

	JSONEntry getEntry(String name)throws JSONException
	{
	    return new JSONEntry((JSONObject) m_object.get(name));
	}
}