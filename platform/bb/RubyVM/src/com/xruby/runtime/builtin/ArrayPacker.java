/**
 * Copyright 2005-2007 Xue Yong Zhi, Jie Li
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
import j2me.lang.CharacterMe;

class ArrayPacker {
	
	public static final int Integer_SIZE = 32;
	public static final int Short_SIZE = 16;
	public static final int Byte_SIZE = 8;
	public static final int Long_SIZE = 64;
	
    //uv_to_utf8:
    //Copyright (C) 1993-2003 Yukihiro Matsumoto
    //TODO java may has library to do this.
    private static int uv_to_utf8(char[] buf, long uv) {
        if (uv <= 0x7f) {
        buf[0] = (char)uv;
        return 1;
        }
        if (uv <= 0x7ff) {
        buf[0] = (char)(((uv>>6)&0xff)|0xc0);
        buf[1] = (char)((uv&0x3f)|0x80);
        return 2;
        }
        if (uv <= 0xffff) {
        buf[0] = (char)(((uv>>12)&0xff)|0xe0);
        buf[1] = (char)(((uv>>6)&0x3f)|0x80);
        buf[2] = (char)((uv&0x3f)|0x80);
        return 3;
        }
        if (uv <= 0x1fffff) {
        buf[0] = (char)(((uv>>18)&0xff)|0xf0);
        buf[1] = (char)(((uv>>12)&0x3f)|0x80);
        buf[2] = (char)(((uv>>6)&0x3f)|0x80);
        buf[3] = (char)((uv&0x3f)|0x80);
        return 4;
        }
        if (uv <= 0x3ffffff) {
        buf[0] = (char)(((uv>>24)&0xff)|0xf8);
        buf[1] = (char)(((uv>>18)&0x3f)|0x80);
        buf[2] = (char)(((uv>>12)&0x3f)|0x80);
        buf[3] = (char)(((uv>>6)&0x3f)|0x80);
        buf[4] = (char)((uv&0x3f)|0x80);
        return 5;
        }
        if (uv <= 0x7fffffff) {
        buf[0] = (char)(((uv>>30)&0xff)|0xfc);
        buf[1] = (char)(((uv>>24)&0x3f)|0x80);
        buf[2] = (char)(((uv>>18)&0x3f)|0x80);
        buf[3] = (char)(((uv>>12)&0x3f)|0x80);
        buf[4] = (char)(((uv>>6)&0x3f)|0x80);
        buf[5] = (char)((uv&0x3f)|0x80);
        return 6;
        }
        throw new RubyException(RubyRuntime.RangeErrorClass, "pack(U): value out of range");
    }

    private static final long utf8_limits[] = {
        0x0,            /* 1 */
        0x80,           /* 2 */
        0x800,          /* 3 */
        0x10000,            /* 4 */
        0x200000,           /* 5 */
        0x4000000,          /* 6 */
        0x80000000,         /* 7 */
    };

    //utf8_to_uv:
    //Copyright (C) 1993-2003 Yukihiro Matsumoto
    private static long[] utf8_to_uv(String str, int p, long lenp) {
        int c = str.charAt(p++) & 0xff;
        long uv = c;
        long n;

        if (0 == (uv & 0x80)) {
            lenp = 1;
            return new long[] {uv, lenp};
        }
        if (0 == (uv & 0x40)) {
            lenp = 1;
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "malformed UTF-8 character");
        }

        if      (0 == (uv & 0x20)) { n = 2; uv &= 0x1f; }
        else if (0 == (uv & 0x10)) { n = 3; uv &= 0x0f; }
        else if (0 == (uv & 0x08)) { n = 4; uv &= 0x07; }
        else if (0 == (uv & 0x04)) { n = 5; uv &= 0x03; }
        else if (0 == (uv & 0x02)) { n = 6; uv &= 0x01; }
        else {
            lenp = 1;
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "malformed UTF-8 character");
        }
        if (n > lenp) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "malformed UTF-8 character (expected " + n + " bytes, given " + lenp + "bytes)");
        }
        lenp = n--;
        if (n != 0) {
        while (n-- != 0) {
            c = str.charAt(p++) & 0xff;
            if ((c & 0xc0) != 0x80) {
            lenp -= n + 1;
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "malformed UTF-8 character");
            }
            else {
            c &= 0x3f;
            uv = uv << 6 | c;
            }
        }
        }
        n = lenp - 1;
        if (uv < utf8_limits[(int)n]) {
        throw new RubyException(RubyRuntime.ArgumentErrorClass, "redundant UTF-8 sequence");
        }
        return new long[] {uv, lenp};
    }

    private static String qpencode(String str, int len) {
        throw new RubyException("Not implemented");
    }

    private static final String  uu_table = "`!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_";
    
    private static String encodes(String str, int charCount, char encodingType) 
    {
    	int startIndex = 0;
    	byte[] charsToEncode = str.getBytes();
    	
    	StringBuffer io2Append = new StringBuffer();
        io2Append.ensureCapacity(charCount * 4 / 3 + 6);
        
        int i = startIndex;
        byte[] lTranslationTable = (encodingType == 'u' ? uu_table : b64_table).getBytes();
        byte lPadding;
        if (encodingType == 'u') {
            if (charCount >= lTranslationTable.length) {
            	throw new RubyException(RubyRuntime.ArgumentErrorClass, 
            			"" + charCount + " is not a correct value for the number of bytes per line in a u directive.  Correct values range from 0 to " + lTranslationTable.length
            			);
            }
            io2Append.append(lTranslationTable[charCount]);
            lPadding = '`';
        } else {
            lPadding = '=';
        }
        while (charCount >= 3) {
            byte lCurChar = charsToEncode[i++];
            byte lNextChar = charsToEncode[i++];
            byte lNextNextChar = charsToEncode[i++];
            io2Append.append((char)(lTranslationTable[077 & (lCurChar >>> 2)]));
            io2Append.append((char)(lTranslationTable[077 & (((lCurChar << 4) & 060) | ((lNextChar >>> 4) & 017))]));
            io2Append.append((char)(lTranslationTable[077 & (((lNextChar << 2) & 074) | ((lNextNextChar >>> 6) & 03))]));
            io2Append.append((char)(lTranslationTable[077 & lNextNextChar]));
            charCount -= 3;
        }
        if (charCount == 2) {
            byte lCurChar = charsToEncode[i++];
            byte lNextChar = charsToEncode[i++];
            io2Append.append((char)(lTranslationTable[077 & (lCurChar >>> 2)]));
            io2Append.append((char)(lTranslationTable[077 & (((lCurChar << 4) & 060) | ((lNextChar >> 4) & 017))]));
            io2Append.append((char)(lTranslationTable[077 & (((lNextChar << 2) & 074) | (('\0' >> 6) & 03))]));
            io2Append.append((char)(lPadding));
        } else if (charCount == 1) {
            byte lCurChar = charsToEncode[i++];
            io2Append.append((char)(lTranslationTable[077 & (lCurChar >>> 2)]));
            io2Append.append((char)(lTranslationTable[077 & (((lCurChar << 4) & 060) | (('\0' >>> 4) & 017))]));
            io2Append.append((char)(lPadding));
            io2Append.append((char)(lPadding));
        }
        io2Append.append('\n');
    	
//        throw new RubyException("Not implemented");
        
        return io2Append.toString();
    }

    private static final String  b64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    private static final int[] b64_xtable = new int[256];
    private static final String sHexDigits = "0123456789abcdef0123456789ABCDEFx";
    
    static{
    	
	    // b64_xtable for decoding Base 64
	    for (int i = 0; i < 256; i++) {
	        b64_xtable[i] = -1;
	    }
	    for (int i = 0; i < 64; i++) {
	        b64_xtable[(int)b64_table.charAt(i)] = i;
	    }
    }
    
    public static RubyArray unpack(String str, String format) {
        int len;
        //int star;
        int s = 0;
        char type;
        final int send = str.length();

        RubyArray ary = new RubyArray();

        int p = 0;
        while (p < format.length()) {
            type = format.charAt(p++);

            if (type == ' ') continue;

            if (format.indexOf(p) == '#') {
                while (p < format.length() && format.charAt(p) != '\n') {
                    p++;
                }
                continue;
            }

            char t;
            if (p >= format.length())
                t = 0;
            else
                t = format.charAt(p);

            //star = 0;
            if (t == '_' || t == '!') {
                final String natstr = "sSiIlL";
                if (natstr.indexOf(type) >= 0) {
                    p++;
                } else {
                    throw new RubyException(RubyRuntime.ArgumentErrorClass, "'" + type + "' allowed only after types " + natstr);
                }
            }

            if (p > format.length()) {
                len = 1;
            } else if (t == '*') {
                //star = 1;
                len = send - s;
                p++;
            } else if (Character.isDigit(t)) {
                int end = p;
                while (end < format.length() - 1 && CharacterMe.isDigit(format.indexOf(end + 1))) {
                    end++;
                }
                len = Integer.parseInt(format.substring(p, end + 1), 10);
            } else {
                len = (type != '@') ? 1 : 0;
            }

            switch (type) {
                case '%':
                    throw new RubyException(RubyRuntime.ArgumentErrorClass, "%% is not supported");

                case 'a':
                    if (len > send - s) len = send - s;
                    ary.add(ObjectFactory.createString(str.substring(s, s + len)));
                    s += len;
                    break;

                case 'c':
                    while (len-- > 0) {
                        int c = str.charAt(s++);
                        if (c > 127)
                            c -= 256;
                        ary.add(ObjectFactory.createFixnum(c));
                    }
                    break;

                case 'C':
                    while (len-- > 0) {
                        char c = str.charAt(s++);
                        ary.add(ObjectFactory.createFixnum(c));
                    }
                    break;

                case 'H':
                {
                	int bits = 0;
                	StringBuffer lElem = new StringBuffer(len);
                	
                    for (int lCurByte = 0; lCurByte < len; lCurByte++) {
                        if ((lCurByte & 1) != 0)
                            bits <<= 4;
                        else
                            bits = str.charAt(s++);
                        
                        char c = sHexDigits.charAt((bits >>> 4) & 15);
                        lElem.append( c );
                    }
                    
                    ary.add(ObjectFactory.createString(lElem)); 
                    
                    break;
                }    
                case 's':
                    while (len-- > 0) {
                        short tmp = 0;
                        for (int j = 0; j < Short_SIZE / Byte_SIZE; ++j) {
                            char c = str.charAt(s++);
                            tmp += (c << (j * 8));
                        }
                        ary.add(ObjectFactory.createFixnum(tmp));
                    }

                    break;

                case 'i':
                case 'l':
                    while (len-- > 0) {
                        int tmp = 0;
                        for (int j = 0; j < Integer_SIZE / Byte_SIZE; ++j) {
                            char c = str.charAt(s++);
                            tmp += (c << (j * 8));
                        }
                        ary.add(ObjectFactory.createFixnum(tmp));
                    }

                    break;

                case 'q':
                    if (str.length() < Long_SIZE / Byte_SIZE) {
                        ary.add(RubyConstant.QNIL);
                    } else {
                        long l = 0;
                        for (int j = 0; j < Long_SIZE / Byte_SIZE; ++j) {
                            long c = str.charAt(s++);
                            l += (c << (j * 8));
                        }
                        ary.add(ObjectFactory.createInteger(l));
                    }
                    break;

                case 'x':
                    if (len > send - s)
                        throw new RubyException(RubyRuntime.ArgumentErrorClass, "x outside of string");
                    s += len;
                    break;

                case 'D':
                case 'd':
                    while (len-- > 0) {
                        long tmp = 0;
                        for (int j = 0; j < Long_SIZE / Byte_SIZE; ++j) {
                            long c = str.charAt(s++);
                            tmp += (c << (j * 8));
                        }
                        ary.add(ObjectFactory.createFloat(Double.longBitsToDouble(tmp)));
                    }
                    break;

                case 'U':
                    if (len > send - s) len = send - s;
                    while (len > 0 && s < send) {
                        long alen = send - s;
                        long l;

                        long[] r = utf8_to_uv(str, s, alen);
                        l = r[0];
                        alen = r[1];
                        s += alen; len--;
                        ary.add(ObjectFactory.createInteger(l));
                    }
                    break;

                case 'N':
                    while (len-- > 0 && s < send) {
                        long tmp = 0;
                        for (int j = Integer_SIZE / Byte_SIZE - 1; j >= 0 && s < send; --j) {
                            long c = str.charAt(s++);
                            tmp += (c << (j * 8));
                        }
                        ary.add(ObjectFactory.createInteger(tmp));
                    }
                    break;

                case 'm': //base64
                    int length = send*3/4;
                    StringBuffer lElem = new StringBuffer(length);
                    
                    int a = -1, b = -1, c = 0, d;
                    int s1 = -1;
                	
                    if (len > send - s) len = send - s;
                    while (len > 0 && s < send) {
                        a = b = c = d = -1;
                        
                        // obtain a
                        s1 = str.charAt(s++);
                        while (((a = b64_xtable[s1]) == -1) && s + 1 < send) {
                        	s1 = str.charAt(s++);
                        }
                        if (a == -1) break;
                        
                        // obtain b
                        s1 = str.charAt(s++);
                        while (((b = b64_xtable[s1]) == -1) && s + 1 < send ) {
                        	s1 = str.charAt(s++);
                        }
                        if (b == -1) break;
                        
                        // obtain c
                        s1 = str.charAt(s++);
                        while (((c = b64_xtable[s1]) == -1) && s + 1 < send) {
                            if (s1 == '=') break;
                            s1 = str.charAt(s++);
                        }
                        if ((s1 == '=') || c == -1) {
                            if (s1 == '=') {
                            	s--;
                                //encode.position(encode.position() - 1);
                            }
                            break;
                        }

                        // obtain d
                        if ( s < send )
                        	s1 = str.charAt(s++);
                        
                        while (((d = b64_xtable[s1]) == -1) && s + 1 < send) {
                            if (s1 == '=') break;
                            s1 = str.charAt(s++);
                        }
                        if ((s1 == '=') || d == -1) {
                            if (s1 == '=') {
                            	s--;
                                //encode.position(encode.position() - 1);
                            }
                            break;
                        }

                        // calculate based on a, b, c and d
                        lElem.append( (char)((a << 2 | b >> 4) & 255) );
                        lElem.append( (char)((b << 4 | c >> 2) & 255) );
                        lElem.append( (char)((c << 6 | d) & 255) );
                    }

                    if (a != -1 && b != -1) {
                        if (c == -1 && s1 == '=') {
                        	lElem.append( (char)((a << 2 | b >> 4) & 255) );
                        } else if(c != -1 && s1 == '=') {
                        	lElem.append( (char)((a << 2 | b >> 4) & 255) );
                        	lElem.append( (char)((b << 4 | c >> 2) & 255) );
                        }
                    }
                    
                    ary.add( ObjectFactory.createString(lElem) );
                    break;
                    
                default:
                    break;
            }
        }

        return ary;
    }

    public static StringBuffer pack(RubyArray array, String format) {
        int len = 0;
        int items = array.size();
        int idx = 0;
        int plen;
        String ptr;
        RubyValue from;

        StringBuffer result = new StringBuffer();

        int p = 0;
        while (p < format.length()) {
            char type = format.charAt(p++);

            if (type == ' ') continue;

            if (format.indexOf(p) == '#') {
                while (p < format.length() && format.charAt(p) != '\n') {
                    p++;
                }
                continue;
            }

            char t;
            if (p >= format.length())
                t = 0;
            else
                t = format.charAt(p);

            if (t == '_' || t == '!') {
                final String natstr = "sSiIlL";
                if (natstr.indexOf(type) >= 0) {
                    p++;
                } else {
                    throw new RubyException(RubyRuntime.ArgumentErrorClass, "'" + type + "' allowed only after types " + natstr );
                }
            }

            if (t == '*') {
                len = "@Xxu".indexOf(t) >= 0 ? 0 : items;
                p++;
            } else if (Character.isDigit(t)) {
                int end = p;
                while (end < format.length() - 1 && CharacterMe.isDigit(format.indexOf(end + 1))) {
                    end++;
                }
                len = Integer.parseInt(format.substring(p, end + 1), 10);
            } else {
                len = 1;
            }

            switch (type) {
                case 'A':
                case 'a':
                case 'Z':
                case 'B':
                case 'b':
                case 'H':
                case 'h':
                    if (items-- > 0)
                        from = array.get(idx++);
                    else
                        throw new RubyException(RubyRuntime.RuntimeErrorClass, "too few for type " + type); // #TODO: message

                    if (from == RubyConstant.QNIL) {
                        ptr = "";
                        plen = 0;
                    } else {
                        ptr = ((RubyString) from).toString();
                        plen = ptr.length();
                    }

                    if (format.charAt(p - 1) == '*') {
                        len = plen;
                    }

                    switch (type) {
                        case 'a':
                        case 'A':
                        case 'Z': {
                            if (plen >= len) {
                                result.append(ptr.substring(0, len));
                                if (format.charAt(p - 1) == '*' && type == 'Z')
                                    result.append('\0');
                            } else {
                                result.append(ptr.substring(0, plen));
                                len -= plen;
                                while (len-- > 0) {
                                    result.append(' ');
                                }
                            }
                        }
                        break;

                        case 'b': {
                            int byte_ = 0;
                            int i, j = 0;

                            if (len > plen) {
                                j = (len - plen + 1) / 2;
                                len = plen;
                            }

                            int pos1 = 0;
                            for (i = 0; i++ < len; pos1++) {
                                if ((ptr.charAt(pos1) & 1) != 0) {
                                    byte_ |= 128;
                                }

                                if ((i & 7) != 0) {
                                    byte_ >>= 1;
                                } else {
                                    char c = (char) (byte_ & 0xff);
                                    result.append(c);
                                    byte_ = 0;
                                }

                                if ((len & 7) != 0) {
                                    char c;
                                    byte_ >>= 7 - (len & 7);
                                    c = (char) (byte_ & 0xff);
                                    result.append(c);
                                }
                                len = j;
                                while (len-- > 0) {
                                    result.append('\0');
                                }
                            }
                        }
                        break;

                        case 'B': {
                            int byte_ = 0;
                            int i, j = 0;

                            if (len > plen) {
                                j = (len - plen + 1) / 2;
                                len = plen;
                            }

                            int pos1 = 0;
                            for (i = 0; i++ < len; pos1++) {
                                byte_ |= (ptr.charAt(pos1) & 1);
                                if ((i & 7) != 0)
                                    byte_ <<= 1;
                                else {
                                    char c = (char) (byte_ & 0xff);
                                    result.append(c);
                                    byte_ = 0;
                                }
                            }

                            if ((len & 7) != 0) {
                                char c;
                                byte_ <<= 7 - (len & 7);
                                c = (char) (byte_ & 0xff);
                                result.append(c);
                            }

                            len = j;
                            while (len-- > 0) {
                                result.append('\0');
                            }
                        }
                        break;

                        case 'h': {
                            int byte_ = 0;
                            int i, j = 0;
                            if (len > plen) {
                                j = (len - plen + 1) / 2;
                                len = plen;
                            }

                            int pos1 = 0;
                            for (i = 0; i++ < len; pos1++) {
                                if (CharacterMe.isLetter(ptr.charAt(pos1))) {
                                    byte_ |= (((ptr.charAt(pos1) & 15) + 9) & 15) << 4;
                                } else {
                                    byte_ |= (ptr.charAt(pos1) & 15) << 4;
                                }

                                if ((i & 1) != 0) {
                                    byte_ >>= 4;
                                } else {
                                    char c = (char) (byte_ & 0xff);
                                    result.append(c);
                                    byte_ = 0;
                                }
                            }

                            if ((len & 1) != 0) {
                                char c = (char) (byte_ & 0xff);
                                result.append(c);
                            }

                            len = j;
                            while (len-- > 0) {
                                result.append('\0');
                            }
                        }
                        break;

                        case 'H': {
                            int byte_ = 0;
                            int i, j = 0;

                            if (len > plen) {
                                j = (len - plen + 1) / 2;
                                len = plen;
                            }

                            int pos1 = 0;
                            for (i = 0; i++ < len; pos1++) {
                                if (CharacterMe.isLetter(ptr.charAt(pos1))) {
                                    byte_ |= ((ptr.charAt(pos1) & 15) + 9) & 15;
                                } else {
                                    byte_ |= ptr.charAt(pos1) & 15;
                                }

                                if ((i & 1) != 0) {
                                    byte_ <<= 4;
                                } else {
                                    char c = (char) (byte_ & 0xff);
                                    result.append(c);
                                    byte_ = 0;
                                }
                            }

                            if ((len & 1) != 0) {
                                char c = (char) (byte_ & 0xff);
                                result.append(c);
                            }

                            len = j;
                            while (len-- > 0) {
                                result.append('\0');
                            }
                        }
                        break;
                    }
                    break;

                case 'c':
                case 'C':
                    while (len-- > 0) {
                        if (items-- > 0)
                            from = array.get(idx++);
                        else
                            throw new RubyException(RubyRuntime.RuntimeErrorClass, "too few for type " + type); // #TODO: message

                        int i = from.toInt();
                        result.append((char) (i & 0xff));
                    }
                    break;

                case 's':
                case 'S':
                    while (len-- > 0) {


                        if (items-- > 0)
                            from = array.get(idx++);
                        else
                            throw new RubyException(RubyRuntime.RuntimeErrorClass, "too few for type " + type); // #TODO: message

                        int i = from.toInt();
                        for (int j = 0; j < Short_SIZE / Byte_SIZE; ++j) {
                            result.append((char) ((i >> (j * 8) & 0xff)));
                        }
                    }
                    break;

                case 'i':
                case 'I':
                case 'l':
                case 'L':
                    while (len-- > 0) {
                        if (items-- > 0)
                            from = array.get(idx++);
                        else
                            throw new RubyException(RubyRuntime.RuntimeErrorClass, "too few for type " + type); // #TODO: message

                        int i = from.toInt();
                        for (int j = 0; j < Integer_SIZE / Byte_SIZE; ++j) {
                            result.append((char) ((i >> (j * 8) & 0xff)));
                        }
                    }
                    break;

                case 'q':// signed quad (64bit) int
                case 'Q'://unsigned quad (64bit) int
                    while (len-- > 0) {
                        if (items-- > 0)
                            from = array.get(idx++);
                        else
                            throw new RubyException(RubyRuntime.RuntimeErrorClass, "too few for type " + type); // #TODO: message

                        long l = RubyTypesUtil.convertToJavaLong(from);

                        for (int i = 0; i < Long_SIZE / Byte_SIZE; ++i) {
                            result.append((char) ((l >> (i * 8) & 0xff)));
                        }
                    }
                    break;

                case 'n': // short (network byte-order)
                    break;

                case 'N': // int (network byte-order)
                    while (len-- > 0) {
                        if (items-- > 0)
                            from = array.get(idx++);
                        else
                            throw new RubyException(RubyRuntime.RuntimeErrorClass, "too few for type " + type); // #TODO: message

                        long l = RubyTypesUtil.convertToJavaLong(from);

                        for (int i = Integer_SIZE / Byte_SIZE - 1; i >= 0 ; --i) {
                            result.append((char) ((l >> (i * 8) & 0xff)));
                        }
                    }
                    break;

                case 'v': // short (VAX byte-order)
                    break;

                case 'V': // long (VAX byte-order)
                    break;

                case 'f':
                case 'F':
                    while (len-- > 0) {
                        float f;

                        if (items-- > 0)
                            from = array.get(idx++);
                        else
                            throw new RubyException(RubyRuntime.RuntimeErrorClass, "too few for type " + type); // #TODO: message

                        if (from instanceof RubyFixnum) {
                            f = (long) (((RubyFixnum) from).toLong() & 0xffffffff);
                        } else if (from instanceof RubyFloat) {
                            f = ((long) ((RubyFloat) from).toFloat() & 0xffffffffffffffffL);
                        } else if (from instanceof RubyBignum) {
                            f = (((RubyBignum) from).getInternal().longValue() & 0xffffffffffffffffL);
                        } else {
                            throw new RubyException(RubyRuntime.RuntimeErrorClass, "can't convert " + from.getRubyClass().getName() + " into Integer" );
                        }

                        int bits = Float.floatToIntBits(f);
                        for (int i = 0; i < Integer_SIZE / Byte_SIZE; ++i) {
                            result.append((char) ((bits >> (i * 8) & 0xff)));
                        }
                    }
                    break;

                case 'e': // single precision float in VAX byte-order
                    break;

                case 'E': // double precision float in VAX byte-order
                    break;

                case 'd':
                case 'D':
                    while (len-- > 0) {
                        if (items-- > 0)
                            from = array.get(idx++);
                        else
                            throw new RubyException(RubyRuntime.RuntimeErrorClass, "too few for type " + type); // #TODO: message

                        double d = RubyTypesUtil.convertToJavaDouble(from);

                        long bits = Double.doubleToLongBits(d);

                        for (int i = 0; i < Long_SIZE / Byte_SIZE; ++i) {
                            result.append((char) ((bits >> (i * 8) & 0xff)));
                        }
                    }
                    break;

                case 'g': // single precision float in network byte-order
                    break;

                case 'G': // double precision float in network byte-order
                    break;

                case 'x': // null byte
                    while (len-- > 0) {
                        result.append('\0');
                    }
                    break;

                case 'X': // back up byte
                    plen = result.length();
                    if (plen < len) {
                        throw new RubyException(RubyRuntime.ArgumentErrorClass, "X outside of string");
                    }
                    result.delete(plen - len, plen);
                    break;

                case '@':
                    len -= result.length();
                    if (len > 0) {
                        while (len-- > 0) {
                            result.append('\0');
                        }
                    }

                    len = -len;
                    if (len > 0) {
                        plen = result.length();
                        if (plen < len) {
                            throw new RubyException(RubyRuntime.ArgumentErrorClass, "X outside of string");
                        }
                        result.delete(plen - len, plen);
                    }
                    break;

                case '%':
                    throw new RubyException(RubyRuntime.ArgumentErrorClass, "% is not supported");

                case 'U':
                    while (len-- > 0) {
                        char[] buf = new char[8];

                        from = array.get(idx++);
                        long l = RubyTypesUtil.convertToJavaLong(from);
                        if (l < 0) {
                            throw new RubyException(RubyRuntime.RangeErrorClass, "pack(U): value (" + from + ") out of range");
                        }
                        int le = uv_to_utf8(buf, l);
                        result.append(buf, 0, le);
                    }
                    break;

                case 'u': // uuencoded string
                case 'm': // base64 encoded string
                    from = array.get(idx++);
                    ptr = from.toString();
                    plen = ptr.length();

                    if (len <= 2) {
                        len = 45;
                    } else {
                        len = len / 3 * 3;
                    }

                    while (plen > 0) {
                        int todo;
                        if (plen > len) {
                            todo = len;
                        } else {
                            todo = plen;
                        }

                        result.append(encodes(ptr, todo, type));
                        plen -= todo;
                        ptr += todo;
                    }
                    break;

                case 'M': // quoted-printable encoded string
                {
                    String str = ((RubyString) array.get(idx++)).toString();
                    if (len <= 1) {
                        len = 72;
                    }

                    result.append(qpencode(str, len));
                }
                break;

                case 'P': // pointer to packed byte string
                case 'p': // pointer to string
                    throw new RubyException("Not implemented");

                case 'w': // BER compressed integer
                    break;

                default:
                    break;
            }
        }

        return result;
    }
}
