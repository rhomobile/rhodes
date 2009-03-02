package org.garret.perst;

import java.io.*;

/**
 * This stream can be used to import data in RMS from resource files.
 * Most of J2ME implementations fully loads in memory accessed resource files so
 * attempt to import large resource file may cause memory overflow.
 * This class allows to split database into several resources files
 * and join them when data is imported.
 * It is expected that resource files has numeric suffixes started with ".1"
 */
public class MultiresourceStream extends InputStream
{
    private String prefix;
    private int suffix;
    private InputStream currStream;

    private boolean openNextResource() throws IOException
    {    
        close();
        suffix += 1;
        currStream = MultiresourceStream.class.getResourceAsStream(prefix + '.' + suffix);
        return currStream != null;
    }
    
    public int read() throws IOException
    {
        int rc = currStream.read();
        if (rc < 0 && openNextResource()) {
            rc = currStream.read();
        }
        return rc;
    }

    public int read(byte b[], int off, int len) throws IOException 
    {
        int rc = currStream.read(b, off, len);
        if (rc <= 0 && openNextResource()) {
            rc = currStream.read(b, off, len);
        }
        return rc;
    }

    public void close() throws IOException 
    {
        if (currStream != null) {
            currStream.close();
            currStream = null;
        }
    }
    
    public MultiresourceStream(String resourcePrefix) throws IOException
    {
        prefix = resourcePrefix;
        openNextResource();
    }
}

    