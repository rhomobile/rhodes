package j2me.math;

import j2me.lang.Comparable;
import j2me.lang.Number;
import j2me.lang.MathEx;

public class HugeInt extends Number implements Comparable {
    int signum;
    int[] mag;
    private int bitLength = -1;
    private int firstNonzeroIntNum = -2;
    private final static long LONG_MASK = 0xffffffffL;
    
    private HugeInt(int[] val) {
        if (val.length == 0)
            throw new NumberFormatException("Zero length BigInteger");

        if (val[0] < 0) {
            mag = makePositive(val);
            signum = -1;
        } else {
            mag = trustedStripLeadingZeroInts(val);
            signum = (mag.length == 0 ? 0 : 1);
        }
    }

    public HugeInt(int signum, byte[] magnitude) {
        this.mag = stripLeadingZeroBytes(magnitude);

        if (signum < -1 || signum > 1)
            throw(new NumberFormatException("Invalid signum value"));

        if (this.mag.length==0) {
            this.signum = 0;
        } else {
            if (signum == 0)
                throw(new NumberFormatException("signum-magnitude mismatch"));
            this.signum = signum;
        }
    }

    public HugeInt(String val, int radix) {
        int cursor = 0, numDigits;
        int len = val.length();

        if (radix < Character.MIN_RADIX || radix > Character.MAX_RADIX)
            throw new NumberFormatException("Radix out of range");
        if (val.length() == 0)
            throw new NumberFormatException("Zero length BigInteger");

        signum = 1;
        int index = val.lastIndexOf('-');
        if (index != -1) {
            if (index == 0) {
                if (val.length() == 1)
                    throw new NumberFormatException("Zero length BigInteger");
                signum = -1;
                cursor = 1;
            } else {
                throw new NumberFormatException("Illegal embedded minus sign");
            }
        }

        while (cursor < len &&
               Character.digit(val.charAt(cursor),radix) == 0)
            cursor++;
        if (cursor == len) {
            signum = 0;
            mag = ZERO.mag;
            return;
        } else {
            numDigits = len - cursor;
        }

        int numBits = (int)(((numDigits * bitsPerDigit[radix]) >>> 10) + 1);
        int numWords = (numBits + 31) /32;
        mag = new int[numWords];

        int firstGroupLen = numDigits % digitsPerInt[radix];
        if (firstGroupLen == 0)
            firstGroupLen = digitsPerInt[radix];
        String group = val.substring(cursor, cursor += firstGroupLen);
        mag[mag.length - 1] = Integer.parseInt(group, radix);
        if (mag[mag.length - 1] < 0)
            throw new NumberFormatException("Illegal digit");
        
        int superRadix = intRadix[radix];
        int groupVal = 0;
        while (cursor < val.length()) {
            group = val.substring(cursor, cursor += digitsPerInt[radix]);
            groupVal = Integer.parseInt(group, radix);
            if (groupVal < 0)
                throw new NumberFormatException("Illegal digit");
            destructiveMulAdd(mag, superRadix, groupVal);
        }
        mag = trustedStripLeadingZeroInts(mag);
    }

    HugeInt(char[] val) {
        int cursor = 0, numDigits;
        int len = val.length;

        signum = 1;
        if (val[0] == '-') {
            if (len == 1)
                throw new NumberFormatException("Zero length BigInteger");
            signum = -1;
            cursor = 1;
        }

        while (cursor < len && Character.digit(val[cursor], 10) == 0)
            cursor++;
        if (cursor == len) {
            signum = 0;
            mag = ZERO.mag;
            return;
        } else {
            numDigits = len - cursor;
        }

        int numWords;
        if (len < 10) {
            numWords = 1;
        } else {    
            int numBits = (int)(((numDigits * bitsPerDigit[10]) >>> 10) + 1);
            numWords = (numBits + 31) /32;
        }
        mag = new int[numWords];
 
        int firstGroupLen = numDigits % digitsPerInt[10];
        if (firstGroupLen == 0)
            firstGroupLen = digitsPerInt[10];
        mag[mag.length-1] = parseInt(val, cursor,  cursor += firstGroupLen);
        
        while (cursor < len) {
            int groupVal = parseInt(val, cursor, cursor += digitsPerInt[10]);
            destructiveMulAdd(mag, intRadix[10], groupVal);
        }
        mag = trustedStripLeadingZeroInts(mag);
    }

