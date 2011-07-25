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
    public class JSONStructIterator
    {
        Dictionary<string, object> m_object;
	    Dictionary<string, object>.Enumerator m_enumStruct;
	    String m_strCurKey = null;
	
        public JSONStructIterator(String szData)
        {
            m_object = (Dictionary<string, object>)JsonParser.JsonDecode(szData);

            m_enumStruct = m_object.GetEnumerator();
            if ( m_enumStruct.MoveNext() )
                m_strCurKey = m_enumStruct.Current.Key;
        }

        public JSONStructIterator(JSONEntry oEntry, String strName)
        {
    	    m_object = (Dictionary<string, object>)oEntry.getObject(strName);
            m_enumStruct = m_object.GetEnumerator();
            if (m_enumStruct.MoveNext())
                m_strCurKey = m_enumStruct.Current.Key;
        }

        public JSONStructIterator(JSONEntry oEntry)
        {
    	    m_object = oEntry.getObject();
            m_enumStruct = m_object.GetEnumerator();
            if (m_enumStruct.MoveNext())
                m_strCurKey = m_enumStruct.Current.Key;
        }

        public boolean isEnd()
        {
            return m_strCurKey == null;
        }

        public void next()
        {
            if (m_enumStruct.MoveNext())
                m_strCurKey = m_enumStruct.Current.Key;
            else
                m_strCurKey = null;
        }

        public void reset()
        {
            m_enumStruct = m_object.GetEnumerator();
            if (m_enumStruct.MoveNext())
                m_strCurKey = m_enumStruct.Current.Key;
        }

        public String getCurKey()
        {
            return isEnd() ? String.Empty : m_strCurKey;
        }

        public String getCurString()
	    {
            return m_enumStruct.Current.Value.ToString();
	    }

        public JSONEntry getCurValue()
        {
    	    if ( isEnd() )
                return new JSONEntry((Dictionary<string, object>)null);

            return new JSONEntry((Dictionary<string, object>)m_enumStruct.Current.Value);
        }
    }
}
