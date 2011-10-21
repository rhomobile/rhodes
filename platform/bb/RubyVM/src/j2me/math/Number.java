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

package j2me.math;

public class Number{

	public static boolean isNumber(Object obj){
    	if ( obj instanceof Long || obj instanceof Integer ||
    		 obj instanceof Short || obj instanceof Byte ||
    		 obj instanceof Double || obj instanceof Float )
    		return true;

		return false;
	}

    public static int intValue(Object obj){
    	if ( obj instanceof Integer )
    		return ((Integer)obj).intValue();
    	else if ( obj instanceof Long )
    		return (int)((Long)obj).longValue();
    	else if ( obj instanceof Short )
    		return ((Short)obj).shortValue();
    	else if ( obj instanceof Byte )
    		return ((Byte)obj).byteValue();
    	else if ( obj instanceof byte[] )
    	{
			String strRes = "";
			try{
				strRes = new String( ((byte[])obj), "UTF-8");
       		}catch(java.io.UnsupportedEncodingException exc)
       		{
       		}

        	return strRes==null || strRes.length() == 0 ? 0 : Integer.parseInt(strRes);
    	}

    	String strRes = obj.toString();
    	return strRes==null || strRes.length() == 0 ? 0 : Integer.parseInt(strRes);
    }

    public static long longValue(Object obj){
    	if ( obj instanceof Long )
    		return ((Long)obj).longValue();
    	else if ( obj instanceof Integer )
    		return ((Integer)obj).longValue();
    	else if ( obj instanceof Short )
    		return ((Short)obj).shortValue();
    	else if ( obj instanceof Byte )
    		return ((Byte)obj).byteValue();
    	else if ( obj instanceof byte[] )
    	{
			String strRes = "";
			try{
				strRes = new String( ((byte[])obj), "UTF-8");
       		}catch(java.io.UnsupportedEncodingException exc)
       		{
       		}

        	return strRes==null || strRes.length() == 0 ? 0 : Long.parseLong(strRes);
    	}

    	String strRes = obj.toString();
    	return strRes==null || strRes.length() == 0 ? 0 : Long.parseLong(strRes);
    }

    public static double doubleValue(Object obj){
    	if ( obj instanceof Double )
    		return ((Double)obj).doubleValue();
    	else if ( obj instanceof Float )
    		return ((Float)obj).doubleValue();
    	else if ( obj instanceof Long )
    		return ((Long)obj).longValue();
    	else if ( obj instanceof Integer )
    		return ((Integer)obj).longValue();
    	else if ( obj instanceof Short )
    		return ((Short)obj).shortValue();
    	else if ( obj instanceof Byte )
    		return ((Byte)obj).byteValue();
    	else if ( obj instanceof byte[] )
    	{
			String strRes = "";
			try{
				strRes = new String( ((byte[])obj), "UTF-8");
       		}catch(java.io.UnsupportedEncodingException exc)
       		{
       		}

       		return strRes==null || strRes.length() == 0 ? 0 : Double.parseDouble(strRes);
    	}

    	String strRes = obj.toString();
    	return strRes==null || strRes.length() == 0 ? 0 : Double.parseDouble(strRes);
    }

    /*

	private Object m_oValue;

	public Number(Object val){
		m_oValue = val;
	}
	public Number(Integer val){
		m_oValue = val;
	}

    public int intValue(){
    	return 0;
    }
    public long longValue(){
    	return 0;
    }

    public float floatValue(){
    	return 0;
    }
    public double doubleValue(){
    	return 0;
    }

    public byte byteValue() {
    	return (byte)intValue();
    }

    public short shortValue() {
    	return (short)intValue();
    }

    */

};
