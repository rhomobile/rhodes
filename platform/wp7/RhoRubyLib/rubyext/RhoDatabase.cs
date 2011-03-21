using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;

namespace rho.rubyext
{
    [RubyClass("Database", Inherits = typeof(object)), Includes(typeof(RhoSQLite3))]
    public class RhoDatabase : RubyObject
    {
        #region Construction

        public RhoDatabase(RubyClass/*!*/ cls) 
            : base(cls) {
        }

        public RhoDatabase(RubyClass/*!*/ cls, MutableString/*!*/ dbName) 
            : base(cls) {
            Reinitialize(this, dbName);
        }

        [RubyConstructor]
        public static RhoDatabase/*!*/ Create(RubyClass/*!*/ self, [NotNull]MutableString/*!*/ dbName)
        {
            return new RhoDatabase(self, dbName);
        }

        [RubyMethod("initialize", RubyMethodAttributes.PrivateInstance)]
        public static RhoDatabase/*!*/ Reinitialize(RhoDatabase/*!*/ self, [NotNull]MutableString/*!*/ dbName)
        {  
            return self;
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