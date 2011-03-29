using System;
using rho.common;

namespace rho.json
{
    public class JSONStructIterator
    {
        //object m_object;
	    //Enumeration m_enumKeys;
	    //String m_strCurKey;
	
        public JSONStructIterator(String szData)
        {
    	    /*m_object = new RhoJSONObject(szData);
    	    m_enumKeys = m_object.keys();
    	    if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		    m_strCurKey = (String)m_enumKeys.nextElement(); */
        }

        public JSONStructIterator(JSONEntry oEntry, String strName)
        {
    	    /*m_object = (RhoJSONObject)oEntry.m_object.get(strName);
    	    m_enumKeys = m_object.keys();
    	    if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		    m_strCurKey = (String)m_enumKeys.nextElement(); */
        }

        public JSONStructIterator(JSONEntry oEntry)
        {
    	    /*m_object = oEntry.m_object;
    	    m_enumKeys = m_object.keys();
    	    if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		    m_strCurKey = (String)m_enumKeys.nextElement();*/
        }

        public boolean isEnd()
        {
            return false;// m_strCurKey == null;
        }

        public void next()
        {
    	   /* if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		    m_strCurKey = (String)m_enumKeys.nextElement();
    	    else
    		    m_strCurKey = null;*/
        }

        public void reset()
        {
    	    /*m_enumKeys = m_object.keys();
    	    if ( !isEnd() )
    		    m_strCurKey = (String)m_enumKeys.nextElement();*/
        }

        public String getCurKey()
        {
            return "";//isEnd() ? new String() : m_strCurKey;
        }

        public String getCurString()
	    {
		    return "";//m_object.getString(m_strCurKey);
	    }

        public JSONEntry getCurValue()
        {
    	    /*if ( isEnd() )
    		    return new JSONEntry( (RhoJSONObject)null );
    	
		    return new JSONEntry( (RhoJSONObject)m_object.getJSONObject(m_strCurKey) ); */

            return null;
        }
    }
}
