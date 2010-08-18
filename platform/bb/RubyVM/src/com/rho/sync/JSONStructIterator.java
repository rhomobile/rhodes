package com.rho.sync;

import org.json.me.RhoJSONException;
import org.json.me.RhoJSONObject;
import java.util.Enumeration;
class JSONStructIterator
{
	RhoJSONObject m_object;
	Enumeration m_enumKeys;
	String m_strCurKey;
	
    JSONStructIterator(String szData)throws RhoJSONException
    {
    	m_object = new RhoJSONObject(szData);
    	m_enumKeys = m_object.keys();
    	if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		m_strCurKey = (String)m_enumKeys.nextElement();
    }

    JSONStructIterator(JSONEntry oEntry, String strName)throws RhoJSONException
    {
    	m_object = (RhoJSONObject)oEntry.m_object.get(strName);
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

	String getCurString()throws RhoJSONException
	{
		return m_object.getString(m_strCurKey);
	}
    
    JSONEntry getCurValue()throws RhoJSONException
    {
    	if ( isEnd() )
    		return new JSONEntry( (RhoJSONObject)null );
    	
		return new JSONEntry( (RhoJSONObject)m_object.getJSONObject(m_strCurKey) );
    }

}
