using System;
using rho.common;

namespace rho.json
{
    class RhoJSONArray
    {
        public RhoJSONArray(String data){}
        public object get(String strName){ return null; }
        public int length(){ return 0; }
    }

    public class JSONArrayIterator
    {
        RhoJSONArray m_array;
        int    m_nCurItem;
	
	    public JSONArrayIterator(String szData)
	    {
	        m_array = new RhoJSONArray(szData);
	        m_nCurItem = 0;
	    }

	    public JSONArrayIterator(JSONEntry oEntry, String strName)
	    {
	        //m_array = (RhoJSONArray)oEntry.m_object.get(strName);
	        m_nCurItem = 0;
	    }

	    private JSONArrayIterator()
	    {
		    m_array = null;
		    m_nCurItem = 0;
	    }

        public boolean isEnd()
	    {
	        return !(m_array != null && m_nCurItem < m_array.length());
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
	        return null;//new JSONEntry( isEnd() ? null : (RhoJSONObject) m_array.get(m_nCurItem) );
	    }

        public JSONArrayIterator getCurArrayIter()
	    {
		    JSONArrayIterator res = new JSONArrayIterator();
		    if ( isEnd() )
			    return res;
		
		    //res.m_array = (RhoJSONArray)m_array.get(m_nCurItem);
	        return res;
	    }
    }
}
