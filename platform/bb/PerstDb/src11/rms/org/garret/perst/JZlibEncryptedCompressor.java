package org.garret.perst;

import com.jcraft.jzlib.*;
import java.io.*;

public class JZlibEncryptedCompressor implements ICompressor
{
    private byte[] initState;
    private byte[] state;
    private ZStream z;

    public JZlibEncryptedCompressor(String cipherKey) 
    { 
        byte[] key = cipherKey.getBytes();

        initState = new byte[256];
        state = new byte[256];
        z = new ZStream();

	for (int counter = 0; counter < 256; ++counter) { 
	    initState[counter] = (byte)counter;
        }
	int index1 = 0;
	int index2 = 0;
	for (int counter = 0; counter < 256; ++counter) {
	    index2 = (key[index1] + initState[counter] + index2) & 0xff;
	    byte temp = initState[counter];
	    initState[counter] = initState[index2];
	    initState[index2] = temp;
	    index1 = (index1 + 1) % key.length;
        }
    }

    private final void crypt(byte[] buf, int len)
    {
        int x = 0, y = 0;
        byte[] state = this.state;
        System.arraycopy(initState, 0, state, 0, state.length);
	for (int i = 0; i < len; i++) {
            x = (x + 1) & 0xff;
            y = (y + state[x]) & 0xff;
            byte temp = state[x];
            state[x] = state[y];
            state[y] = temp;
            int nextState = (state[x] + state[y]) & 0xff;
	    buf[i] ^= state[nextState];
        }
    }

    public int compress(byte[] dst, byte[] src, int srcSize) throws IOException {
        ZStream z = this.z;
        z.deflateInit(JZlib.Z_DEFAULT_COMPRESSION, false);
        z.next_in = src;
        z.next_in_index = 0;
        z.avail_in = srcSize;
        z.next_out = dst;
        z.next_out_index = 0;
        z.avail_out = srcSize;
        int err = z.deflate(JZlib.Z_FINISH);
        int dstSize;
        if (err != JZlib.Z_STREAM_END || z.avail_out == 0) {
            System.arraycopy(src, 0, dst, 0, srcSize);
            dstSize = srcSize;
        } else { 
            dstSize = srcSize - z.avail_out;
        }
        crypt(dst, dstSize);
        return dstSize;
        
    }

    public int decompress(byte[] dst, byte[] src, int srcSize) throws IOException {
        crypt(src, srcSize);
        if (srcSize == dst.length) { 
            System.arraycopy(src, 0, dst, 0, srcSize);
            return srcSize;
        } else { 
            ZStream z = this.z;
            z.inflateInit(false);
            z.next_in = src;
            z.next_in_index = 0;
            z.avail_in = srcSize;
            z.next_out = dst;
            z.next_out_index = 0;
            z.avail_out = dst.length;
            int err = z.inflate(JZlib.Z_FINISH);
            return err != JZlib.Z_STREAM_END ? -1 : dst.length - z.avail_out;
        }
    }
}