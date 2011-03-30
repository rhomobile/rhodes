using System;
using System.Collections.Generic;
using rho.common;
using fastJSON;

namespace rho.json
{
    public class JSONArrayIterator
    {
        List<Object> m_array;
        int    m_nCurItem;
	
	    public JSONArrayIterator(String szData)
	    {
            m_array = (List<Object>)JsonParser.JsonDecode(szData);
	        m_nCurItem = 0;
	    }

	    public JSONArrayIterator(JSONEntry oEntry, String strName)
	    {
	        m_array = (List<Object>)oEntry.getObject(strName);
	        m_nCurItem = 0;
	    }

	    private JSONArrayIterator()
	    {
		    m_array = null;
		    m_nCurItem = 0;
	    }

        public boolean isEnd()
	    {
	        return !(m_array != null && m_nCurItem < m_array.Count);
	    }

        public void next()
	    {
	        m_nCurItem++;
	    }

        public void reset(int nPos)
        {
    	    m_nCurItem = nPos;
        }

        public int getCurPos() { return m_nCurItem; }

        public JSONEntry getCurItem()
	    {
            return new JSONEntry(isEnd() ? null : (Dictionary<string, object>)m_array[m_nCurItem]);
	    }

        public JSONArrayIterator getCurArrayIter()
	    {
		    JSONArrayIterator res = new JSONArrayIterator();
		    if ( isEnd() )
			    return res;

            res.m_array = (List<Object>)m_array[m_nCurItem];
	        return res;
	    }
    }
}
