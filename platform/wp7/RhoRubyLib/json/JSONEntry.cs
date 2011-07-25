/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
            if (m_object == null )
                return new JSONEntry((Dictionary<string, object>)null);

            return new JSONEntry((Dictionary<string, object>)m_object[name]);
	    }
    }
}
