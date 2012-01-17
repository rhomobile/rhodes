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

        public Object findHashParam(String name)
        {
    	    if ( !(m_pParams is Hash) ) 
    		    return null;
            
            Object val = null;
            Hash values = (Hash)m_pParams;
            values.TryGetValue(CRhoRuby.CreateSymbol(name), out val);
            return (val != null) ? val : null;
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
