using System;

namespace rho.db
{
    public class CSqliteStorage : IDBStorage
    {

        public void open(string strPath, string strSqlScript, string strEncryptionInfo)
        {
        }

        public void close()
        {
        }

        public IDBResult executeSQL(string strStatement, object[] values, common.boolean bReportNonUnique)
        {
            return null;
        }

        public void executeBatchSQL(string strStatement)
        {
        }

        public IDBResult createResult()
        {
            return null;
        }

        public void deleteAllFiles(string strPath)
        {
            
        }

        public void startTransaction()
        {
            
        }

        public void commit()
        {
            
        }

        public void rollback()
        {
            
        }

        public void onBeforeCommit()
        {
            
        }

        public void setDbCallback(IDBCallback callback)
        {
            
        }

        public string[] getAllTableNames()
        {
            return null;
        }

        public common.boolean isTableExists(string strName)
        {
            return false;
        }

        public common.boolean isDbFileExists(string strPath)
        {
            return false;
        }

        public void createTriggers()
        {
            
        }
    }
}