    private int parseInt(char[] source, int start, int end) {
        int result = Character.digit(source[start++], 10);
        if (result == -1)
            throw new NumberFormatException(new String(source));

        for (int index = start; index<end; index++) {
            int nextVal = Character.digit(source[index], 10);
            if (nextVal == -1)
                throw new NumberFormatException(new String(source));
            result = 10*result + nextVal;
        }

        return result;
    }

    private static long bitsPerDigit[] = { 0, 0,
        1024, 1624, 2048, 2378, 2648, 2875, 3072, 3247, 3402, 3543, 3672,
        3790, 3899, 4001, 4096, 4186, 4271, 4350, 4426, 4498, 4567, 4633,
        4696, 4756, 4814, 4870, 4923, 4975, 5025, 5074, 5120, 5166, 5210,
                                           5253, 5295};

    private static void destructiveMulAdd(int[] x, int y, int z) {
        long ylong = y & LONG_MASK;
        long zlong = z & LONG_MASK;
        int len = x.length;

        long product = 0;
        long carry = 0;
        for (int i = len-1; i >= 0; i--) {
            product = ylong * (x[i] & LONG_MASK) + carry;
            x[i] = (int)product;
            carry = product >>> 32;
        }

        long sum = (x[len-1] & LONG_MASK) + zlong;
        x[len-1] = (int)sum;
        carry = sum >>> 32;
        for (int i = len-2; i >= 0; i--) {
            sum = (x[i] & LONG_MASK) + carry;
            x[i] = (int)sum;
            carry = sum >>> 32;
        }
    }

    public HugeInt(String val) {
        this(val, 10);
    }

    private HugeInt(int[] magnitude, int signum) {
        this.signum = (magnitude.length==0 ? 0 : signum);
        this.mag = magnitude;
    }

    HugeInt(HugeIntHelper val, int sign) {
        if (val.offset > 0 || val.value.length != val.intLen) {
            mag = new int[val.intLen];
            for(int i=0; i<val.intLen; i++)
                mag[i] = val.value[val.offset+i];
        } else {
            mag = val.value;
        }

        this.signum = (val.intLen == 0) ? 0 : sign;
    }

    public static HugeInt valueOf(long val) {
    	if (val == 0)
    	    return ZERO;
    	if (val > 0 && val <= MAX_CONSTANT)
    	    return posConst[(int) val];
    	else if (val < 0 && val >= -MAX_CONSTANT)
    	    return negConst[(int) -val];

    	return new HugeInt(val);
    }

    private HugeInt(long val) {
        if (val < 0) {
            signum = -1;
            val = -val;
        } else {
            signum = 1;
        }

        int highWord = (int)(val >>> 32);
        if (highWord==0) {
            mag = new int[1];
            mag[0] = (int)val;
        } else {
            mag = new int[2];
            mag[0] = highWord;
            mag[1] = (int)val;
        }
    }

    private static HugeInt valueOf(int val[]) {
        return (val[0]>0 ? new HugeInt(val, 1) : new HugeInt(val));
    }

    private final static int MAX_CONSTANT = 16;
    private static HugeInt posConst[] = new HugeInt[MAX_CONSTANT+1];
    private static HugeInt negConst[] = new HugeInt[MAX_CONSTANT+1];
    static {
		for (int i = 1; i <= MAX_CONSTANT; i++) {
		    int[] magnitude = new int[1];
		    magnitude[0] = (int) i;
		    posConst[i] = new HugeInt(magnitude,  1);
		    negConst[i] = new HugeInt(magnitude, -1);
		}
    }

    public static final HugeInt ZERO = new HugeInt(new int[0], 0);
    public static final HugeInt ONE = valueOf(1);
    public static final HugeInt TEN = valueOf(10);
    
    public HugeInt add(HugeInt val) {
        int[] resultMag;
	if (val.signum == 0)
            return this;
	if (signum == 0)
	    return val;
	if (val.signum == signum)
            return new HugeInt(add(mag, val.mag), signum);

        int cmp = intArrayCmp(mag, val.mag);
        if (cmp==0)
            return ZERO;
        resultMag = (cmp>0 ? subtract(mag, val.mag)
                           : subtract(val.mag, mag));
        resultMag = trustedStripLeadingZeroInts(resultMag);

        return new HugeInt(resultMag, cmp*signum);
    }

