package com.rhomobile.rhodes.http;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;

import android.util.Log;

/** 
 * HTTP input stream
 */
public class HttpInputStream extends InputStream
{
    public static int MAX_LINE_LENGTH=8192;
    
    private BufferedInputStream in;
    private int chunksize=0;
    private boolean chunking=false;
    private int contentLength=-1;

    /** 
     * Buffer for readLine.
     */
    static class CharBuffer
    {
        char[] chars = new char[128];
        int size=0;
    };
    CharBuffer charBuffer = new CharBuffer();
    
    /** 
     * Constructor
     */
    public HttpInputStream( InputStream in)
    {
        this.in = new BufferedInputStream(in);
    }

    /** 
     * @param chunking 
     */
    public void chunking(boolean chunking)
    {
        this.chunking=chunking;
    }

    /** 
     * Set the content length.
     */
    public void setContentLength(int len)
    {
        contentLength=len;
    }
    
    /** 
     * Get the content length.
     */
    int getContentLength()
    {
        return contentLength;
    }

    /** 
     * Read a line ended by CR or CRLF or LF.
     */
    public String readLine() throws IOException
    {
        CharBuffer buf = readCharBufferLine();
        if (buf==null)
            return null;
        return new String(buf.chars,0,buf.size);
    }
    
    /** 
     * Read a line ended by CR or CRLF or LF.
     */
    CharBuffer readCharBufferLine() throws IOException
    {
        BufferedInputStream in = this.in;
        
        int room = charBuffer.chars.length;
        charBuffer.size=0;
        int c=0;  
        boolean cr = false;

    LineLoop:
        while (charBuffer.size<MAX_LINE_LENGTH &&
               (c=chunking?read():in.read())!=-1)
        {
            switch(c)
            {
              case 10:
                  break LineLoop;
        
              case 13:
                  cr = true;
                  if (!chunking)
                      in.mark(2);
                  break;
        
              default:
                  if(cr)
                  {
                      if (chunking)
                          Log.w(this.getClass().getSimpleName(), "Cannot handle CR in chunking mode");
                      
                      in.reset();
                      break LineLoop;
                  }
                  else
                  {
                      if (--room < 0)
                      {
                          int newLength = charBuffer.chars.length << 1;
                          newLength = Math.min(MAX_LINE_LENGTH,newLength);
                          char[] old = charBuffer.chars;
                          charBuffer.chars =new char[newLength];
                          room = charBuffer.chars.length-charBuffer.size-1;
                          System.arraycopy(old,0,charBuffer.chars,0,charBuffer.size);
                      }
                      charBuffer.chars[charBuffer.size++] = (char) c;
                  }
                  break;
            }    
        }

        if (c==-1 && charBuffer.size==0)
            return null;

        return charBuffer;
    }
    
    public int read() throws IOException
    {
        if (chunking)
        {   
            int b=-1;
            if (chunksize<=0 && getChunkSize()<=0)
                return -1;
            b=in.read();
            chunksize=(b<0)?-1:(chunksize-1);
            return b;
        }

        if (contentLength==0)
            return -1;
        int b=in.read();
        if (contentLength>0)
            contentLength--;
        return b;
    }
 
    public int read(byte b[]) throws IOException
    {
        int len = b.length;
    
        if (chunking)
        {   
            if (chunksize<=0 && getChunkSize()<=0)
                return -1;
            if (len > chunksize)
                len=chunksize;
            len=in.read(b,0,len);
            chunksize=(len<0)?-1:(chunksize-len);
        }
        else
        {
            if (contentLength==0)
                return -1;
            if (len>contentLength && contentLength>=0)
                len=contentLength;
            len=in.read(b,0,len);
            if (contentLength>0 && len>0)
                contentLength-=len;
        }

        return len;
    }
 
    public int read(byte b[], int off, int len) throws IOException
    {
        if (chunking)
        {   
            if (chunksize<=0 && getChunkSize()<=0)
                return -1;
            if (len > chunksize)
                len=chunksize;
            len=in.read(b,off,len);
            chunksize=(len<0)?-1:(chunksize-len);
        }
        else
        {
            if (contentLength==0)
                return -1;
            if (len>contentLength && contentLength>=0)
                len=contentLength;
            len=in.read(b,off,len);
            if (contentLength>0 && len>0)
                contentLength-=len;
        }

        return len;
    }
    
    public long skip(long len) throws IOException
    {
        if (chunking)
        {   
            if (chunksize<=0 && getChunkSize()<=0)
                return -1;
            if (len > chunksize)
                len=chunksize;
            len=in.skip(len);
            chunksize=(len<0)?-1:(chunksize-(int)len);
        }
        else
        {
            len=in.skip(len);
            if (contentLength>0 && len>0)
                contentLength-=len;
        }
        return len;
    }

    /** 
     * Available bytes to read without blocking.
     */
    public int available() throws IOException
    {
        if (chunking)
        {
            int len = in.available();
            if (len<=chunksize)
                return len;
            return chunksize;
        }
        
        return in.available();
    }
 
    /** 
     * Close stream.
     */
    public void close() throws IOException
    {
        chunksize=-1;
    }

    private int getChunkSize() throws IOException
    {
        if (chunksize<0)
            return -1;
        
        chunksize=-1;

        // Get next non blank line
        chunking=false;
        String line=readLine();
        while(line!=null && line.length()==0)
            line=readLine();
        chunking=true;
        
        // Handle early EOF or error in format
        if (line==null)
            return -1;
        
        // Get chunksize
        int i=line.indexOf(';');
        if (i>0)
            line=line.substring(0,i).trim();

        chunksize = Integer.parseInt(line,16);
        
        // check for EOF
        if (chunksize==0)
        {
            chunksize=-1;
            chunking=false;
        }

        return chunksize;
    }
};
