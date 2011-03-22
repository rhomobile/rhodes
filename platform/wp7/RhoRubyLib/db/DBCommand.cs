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
using System.Collections.Generic;

namespace rho.db
{
    public class DBCommand
    {
        private Sqlite3.sqlite3 _db;
        private List<Binding> _bindings;
        public string CommandText { get; set; }

        private static string[] _datetimeFormats = new string[] {
      "THHmmss",
      "THHmm",
      "HH:mm:ss",
      "HH:mm",
      "HH:mm:ss.FFFFFFF",
      "yy-MM-dd",
      "yyyy-MM-dd",
      "yyyy-MM-dd HH:mm:ss.FFFFFFF",
      "yyyy-MM-dd HH:mm:ss",
      "yyyy-MM-dd HH:mm",                               
      "yyyy-MM-ddTHH:mm:ss.FFFFFFF",
      "yyyy-MM-ddTHH:mm",
      "yyyy-MM-ddTHH:mm:ss",
      "yyyyMMddHHmmss",
      "yyyyMMddHHmm",
      "yyyyMMddTHHmmssFFFFFFF",
      "yyyyMMdd"
    };

        internal DBCommand(Sqlite3.sqlite3 db)
        {
            _db = db;
            _bindings = new List<Binding>();
            CommandText = "";
        }

        public int ExecuteNonQueryEx()
        {
            int n = Sqlite3.sqlite3_exec(_db, CommandText, 0, 0, 0);
            if (n != Sqlite3.SQLITE_OK)
                throw new DBException(n, SQLiteLastError());
            return 1;
        }

        public Object ExecuteScalar()
        {
            Object retObj = null;
            var stmt = Prepare();
            var cols = new System.Reflection.PropertyInfo[Sqlite3.sqlite3_column_count(stmt)];
            if (Sqlite3.sqlite3_step(stmt) == Sqlite3.SQLITE_ROW)
            {
                if (cols.Length > 0)
                {
                    var type = Sqlite3.sqlite3_column_type(stmt, 0);
                    switch (type)
                    {
                        case Sqlite3.SQLITE_INTEGER:
                            retObj = Sqlite3.sqlite3_column_int64(stmt, 0);
                            break;
                        case Sqlite3.SQLITE_FLOAT:
                            retObj = Sqlite3.sqlite3_column_double(stmt, 0);
                            break;
                        case Sqlite3.SQLITE_BLOB:
                            retObj = Sqlite3.sqlite3_column_blob(stmt, 0);
                            break;
                        case Sqlite3.SQLITE_TEXT:
                            retObj = Sqlite3.sqlite3_column_text(stmt, 0);
                            break;
                        case Sqlite3.SQLITE_NULL:
                            break;
                    }
                }
            }
            Sqlite3.sqlite3_finalize(ref stmt);
            return retObj;
        }

        public int ExecuteNonQuery()
        {
            Sqlite3.Vdbe stmt = Prepare();
            var r = Sqlite3.sqlite3_step(stmt);
            switch (r)
            {
                case Sqlite3.SQLITE_ERROR:
                    string msg = Sqlite3.sqlite3_errmsg(_db);
                    Sqlite3.sqlite3_finalize(ref stmt);
                    throw new DBException(r, msg);
                case Sqlite3.SQLITE_DONE:
                    int rowsAffected = Sqlite3.sqlite3_changes(_db);
                    Sqlite3.sqlite3_finalize(ref stmt);
                    return rowsAffected;
                case Sqlite3.SQLITE_CANTOPEN:
                    Sqlite3.sqlite3_finalize(ref stmt);
                    throw new DBException(r, "Cannot open database file");
                case Sqlite3.SQLITE_CONSTRAINT:
                    string msgC = Sqlite3.sqlite3_errmsg(_db);
                    Sqlite3.sqlite3_finalize(ref stmt);
                    throw new DBException(r, msgC);
                default:
                    Sqlite3.sqlite3_finalize(ref stmt);
                    throw new DBException(r, "Unknown error");
            }
        }

        public int ExecuteNonQuery<T>(T toInsert)
        {
            Sqlite3.Vdbe stmt = new Sqlite3.Vdbe();
            int n = Sqlite3.sqlite3_prepare_v2(_db, CommandText, CommandText.Length, ref stmt, 0);
            if (n != Sqlite3.SQLITE_OK)
                throw new DBException(n, SQLiteLastError());

            var props = GetProps(typeof(T));
            var ncols = Sqlite3.sqlite3_column_count(stmt);
            var cols = new System.Reflection.PropertyInfo[ncols];

            _bindings.Clear();
            for (int i = 0; i < props.Length; i++)
                Bind("@" + props[i].Name, props[i].GetValue(toInsert, null));
            BindAll(stmt);
            var r = Sqlite3.sqlite3_step(stmt);
            switch (r)
            {
                case Sqlite3.SQLITE_ERROR:
                    string msg = Sqlite3.sqlite3_errmsg(_db);
                    Sqlite3.sqlite3_finalize(ref stmt);
                    throw new DBException(r, msg);
                case Sqlite3.SQLITE_DONE:
                    int rowsAffected = Sqlite3.sqlite3_changes(_db);
                    Sqlite3.sqlite3_finalize(ref stmt);
                    return rowsAffected;
                case Sqlite3.SQLITE_CANTOPEN:
                    Sqlite3.sqlite3_finalize(ref stmt);
                    throw new DBException(r, "Cannot open database file");
                case Sqlite3.SQLITE_CONSTRAINT:
                    string msgC = Sqlite3.sqlite3_errmsg(_db);
                    Sqlite3.sqlite3_finalize(ref stmt);
                    throw new DBException(r, msgC);
                default:
                    Sqlite3.sqlite3_finalize(ref stmt);
                    throw new DBException(r, "Unknown error");
            }
        }