    private static int[] add(int[] x, int[] y) {
        if (x.length < y.length) {
            int[] tmp = x;
            x = y;
            y = tmp;
        }

        int xIndex = x.length;
        int yIndex = y.length;
        int result[] = new int[xIndex];
        long sum = 0;

        while(yIndex > 0) {
            sum = (x[--xIndex] & LONG_MASK) + 
                  (y[--yIndex] & LONG_MASK) + (sum >>> 32);
            result[xIndex] = (int)sum;
        }

        boolean carry = (sum >>> 32 != 0);
        while (xIndex > 0 && carry)
            carry = ((result[--xIndex] = x[xIndex] + 1) == 0);

        while (xIndex > 0)
            result[--xIndex] = x[xIndex];

        if (carry) {
            int newLen = result.length + 1;
            int temp[] = new int[newLen];
            for (int i = 1; i<newLen; i++)
                temp[i] = result[i-1];
            temp[0] = 0x01;
            result = temp;
        }
        return result;
    }

    public HugeInt subtract(HugeInt val) {
        int[] resultMag;
	if (val.signum == 0)
            return this;
	if (signum == 0)
	    return val.negate();
	if (val.signum != signum)
            return new HugeInt(add(mag, val.mag), signum);

        int cmp = intArrayCmp(mag, val.mag);
        if (cmp==0)
            return ZERO;
        resultMag = (cmp>0 ? subtract(mag, val.mag)
                           : subtract(val.mag, mag));
        resultMag = trustedStripLeadingZeroInts(resultMag);
        return new HugeInt(resultMag, cmp*signum);
    }

    private static int[] subtract(int[] big, int[] little) {
        int bigIndex = big.length;
        int result[] = new int[bigIndex];
        int littleIndex = little.length;
        long difference = 0;

        // Subtract common parts of both numbers
        while(littleIndex > 0) {
            difference = (big[--bigIndex] & LONG_MASK) - 
                         (little[--littleIndex] & LONG_MASK) +
                         (difference >> 32);
            result[bigIndex] = (int)difference;
        }

        boolean borrow = (difference >> 32 != 0);
        while (bigIndex > 0 && borrow)
            borrow = ((result[--bigIndex] = big[bigIndex] - 1) == -1);

        while (bigIndex > 0)
            result[--bigIndex] = big[bigIndex];

        return result;
    }

    public HugeInt multiply(HugeInt val) {
        if (signum == 0 || val.signum==0)
	    return ZERO;
        
        int[] result = multiplyToLen(mag, mag.length, 
                                     val.mag, val.mag.length, null);
        result = trustedStripLeadingZeroInts(result);
        return new HugeInt(result, signum*val.signum);
    }

    private int[] multiplyToLen(int[] x, int xlen, int[] y, int ylen, int[] z) {
        int xstart = xlen - 1;
        int ystart = ylen - 1;

        if (z == null || z.length < (xlen+ ylen))
            z = new int[xlen+ylen];

        long carry = 0;
        for (int j=ystart, k=ystart+1+xstart; j>=0; j--, k--) {
            long product = (y[j] & LONG_MASK) *
                           (x[xstart] & LONG_MASK) + carry;
            z[k] = (int)product;
            carry = product >>> 32;
        }
        z[xstart] = (int)carry;

        for (int i = xstart-1; i >= 0; i--) {
            carry = 0;
            for (int j=ystart, k=ystart+1+i; j>=0; j--, k--) {
                long product = (y[j] & LONG_MASK) * 
                               (x[i] & LONG_MASK) + 
                               (z[k] & LONG_MASK) + carry;
                z[k] = (int)product;
                carry = product >>> 32;
            }
            z[i] = (int)carry;
        }
        return z;
    }

