using System;
using rho.common;

namespace rho.db
{
    interface IDBStorage
    {
        void open(String strPath, String strSqlScript, String strEncryptionInfo);
        void close();
	
        IDBResult executeSQL(String strStatement, Object[] values, boolean bReportNonUnique);
	
        void executeBatchSQL(String strStatement);
	
        IDBResult createResult();
	
        void deleteAllFiles(String strPath);
	
        void startTransaction();
        void commit();
        void rollback();
        void onBeforeCommit();
	
        void setDbCallback(IDBCallback callback);
	
        String[] getAllTableNames();
        boolean isTableExists(String strName);
	
        boolean isDbFileExists(String strPath);
        void createTriggers();
    }
}
