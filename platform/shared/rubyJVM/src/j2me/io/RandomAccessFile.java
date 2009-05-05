package j2me.io;

import java.io.*;
import j2me.nio.channels.*;

public class RandomAccessFile  
{
    public RandomAccessFile(String name, String mode)
        throws FileNotFoundException
    {
        this(name != null ? new File(name) : null, mode);
    }
        
    public RandomAccessFile(File file, String mode)
        throws FileNotFoundException
    {
    }
    
    public void setLength(long newLength) throws IOException
    {
        //TODO: setLength
        throw new RuntimeException("Not Implemented");
    }
    public void seek(long pos) throws IOException
    {
        //TODO: seek
        throw new RuntimeException("Not Implemented");
    }
    public long length() throws IOException
    {
        //TODO: length
        throw new RuntimeException("Not Implemented");
        //return 0;
    }
    public long getFilePointer() throws IOException
    {
        //TODO: getFilePointer
        throw new RuntimeException("Not Implemented");
        //return 0;
    }
    public void close() throws IOException {
        //TODO: close
        throw new RuntimeException("Not Implemented");
    }
    public final FileChannel getChannel() {
        return new FileChannel();
    }
    public final void writeBytes(String s) throws IOException {
                throw new IOException();
                //TODO: writeBytes
    }
    public int read(byte b[]) throws IOException {
                throw new IOException();
                //TODO: read
    }
    public final String readLine() throws IOException {
                throw new IOException();
                //TODO: readLine
        
    }
}
