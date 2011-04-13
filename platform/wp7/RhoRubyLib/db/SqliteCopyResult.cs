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