    private static final int[] squareToLen(int[] x, int len, int[] z) {
        int zlen = len << 1;
        if (z == null || z.length < zlen)
            z = new int[zlen];
        
        int lastProductLowWord = 0;
        for (int j=0, i=0; j<len; j++) {
            long piece = (x[j] & LONG_MASK);
            long product = piece * piece;
            z[i++] = (lastProductLowWord << 31) | (int)(product >>> 33);
            z[i++] = (int)(product >>> 1);
            lastProductLowWord = (int)product;
        }

        for (int i=len, offset=1; i>0; i--, offset+=2) {
            int t = x[i-1];
            t = mulAdd(z, x, offset, i-1, t);
            addOne(z, offset-1, i, t);
        }

        primitiveLeftShift(z, zlen, 1);
        z[zlen-1] |= x[len-1] & 1;

        return z;
    }

    public HugeInt divide(HugeInt val) {
        HugeIntHelper q = new HugeIntHelper(),
                          r = new HugeIntHelper(),
                          a = new HugeIntHelper(this.mag),
                          b = new HugeIntHelper(val.mag);

        a.divide(b, q, r);
        return new HugeInt(q, this.signum * val.signum);
    }

    public HugeInt[] divideAndRemainder(HugeInt val) {
        HugeInt[] result = new HugeInt[2];
        HugeIntHelper q = new HugeIntHelper(),
                          r = new HugeIntHelper(),
                          a = new HugeIntHelper(this.mag),
                          b = new HugeIntHelper(val.mag);
        a.divide(b, q, r);
        result[0] = new HugeInt(q, this.signum * val.signum);
        result[1] = new HugeInt(r, this.signum);
        return result;
    }

    public HugeInt remainder(HugeInt val) {
    	HugeIntHelper q = new HugeIntHelper(),
                          r = new HugeIntHelper(),
                          a = new HugeIntHelper(this.mag),
                          b = new HugeIntHelper(val.mag);

        a.divide(b, q, r);
        return new HugeInt(r, this.signum);
    }

    public HugeInt pow(long exponent) {
    	if (exponent < 0)
    	    throw new ArithmeticException("Negative exponent");
    	if (signum==0)
    	    return (exponent==0 ? ONE : this);

            int newSign = (signum<0 && (exponent&1)==1 ? -1 : 1);
    	int[] baseToPow2 = this.mag;
            int[] result = {1};

    	while (exponent != 0) {
    	    if ((exponent & 1)==1) {
    		result = multiplyToLen(result, result.length, 
                                           baseToPow2, baseToPow2.length, null);
    		result = trustedStripLeadingZeroInts(result);
    	    }
    	    if ((exponent >>>= 1) != 0) {
                    baseToPow2 = squareToLen(baseToPow2, baseToPow2.length, null);
    		baseToPow2 = trustedStripLeadingZeroInts(baseToPow2);
    	    }
    	}
    	return new HugeInt(result, newSign);
    }

    static void primitiveLeftShift(int[] a, int len, int n) {
        if (len == 0 || n == 0)
            return;

        int n2 = 32 - n;
        for (int i=0, c=a[i], m=i+len-1; i<m; i++) {
            int b = c;
            c = a[i+1];
            a[i] = (b << n) | (c >>> n2);
        }
        a[len-1] <<= n;
    }

    public HugeInt abs() {
    	return (signum >= 0 ? this : this.negate());
    }

    public HugeInt negate() {
    	return new HugeInt(this.mag, -this.signum);
        }

    public int signum() {
        return this.signum;
    }

    public HugeInt mod(HugeInt m) {
    	if (m.signum <= 0)
    	    throw new ArithmeticException("BigInteger: modulus not positive");

    	HugeInt result = this.remainder(m);
    	return (result.signum >= 0 ? result : result.add(m));
        }

    static int[] bnExpModThreshTable = {7, 25, 81, 241, 673, 1793,
                                                Integer.MAX_VALUE}; // Sentinel

    static int mulAdd(int[] out, int[] in, int offset, int len, int k) {
        long kLong = k & LONG_MASK;
        long carry = 0;

        offset = out.length-offset - 1;
        for (int j=len-1; j >= 0; j--) {
            long product = (in[j] & LONG_MASK) * kLong +
                           (out[offset] & LONG_MASK) + carry;
            out[offset--] = (int)product;
            carry = product >>> 32;
        }
        return (int)carry;
    }

