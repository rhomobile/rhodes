package com.rhomobile.rhodes.http.util;

import java.io.IOException;
import java.util.Dictionary;
import java.util.Enumeration;

import android.util.Log;

/** 
 * URI wrapper
 */
@SuppressWarnings("unchecked")
public class URI {

	private String path;
    private String query;
    private UrlEncoded parameters = new UrlEncoded();
    private boolean modified=false;
    private boolean encodeNulls=false;
    
    /** Construct from a String can contain both a path and
     * encoded query parameters.
     * @param uri The uri path and optional encoded query parameters.
     */
    public URI(String uri)
    {
        path = uri;
        int q;
        if ((q=uri.indexOf('?'))>=0)
        {
            if ((q+1)<uri.length())
            {
                try{
                    query=uri.substring(q+1);
                    parameters.read(uri.substring(q+1));
                    path=uri.substring(0,q);
                }
                catch(IOException e){
                    Log.e(this.getClass().getSimpleName(), e.getMessage());
                }
            }
            else
                path=uri.substring(0,q);
        }
        path = UrlEncoded.decode(path);
    }
    
    
    /** 
     * Get the uri path
     */
    public String getPath()
    {
        return path;
    }
    
    /** 
     * Get the uri path
     */
    public void setPath(String path)
    {
        this.path=path;
    }
    
    /** 
     * Get the uri query String
     */
    public String getQuery()
    {
        if (modified)
            query = parameters.encode(encodeNulls);
        return query;
    }
    
    /** 
     * Set if this URI should encode nulls as an empty = clause
     */
    public void encodeNulls(boolean b)
    {
        this.encodeNulls=b;
    }
    
    /** 
     * Get the uri query parameters
     */
    public Dictionary queryContent()
    {
        return parameters;
    }
    
    /** 
     * Get the uri query parameters names
     */
    public Enumeration getParameterNames()
    {
        return parameters.keys();
    }
    
    /** 
     * Get the uri query parameters
     */
    public Dictionary getParameters()
    {
        modified=true;
        return parameters;
    }
    
    /** Clear the URI parameters
     */
    public void clearParameters()
    {
        modified=true;
        parameters.clear();
    }
    
    /** 
     * Add encoded parameters
     */
    public void put(String encoded)
    {
        try
        {
            UrlEncoded params = new UrlEncoded(encoded);
            put(params);
        }
        catch(IOException e){
            Log.e(this.getClass().getSimpleName(), e.getMessage());
        }
    }
    
    
    /** 
     * Add name value pair to the uri query parameters
     */
    public void put(String name, String value)
    {
        modified=true;
        if (name!=null && value!=null)
            parameters.put(name,value);
    }
    
    
    /** 
     * Add named multi values to the uri query parameters
     */
    public void put(String name, String[] values)
    {
        modified=true;
        if (name!=null && values!=null)
            parameters.putValues(name,values);
    }
    
    
    /** 
     * Add dictionary to the uri query parameters
     */
    public void put(Dictionary values)
    {
        modified=true;
        Enumeration keys= values.keys();
        while(keys.hasMoreElements())
        {
            Object key = keys.nextElement();
            parameters.put(key,values.get(key));
        }
    }

    
    /** Get named value 
     */
    public String get(String name)
    {
        return (String)parameters.get(name);
    }
    
    
    /** Get named multiple values
     */
    public String[] getValues(String name)
    {
        return parameters.getValues(name);
    }
    
    
    /** Remove named value 
     */
    public void remove(String name)
    {
        modified=true;
        parameters.remove(name);
    }
    
    
    /** @return the URI string encoded.
     */
    public String toString()
    {
        String result = encodePath( path );
        if (modified)
            getQuery();
        if (query!=null && query.length()>0)
            result+="?"+query;
        return result;
    }

    
    /**
     * Encode a URI path.
     */
    public static String encodePath(String path)
    {
        if (path==null || path.length()==0)
            return path;
        
        StringBuffer buf = new StringBuffer(path.length()<<1);
        encodePath(buf,path);
        return buf.toString();
    }
        
    
    /** 
     * Encode a URI path.
     */
    public static void encodePath(StringBuffer buf, String path)
    {
        synchronized(buf)
        {
            for (int i=0;i<path.length();i++)
            {
                char c=path.charAt(i);
                switch(c)
                {
                  case '%':
                      buf.append("%25");
                      continue;
                  case '?':
                      buf.append("%3F");
                      continue;
                  case '&':
                      buf.append("%26");
                      continue;
                  case ' ':
                      buf.append("%20");
                      continue;
                  default:
                      buf.append(c);
                      continue;
                }
            }
        }
    }
}
