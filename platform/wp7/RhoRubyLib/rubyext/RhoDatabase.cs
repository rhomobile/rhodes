using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using rho.db;

namespace rho.rubyext
{
    [RubyModule("SQLite3")]
    public static class RhoSQLite3
    {
        [RubyClass("Database")]
        public class RhoDatabase
        {
            DBAdapter m_db;

            RhoDatabase() { }

            #region Construction

            [RubyConstructor]
            public static RhoDatabase/*!*/ Create(RubyClass/*!*/ self, [NotNull]MutableString/*!*/ dbName, [NotNull]MutableString/*!*/ dbPartition)
            {
                RhoDatabase rbDB = new RhoDatabase();
                rbDB.m_db = new DBAdapter();
                rbDB.m_db.rb_open(dbName.ToString(), dbPartition.ToString());

                return rbDB;
            }

            #endregion

            #region Public Instance Methods

            [RubyMethod("close")]
            public static int Close(RhoDatabase/*!*/ self)
            {
                self.m_db.close();

                return 0;
            }

            [RubyMethod("commit")]
            public static int Commit(RhoDatabase/*!*/ self)
            {
                self.m_db.commit();

                return 0;
            }

            [RubyMethod("destroy_tables")]
            public static int destroyTables(RhoDatabase/*!*/ self, long arInclude, long arExclude)
            {
                return 0;
            }

            [RubyMethod("execute")]
            public static RubyArray Execute(RhoDatabase/*!*/ self, MutableString/*!*/ sqlStatement, Boolean isBatch, RubyArray args)
            {
                RubyArray retArr = new RubyArray();

                if ( isBatch )
                {
                   self.m_db.Lock();
                   self.m_db.executeBatchSQL(sqlStatement.ToString());
                   self.m_db.Unlock();
                }
                else
                {
                    self.m_db.Lock();

                    IDBResult dbRes = self.m_db.executeSQL(sqlStatement.ToString(), args.ToArray());
                    if(dbRes != null)
                    {
                        while (!dbRes.isEnd())
                        {
                            retArr.Add(dbRes.getCurData());
                            dbRes.next();
                        }
                    }

                    self.m_db.Unlock();
                }

                return retArr;
            }

            [RubyMethod("is_ui_waitfordb")]
            public static Boolean isUiWaitForDb(RhoDatabase/*!*/ self)
            {
                return self.m_db.isUIWaitDB();
            }

            [RubyMethod("lock_db")]
            public static int Lock(RhoDatabase/*!*/ self)
            {
                self.m_db.Lock();

                return 0;
            }

            [RubyMethod("rollback")]
            public static int Rollback(RhoDatabase/*!*/ self)
            {
                self.m_db.rollback();

                return 0;
            }

            [RubyMethod("start_transaction")]
            public static int startTransaction(RhoDatabase/*!*/ self)
            {
                self.m_db.startTransaction();

                return 0;
            }

            [RubyMethod("table_exist?")]
            public static Boolean isTableExist(RhoDatabase/*!*/ self, MutableString/*!*/ tblName)
            {
                return self.m_db.isTableExist( tblName.ToString() );
            }

            [RubyMethod("unlock_db")]
            public static int Unlock(RhoDatabase/*!*/ self)
            {
                self.m_db.Unlock();

                return 0;
            }

            #endregion

            #region Helpers

            #endregion
        }
    }
}