    static int addOne(int[] a, int offset, int mlen, int carry) {
        offset = a.length-1-mlen-offset;
        long t = (a[offset] & LONG_MASK) + (carry & LONG_MASK);
        
        a[offset] = (int)t;
        if ((t >>> 32) == 0)
            return 0;
        while (--mlen >= 0) {
            if (--offset < 0) { // Carry out of number
                return 1;
            } else {
                a[offset]++;
                if (a[offset] != 0)
                    return 0;
            }
        }
        return 1;
    }

    public HugeInt shiftLeft(int n) {
        if (signum == 0)
            return ZERO;
        if (n==0)
            return this;
        if (n<0)
            return shiftRight(-n);

        int nInts = n >>> 5;
        int nBits = n & 0x1f;
        int magLen = mag.length;
        int newMag[] = null;

        if (nBits == 0) {
            newMag = new int[magLen + nInts];
            for (int i=0; i<magLen; i++)
                newMag[i] = mag[i];
        } else {
            int i = 0;
            int nBits2 = 32 - nBits;
            int highBits = mag[0] >>> nBits2;
            if (highBits != 0) {
                newMag = new int[magLen + nInts + 1];
                newMag[i++] = highBits;
            } else {
                newMag = new int[magLen + nInts];
            }
            int j=0;
            while (j < magLen-1)
                newMag[i++] = mag[j++] << nBits | mag[j] >>> nBits2;
            newMag[i] = mag[j] << nBits;
        }

        return new HugeInt(newMag, signum);
    }

    public HugeInt shiftRight(int n) {
        if (n==0)
            return this;
        if (n<0)
            return shiftLeft(-n);

        int nInts = n >>> 5;
        int nBits = n & 0x1f;
        int magLen = mag.length;
        int newMag[] = null;

        if (nInts >= magLen)
            return (signum >= 0 ? ZERO : negConst[1]);

        if (nBits == 0) {
            int newMagLen = magLen - nInts;
            newMag = new int[newMagLen];
            for (int i=0; i<newMagLen; i++)
                newMag[i] = mag[i];
        } else {
            int i = 0;
            int highBits = mag[0] >>> nBits;
            if (highBits != 0) {
                newMag = new int[magLen - nInts];
                newMag[i++] = highBits;
            } else {
                newMag = new int[magLen - nInts -1];
            }

            int nBits2 = 32 - nBits;
            int j=0;
            while (j < magLen - nInts - 1)
                newMag[i++] = (mag[j++] << nBits2) | (mag[j] >>> nBits);
        }

        if (signum < 0) {
            boolean onesLost = false;
            for (int i=magLen-1, j=magLen-nInts; i>=j && !onesLost; i--)
                onesLost = (mag[i] != 0);
            if (!onesLost && nBits != 0)
                onesLost = (mag[magLen - nInts - 1] << (32 - nBits) != 0);

            if (onesLost)
                newMag = javaIncrement(newMag);
        }

        return new HugeInt(newMag, signum);
    }

    int[] javaIncrement(int[] val) {
        int lastSum = 0;
        for (int i=val.length-1;  i >= 0 && lastSum == 0; i--)
            lastSum = (val[i] += 1);
        if (lastSum == 0) {
            val = new int[val.length+1];
            val[0] = 1;
        }
        return val;
    }

    public HugeInt and(HugeInt val) {
    	int[] result = new int[MathEx.max(intLength(), val.intLength())];
    	for (int i=0; i<result.length; i++)
    	    result[i] = (int) (getInt(result.length-i-1)
    				& val.getInt(result.length-i-1));

    	return valueOf(result);
        }

    public HugeInt or(HugeInt val) {
    	int[] result = new int[MathEx.max(intLength(), val.intLength())];
    	for (int i=0; i<result.length; i++)
    	    result[i] = (int) (getInt(result.length-i-1)
    				| val.getInt(result.length-i-1));

    	return valueOf(result);
        }

    public HugeInt xor(HugeInt val) {
    	int[] result = new int[MathEx.max(intLength(), val.intLength())];
    	for (int i=0; i<result.length; i++)
    	    result[i] = (int) (getInt(result.length-i-1)
    				^ val.getInt(result.length-i-1));

    	return valueOf(result);
        }
    
    public HugeInt not() {
    	int[] result = new int[intLength()];
    	for (int i=0; i<result.length; i++)
    	    result[i] = (int) ~getInt(result.length-i-1);

    	return valueOf(result);
    }

