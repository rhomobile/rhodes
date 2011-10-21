/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
