using System;
using System.Net;
using System.Windows.Shapes;
using Community.CsharpSqlite;
using System.Collections;
using IronRuby.Runtime;
using IronRuby.Builtins;
using rho.common;

namespace rho.db
{
    public class CSqliteResult : IDBResult
    {
        private Sqlite3.Vdbe m_st;
        private CSqliteCopyResult m_resCopy;
        private boolean m_bNonUnique = false;
        private boolean m_bNoCopy = false;

        public CSqliteResult()
        {
        }

        public CSqliteResult(boolean bNonUnique)
	    {
		    m_bNonUnique = bNonUnique;
	    }

        public CSqliteResult(Sqlite3.Vdbe stmt, boolean bNoCopy)
        {
            m_bNoCopy = bNoCopy;
            m_st = stmt;
        }

        public boolean isNonUnique()
        {
            return m_bNonUnique;
        }

        public int getColCount()
        {
            if (m_resCopy != null)
                return m_resCopy.getColCount();

            return Sqlite3.sqlite3_data_count(m_st);
        }

        public String getColName(int nCol)
        {
            if (m_resCopy != null)
                return m_resCopy.getColName(nCol);

            return Sqlite3.sqlite3_column_name(m_st, nCol);
        }

        public String getOrigColName(int nCol)
        {
            if (m_resCopy != null)
                return m_resCopy.getOrigColName(nCol);

            return Sqlite3.sqlite3_column_name(m_st, nCol);
        }

        public boolean isEnd()
        {
            if (m_resCopy != null)
                return m_resCopy.isEnd();

            return m_st == null;
        }

        public void next()
        {
            if (m_resCopy != null)
                m_resCopy.next();
            else  if (m_st != null)
            {
                int res = executeStatement();
                checkError();
                if (res != Sqlite3.SQLITE_ROW)
                    close();
            }
        }

        public void checkError()
        {
            if (m_st == null)
                return;

            int res = Sqlite3.sqlite3_errcode(m_st.db);
            if (res != Sqlite3.SQLITE_OK && res != Sqlite3.SQLITE_ROW && res != Sqlite3.SQLITE_DONE)
            {
                close();
                throw new DBException(res, Sqlite3.sqlite3_errmsg(m_st.db));
            }
        }

        public int executeStatement()
        {
            int rc = Sqlite3.sqlite3_step(m_st);

            if (rc == Sqlite3.SQLITE_ROW && !m_bNoCopy)
            {
                m_resCopy = new CSqliteCopyResult(this);
                close();
            }

            return rc;
        }

        public void Dispose()
        {
            close();
        }

        public void close()
        {
            if (m_st != null)
            {
                Sqlite3.sqlite3_finalize(ref m_st);
                m_st = null;
            }
        }

        public String getStringByIdx(int nCol)
        {
            Object val = getCurValue(nCol);
            return val != null ? val.ToString() : ""; 
        }

        public int getIntByIdx(int nCol)
        {
            Object val = getCurValue(nCol);
            return val != null ? Convert.ToInt32(val) : 0; 
        }

        public long getLongByIdx(int nCol)
        {
            Object val = getCurValue(nCol);
            return val != null ? (long)val : 0; 
        }

        public String getUInt64ByIdx(int nCol)
        {
            return getStringByIdx(nCol);
        }

        public Object /*RubyValue*/ getRubyValueByIdx(int nCol)
        {
            Object val = getCurValue(nCol);

            if (val == null)
			    return null;
		    else if ( val is string || val is String)
                return CRhoRuby.Instance.createString((String)val);
		    else if ( val is int )
			    return (int)val;
		    else if ( val is short )
			    return (short)val;
		    else if ( val is long)
			    return (long)val;
		    else if ( val is float )
			    return (float)val;
		    else if ( val is double)
			    return (double)val;
		    else if (val is byte[])
                return CRhoRuby.Instance.createString(((byte[])val).ToString());

            return CRhoRuby.Instance.createString(val.ToString());
        }

        public boolean isNullByIdx(int nCol)
        {
            Object val = getCurValue(nCol);
            return val == null; 
        }

        public Object /*RubyValue*/ getRubyValue(String colname)
        {
            return getRubyValueByIdx( findColIndex(colname) );
        }

        public int getInt(String colname)
        {
            return getIntByIdx(findColIndex(colname));
        }

        public String getString(String colname)
        {
            return getStringByIdx(findColIndex(colname));
        }

        public Object[] getCurData()
        {
            if (m_resCopy != null)
                return m_resCopy.getCurData();
            
            String[] cols = getColumnNames();
            Object[] res = new Object[cols.Length];
            for (int i = 0; i < cols.Length; i++)
            {
                int type = Sqlite3.sqlite3_column_type(m_st, i);
                switch (type)
                {
                    case Sqlite3.SQLITE_INTEGER:
                        res[i] = Sqlite3.sqlite3_column_int64(m_st, i);
                        break;
                    case Sqlite3.SQLITE_FLOAT:
                        res[i] = Sqlite3.sqlite3_column_double(m_st, i);
                        break;
                    case Sqlite3.SQLITE_BLOB:
                        res[i] = Sqlite3.sqlite3_column_blob(m_st, i);
                        break;
                    case Sqlite3.SQLITE_TEXT:
                        res[i] = Sqlite3.sqlite3_column_text(m_st, i);
                        if (res[i] == null)
                            res[i] = String.Empty;
                        break;
                    case Sqlite3.SQLITE_NULL:
                        res[i] = null;
                        break;
                }
            }

            return res;
        }

        #region Helpers

        private Object getCurValue(int nCol)
        {
            if (m_resCopy != null)
                return m_resCopy.getCurValue(nCol);

            return getCurData()[nCol];
        }

        private String[] getColumnNames()
        {
            int cnt = Sqlite3.sqlite3_data_count(m_st);
            String[] res = new String[cnt];
            for (int i = 0; i < cnt; i++)
            {
                res[i] = Sqlite3.sqlite3_column_name(m_st, i);
            }

            return res;
        }

        private int findColIndex(String colname)
        {
            if (m_resCopy != null)
                return m_resCopy.findColIndex(colname);
            
            int cnt = Sqlite3.sqlite3_data_count(m_st);

            for (int i = 0; i < cnt; i++)
            {
                if (colname == Sqlite3.sqlite3_column_name(m_st, i))
                    return i;
            }

            return - 1;
        }

        #endregion
    }
}
