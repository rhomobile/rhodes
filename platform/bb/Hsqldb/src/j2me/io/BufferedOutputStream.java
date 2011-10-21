package j2me.io;

import java.io.OutputStream;
import java.io.IOException;

public class BufferedOutputStream extends FilterOutputStream{

    public BufferedOutputStream(OutputStream out, int size) {
/*    	super(out);
        if (size <= 0) {
            throw new IllegalArgumentException("Buffer size <= 0");
        }
    	buf = new byte[size];*/
    }

    public synchronized void write(int b) throws IOException {
    /*	if (count >= buf.length) {
    	    flushBuffer();
    	}
    	buf[count++] = (byte)b;*/
    }

    public synchronized void write(byte b[], int off, int len) throws IOException {
    	/*if (len >= buf.length) {
    	    flushBuffer();
    	    out.write(b, off, len);
    	    return;
    	}
    	if (len > buf.length - count) {
    	    flushBuffer();
    	}
    	System.arraycopy(b, off, buf, count, len);
    	count += len;*/
    }

}
