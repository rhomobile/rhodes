using System;
using System.Net;
using rho.common;
using IronRuby.Builtins;

namespace rho.common
{
    public class RhoParamArray : RhoParams
    {
	    RubyArray m_array;
        public RhoParamArray(RhoParams oParams, String name) : base(oParams)
        {
            m_array = null;
            Object ar = findHashParam(name);
            if (ar != null && (ar is RubyArray))
                m_array = (RubyArray)ar;    
        }

        public int size()
        {
            if ( m_array  == null )
                return 0;

            return m_array.Count;
        }

        public RhoParams getItem(int nIndex)
        {
            return new RhoParams(m_array[nIndex]);
        }

    }
}