    public HugeInt andNot(HugeInt val) {
        int[] result = new int[MathEx.max(intLength(), val.intLength())];
        for (int i=0; i<result.length; i++)
            result[i] = (int) (getInt(result.length-i-1)
                                & ~val.getInt(result.length-i-1));

        return valueOf(result);
    }
    
    public boolean testBit(int n) {
        if (n<0)
            throw new ArithmeticException("Negative bit address");

        return (getInt(n/32) & (1 << (n%32))) != 0;
    }

    public int bitLength() {
        if (bitLength == -1) {
            if (signum == 0) {
                bitLength = 0;
            } else {
                int magBitLength = ((mag.length-1) << 5) + bitLen(mag[0]);

                if (signum < 0) {
                    boolean pow2 = (bitCnt(mag[0]) == 1);
                    for(int i=1; i<mag.length && pow2; i++)
                        pow2 = (mag[i]==0);

                    bitLength = (pow2 ? magBitLength-1 : magBitLength);
                } else {
                    bitLength = magBitLength;
                }
            }
        }
        return bitLength;
    }

    static int bitLen(int w) {
        return
         (w < 1<<15 ?
          (w < 1<<7 ?
           (w < 1<<3 ?
            (w < 1<<1 ? (w < 1<<0 ? (w<0 ? 32 : 0) : 1) : (w < 1<<2 ? 2 : 3)) :
            (w < 1<<5 ? (w < 1<<4 ? 4 : 5) : (w < 1<<6 ? 6 : 7))) :
           (w < 1<<11 ?
            (w < 1<<9 ? (w < 1<<8 ? 8 : 9) : (w < 1<<10 ? 10 : 11)) :
            (w < 1<<13 ? (w < 1<<12 ? 12 : 13) : (w < 1<<14 ? 14 : 15)))) :
          (w < 1<<23 ?
           (w < 1<<19 ?
            (w < 1<<17 ? (w < 1<<16 ? 16 : 17) : (w < 1<<18 ? 18 : 19)) :
            (w < 1<<21 ? (w < 1<<20 ? 20 : 21) : (w < 1<<22 ? 22 : 23))) :
           (w < 1<<27 ?
            (w < 1<<25 ? (w < 1<<24 ? 24 : 25) : (w < 1<<26 ? 26 : 27)) :
            (w < 1<<29 ? (w < 1<<28 ? 28 : 29) : (w < 1<<30 ? 30 : 31)))));
    }

    static int bitCnt(int val) {
        val -= (0xaaaaaaaa & val) >>> 1;
        val = (val & 0x33333333) + ((val >>> 2) & 0x33333333);
        val = val + (val >>> 4) & 0x0f0f0f0f;
        val += val >>> 8;
        val += val >>> 16;
        return val & 0xff;
    }

    public int compareTo(Object valO) {
        HugeInt val = (HugeInt)valO;
        return (signum==val.signum
                ? signum*intArrayCmp(mag, val.mag)
                : (signum>val.signum ? 1 : -1));
    }

    private static int intArrayCmp(int[] arg1, int[] arg2) {
        if (arg1.length < arg2.length)
            return -1;
        if (arg1.length > arg2.length)
            return 1;

        for (int i=0; i<arg1.length; i++) {
            long b1 = arg1[i] & LONG_MASK;
            long b2 = arg2[i] & LONG_MASK;
            if (b1 < b2)
                return -1;
            if (b1 > b2)
                return 1;
        }
        return 0;
    }

    public boolean equals(Object x) {
    	if (x == this)
    	    return true;

    	if (!(x instanceof HugeInt))
    	    return false;
    	HugeInt xInt = (HugeInt) x;

    	if (xInt.signum != signum || xInt.mag.length != mag.length)
    	    return false;

    	for (int i=0; i<mag.length; i++)
    	    if (xInt.mag[i] != mag[i])
    		return false;

    	return true;
        }

    public HugeInt min(HugeInt val) {
        return (compareTo(val)<0 ? this : val);
    }

    public HugeInt max(HugeInt val) {
        return (compareTo(val)>0 ? this : val);
    }
    
