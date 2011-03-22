using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Community.CsharpSqlite;
using rho.common;

namespace rho.db
{
    public class CSqliteResult : IDBResult
    {

        public CSqliteResult()
        {
        }

        public CSqliteResult(Sqlite3.Vdbe stmt)
        {
        }

        public int getColCount()
        {
            return 1;
        }

        public String getColName(int nCol)
        {
            return null;
        }

        public String getOrigColName(int nCol)
        {
            return null;
        }

        //New
        public boolean isEnd()
        {
            return true;
        }

        public boolean isOneEnd()
        {
            return true;
        }

        public void next()
        {
        }

        public String getStringByIdx(int nCol)
        {
            return null;
        }

        public int getIntByIdx(int nCol)
        {
            return 1;
        }

        public long getLongByIdx(int nCol)
        {
            return 1;
        }

        public String getUInt64ByIdx(int nCol)
        {
            return null;
        }

        public Object /*RubyValue*/ getRubyValueByIdx(int nCol)
        {
            return null;
        }

        public boolean isNullByIdx(int nCol)
        {
            return true;
        }

        public Object /*RubyValue*/ getRubyValue(String colname)
        {
            return null;
        }

        public int getInt(String colname)
        {
            return 1;
        }

        public String getString(String colname)
        {
            return null;
        }

        public Object[] getCurData()
        {
            return null;
        }

        public boolean isNonUnique()
        {
            return true;
        }

        public void close()
        {
        }
    }
}