        public IEnumerable<T> ExecuteQuery<T>() where T : new()
        {
            var stmt = Prepare();

            var props = GetProps(typeof(T));
            var cols = new System.Reflection.PropertyInfo[Sqlite3.sqlite3_column_count(stmt)];
            for (int i = 0; i < cols.Length; i++)
                cols[i] = MatchColProp(Sqlite3.sqlite3_column_name(stmt, i), props);
            while (Sqlite3.sqlite3_step(stmt) == Sqlite3.SQLITE_ROW)
            {
                var obj = new T();
                for (int i = 0; i < cols.Length; i++)
                {
                    if (cols[i] == null)
                        continue;
                    var val = ReadCol(stmt, i, cols[i].PropertyType);
                    cols[i].SetValue(obj, val, null);
                }
                yield return obj;
            }
            Sqlite3.sqlite3_finalize(ref stmt);
        }

        public void Bind(string name, object val)
        {
            _bindings.Add(new Binding
            {
                Name = name,
                Value = val
            });
        }
        public void Bind(object val)
        {
            Bind(null, val);
        }
        public override string ToString()
        {
            return CommandText;
        }
        Sqlite3.Vdbe Prepare()
        {
            Sqlite3.Vdbe ppStmt = new Sqlite3.Vdbe();
            int n = Sqlite3.sqlite3_prepare_v2(_db, CommandText, CommandText.Length, ref ppStmt, 0);
            if (n != Sqlite3.SQLITE_OK)
                throw new DBException(n, SQLiteLastError());
            BindAll(ppStmt);
            return ppStmt;
        }

        SQLiteDateFormats dateFormat = (SQLiteDateFormats)Enum.Parse(typeof(SQLiteDateFormats), "ISO8601", true);

        public enum SQLiteDateFormats
        {
            /// <summary>
            /// Using ticks is not recommended and is not well supported with LINQ.
            /// </summary>
            Ticks = 0,
            /// <summary>
            /// The default format for this provider.
            /// </summary>
            ISO8601 = 1,
            /// <summary>
            /// JulianDay format, which is what SQLite uses internally
            /// </summary>
            JulianDay = 2
        }
        public string DateToString(DateTime dateValue)
        {
            switch (dateFormat)
            {
                case SQLiteDateFormats.Ticks:
                    return dateValue.Ticks.ToString(CultureInfo.InvariantCulture);
                case SQLiteDateFormats.JulianDay:
                    return ToJulianDay(dateValue).ToString(CultureInfo.InvariantCulture);
                default:
                    return dateValue.ToString(_datetimeFormats[7], CultureInfo.InvariantCulture);
            }
        }
        public double ToJulianDay(DateTime value)
        {
            return value.ToOADate() + 2415018.5;
        }
        public DateTime ToDateTime(string dateText)
        {
            switch (dateFormat)
            {
                case SQLiteDateFormats.Ticks:
                    return new DateTime(Convert.ToInt64(dateText, CultureInfo.InvariantCulture));
                case SQLiteDateFormats.JulianDay:
                    return ToDateTime(Convert.ToDouble(dateText, CultureInfo.InvariantCulture));
                default:
                    return DateTime.ParseExact(dateText, _datetimeFormats, DateTimeFormatInfo.InvariantInfo, DateTimeStyles.None);
            }
        }
        public DateTime ToDateTime(double julianDay)
        {
            return DateTime.FromOADate(julianDay - 2415018.5);
        }

        public static bool ToBoolean(object source)
        {
            if (source is bool) return (bool)source;

            return ToBoolean(source.ToString());
        }

        /// <summary>
        /// Convert a string to true or false.
        /// </summary>
        /// <param name="source">A string representing true or false</param>
        /// <returns></returns>
        /// <remarks>
        /// "yes", "no", "y", "n", "0", "1", "on", "off" as well as Boolean.FalseString and Boolean.TrueString will all be
        /// converted to a proper boolean value.
        /// </remarks>
        public static bool ToBoolean(string source)
        {
            if (String.Compare(source, bool.TrueString, StringComparison.OrdinalIgnoreCase) == 0) return true;
            else if (String.Compare(source, bool.FalseString, StringComparison.OrdinalIgnoreCase) == 0) return false;

            switch (source.ToLower(CultureInfo.InvariantCulture))
            {
                case "yes":
                case "y":
                case "1":
                case "on":
                    return true;
                case "no":
                case "n":
                case "0":
                case "off":
                    return false;
                default:
                    throw new ArgumentException("source");
            }
        }


