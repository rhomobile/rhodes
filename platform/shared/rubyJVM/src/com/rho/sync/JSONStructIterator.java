package com.rho.sync;

import org.json.me.JSONException;
import org.json.me.JSONObject;
import java.util.Enumeration;
class JSONStructIterator
{
	JSONObject m_object;
	Enumeration m_enumKeys;
	String m_strCurKey;
	
    JSONStructIterator(String szData)throws JSONException
    {
    	m_object = new JSONObject(szData);
    	m_enumKeys = m_object.keys();
    	if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		m_strCurKey = (String)m_enumKeys.nextElement();
    }

    JSONStructIterator(JSONEntry oEntry, String strName)throws JSONException
    {
    	m_object = (JSONObject)oEntry.m_object.get(strName);
    	m_enumKeys = m_object.keys();
    	if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		m_strCurKey = (String)m_enumKeys.nextElement();
    }

    JSONStructIterator(JSONEntry oEntry)
    {
    	m_object = oEntry.m_object;
    	m_enumKeys = m_object.keys();
    	if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		m_strCurKey = (String)m_enumKeys.nextElement();
    }

    boolean isEnd()
    {
        return m_strCurKey == null;
    }

    void  next()
    {
    	if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		m_strCurKey = (String)m_enumKeys.nextElement();
    	else
    		m_strCurKey = null;
    }

    void reset()
    {
    	m_enumKeys = m_object.keys();
    	if ( !isEnd() )
    		m_strCurKey = (String)m_enumKeys.nextElement();
    }

    String getCurKey()
    {
        return isEnd() ? new String() : m_strCurKey;
    }

	String getCurString()throws JSONException
	{
		return m_object.getString(m_strCurKey);
	}
    
    JSONEntry getCurValue()throws JSONException
    {
    	if ( isEnd() )
    		return new JSONEntry( (JSONObject)null );
    	
		return new JSONEntry( (JSONObject)m_object.getJSONObject(m_strCurKey) );
    }

}
