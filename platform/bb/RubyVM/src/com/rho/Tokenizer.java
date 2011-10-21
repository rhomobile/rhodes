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

public class Tokenizer {

	String m_str;
	String m_delims;
	int m_length;
	int m_position;

	public Tokenizer(String str,String delims) {
		m_delims = delims;
		m_str = str;
		m_length = str.length();
		m_position = 0;
	}

	public String nextToken() {
		//eatDelimeters();
		int start = m_position;
		while (m_position<m_length && m_delims.indexOf(m_str.charAt(m_position))==-1) {
			m_position++;
		}

		String strToken = m_str.substring(start,m_position);
		eatDelimeters();
		return strToken;
	}

	public void eatDelimeters()
	{
		if ( m_position == m_length )
			m_position++;
		else if (m_position<m_length) {
			char c = m_str.charAt(m_position);
			if (m_delims.indexOf(c)>=0) {
				m_position++;
			} else {
				return;
			}

		}
	}

    public boolean hasMoreTokens() {
    	//eatDelimeters();
    	return (m_position <= m_length);
    }


}
