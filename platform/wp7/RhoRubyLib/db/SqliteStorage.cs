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
using System.Globalization;
using rho.common;

namespace rho.db
{
    public class CSqliteStorage : IDBStorage
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
            new RhoLogger("SqliteStorage");

        private boolean m_bPendingTransaction = false;
        private Sqlite3.sqlite3 m_db;
        private int m_nInsideTransaction = 0;
        private boolean m_bNeedProcessCallback = false;
        private IDBCallback m_dbCallback;

        public void open(string strPath, string strSqlScript, string strEncryptionInfo)
        {
            int res = 0;
            if (m_db != null)
                res = Sqlite3.sqlite3_close(m_db);

            if (res != Sqlite3.SQLITE_OK)
                throw new DBException(res, DBLastError());  
    
            m_db = null;
        }

        public void commit()
        {
            if (m_nInsideTransaction > 0)
                m_nInsideTransaction--;
            
            int res = 0;    
            if (m_db != null)
                    res =Sqlite3.sqlite3_exec(m_db, "COMMIT", 0, 0, 0);

            if (res != Sqlite3.SQLITE_OK)
                throw new DBException(res, DBLastError());
        }

        public IDBResult createResult()
        {
            return new CSqliteResult();
        }

        public void createTriggers()
        {
            //TODO: createTriggers
            //String strTriggers = CRhoFile.readStringFromFile("apps/db/syncdb.triggers");
            //executeBatchSQL(strTriggers);
        }

        public void deleteAllFiles(String strPath)
        {
            CRhoFile.deleteFile(strPath);
            CRhoFile.deleteFile(strPath + "-journal");
        }

        public IDBResult executeSQL(string strStatement, object[] values, common.boolean bReportNonUnique)
        {
            int res = Sqlite3.sqlite3_exec(m_db, strStatement, 0, 0, 0);
            if (res != Sqlite3.SQLITE_OK)
                throw new DBException(res, DBLastError());
        }

        public void executeBatchSQL(string strStatement)
        {
            if (m_db == null)
                throw new Exception("executeSQL: m_db == null");

            var stmt = Prepare(strStatement, values);
            
            return new CSqliteResult(stmt);
        }

        public IDBResult createResult()
        {
            IDBResult res = executeSQL("SELECT name FROM sqlite_master WHERE type='table'", null, false);

            Vector<Object> arTables = new Vector<Object>();
            for (; !res.isEnd(); res.next())
            {
                arTables.addElement(res.getCurData()[0]);
            }

            String[] vecTables = new String[arTables.size()];
            for (int i = 0; i < arTables.size(); i++)
                vecTables[i] = (String)arTables.elementAt(i);

            return vecTables;
        }

        public boolean isDbFileExists(string strPath)
        {
            return CRhoFile.isFileExist(strPath);
        }
        
        public boolean isTableExists(string strName)
        {
            Object[] vals = { strName };
		    IDBResult res = executeSQL("SELECT name FROM sqlite_master WHERE type='table' AND name=?", vals, false );
		    boolean bRes = !res.isEnd();
		    res.close();
		
		    return bRes;
        }

        public void startTransaction()
        {
            if (!m_bNeedProcessCallback)
                return;
            processCallbackData();
        }

        public void commit()
        {
            try
            {
                String dbURI = RHODESAPP().resolveDBFilesPath(strPath);
                dbURI = CFilePath.removeFirstSlash(dbURI);
                boolean bEncrypted = strEncryptionInfo != null && strEncryptionInfo.length() > 0;
                //DatabaseSecurityOptions dbso = new DatabaseSecurityOptions(bEncrypted);

                boolean bExist = CRhoFile.isFileExist(strPath);

                int res = Sqlite3.sqlite3_open(dbURI, ref m_db);
                if (res != Sqlite3.SQLITE_OK)
                    throw new DBException(res, "Could not open database file: " + strPath);

                if ( !bExist )
                    createSchema(strSqlScript);

                /*startTransaction();
                try
                {
                    executeBatchSQL(strSqlScript);
                    createTriggers();
                }
                catch (DBException exc)
                {
                    rollback();
                    throw exc;
                }
                commit();*/


                if (m_bPendingTransaction)
                    startTransaction();

                m_bPendingTransaction = false;
            }
            catch (Exception exc)
            {
                throw new DBException(exc);
            }
            finally
            {
                m_nInsideTransaction = 0;
            }
        }

