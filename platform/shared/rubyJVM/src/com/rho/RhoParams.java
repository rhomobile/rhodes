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

