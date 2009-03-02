package org.garret.perst;

/** 
 * Convert different type of keys to 64-bit long value used in PATRICIA trie 
 * (Practical Algorithm To Retrieve Information Coded In Alphanumeric)
 */
public class PatriciaTrieKey {
    /**
     * Bit mask representing bit vector.
     * The last digit of the key is the right most bit of the mask
     */
    public final long mask;

    /**
     * Length of bit vector (can not be larger than 64)
     */
    public final int  length;

    public PatriciaTrieKey(long mask, int length) { 
        this.mask = mask;
        this.length = length;
    }

    public static PatriciaTrieKey fromIpAddress(String addr) throws NumberFormatException { 
        long mask = 0;
        int pos = 0;
        int len = 0;
        do { 
            int dot = addr.indexOf('.', pos);
            String part = dot < 0 ? addr.substring(pos) : addr.substring(pos, dot);
            pos = dot + 1;
            int b = Integer.parseInt(part, 10);
            mask = (mask << 8) | (b & 0xFF);
            len += 8;
        } while (pos > 0);
        return new PatriciaTrieKey(mask, len);
    }

    public static PatriciaTrieKey fromDecimalDigits(String digits) { 
        long mask = 0;
        int n = digits.length();
        Assert.that(n <= 16);
        for (int i = 0; i < n; i++) {
            char ch = digits.charAt(i);
            Assert.that(ch >= '0' && ch <= '9');
            mask = (mask << 4) | (ch - '0');
        }
        return new PatriciaTrieKey(mask, n*4);
    }    

    public static PatriciaTrieKey from7bitString(String str) { 
        long mask = 0;
        int n = str.length();
        Assert.that(n*7 <= 64);
        for (int i = 0; i < n; i++) {
            char ch = str.charAt(i);
            mask = (mask << 7) | (ch & 0x7F);
        }
        return new PatriciaTrieKey(mask, n*7);
    }

    public static PatriciaTrieKey from8bitString(String str) { 
        long mask = 0;
        int n = str.length();
        Assert.that(n <= 8);
        for (int i = 0; i < n; i++) {
            char ch = str.charAt(i);
            mask = (mask << 8) | (ch & 0xFF);
        }
        return new PatriciaTrieKey(mask, n*8);
    }    

    public static PatriciaTrieKey fromByteArray(byte[] arr) { 
        long mask = 0;
        int n = arr.length;
        Assert.that(n <= 8);
        for (int i = 0; i < n; i++) {
            mask = (mask << 8) | (arr[i] & 0xFF);
        }
        return new PatriciaTrieKey(mask, n*8);
    }
}