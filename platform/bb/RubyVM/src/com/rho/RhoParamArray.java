package com.rho;

import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;

public class RhoParamArray extends RhoParams
{
	RubyArray m_array;
    public RhoParamArray(RhoParams oParams, String name)
    {
        super(oParams);
        m_array = null;
        RubyValue ar = findHashParam(name);
        if (ar != null && (ar instanceof RubyArray))
            m_array = (RubyArray)ar;
            
    }

    public int size()
    {
        if ( m_array  == null )
            return 0;

        return m_array.size();
    }

    public RhoParams getItem(int nIndex)
    {
        return new RhoParams(m_array.get(nIndex));
    }

}

