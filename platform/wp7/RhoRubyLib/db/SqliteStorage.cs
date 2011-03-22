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
    public class SqliteStorage : IDBStorage, IDisposable
    {
        private Sqlite3.sqlite3 _db;
        private bool _open;
        public string callbackError;
        public bool TransactionOpened = false;

        public void close()
        {
            if (_open)
            {
                Sqlite3.sqlite3_close(_db);
                _db = null;
                _open = false;
            }
        }

        public void commit()
        {
            int n = Sqlite3.sqlite3_exec(_db, "COMMIT", 0, 0, 0);
            if (n != Sqlite3.SQLITE_OK)
                throw new DBException(n, DBLastError());
            TransactionOpened = false;
        }

        public IDBResult createResult()
        {
            IDBResult res = null;



            return res;
        }

        public void deleteAllFiles(String strPath)
        {
        }

        public void Dispose()
        {
            close();
        }

        public void executeBatchSQL(String strStatement)
        {
            var cmd = CreateCommand(strStatement);
            Console.Error.WriteLine("Executing " + cmd);
            cmd.ExecuteNonQuery();
        }

        public IDBResult executeSQL(String strStatement, Object[] values, boolean bReportNonUnique)
        {
            IDBResult res = null;

            var cmd = CreateCommand(strStatement, values);
            Console.Error.WriteLine("Executing " + cmd);
            cmd.ExecuteNonQuery();

            return res;
        }

        public void open(String strPath, String strSqlScript, String strEncryptionInfo)
        {
            int n = Sqlite3.sqlite3_open(strPath, ref _db);
            if (n != Sqlite3.SQLITE_OK)
                throw new DBException(n, "Could not open database file: " + strPath);
            string errMsg = string.Empty;
            n = Sqlite3.sqlite3_exec(_db, "PRAGMA journal_mode=PERSIST", (Sqlite3.dxCallback)this.callback, null, ref errMsg);
            if (n != Sqlite3.SQLITE_OK)
            {
                Sqlite3.sqlite3_close(_db);
                _db = null;
                _open = false;
                throw new DBException(n, "Cannot set journal mode to PERSIST: " + strPath);
            }
            _open = true;
        }

        public void rollback()
        {
            int n = Sqlite3.sqlite3_exec(_db, "ROLLBACK", 0, 0, 0);
            if (n != Sqlite3.SQLITE_OK)
                throw new DBException(n, DBLastError());
            TransactionOpened = false;
        }

        void setDbCallback(IDBCallback callback)
        {
        }

        public void startTransaction()
        {
            int n = Sqlite3.sqlite3_exec(_db, "BEGIN", 0, 0, 0);
            if (n != Sqlite3.SQLITE_OK)
                throw new DBException(n, DBLastError());
            TransactionOpened = true;
        }

        #region Helpers

        private int callback(object pArg, System.Int64 nArg, object azArgs, object azCols)
        {
            int i;
            string[] azArg = (string[])azArgs;
            string[] azCol = (string[])azCols;
            String sb = "";// = new String();
            for (i = 0; i < nArg; i++)
                sb += azCol[i] + " = " + azArg[i] + "\n";
            callbackError += sb.ToString();
            return 0;
        }

        private DBCommand CreateCommand(string cmdText, params object[] ps)
        {
            if (!_open)
            {
                throw new DBException(Sqlite3.SQLITE_ERROR, "Cannot create commands from unopened database");
            }
            else
            {
                var cmd = new DBCommand(_db);
                cmd.CommandText = cmdText;
                foreach (var o in ps)
                {
                    cmd.Bind(o);
                }
                return cmd;
            }
        }

        private DBCommand CreateCommand(string cmdText)
        {
            if (!_open)
            {
                throw new DBException(Sqlite3.SQLITE_ERROR, "Cannot create commands from unopened database");
            }
            else
            {
                var cmd = new DBCommand(_db);
                cmd.CommandText = cmdText;
                return cmd;
            }
        }

        private string DBLastError()
        {
            return Sqlite3.sqlite3_errmsg(_db);
        }

        #endregion
    }
}
