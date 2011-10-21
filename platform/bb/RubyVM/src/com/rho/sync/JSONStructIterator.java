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

package com.rho.sync;

import org.json.me.RhoJSONException;
import org.json.me.RhoJSONObject;
import java.util.Enumeration;
class JSONStructIterator
{
	RhoJSONObject m_object;
	Enumeration m_enumKeys;
	String m_strCurKey;

    JSONStructIterator(String szData)throws RhoJSONException
    {
    	m_object = new RhoJSONObject(szData);
    	m_enumKeys = m_object.keys();
    	if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		m_strCurKey = (String)m_enumKeys.nextElement();
    }

    JSONStructIterator(JSONEntry oEntry, String strName)throws RhoJSONException
    {
    	m_object = (RhoJSONObject)oEntry.m_object.get(strName);
    	m_enumKeys = m_object.keys();
    	if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		m_strCurKey = (String)m_enumKeys.nextElement();
    }

    JSONStructIterator(JSONEntry oEntry)
    {
    	m_object = oEntry.m_object;
    	m_enumKeys = m_object.keys();
    	if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		m_strCurKey = (String)m_enumKeys.nextElement();
    }

    boolean isEnd()
    {
        return m_strCurKey == null;
    }

    void  next()
    {
    	if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		m_strCurKey = (String)m_enumKeys.nextElement();
    	else
    		m_strCurKey = null;
    }

    void reset()
    {
    	m_enumKeys = m_object.keys();
    	if ( m_enumKeys != null && m_enumKeys.hasMoreElements() )
    		m_strCurKey = (String)m_enumKeys.nextElement();
    }

    String getCurKey()
    {
        return isEnd() ? new String() : m_strCurKey;
    }

	String getCurString()throws RhoJSONException
	{
		return m_object.getString(m_strCurKey);
	}

    JSONEntry getCurValue()throws RhoJSONException
    {
    	if ( isEnd() )
    		return new JSONEntry( (RhoJSONObject)null );

		return new JSONEntry( (RhoJSONObject)m_object.getJSONObject(m_strCurKey) );
    }

}
