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

package j2me.lang;

public class StringBufferMe {
/*
    public static int indexOf(StringBuffer strThis, String str) {
    	return indexOf(strThis,str, 0);
    }

    public static int indexOf(StringBuffer strThis, String str, int fromIndex) {
    	return strThis.toString().indexOf(str, fromIndex);
    }
    
    public static StringBuffer replace( StringBuffer strThis, int start, int end, String str) {
    	AbstractStringBuilder_replace(strThis, start, end, str);
        return strThis;
    }

    public static StringBuffer AbstractStringBuilder_replace(StringBuffer strThis, int start, int end, String str) {
        if (start < 0)
        	throw new StringIndexOutOfBoundsException(start);
        
        int count = strThis.length();
		if (end > count)
		    end = count;
		if (start > end)
		    throw new StringIndexOutOfBoundsException("start > end");

		int len = str.length();
		int newCount = count + len - (end - start);
		if (newCount > strThis.capacity())
			strThis.ensureCapacity(newCount);
	
		if ( len != end-start)
		{
			for( int i = 0; i < count-end; i++ )
				strThis.setCharAt(start + len+i, strThis.charAt(end+i));
		}
		int i = start;
		for( ; i < Math.min(count,len); i++ )
			strThis.setCharAt(i, str.charAt(i-start));

		for( ; i < len; i++ )
			strThis.append(str.charAt(i-start));
		
//        System.arraycopy(value, end, value, start + len, count - end);
//        str.getChars(value, start);
        count = newCount;
        return strThis;
    }*/
    
}