        public void rollback()
        {
            if (m_nInsideTransaction > 0)
                m_nInsideTransaction--;

            int res = 0;
            if (m_db != null) 
                res = Sqlite3.sqlite3_exec(m_db, "ROLLBACK", 0, 0, 0);
            
            if (res != Sqlite3.SQLITE_OK)
                throw new DBException(res, DBLastError());
        }

        public void onBeforeCommit()
        {
            m_dbCallback = callback;
        }

        public void setDbCallback(IDBCallback callback)
        {
            if (m_db == null)
                m_bPendingTransaction = true;
            else
            {
                int res = Sqlite3.sqlite3_exec(m_db, "BEGIN", 0, 0, 0);
                if (res != Sqlite3.SQLITE_OK)
                    throw new DBException(res, DBLastError());
            }

            m_nInsideTransaction++;
            m_bNeedProcessCallback = false;
        }

        #region Helpers

        private void createSchema(String strSqlScript)
        {
            //String strSqlScript;
            //strSqlScript = CRhoFile.readStringFromFile(RHODESAPP().canonicalizeRhoUrl("db/syncdb.schema"));

            if ( strSqlScript.length() == 0 )
            {
                LOG.ERROR("createSchema failed. Cannot read schema: " + strSqlScript);
                return;
            }

            startTransaction();
            try
            {
                executeBatchSQL(strSqlScript);
                createTriggers();
            }
            catch (DBException exc)
            {
                rollback();
                throw exc;
            }
            commit();
        }

        private string DBLastError()
        {
            return Sqlite3.sqlite3_errmsg(m_db);
        }

        private void Bind(Sqlite3.Vdbe stmt, Object[] values)
        {
            for (int i = 0; values != null && i < values.Length; i++)
            {
                int res = 0;
                var obj = values[i];
                if (obj == null)
                {
                    res = Sqlite3.sqlite3_bind_null(stmt, i + 1);
                    if (res > 0) throw new DBException(res, DBLastError());
                    continue;
                }
                if (obj is Byte || obj is UInt16 || obj is SByte || obj is Int16 || obj is Int32 || obj is Boolean)
                {
                    res = Sqlite3.sqlite3_bind_int(stmt, i + 1, Convert.ToInt32(obj, CultureInfo.InvariantCulture));
                    if (res > 0) throw new DBException(res, DBLastError());
                    continue;
                }
                if (obj is UInt32 || obj is Int64)
                {
                    res = Sqlite3.sqlite3_bind_int64(stmt, i + 1, Convert.ToInt64(obj, CultureInfo.InvariantCulture));
                    if (res > 0) throw new DBException(res, DBLastError());
                    continue;
                }
                if (obj is Single || obj is Double || obj is Decimal)
                {
                    res = Sqlite3.sqlite3_bind_double(stmt, i + 1, Convert.ToDouble(obj, CultureInfo.InvariantCulture));
                    if (res > 0) throw new DBException(res, DBLastError());
                    continue;
                }
                if (obj is String)
                {
                    res = Sqlite3.sqlite3_bind_text(stmt, i + 1, obj.ToString(), -1, null);
                    if (res > 0) throw new DBException(res, DBLastError());
                    continue;
                }
                if (obj is byte[])
                {
                    res = Sqlite3.sqlite3_bind_blob(stmt, i + 1, (byte[])obj, ((byte[])obj).Length, null);
                    if (res > 0) throw new DBException(res, DBLastError());
                    continue;
                }
            }
        }

        private Sqlite3.Vdbe Prepare(String strStatement, Object[] values)
        {
            Sqlite3.Vdbe ppStmt=new Sqlite3.Vdbe();
            int res = Sqlite3.sqlite3_prepare_v2(m_db, strStatement, strStatement.Length, ref ppStmt, 0);
            if ( res != Sqlite3.SQLITE_OK)
                throw new DBException(res, DBLastError());
            
            Bind(ppStmt, values);
            return ppStmt;
        }

        private void processCallbackData()
    	{
        }

        private static CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }

        #endregion
    }
}
