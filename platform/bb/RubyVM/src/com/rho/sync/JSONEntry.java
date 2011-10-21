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

class JSONEntry
{
    RhoJSONObject m_object;

	JSONEntry(RhoJSONObject obj)
	{
	    m_object = obj;
	}

	JSONEntry(String szData)throws RhoJSONException
	{
	    m_object = new RhoJSONObject(szData);
	}

	boolean isEmpty()
	{
	    return m_object == null;
	}

	boolean hasName(String name)
	{
		return m_object.has(name);
	}

	static String quoteValue(String str)
	{
		return RhoJSONObject.quote(str);
	}

	String getString(String name)throws RhoJSONException
	{
	    String szRes = null;
	    if ( m_object.has(name))
	    	szRes = m_object.getString(name);

	    return szRes;
	}

	String getString()
	{
		return m_object.toString();
	}

	int getInt(String name)throws RhoJSONException
	{
	    int nRes = 0;
	    if ( m_object.has(name))
	    	nRes = m_object.getInt(name);

	    return nRes;
	}

	long getLong(String name)throws RhoJSONException
	{
	    long nRes = 0;
	    if ( m_object.has(name))
	    	nRes = m_object.getLong(name);

	    return nRes;
	}

	long getUInt64(String name)throws RhoJSONException
	{
	    return getLong(name);
	}

	JSONEntry getEntry(String name)throws RhoJSONException
	{
	    return new JSONEntry((RhoJSONObject) m_object.get(name));
	}
}