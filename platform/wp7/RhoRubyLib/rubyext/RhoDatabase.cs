using Microsoft.Scripting.Utils;
using Microsoft.Scripting.Runtime;
using IronRuby.Runtime;
using IronRuby.Builtins;
using System;
using System.Collections.Generic;
using rho.db;
using rho.common;

namespace rho.rubyext
{
    [RubyModule("SQLite3")]
    public static class RhoSQLite3
    {
        [RubyClass("Database")]
        public class RhoDatabase
        {
            static CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }

            static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
                new RhoLogger("RhoDatabase");

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
            public static void Close(RhoDatabase/*!*/ self)
            {
                try
                {
                    self.m_db.close();
                }
                catch (Exception ex)
                {
                    LOG.HandleRubyException(ex, RhoRuby.rubyContext.CurrentException, "close");
                }
            }

            [RubyMethod("commit")]
            public static void Commit(RhoDatabase/*!*/ self)
            {
                try
                {
                    self.m_db.commit();
                }
                catch (Exception ex)
                {
                    LOG.HandleRubyException(ex, RhoRuby.rubyContext.CurrentException, "commit");
                }
            }

            [RubyMethod("destroy_tables")]
            public static void destroyTables(RhoDatabase/*!*/ self, RubyArray arInclude, RubyArray arExclude)
            {
                try
                {
                    Vector<String> vecIncludes = RhoRuby.makeVectorStringFromArray(arInclude);
                    Vector<String> vecExcludes = RhoRuby.makeVectorStringFromArray(arExclude);

                    self.m_db.rb_destroy_tables(vecIncludes, vecExcludes);
                }
                catch (Exception ex)
                {
                    LOG.HandleRubyException(ex, RhoRuby.rubyContext.CurrentException, "destroy_tables");
                }
            }

            [RubyMethod("execute")]
            public static RubyArray Execute(RhoDatabase/*!*/ self, MutableString/*!*/ sqlStatement, Boolean isBatch, RubyArray args)
            {
                try
                {
                    RubyArray retArr = new RubyArray();

                    if (isBatch)
                    {
                        self.m_db.executeBatchSQL(sqlStatement.ToString());
                    }
                    else
                    {
                        Object[] values = null;
                        if (args != null && args.Count > 0)
                        {
                            if (args[0] != null && args[0] is RubyArray)
                                values = ((RubyArray)args[0]).ToArray();
                            else
                                values = args.ToArray();
                        }

                        try
                        {
                            self.m_db.Lock();
                            using (IDBResult rows = self.m_db.executeSQL(sqlStatement.ToString(), values, true))
                            {
                                if (rows != null)
                                {
                                    MutableString[] colNames = null;
                                    for (; !rows.isEnd(); rows.next())
                                    {
                                        IDictionary<object, object> map = new Dictionary<object, object>();
                                        Hash row = new Hash(map);
                                        for (int nCol = 0; nCol < rows.getColCount(); nCol++)
                                        {
                                            if (colNames == null)
                                                colNames = getOrigColNames(rows);

                                            row.Add(colNames[nCol], rows.getRubyValueByIdx(nCol));
                                        }
                                        retArr.Add(row);
                                    }
                                }
                            }
                        }
                        finally
                        {
                            self.m_db.Unlock();
                        }
                    }

                    return retArr;
                }catch (Exception exc)
                {
                    LOG.HandleRubyException(exc, RhoRuby.rubyContext.CurrentException, "execute");
                    return null;
                }
            }

            [RubyMethod("is_ui_waitfordb")]
            public static Boolean isUiWaitForDb(RhoDatabase/*!*/ self)
            {
                Boolean res = false;
                try
                {
                    res = self.m_db.isUIWaitDB();
                }
                catch (Exception ex)
                {
                    LOG.HandleRubyException(ex, RhoRuby.rubyContext.CurrentException, "is_ui_waitfordb");
                }

                return res;
            }

            [RubyMethod("lock_db")]
            public static void Lock(RhoDatabase/*!*/ self)
            {
                try
                {
                    self.m_db.Lock();
                }
                catch (Exception ex)
                {
                    LOG.HandleRubyException(ex, RhoRuby.rubyContext.CurrentException, "lock_db");
                }
            }

            [RubyMethod("rollback")]
            public static void Rollback(RhoDatabase/*!*/ self)
            {
                try
                {
                    self.m_db.rollback();
                }
                catch (Exception ex)
                {
                    LOG.HandleRubyException(ex, RhoRuby.rubyContext.CurrentException, "rollback");
                }
            }

            [RubyMethod("start_transaction")]
            public static void startTransaction(RhoDatabase/*!*/ self)
            {
                try
                {
                    self.m_db.startTransaction();
                }
                catch (Exception ex)
                {
                    LOG.HandleRubyException(ex, RhoRuby.rubyContext.CurrentException, "start_transaction");
                }
            }

            [RubyMethod("table_exist?")]
            public static Boolean isTableExist(RhoDatabase/*!*/ self, MutableString/*!*/ tblName)
            {
                Boolean res = false;
                try
                {
                    res = self.m_db.isTableExist(tblName.ToString());
                }
                catch (Exception ex)
                {
                    LOG.HandleRubyException(ex, RhoRuby.rubyContext.CurrentException, "table_exist?");
                }

                return res;
            }

            [RubyMethod("unlock_db")]
            public static void Unlock(RhoDatabase/*!*/ self)
            {
                try
                {
                    self.m_db.Unlock();
                }
                catch (Exception ex)
                {
                    LOG.HandleRubyException(ex, RhoRuby.rubyContext.CurrentException, "unlock_db");
                }
            }

            #endregion

            #region Helpers

            private static MutableString[] getOrigColNames(IDBResult rows)
            {
                MutableString[] colNames = new MutableString[rows.getColCount()];
                for (int nCol = 0; nCol < rows.getColCount(); nCol++)
                    colNames[nCol] = MutableString.Create(rows.getOrigColName(nCol));

                return colNames;
            }

            #endregion
        }
    }
}