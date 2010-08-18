package com.rho.sync;

import org.json.me.RhoJSONArray;
import org.json.me.RhoJSONException;
import org.json.me.RhoJSONObject;

class JSONArrayIterator
{
    RhoJSONArray m_array;
    int    m_nCurItem;
	
	JSONArrayIterator(String szData)throws RhoJSONException
	{
	    m_array = new RhoJSONArray(szData);
	    m_nCurItem = 0;
	}

	JSONArrayIterator(JSONEntry oEntry, String strName)throws RhoJSONException
	{
	    m_array = (RhoJSONArray)oEntry.m_object.get(strName);
	    m_nCurItem = 0;
	}

	private JSONArrayIterator()
	{
		m_array = null;
		m_nCurItem = 0;
	}
	
	boolean isEnd()
	{
	    return !(m_array != null && m_nCurItem < m_array.length());
	}

	void  next()
	{
	    m_nCurItem++;
	}

    void    reset(int nPos)
    {
    	m_nCurItem = nPos;
    }
    
    int     getCurPos(){ return m_nCurItem; }
	
	JSONEntry getCurItem()throws RhoJSONException
	{
	    return new JSONEntry( isEnd() ? null : (RhoJSONObject) m_array.get(m_nCurItem) );
	}
	
	JSONArrayIterator getCurArrayIter()throws RhoJSONException
	{
		JSONArrayIterator res = new JSONArrayIterator();
		if ( isEnd() )
			return res;
		
		res.m_array = (RhoJSONArray)m_array.get(m_nCurItem);
	    return res;
	}
	
}
