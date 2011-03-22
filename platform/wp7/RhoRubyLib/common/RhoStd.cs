using System;

namespace rho.common
{
    public struct boolean
    {
        private bool m_bValue;
        public boolean(bool b) { m_bValue = b; }
        public static implicit operator boolean(bool b) { return new boolean(b); }
        public static implicit operator bool(boolean b) { return b.m_bValue; }
    };

    public static class ExtensionMethods
    {
        public static int length(this String str)
        {
            return str.Length;
        }

        public static String substring(this String str, int start, int end)
        {
            return str.Substring(start, end-start);
        }

        public static String substring(this String str, int start)
        {
            return str.Substring(start);
        }

        public static Char charAt(this String str, int pos)
        {
            return str[pos];
        }

        public static String toUpperCase(this String str)
        {
            return str.ToUpper();
        }

        public static int indexOf(this String str, Char ch)
        {
            return str.IndexOf(ch);
        }
        public static int indexOf(this String str, String test)
        {
            return str.IndexOf(test);
        }

        public static int lastIndexOf(this String str, Char ch)
        {
            return str.LastIndexOf(ch);
        }

        public static int compareTo(this String str, String strToCmp)
        {
            return str.CompareTo(strToCmp);
        }

        public static String trim(this String str)
        {
            return str.Trim();
        }

        public static bool startsWith(this String str, String test)
        {
            return str.StartsWith(test);
        }

        public static bool endsWith(this String str, String test)
        {
            return str.EndsWith(test);
        }

        public static bool equalsIgnoreCase(this String str, String test)
        {
            return str.ToUpper().CompareTo(test.ToUpper()) == 0;
        }

        public static bool equals(this String str, String test)
        {
            return str.CompareTo(test) == 0;
        }
    }

    public class Vector<TValue> : System.Collections.Generic.List<TValue>
    {
        public int size()
        {
            return base.Count;
        }

        public TValue elementAt(int pos)
        {
            return base[pos];
        }

        public void addElement(TValue value)
        {
            base.Add(value);
        }
    }

    public class Hashtable<TKey, TValue> : System.Collections.Generic.Dictionary<TKey, TValue>
    {
        public TValue get(TKey key)
        {
            try
            {
                return base[key];
            }catch (Exception){
            }
            return default(TValue);
        }

        public boolean containsKey(TKey key)
        {
            return base.ContainsKey(key);
        }

        public int size()
        {
            return base.Count;
        }

        public void clear()
        {
            base.Clear();
        }

        public void put(TKey key, TValue value)
        {
            base.Add(key, value);
        }
    }

    public class Mutex
    {
        public void Lock()
        {
        }

        public void Unlock()
        {
        }
    }

    public class RhoClassFactory
    {
        public static rho.db.IDBStorage createDBStorage() { return new rho.db.CSqliteStorage(); }
    }
}
