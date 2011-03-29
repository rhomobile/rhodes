using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;

namespace rho.rubyext
{
    [RubyModule("SQLite3")]
    public static class RhoSQLite3
    {
        [RubyClass("Database")]
        public class RhoDatabase
        {
            protected RhoDatabase()
            {
            }

            #region Construction

            [RubyConstructor]
            public static RhoDatabase/*!*/ Create(RubyClass/*!*/ self, [NotNull]MutableString/*!*/ dbName, [NotNull]MutableString/*!*/ dbPartition)
            {
                return new RhoDatabase();
            }

            #endregion

            #region Public Instance Methods

            [RubyMethod("close")]
            public static void Close(RhoDatabase/*!*/ self)
            {
            }

            [RubyMethod("commit")]
            public static void Commit(RhoDatabase/*!*/ self)
            {
            }

            [RubyMethod("destroy_tables")]
            public static void destroyTables(RhoDatabase/*!*/ self, RubyArray arInclude, RubyArray arExclude)
            {
            }

            [RubyMethod("execute")]
            public static RubyArray Execute(RhoDatabase/*!*/ self, MutableString/*!*/ sqlStatement, Boolean isBatch, RubyArray args)
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
            public static void Lock(RhoDatabase/*!*/ self)
            {
            }

            [RubyMethod("rollback")]
            public static void Rollback(RhoDatabase/*!*/ self)
            {
 
            }

            [RubyMethod("start_transaction")]
            public static void startTransaction(RhoDatabase/*!*/ self)
            {
            }

            [RubyMethod("table_exist?")]
            public static Boolean isTableExist(RhoDatabase/*!*/ self, MutableString/*!*/ tblName)
            {
                return true;
            }

            [RubyMethod("unlock_db")]
            public static void Unlock(RhoDatabase/*!*/ self)
            {
            }

            #endregion

            #region Helpers

            #endregion
        }
    }
}