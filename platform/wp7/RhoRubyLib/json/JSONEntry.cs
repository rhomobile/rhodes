using System;
using System.Collections.Generic;
using rho.common;
using fastJSON;

namespace rho.json
{
    public class JSONEntry
    {
        Dictionary<string, object> m_object;

        public JSONEntry(Dictionary<string, object> obj)
	    {
	        m_object = obj;
	    }

	    public JSONEntry(String szData)
	    {
            m_object = (Dictionary<string, object>)JsonParser.JsonDecode(szData);
	    }

        public boolean isEmpty()
	    {
	        return m_object == null;
	    }

        public boolean hasName(String name)
	    {
		    return m_object.ContainsKey(name);
	    }
	
	    public static String quoteValue(String str)
	    {
            return JsonParser.SerializeString(str);
	    }

        public Dictionary<string, object> getObject()
        {
            return m_object;
        }

        public Object getObject(String name)
        {
            Object oRes = null;
            if (m_object.ContainsKey(name))
                oRes = m_object[name];

            return oRes;
        }

        public String getString(String name)
	    {
	        String szRes = null;
            if (m_object.ContainsKey(name))
	    	    szRes = m_object[name].ToString();
	
	        return szRes;
	    }

        public String getString()
	    {
		    return m_object.ToString();
	    }

        public int getInt(String name)
	    {
            String szValue = getString(name);
            if (szValue == null || szValue.Length == 0)
                return 0;

            return int.Parse(szValue);
	    }

        public long getLong(String name)
	    {
            String szValue = getString(name);
            if (szValue == null || szValue.Length == 0)
                return 0;

            return long.Parse(szValue);
        }

        public long getUInt64(String name)
	    {
	        return getLong(name);
	    }

        public JSONEntry getEntry(String name)
	    {
            return new JSONEntry((Dictionary<string, object>)m_object[name]);
	    }
    }
}
