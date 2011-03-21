using System;

namespace rho.common
{
    struct boolean
    {
        private bool m_bValue;
        public boolean(bool b) { m_bValue = b; }
        public static implicit operator boolean(bool b) { return new boolean(b); }
    };

    public class Hashtable<TKey, TValue> : System.Collections.Generic.Dictionary<TKey, TValue>
    {
    }

    public class Mutex
    {
    }
}