    public int hashCode() {
        int hashCode = 0;

        for (int i=0; i<mag.length; i++)
            hashCode = (int)(31*hashCode + (mag[i] & LONG_MASK));

        return hashCode * signum;
    }

    public String toString(int radix) {
    	if (signum == 0)
    	    return "0";
    	if (radix < Character.MIN_RADIX || radix > Character.MAX_RADIX)
    	    radix = 10;

    	int maxNumDigitGroups = (4*mag.length + 6)/7;
    	String digitGroup[] = new String[maxNumDigitGroups];
            
    	HugeInt tmp = this.abs();
    	int numGroups = 0;
    	while (tmp.signum != 0) {
                HugeInt d = longRadix[radix];

                HugeIntHelper q = new HugeIntHelper(),
                                  r = new HugeIntHelper(),
                                  a = new HugeIntHelper(tmp.mag),
                                  b = new HugeIntHelper(d.mag);
                a.divide(b, q, r);
                HugeInt q2 = new HugeInt(q, tmp.signum * d.signum);
                HugeInt r2 = new HugeInt(r, tmp.signum * d.signum);

                digitGroup[numGroups++] = Long.toString(r2.longValue(), radix);
                tmp = q2;
    	}

    	StringBuffer buf = new StringBuffer(numGroups*digitsPerLong[radix]+1);
    	if (signum<0)
    	    buf.append('-');
    	buf.append(digitGroup[numGroups-1]);

    	for (int i=numGroups-2; i>=0; i--) {
    	    int numLeadingZeros = digitsPerLong[radix]-digitGroup[i].length();
    	    if (numLeadingZeros != 0)
    		buf.append(zeros[numLeadingZeros]);
    	    buf.append(digitGroup[i]);
    	}
    	return buf.toString();
        }

    private static String zeros[] = new String[64];
    static {
        zeros[63] =
            "000000000000000000000000000000000000000000000000000000000000000";
        for (int i=0; i<63; i++)
            zeros[i] = zeros[63].substring(0, i);
    }

    public String toString() {
        return toString(10);
    }

    public byte[] toByteArray() {
        int byteLen = bitLength()/8 + 1;
        byte[] byteArray = new byte[byteLen];

        for (int i=byteLen-1, bytesCopied=4, nextInt=0, intIndex=0; i>=0; i--) {
            if (bytesCopied == 4) {
                nextInt = getInt(intIndex++);
                bytesCopied = 1;
            } else {
                nextInt >>>= 8;
                bytesCopied++;
            }
            byteArray[i] = (byte)nextInt;
        }
        return byteArray;
    }

    public int intValue() {
        int result = 0;
        result = getInt(0);
        return result;
    }

    public long longValue() {
        long result = 0;

        for (int i=1; i>=0; i--)
            result = (result << 32) + (getInt(i) & LONG_MASK);
        return result;
    }

    public float floatValue() {
        return Float.valueOf(this.toString()).floatValue();
    }

    public double doubleValue() {
        return Double.valueOf(this.toString()).doubleValue();
    }

    private static int[] trustedStripLeadingZeroInts(int val[]) {
        int keep;

        for (keep=0; keep<val.length && val[keep]==0; keep++)
            ;

        if (keep > 0) {
            int result[] = new int[val.length - keep];
            for(int i=0; i<val.length - keep; i++)
               result[i] = val[keep+i];
            return result; 
        }
        return val;
    }

    private static int[] stripLeadingZeroBytes(byte a[]) {
        int byteLength = a.length;
        int keep;

        for (keep=0; keep<a.length && a[keep]==0; keep++)
            ;

        int intLength = ((byteLength - keep) + 3)/4;
        int[] result = new int[intLength];
        int b = byteLength - 1;
        for (int i = intLength-1; i >= 0; i--) {
            result[i] = a[b--] & 0xff;
            int bytesRemaining = b - keep + 1;
            int bytesToTransfer = MathEx.min(3, bytesRemaining);
            for (int j=8; j <= 8*bytesToTransfer; j += 8)
                result[i] |= ((a[b--] & 0xff) << j);
        }
        return result;
    }

