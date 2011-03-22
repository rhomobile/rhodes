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
                return 1;
            }

            [RubyMethod("commit")]
            public static int Commit(RhoDatabase/*!*/ self)
            {
                return 1;
            }

            [RubyMethod("destroy_tables")]
            public static int destroyTables(RhoDatabase/*!*/ self, long arInclude, long arExclude)
            {
                return 1;
            }

            [RubyMethod("execute")]
            public static RubyArray Execute(RhoDatabase/*!*/ self, MutableString/*!*/ sqlStatement)
            {
                RubyArray ret = new RubyArray();
                return ret;
            }

            [RubyMethod("is_ui_waitfordb")]
            public static Boolean isUiWaitForDb(RhoDatabase/*!*/ self)
            {
                return true;
            }

            [RubyMethod("lock_db")]
            public static int Lock(RhoDatabase/*!*/ self)
            {
                return 1;
            }

            [RubyMethod("rollback")]
            public static int Rollback(RhoDatabase/*!*/ self)
            {
                return 1;
            }

            [RubyMethod("start_transaction")]
            public static int startTransaction(RhoDatabase/*!*/ self)
            {
                return 1;
            }

            [RubyMethod("table_exist?")]
            public static Boolean isTableExist(RhoDatabase/*!*/ self, MutableString/*!*/ tblName)
            {
                return true;
            }

            [RubyMethod("unlock_db")]
            public static int Unlock(RhoDatabase/*!*/ self)
            {
                return 1;
            }

            #endregion

            #region Helpers

            #endregion
        }
    }
}