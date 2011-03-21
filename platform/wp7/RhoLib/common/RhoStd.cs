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
    }

    public class Hashtable<TKey, TValue> : System.Collections.Generic.Dictionary<TKey, TValue>
    {
    }

    public class Mutex
    {
    }

    public class RhoClassFactory
    {
        public static rho.db.IDBStorage createDBStorage() { return null; }
    }
}
