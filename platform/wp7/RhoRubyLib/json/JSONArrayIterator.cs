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
