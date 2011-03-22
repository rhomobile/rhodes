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
    }

    public class Hashtable<TKey, TValue> : System.Collections.Generic.Dictionary<TKey, TValue>
    {
        public TValue get(TKey key)
        {
            return base[key];
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
    }

    public class RhoClassFactory
    {
        public static rho.db.IDBStorage createDBStorage() { return null; }
    }
}
