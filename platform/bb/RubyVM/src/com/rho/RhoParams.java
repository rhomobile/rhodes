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

package com.rho;

import java.util.Hashtable;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;

public class RhoParams
{
	RubyValue m_pParams;

    public RhoParams(RubyValue p)
    {
        m_pParams = p;
    }

    public RhoParams(RhoParams copy)
    {
        m_pParams = copy.m_pParams;
    }

    RubyValue findHashParam(String name)
    {
    	if ( !(m_pParams instanceof RubyHash) )
    		return null;

    	return ((RubyHash)m_pParams).getRaw(ObjectFactory.createSymbol(name));
    }

    public String getString(String szName)
    {
        return getString(szName, "");
    }

    public String getString(String szName, String szDefValue)
    {
    	RubyValue value = findHashParam(szName);
        String strRes = value != null ? value.toString() : "";
        if (strRes.length() == 0 && szDefValue != null && szDefValue.length() > 0)
            strRes = szDefValue;

        return strRes;
    }

    public Hashtable getHash(String szName)
    {
    	RubyValue value = findHashParam(szName);
        if ( value == null || !(value instanceof RubyHash))
            return new Hashtable();

        return RhoRuby.enum_strhash(value);
    }

    public boolean getBool(String name)
    {
    	String strValue = getString(name);
    	if ( strValue.length() == 0 )
    		return false;

        return strValue.compareTo("1") == 0 || strValue.compareTo("true") == 0;
    }
}

