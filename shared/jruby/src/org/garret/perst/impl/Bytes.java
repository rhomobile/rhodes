package org.garret.perst.impl;

import org.garret.perst.StorageError;
import java.io.UnsupportedEncodingException;

//
// Class for packing/unpacking data
//
public class Bytes {
    public static short unpack2(byte[] arr, int offs) { 
        return (short)((arr[offs] << 8) | (arr[offs+1] & 0xFF));
    }
    public static int unpack4(byte[] arr, int offs) { 
        return (arr[offs] << 24) | ((arr[offs+1] & 0xFF) << 16)
            | ((arr[offs+2] & 0xFF) << 8) | (arr[offs+3] & 0xFF);
    }
    public static long unpack8(byte[] arr, int offs) { 
        return ((long)unpack4(arr, offs) << 32)
            | (unpack4(arr, offs+4) & 0xFFFFFFFFL);
    }
    public static float unpackF4(byte[] arr, int offs) { 
        return Float.intBitsToFloat(Bytes.unpack4(arr, offs));
    }
    public static double unpackF8(byte[] arr, int offs) { 
        return Double.longBitsToDouble(Bytes.unpack8(arr, offs));
    }

    public static void pack2(byte[] arr, int offs, short val) { 
        arr[offs] = (byte)(val >> 8);
        arr[offs+1] = (byte)val;
    }
    public static void pack4(byte[] arr, int offs, int val) { 
        arr[offs] = (byte)(val >> 24);
        arr[offs+1] = (byte)(val >> 16);
        arr[offs+2] = (byte)(val >> 8);
        arr[offs+3] = (byte)val;
    }
    public static void pack8(byte[] arr, int offs, long val) { 
        pack4(arr, offs, (int)(val >> 32));
        pack4(arr, offs+4, (int)val);
    }
    public static void packF4(byte[] arr, int offs, float val) { 
        pack4(arr, offs,  Float.floatToIntBits(val));
    }
    public static void packF8(byte[] arr, int offs, double val) { 
        pack8(arr, offs, Double.doubleToLongBits(val));
    }

    public static int sizeof(String str, String encoding) { 
        try { 
            return str == null ? 4 
                : encoding == null ? 4 + str.length()*2 : 4 + new String(str).getBytes(encoding).length;
        } catch (UnsupportedEncodingException x) { 
            throw new StorageError(StorageError.UNSUPPORTED_ENCODING);
        }
    }
    public static int sizeof(byte[] arr, int offs) { 
        int len = unpack4(arr, offs);        
        if (len >= 0) { 
            return 4 + len*2;
        } else if (len < -1) { 
            return 4-2-len;
        } else { 
            return 4;
        }
    }
}