        void BindAll(Sqlite3.Vdbe stmt)
        {
            int nextIdx = 1;
            foreach (var b in _bindings)
            {
                if ((b.Name != null) && (CommandText.IndexOf(b.Name) != -1))
                    b.Index = Sqlite3.sqlite3_bind_parameter_index(stmt, b.Name);
                else
                    b.Index = nextIdx++;
            }
            for (int c = 0; c < _bindings.Count; c++)
            {
                var b = _bindings[c];
                if (b.Value == null)
                {
                    int n = Sqlite3.sqlite3_bind_null(stmt, b.Index);
                    if (n > 0) throw new DBException(n, SQLiteLastError());
                    continue;
                }
                if (b.Value is Byte || b.Value is UInt16 || b.Value is SByte || b.Value is Int16 || b.Value is Int32 || b.Value is Boolean)
                {
                    int n = Sqlite3.sqlite3_bind_int(stmt, b.Index, Convert.ToInt32(b.Value, CultureInfo.InvariantCulture));
                    if (n > 0) throw new DBException(n, SQLiteLastError());
                    continue;
                }
                if (b.Value is UInt32 || b.Value is Int64)
                {
                    int n = Sqlite3.sqlite3_bind_int64(stmt, b.Index, Convert.ToInt64(b.Value, CultureInfo.InvariantCulture));
                    if (n > 0) throw new DBException(n, SQLiteLastError());
                    continue;
                }
                if (b.Value is Single || b.Value is Double || b.Value is Decimal)
                {
                    int n = Sqlite3.sqlite3_bind_double(stmt, b.Index, Convert.ToDouble(b.Value, CultureInfo.InvariantCulture));
                    if (n > 0) throw new DBException(n, SQLiteLastError());
                    continue;
                }
                if (b.Value is String)
                {
                    int n = Sqlite3.sqlite3_bind_text(stmt, b.Index, b.Value.ToString(), -1, null);
                    if (n > 0) throw new DBException(n, SQLiteLastError());
                    continue;
                }
                if (b.Value is byte[])
                {
                    int n = Sqlite3.sqlite3_bind_blob(stmt, b.Index, (byte[])b.Value, ((byte[])b.Value).Length, null);
                    if (n > 0) throw new DBException(n, SQLiteLastError());
                    continue;
                }
                if (b.Value is DateTime)
                {
                    int n = Sqlite3.sqlite3_bind_text(stmt, b.Index, DateToString((DateTime)b.Value), -1, null);
                    if (n > 0) throw new DBException(n, SQLiteLastError());
                    continue;
                }
            }
        }

        class Binding
        {
            public string Name { get; set; }
            public object Value { get; set; }
            public int Index { get; set; }
        }

        object ReadCol(Sqlite3.Vdbe stmt, int index, Type clrType)
        {
            var type = Sqlite3.sqlite3_column_type(stmt, index);
            if (type == Sqlite3.SQLITE_NULL)
                return null;
            if (clrType == typeof(Byte) || clrType == typeof(UInt16) || clrType == typeof(SByte) || clrType == typeof(Int16) || clrType == typeof(Int32))
                return Convert.ChangeType(Sqlite3.sqlite3_column_int(stmt, index), clrType, System.Threading.Thread.CurrentThread.CurrentCulture);
            if (clrType == typeof(UInt32) || clrType == typeof(Int64))
                return Convert.ChangeType(Sqlite3.sqlite3_column_int64(stmt, index), clrType, System.Threading.Thread.CurrentThread.CurrentCulture);
            if (clrType == typeof(Single) || clrType == typeof(Double) || clrType == typeof(Decimal))
                return Convert.ChangeType(Sqlite3.sqlite3_column_double(stmt, index), clrType, System.Threading.Thread.CurrentThread.CurrentCulture);
            if (clrType == typeof(String))
                return Convert.ChangeType(Sqlite3.sqlite3_column_text(stmt, index), clrType, System.Threading.Thread.CurrentThread.CurrentCulture);
            if (clrType == typeof(byte[]))
                return Sqlite3.sqlite3_column_blob(stmt, index);
            if (clrType == typeof(DateTime))
                return ToDateTime(Sqlite3.sqlite3_column_text(stmt, index));
            if (clrType == typeof(Boolean))
                return ToBoolean(Sqlite3.sqlite3_column_int(stmt, index));
            throw new NotSupportedException("Don't know how to read " + clrType);

        }
        static System.Reflection.PropertyInfo[] GetProps(Type t)
        {
            return t.GetProperties();
        }
        static System.Reflection.PropertyInfo MatchColProp(string colName, System.Reflection.PropertyInfo[] props)
        {
            foreach (var p in props)
            {
                if (p.Name == colName)
                {
                    return p;
                }
            }
            return null;
        }

        string SQLiteLastError()
        {
            return Sqlite3.sqlite3_errmsg(_db);
        }
    }
}
