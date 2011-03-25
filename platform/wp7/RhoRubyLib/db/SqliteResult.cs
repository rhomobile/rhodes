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
        //private SqliteCopyResult m_resCopy;
        private boolean m_bNonUnique = false;
        public CSqliteResult()
        {
        }

        public CSqliteResult(Sqlite3.Vdbe stmt)
        {
            m_st = stmt;
            if (m_st != null)
                executeStatement(stmt);
        }

        public int getColCount()
        {
            return Sqlite3.sqlite3_data_count(m_st);
        }

        public String getColName(int nCol)
        {
            return Sqlite3.sqlite3_column_name(m_st, nCol);
        }

        public String getOrigColName(int nCol)
        {
            return Sqlite3.sqlite3_column_name(m_st, nCol);
        }

        //New
        public boolean isEnd()
        {
            return m_st == null;
        }

        public boolean isOneEnd()
        {
            return m_st == null;
        }

        public void next()
        {
            if (m_st != null)
                executeStatement(m_st);
        }

        public String getStringByIdx(int nCol)
        {
            String val = Sqlite3.sqlite3_column_text(m_st, nCol);
            return val != null ? val : String.Empty;
        }

        public int getIntByIdx(int nCol)
        {
            return Sqlite3.sqlite3_column_int(m_st, nCol);
        }

        public long getLongByIdx(int nCol)
        {
            return Sqlite3.sqlite3_column_int64(m_st, nCol);
        }

        public String getUInt64ByIdx(int nCol)
        {
            return getStringByIdx(nCol);
        }

        public Object /*RubyValue*/ getRubyValueByIdx(int nCol)
        {
            Object val = getCurData()[nCol];
            String d = val.ToString();
            return MutableString.Create(val.ToString());
        }

        public boolean isNullByIdx(int nCol)
        {
            return Sqlite3.sqlite3_column_type(m_st, nCol) == Sqlite3.SQLITE_NULL;
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
            String[] cols = getColumnNames();
            Object[] res = new Object[cols.Length];
            for (int i = 0; i < cols.Length; i++)
            {
                Sqlite3.Mem val = Sqlite3.sqlite3_column_value(m_st, i);
                switch (val.type)
                {
                    case Sqlite3.SQLITE_NULL:
                        res[i] = null;
                        break;
                    case Sqlite3.SQLITE_INTEGER:
                        res[i] = val.r;
                        break;
                    case Sqlite3.SQLITE_FLOAT:
                        res[i] = val.r;
                        break;
                    case Sqlite3.SQLITE_TEXT:
                        res[i] = val.z;
                        break;
                    case Sqlite3.SQLITE_BLOB:
                        res[i] = val.zBLOB;
                        break;
                    default:
                        break;
                }
            }

            return res;
        }

        public boolean isNonUnique()
        {
            return m_bNonUnique;
        }

        public void close()
        {
            if (m_st != null)
            {
                int res = Sqlite3.sqlite3_finalize( ref m_st );
                if (res != Sqlite3.SQLITE_OK)
                    throw new DBException(res, DBLastError());

                m_st = null;
            }
        }

        #region Helpers

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

        private string DBLastError()
        {
            return Sqlite3.sqlite3_errmsg(m_st.db);
        }

        private bool executeStatement(Sqlite3.Vdbe pStmt)
        {
            int res = Sqlite3.sqlite3_step(pStmt);

            if (res != Sqlite3.SQLITE_ROW)
            {
                close();
                if (res != Sqlite3.SQLITE_OK && res != Sqlite3.SQLITE_ROW && res != Sqlite3.SQLITE_DONE)
                    throw new DBException(res, DBLastError());
            }

            return res == Sqlite3.SQLITE_ROW;
        }

        private int findColIndex(String colname)
        {
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
