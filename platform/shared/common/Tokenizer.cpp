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

#include "Tokenizer.h"

namespace rho{
namespace common{

CTokenizer::CTokenizer(String str,String delims) {
	m_delims = delims;
	m_str = str;
	m_length = str.length();
	m_position = 0;
}

String CTokenizer::nextToken()
{
	//eatDelimeters();
	int start = m_position;
	while (m_position<m_length && m_delims.find(m_str.at(m_position)) == String::npos ) {
		m_position++;
	}

    String strToken = m_str.substr(start,m_position-start);
	eatDelimeters();
	return strToken;
}

void CTokenizer::eatDelimeters()
{
    if ( m_position == m_length )
		m_position++;
	else if (m_position<m_length)
    {
        char c = m_str.at(m_position);
		if (m_delims.find(c) != String::npos ) {
			m_position++;
		} else {
			return;
		}
	}
}

boolean CTokenizer::hasMoreTokens()
{
	//eatDelimeters();
	return (m_position <= m_length);
}

}
}
