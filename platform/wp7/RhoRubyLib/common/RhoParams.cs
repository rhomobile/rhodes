using System;
using IronRuby.Builtins;
using System.Collections.Generic;


namespace rho.common
{
    public class RhoParams
    {
        protected Object m_pParams;

        public RhoParams(Object p)
        {
            m_pParams = p;
        }

        public RhoParams(RhoParams copy)
        {
            m_pParams = copy.m_pParams;
        }

        protected Object findHashParam(String name)
        {
    	    if ( !(m_pParams is Hash) ) 
    		    return null;

            return ((Hash)m_pParams)[name];
         }

        public String getString(String szName)
        {
            return getString(szName, "");
        }

        public String getString(String szName, String szDefValue)
        {
            Object value = findHashParam(szName);
            String strRes = value != null ? value.ToString() : "";
            if (strRes.length() == 0 && szDefValue != null && szDefValue.length() > 0)
                strRes = szDefValue;

            return strRes;
        }

        public Hashtable<String, String> getHash(String szName)
        {
    	    Object value = findHashParam(szName);
            if ( value == null || !(value is Hash))
                return new Hashtable<String, String>();

            return CRhoRuby.enum_strhash(value); 
        }

        public boolean getBool(String name)
        {
            String strValue = getString(name);
            if (strValue.length() == 0)
                return false;

            return strValue.compareTo("1") == 0 || strValue.compareTo("true") == 0;
        }
    }
}
