package com.rhomobile.rhodes.http.util;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.StringTokenizer;
import java.util.Vector;

import android.util.Log;

@SuppressWarnings("unchecked")
public class UrlEncoded extends Hashtable
{
    private static final long serialVersionUID = -767968254490942884L;
	
	public static final String noValue="";
    
    public UrlEncoded()
    {
        super(10);
    }
    
    public UrlEncoded(String s) throws IOException
    {
        super(10);
        read(s);
    }
    
    public void read(String string_input) throws IOException
    {
        addParamsTo(string_input,this);
    }
    
    /** 
     * Get value
     */
    public Object get(Object key)
    {
        Object o = super.get(key);
        if (o!=null)
        {
            if (o instanceof Vector)
            {
                Vector v=(Vector)o;
                if (v.size()>0)
                {
                    String value=v.elementAt(0).toString();
                    for (int i=1; i<v.size(); i++)
                        value += ','+v.elementAt(i).toString();
                    return value;
                }
                return null;
            }

            if (o instanceof String[])
            {
                String[] a=(String[])o;
                if (a.length>0)
                {
                    StringBuffer buf = new StringBuffer(a[0]);
                    for (int i=1; i<a.length; i++)
                    {
                        buf.append(',');
                        buf.append(a[i]);
                    }
                    return buf.toString();
                }
                return null;
            }
        }
        return o;
    }


    /** 
     * Get multiple values as an array.
     */
    public String[] getValues(String key)
    {
        Object o = super.get(key);
        if (o==null)
            return null;
        if (o instanceof String[])
            return (String[])o;
        if (o instanceof Vector)
        {
            Vector v = (Vector)o;
            String[] a = new String[v.size()];
            for (int i=v.size();i-->0;)
                a[i]=v.elementAt(i).toString();
            return a;
        }

        String[] a = new String[1];
        a[0]=o.toString();
        return a;
    }

    /** 
     * Set a multi valued parameter
     */
    public void putValues(String key, String[] values)
    {
        super.put(key,values);
    }

    /**
     * Add encoded parameters to Dictionary.
     */
    public static void addParamsTo(String content,UrlEncoded url)
    {
        String name;
        String value;

        StringTokenizer tokenizer = new StringTokenizer(content, "&", false);

        while ((tokenizer.hasMoreTokens()))
        {
            // take the first token string, which should be an assignment statement
            String substring = tokenizer.nextToken();

            // breaking it at the "=" sign
            int i = substring.indexOf('=');
            if (i<0)
            {
                name=decode(substring);
                value=noValue;
            }
            else
            {
                name  = decode(substring.substring(0,i++).trim());
                if (i>=substring.length())
                    value=noValue;
                else
                {
                    value =
                        substring.substring(i,substring.length()).trim();
                    value = decode(value);
                }
            }

            if (name.length() > 0)
            {
                Object o = url.getObject(name);
                if (o!=null)
                {
                    if (o instanceof Vector)
                        ((Vector)o).addElement(value);
                    else
                    {
                        Vector v = new Vector();
                        v.addElement(o);
                        v.addElement(value);
                        url.put(name,v);
                    }
                }
                else
                   url.put(name,value);
            }
        }
    }

    
    /** 
     * Encode Hashtable with % encoding
     */
    public String encode()
    {
        return encode(false);
    }

    
    /** 
     * Encode Hashtable with % encoding
     */
    public String encode(boolean equalsForNullValue)
    {
        Enumeration keys=keys();
        String separator="";
        StringBuffer result = new StringBuffer(128);
        while(keys.hasMoreElements())
        {
            String key = keys.nextElement().toString();
            String[] values = getValues(key);

            for (int v=0; v<values.length;v++)
            {
                result.append(separator);
                result.append(encode(key));

                if (values[v].length()>0)
                {
                    result.append('=');
                    result.append(encode(values[v]));
                }
                else if (equalsForNullValue)
                    result.append('=');

                separator="&";
            }
        }
        return result.toString();
    }

    
    /** 
     * Perform URL encoding.
     */
    public static String encode(String string)
    {
        byte[] bytes=null;
        try
        {
            bytes=string.getBytes("UTF-8");
        }
        catch(UnsupportedEncodingException e)
        {
            Log.w("UrlEncoded",e.getMessage());
            bytes=string.getBytes();
        }

        int len=bytes.length;
        byte[] encoded= new byte[bytes.length*3];
        int n=0;
        boolean noEncode=true;

        for (int i=0;i<len;i++)
        {
            byte b = bytes[i];

            if (b==' ')
            {
                noEncode=false;
                encoded[n++]=(byte)'+';
            }
            else if (b>='a' && b<='z' ||
                     b>='A' && b<='Z' ||
                     b>='0' && b<='9')
            {
                encoded[n++]=b;
            }
            else
            {
                noEncode=false;
                encoded[n++]=(byte)'%';
                byte nibble= (byte) ((b&0xf0)>>4);
                if (nibble>=10)
                    encoded[n++]=(byte)('A'+nibble-10);
                else
                    encoded[n++]=(byte)('0'+nibble);
                nibble= (byte) (b&0xf);
                if (nibble>=10)
                    encoded[n++]=(byte)('A'+nibble-10);
                else
                    encoded[n++]=(byte)('0'+nibble);
            }
        }

        if (noEncode)
            return string;

        try
        {
            return new String(encoded,0,n,"UTF-8");
        }
        catch(UnsupportedEncodingException e)
        {
            Log.w("UrlEncoded", e.getMessage());
            return new String(encoded,0,n);
        }
    }

    
    /** 
     * Decode String with % encoding
     */
    public static String decode(String encoded)
    {
        int len=encoded.length();
        char[] characters = encoded.toCharArray();
        byte[] bytes=new byte[len];
        int n=0;
        boolean noDecode=true;

        for (int i=0;i<len;i++)
        {
            char c = characters[i];
            if (c<0||c>0x7f)
                throw new IllegalArgumentException("Not encoded");

            byte b = (byte)(0x7f & c);

            if (c=='+')
            {
                noDecode=false;
                b=(byte)' ';
            }
            else if (c=='%' && (i+2)<len)
            {
                noDecode=false;
                b=(byte)(0xff&Integer.parseInt(encoded.substring(i+1,i+3),16));
                i+=2;
            }

            bytes[n++]=b;
        }

        if (noDecode)
            return encoded;

        try
        {
            return new String(bytes,0,n,"UTF-8");
        }
        catch(UnsupportedEncodingException e)
        {
            Log.w("UrlEncoded", e.getMessage());
            return new String(bytes,0,n);
        }

    }

    public Object getObject(Object key)
    {
        return super.get(key);
    }
}