    private static int[] makePositive(int a[]) {
        int keep, j;

        for (keep=0; keep<a.length && a[keep]==-1; keep++)
            ;

        for (j=keep; j<a.length && a[j]==0; j++)
            ;
        int extraInt = (j==a.length ? 1 : 0);
        int result[] = new int[a.length - keep + extraInt];

        for (int i = keep; i<a.length; i++)
            result[i - keep + extraInt] = ~a[i];

        for (int i=result.length-1; ++result[i]==0; i--)
            ;

        return result;
    }

    private static int digitsPerLong[] = {0, 0,
        62, 39, 31, 27, 24, 22, 20, 19, 18, 18, 17, 17, 16, 16, 15, 15, 15, 14,
        14, 14, 14, 13, 13, 13, 13, 13, 13, 12, 12, 12, 12, 12, 12, 12, 12};

    private static HugeInt longRadix[] = {null, null,
        valueOf(0x4000000000000000L), valueOf(0x383d9170b85ff80bL),
        valueOf(0x4000000000000000L), valueOf(0x6765c793fa10079dL),
        valueOf(0x41c21cb8e1000000L), valueOf(0x3642798750226111L),
        valueOf(0x1000000000000000L), valueOf(0x12bf307ae81ffd59L),
        valueOf( 0xde0b6b3a7640000L), valueOf(0x4d28cb56c33fa539L),
        valueOf(0x1eca170c00000000L), valueOf(0x780c7372621bd74dL),
        valueOf(0x1e39a5057d810000L), valueOf(0x5b27ac993df97701L),
        valueOf(0x1000000000000000L), valueOf(0x27b95e997e21d9f1L),
        valueOf(0x5da0e1e53c5c8000L), valueOf( 0xb16a458ef403f19L),
        valueOf(0x16bcc41e90000000L), valueOf(0x2d04b7fdd9c0ef49L),
        valueOf(0x5658597bcaa24000L), valueOf( 0x6feb266931a75b7L),
        valueOf( 0xc29e98000000000L), valueOf(0x14adf4b7320334b9L),
        valueOf(0x226ed36478bfa000L), valueOf(0x383d9170b85ff80bL),
        valueOf(0x5a3c23e39c000000L), valueOf( 0x4e900abb53e6b71L),
        valueOf( 0x7600ec618141000L), valueOf( 0xaee5720ee830681L),
        valueOf(0x1000000000000000L), valueOf(0x172588ad4f5f0981L),
        valueOf(0x211e44f7d02c1000L), valueOf(0x2ee56725f06e5c71L),
        valueOf(0x41c21cb8e1000000L)};

    private static int digitsPerInt[] = {0, 0, 30, 19, 15, 13, 11,
        11, 10, 9, 9, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6,
        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5};

    private static int intRadix[] = {0, 0,
        0x40000000, 0x4546b3db, 0x40000000, 0x48c27395, 0x159fd800,
        0x75db9c97, 0x40000000, 0x17179149, 0x3b9aca00, 0xcc6db61,
        0x19a10000, 0x309f1021, 0x57f6c100, 0xa2f1b6f,  0x10000000,
        0x18754571, 0x247dbc80, 0x3547667b, 0x4c4b4000, 0x6b5a6e1d,
        0x6c20a40,  0x8d2d931,  0xb640000,  0xe8d4a51,  0x1269ae40,
        0x17179149, 0x1cb91000, 0x23744899, 0x2b73a840, 0x34e63b41,
        0x40000000, 0x4cfa3cc1, 0x5c13d840, 0x6d91b519, 0x39aa400
    };

    private int intLength() {
        return bitLength()/32 + 1;
    }

    private int signInt() {
        return (int) (signum < 0 ? -1 : 0);
    }

    private int getInt(int n) {
        if (n < 0)
            return 0;
        if (n >= mag.length)
            return signInt();

        int magInt = mag[mag.length-n-1];

        return (int) (signum >= 0 ? magInt :
                       (n <= firstNonzeroIntNum() ? -magInt : ~magInt));
    }

     private int firstNonzeroIntNum() {
        if (firstNonzeroIntNum == -2) {
            int i;
            for (i=mag.length-1; i>=0 && mag[i]==0; i--)
                ;
            firstNonzeroIntNum = mag.length-i-1;
        }
        return firstNonzeroIntNum;
    }

    private static final long serialVersionUID = -8287574255936472291L;
}
