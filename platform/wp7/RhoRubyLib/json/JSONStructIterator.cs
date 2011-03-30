using System;
using System.Collections.Generic;
using rho.common;
using fastJSON;

namespace rho.json
{
    public class JSONStructIterator
    {
        Dictionary<string, object> m_object;
	    Dictionary<string, object>.Enumerator m_enumStruct;
	    String m_strCurKey = null;
	
        public JSONStructIterator(String szData)
        {
            m_object = (Dictionary<string, object>)JsonParser.JsonDecode(szData);

            m_enumStruct = m_object.GetEnumerator();
            if ( m_enumStruct.MoveNext() )
                m_strCurKey = m_enumStruct.Current.Key;
        }

        public JSONStructIterator(JSONEntry oEntry, String strName)
        {
    	    m_object = (Dictionary<string, object>)oEntry.getObject(strName);
            m_enumStruct = m_object.GetEnumerator();
            if (m_enumStruct.MoveNext())
                m_strCurKey = m_enumStruct.Current.Key;
        }

        public JSONStructIterator(JSONEntry oEntry)
        {
    	    m_object = oEntry.getObject();
            m_enumStruct = m_object.GetEnumerator();
            if (m_enumStruct.MoveNext())
                m_strCurKey = m_enumStruct.Current.Key;
        }

        public boolean isEnd()
        {
            return m_strCurKey == null;
        }

        public void next()
        {
            if (m_enumStruct.MoveNext())
                m_strCurKey = m_enumStruct.Current.Key;
            else
                m_strCurKey = null;
        }

        public void reset()
        {
            m_enumStruct = m_object.GetEnumerator();
            if (m_enumStruct.MoveNext())
                m_strCurKey = m_enumStruct.Current.Key;
        }

        public String getCurKey()
        {
            return isEnd() ? String.Empty : m_strCurKey;
        }

        public String getCurString()
	    {
            return m_enumStruct.Current.Value.ToString();
	    }

        public JSONEntry getCurValue()
        {
    	    if ( isEnd() )
                return new JSONEntry((Dictionary<string, object>)null);

            return new JSONEntry((Dictionary<string, object>)m_enumStruct.Current.Value);
        }
    }
}
