using System;
using rho.common;

namespace rho.sync
{
    public class RhoJSONObject
    {
        public RhoJSONObject(String data){}
        public boolean has(String name){return false;}
        public static String quote(String name){ return name;}
        public String toString(){ return "";}
        public String getString(String name){ return "";}
        public int getInt(String name){ return 0;}
        public long getLong(String name){ return 0;}
    }

    public class JSONEntry
    {
        RhoJSONObject m_object;
	
	    public JSONEntry(RhoJSONObject obj)
	    {
	        m_object = obj;
	    }

	    public JSONEntry(String szData)
	    {
	        m_object = new RhoJSONObject(szData);
	    }

        public boolean isEmpty()
	    {
	        return m_object == null;
	    }

        public boolean hasName(String name)
	    {
		    return m_object.has(name);
	    }
	
	    public static String quoteValue(String str)
	    {
		    return RhoJSONObject.quote(str);
	    }

        public String getString(String name)
	    {
	        String szRes = null;
	        if ( m_object.has(name))
	    	    szRes = m_object.getString(name);	    	
	
	        return szRes;
	    }

        public String getString()
	    {
		    return m_object.toString();
	    }

        public int getInt(String name)
	    {
	        int nRes = 0;
	        if ( m_object.has(name))
	    	    nRes = m_object.getInt(name);	    	
	
	        return nRes;
	    }

        public long getLong(String name)
	    {
	        long nRes = 0;
	        if ( m_object.has(name))
	    	    nRes = m_object.getLong(name);	    	
	
	        return nRes;
	    }

        public long getUInt64(String name)
	    {
	        return getLong(name);
	    }

        public JSONEntry getEntry(String name)
	    {
            return null;// new JSONEntry((RhoJSONObject)m_object.get(name));
	    }
    }
}
