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
using rho.common;

namespace rho.db
{
    public class CSqliteCopyResult
    {
        private Vector<Object[]> m_arRows = new Vector<Object[]>();
	    private int m_nCurPos = 0;
	    private Vector<String> m_arColumns = new Vector<String>();

	    public CSqliteCopyResult(IDBResult res)
	    {
		    for( int i = 0; i < res.getColCount(); i++)
			    m_arColumns.addElement(res.getColName(i));

		    for( ; !res.isEnd(); res.next() )
		    {
			    m_arRows.addElement(res.getCurData());
		    }
	    }

        public void close()
	    {
		    m_arRows = new Vector<Object[]>();
		    m_nCurPos = 0;
	    }

	    public boolean isEnd()
	    {
		    return m_nCurPos >= m_arRows.size();
	    }

	    public void next()
	    {
		    m_nCurPos++;
	    }

	    public int getColCount()
	    {
		    return m_arColumns.size();
	    }

	    public String getColName(int nCol)
	    {
		    return (String)m_arColumns.elementAt(nCol);
	    }

	    public String getOrigColName(int nCol)
	    {
		    return (String)m_arColumns.elementAt(nCol);
    	}

	    public Object[] getCurData()
	    {
		    return (Object[])m_arRows.elementAt(m_nCurPos);
	    }

	    public boolean isNonUnique() {
		    return false;
	    }

        public int findColIndex(String colname)
        {
            for (int i = 0; i < m_arColumns.size(); i++)
            {
                if (((String)m_arColumns.elementAt(i)).equalsIgnoreCase(colname))
                    return i;
            }

            return -1;
        }

        public Object getCurValue(int nCol)
        {
    	    return ((Object[])m_arRows.elementAt(m_nCurPos))[nCol];
        }
    }
}
