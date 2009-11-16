package j2me.math;

public class BigInteger
{

    private int signum; // -1 means -ve; +1 means +ve; 0 means 0;
    private int[] magnitude; // array of ints with [0] being the most significant
 //   private int nBits = -1; // cache bitCount() value
 //   private int nBitLength = -1; // cache bitLength() value
    private static final long IMASK = 0xffffffffL;
//    private long mQuote = -1L; // -m^(-1) mod b, b = 2^32 (see Montgomery mult.)
    private int firstNonzeroIntNum = -2;
    private int bitLength = -1;
    
    private BigInteger(int sig, int[] mag)
    {
    	signum = sig;
        if (mag.length > 0)
        {
            int i = 0;
            while (i < mag.length && mag[i] == 0)
            {
                i++;
            }
            if (i == 0)
            {
                magnitude = mag;
            }
            else
            {
                // strip leading 0 bytes
                int[] newMag = new int[mag.length - i];
                System.arraycopy(mag, i, newMag, 0, newMag.length);
                magnitude = newMag;
                if (newMag.length == 0)
                	signum = 0;
            }
        }
        else
        {
            magnitude = mag;
            signum = 0;
        }
    }

    public BigInteger(String sval) throws NumberFormatException
    {
        this(sval, 10);
    }

    public BigInteger(String sval, int rdx) throws NumberFormatException
    {
        if (sval.length() == 0)
        {
            throw new NumberFormatException("Zero length BigInteger");
        }

        if (rdx < Character.MIN_RADIX || rdx > Character.MAX_RADIX)
        {
            throw new NumberFormatException("Radix out of range");
        }

        int index = 0;
        signum = 1;

        if (sval.charAt(0) == '-')
        {
            if (sval.length() == 1)
            {
                throw new NumberFormatException("Zero length BigInteger");
            }

            signum = -1;
            index = 1;
        }

        // strip leading zeros from the string value
        while (index < sval.length() && Character.digit(sval.charAt(index), rdx) == 0)
        {
            index++;
        }

        if (index >= sval.length())
        {
            // zero value - we're done
        	signum = 0;
            magnitude = new int[0];
            return;
        }

        //////
        // could we work out the max number of ints required to store
        // sval.length digits in the given base, then allocate that
        // storage in one hit?, then generate the magnitude in one hit too?
        //////

        BigInteger b = BigInteger.ZERO;
        BigInteger r = valueOf(rdx);
        while (index < sval.length())
        {
            // (optimise this by taking chunks of digits instead?)
            b = b.multiply(r).add(valueOf(Character.digit(sval.charAt(index), rdx)));
            index++;
        }

        magnitude = b.magnitude;
        return;
    }

    public BigInteger(byte[] bval) throws NumberFormatException
    {
        if (bval.length == 0)
        {
            throw new NumberFormatException("Zero length BigInteger");
        }

        signum = 1;
        if (bval[0] < 0)
        {
        	signum = -1;
        }
        magnitude = makeMagnitude(bval, signum);
        if (magnitude.length == 0) {
        	signum = 0;
        }
    }

    /**
     * If sign >= 0, packs bytes into an array of ints, most significant first
     * If sign <  0, packs 2's complement of bytes into 
     * an array of ints, most significant first,
     * adding an extra most significant byte in case bval = {0x80, 0x00, ..., 0x00}
     *
     * @param bval
     * @param sign
     * @return
     */
    private int[] makeMagnitude(byte[] bval, int sign)
    {
        if (sign >= 0) {
            int i;
            int[] mag;
            int firstSignificant;

            // strip leading zeros
            for (firstSignificant = 0; firstSignificant < bval.length
                    && bval[firstSignificant] == 0; firstSignificant++);

            if (firstSignificant >= bval.length)
            {
                return new int[0];
            }

            int nInts = (bval.length - firstSignificant + 3) / 4;
            int bCount = (bval.length - firstSignificant) % 4;            
            if (bCount == 0)
                bCount = 4;

            mag = new int[nInts];
            int v = 0;
            int magnitudeIndex = 0;
            for (i = firstSignificant; i < bval.length; i++)
            {
                v <<= 8;
                v |= bval[i] & 0xff;
                bCount--;
                if (bCount <= 0)
                {
                    mag[magnitudeIndex] = v;
                    magnitudeIndex++;
                    bCount = 4;
                    v = 0;
                }
            }
            return mag;
        }
        else {
            int i;
            int[] mag;
            int firstSignificant;
            

            // strip leading -1's
            for (firstSignificant = 0; firstSignificant < bval.length - 1
                    && bval[firstSignificant] == 0xff; firstSignificant++);

            int nBytes = bval.length;
            boolean leadingByte = false;

            // check for -2^(n-1)
            if (bval[firstSignificant] == 0x80) {
                for (i = firstSignificant + 1; i < bval.length; i++) {
                    if (bval[i] != 0) {
                        break;
                    }
                }
                if (i == bval.length) {
                    nBytes++;
                    leadingByte = true;
                }
            }

            int nInts = (nBytes - firstSignificant + 3) / 4;
            int bCount = (nBytes - firstSignificant) % 4;
            if (bCount == 0)
                bCount = 4;


            mag = new int[nInts];
            int v = 0;
            int magnitudeIndex = 0;
            if (leadingByte) {
                bCount--;
                if (bCount <= 0)
                {
                    magnitudeIndex++;
                    bCount = 4;
                }
            }
            for (i = firstSignificant; i < bval.length; i++)
            {
                v <<= 8;
                v |= ~bval[i] & 0xff;
                bCount--;
                if (bCount <= 0)
                {
                    mag[magnitudeIndex] = v;
                    magnitudeIndex++;
                    bCount = 4;
                    v = 0;
                }
            }
            return inc(mag);
        }

    }

    public BigInteger(int sign, byte[] mag) throws NumberFormatException
    {
        if (sign < -1 || sign > 1)
        {
            throw new NumberFormatException("Invalid sign value");
        }

        if (sign == 0)
        {
            this.signum = 0;
            this.magnitude = new int[0];
            return;
        }

        // copy bytes
        this.magnitude = makeMagnitude(mag, 1);
        this.signum = sign;
    }

    /**
     * return a = a + b - b preserved.
     */
    private int[] add(int[] a, int[] b)
    {
        int tI = a.length - 1;
        int vI = b.length - 1;
        long m = 0;

        while (vI >= 0)
        {
            m += (((long)a[tI]) & IMASK) + (((long)b[vI--]) & IMASK);
            a[tI--] = (int)m;
            m >>>= 32;
        }

        while (tI >= 0 && m != 0)
        {
            m += (((long)a[tI]) & IMASK);
            a[tI--] = (int)m;
            m >>>= 32;
        }

        return a;
    }

    /**
     * return a = a + 1.
     */
    private int[] inc(int[] a)
    {
        int tI = a.length - 1;
        long m = 0;

        m = (((long)a[tI]) & IMASK) + 1L;
        a[tI--] = (int)m;
        m >>>= 32;

        while (tI >= 0 && m != 0)
        {
            m += (((long)a[tI]) & IMASK);
            a[tI--] = (int)m;
            m >>>= 32;
        }

        return a;
    }

    public BigInteger add(BigInteger val) throws ArithmeticException
    {
        if (val.signum == 0 || val.magnitude.length == 0)
            return this;
        if (this.signum == 0 || this.magnitude.length == 0)
            return val;

        if (val.signum < 0)
        {
            if (this.signum > 0)
                return this.subtract(val.negate());
        }
        else
        {
            if (this.signum < 0)
                return val.subtract(this.negate());
        }

        // both BigIntegers are either +ve or -ve; set the sign later

        int[] mag, 
        op;

        if (this.magnitude.length < val.magnitude.length)
        {
            mag = new int[val.magnitude.length + 1];

            System.arraycopy(val.magnitude, 0, mag, 1, val.magnitude.length);
            op = this.magnitude;
        }
        else
        {
            mag = new int[this.magnitude.length + 1];

            System.arraycopy(this.magnitude, 0, mag, 1, this.magnitude.length);
            op = val.magnitude;
        }

        return new BigInteger(this.signum, add(mag, op));
    }

    private final static byte[] bitCounts = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1,
        2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4,
        4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3,
        4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5,
        3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2,
        3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3,
        3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6,
        7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6,
        5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5,
        6, 6, 7, 6, 7, 7, 8};

    private int bitLength(int indx, int[] mag)
    {
        int bitLength;

        if (mag.length == 0)
        {
            return 0;
        }
        else
        {
            while (indx != mag.length && mag[indx] == 0)
            {
                indx++;
            }

            if (indx == mag.length)
            {
                return 0;
            }

            // bit length for everything after the first int
            bitLength = 32 * ((mag.length - indx) - 1);

            // and determine bitlength of first int
            bitLength += bitLen(mag[indx]);

            if (signum < 0)
            {
                // Check if magnitude is a power of two
                boolean pow2 = ((bitCounts[mag[indx] & 0xff])
                        + (bitCounts[(mag[indx] >> 8) & 0xff])
                        + (bitCounts[(mag[indx] >> 16) & 0xff]) + (bitCounts[(mag[indx] >> 24) & 0xff])) == 1;

                for (int i = indx + 1; i < mag.length && pow2; i++)
                {
                    pow2 = (mag[i] == 0);
                }

                bitLength -= (pow2 ? 1 : 0);
            }
        }

        return bitLength;
    }


    //
    // bitLen(val) is the number of bits in val.
    //
    static int bitLen(int w)
    {
        // Binary search - decision tree (5 tests, rarely 6)
        return (w < 1 << 15 ? (w < 1 << 7
                ? (w < 1 << 3 ? (w < 1 << 1
                        ? (w < 1 << 0 ? (w < 0 ? 32 : 0) : 1)
                        : (w < 1 << 2 ? 2 : 3)) : (w < 1 << 5
                        ? (w < 1 << 4 ? 4 : 5)
                        : (w < 1 << 6 ? 6 : 7)))
                : (w < 1 << 11
                        ? (w < 1 << 9 ? (w < 1 << 8 ? 8 : 9) : (w < 1 << 10 ? 10 : 11))
                        : (w < 1 << 13 ? (w < 1 << 12 ? 12 : 13) : (w < 1 << 14 ? 14 : 15)))) : (w < 1 << 23 ? (w < 1 << 19
                ? (w < 1 << 17 ? (w < 1 << 16 ? 16 : 17) : (w < 1 << 18 ? 18 : 19))
                : (w < 1 << 21 ? (w < 1 << 20 ? 20 : 21) : (w < 1 << 22 ? 22 : 23))) : (w < 1 << 27
                ? (w < 1 << 25 ? (w < 1 << 24 ? 24 : 25) : (w < 1 << 26 ? 26 : 27))
                : (w < 1 << 29 ? (w < 1 << 28 ? 28 : 29) : (w < 1 << 30 ? 30 : 31)))));
    }

    public int compareTo(Object o)
    {
        return compareTo((BigInteger)o);
    }

    /**
     * unsigned comparison on two arrays - note the arrays may
     * start with leading zeros.
     */
    private int compareTo(int xIndx, int[] x, int yIndx, int[] y)
    {
        while (xIndx != x.length && x[xIndx] == 0)
        {
            xIndx++;
        }

        while (yIndx != y.length && y[yIndx] == 0)
        {
            yIndx++;
        }

        if ((x.length - xIndx) < (y.length - yIndx))
        {
            return -1;
        }

        if ((x.length - xIndx) > (y.length - yIndx))
        {
            return 1;
        }

        // lengths of magnitudes the same, test the magnitude values

        while (xIndx < x.length)
        {
            long v1 = (long)(x[xIndx++]) & IMASK;
            long v2 = (long)(y[yIndx++]) & IMASK;
            if (v1 < v2)
            {
                return -1;
            }
            if (v1 > v2)
            {
                return 1;
            }
        }

        return 0;
    }

    /**
     * return z = x / y - done in place (z value preserved, x contains the
     * remainder)
     */
    private int[] divide(int[] x, int[] y)
    {
        int xyCmp = compareTo(0, x, 0, y);
        int[] count;

        if (xyCmp > 0)
        {
            int[] c;

            int shift = bitLength(0, x) - bitLength(0, y);

            if (shift > 1)
            {
                c = shiftLeft(y, shift - 1);
                count = shiftLeft(ONE.magnitude, shift - 1);
                if (shift % 32 == 0)
                {
                    // Special case where the shift is the size of an int.
                    int countSpecial[] = new int[shift / 32 + 1];
                    System.arraycopy(count, 0, countSpecial, 1, countSpecial.length - 1);
                    countSpecial[0] = 0;
                    count = countSpecial;
                }
            }
            else
            {
                c = new int[x.length];
                count = new int[1];

                System.arraycopy(y, 0, c, c.length - y.length, y.length);
                count[0] = 1;
            }

            int[] iCount = new int[count.length];

            subtract(0, x, 0, c);
            System.arraycopy(count, 0, iCount, 0, count.length);

            int xStart = 0;
            int cStart = 0;
            int iCountStart = 0;

            for (; ; )
            {
                int cmp = compareTo(xStart, x, cStart, c);

                while (cmp >= 0)
                {
                    subtract(xStart, x, cStart, c);
                    add(count, iCount);
                    cmp = compareTo(xStart, x, cStart, c);
                }

                xyCmp = compareTo(xStart, x, 0, y);

                if (xyCmp > 0)
                {
                    if (x[xStart] == 0)
                    {
                        xStart++;
                    }

                    shift = bitLength(cStart, c) - bitLength(xStart, x);

                    if (shift == 0)
                    {
                        c = shiftRightOne(cStart, c);
                        iCount = shiftRightOne(iCountStart, iCount);
                    }
                    else
                    {
                        c = shiftRight(cStart, c, shift);
                        iCount = shiftRight(iCountStart, iCount, shift);
                    }

                    if (c[cStart] == 0)
                    {
                        cStart++;
                    }

                    if (iCount[iCountStart] == 0)
                    {
                        iCountStart++;
                    }
                }
                else if (xyCmp == 0)
                {
                    add(count, ONE.magnitude);
                    for (int i = xStart; i != x.length; i++)
                    {
                        x[i] = 0;
                    }
                    break;
                }
                else
                {
                    break;
                }
            }
        }
        else if (xyCmp == 0)
        {
            count = new int[1];

            count[0] = 1;
        }
        else
        {
            count = new int[1];

            count[0] = 0;
        }

        return count;
    }

    public BigInteger divide(BigInteger val) throws ArithmeticException
    {
        if (val.signum == 0)
        {
            throw new ArithmeticException("Divide by zero");
        }

        if (signum == 0)
        {
            return BigInteger.ZERO;
        }

        if (val.compareTo(BigInteger.ONE) == 0)
        {
            return this;
        }

        int[] mag = new int[this.magnitude.length];
        System.arraycopy(this.magnitude, 0, mag, 0, mag.length);

        return new BigInteger(this.signum * val.signum, divide(mag, val.magnitude));
    }

    public int intValue()
    {
        if (magnitude.length == 0)
        {
            return 0;
        }

        if (signum < 0)
        {
            return -magnitude[magnitude.length - 1];
        }
        else
        {
            return magnitude[magnitude.length - 1];
        }
    }

    public long longValue()
    {
        long val = 0;

        if (magnitude.length == 0)
        {
            return 0;
        }

        if (magnitude.length > 1)
        {
            val = ((long)magnitude[magnitude.length - 2] << 32)
                    | (magnitude[magnitude.length - 1] & IMASK);
        }
        else
        {
            val = (magnitude[magnitude.length - 1] & IMASK);
        }

        if (signum < 0)
        {
            return -val;
        }
        else
        {
            return val;
        }
    }

    /**
     * return x with x = y * z - x is assumed to have enough space.
     */
    private int[] multiply(int[] x, int[] y, int[] z)
    {
        for (int i = z.length - 1; i >= 0; i--)
        {
            long a = z[i] & IMASK;
            long value = 0;

            for (int j = y.length - 1; j >= 0; j--)
            {
                value += a * (y[j] & IMASK) + (x[i + j + 1] & IMASK);

                x[i + j + 1] = (int)value;

                value >>>= 32;
            }

            x[i] = (int)value;
        }

        return x;
    }

    public BigInteger multiply(BigInteger val)
    {
        if (signum == 0 || val.signum == 0)
            return BigInteger.ZERO;

        int[] res = new int[magnitude.length + val.magnitude.length];

        return new BigInteger(signum * val.signum, multiply(res, magnitude, val.magnitude));
    }

    public BigInteger negate()
    {
        return new BigInteger( -signum, magnitude);
    }

    /**
     * do a left shift - this returns a new array.
     */
    private int[] shiftLeft(int[] mag, int n)
    {
        int nInts = n >>> 5;
        int nBits = n & 0x1f;
        int magLen = mag.length;
        int newMag[] = null;

        if (nBits == 0)
        {
            newMag = new int[magLen + nInts];
            for (int i = 0; i < magLen; i++)
            {
                newMag[i] = mag[i];
            }
        }
        else
        {
            int i = 0;
            int nBits2 = 32 - nBits;
            int highBits = mag[0] >>> nBits2;

            if (highBits != 0)
            {
                newMag = new int[magLen + nInts + 1];
                newMag[i++] = highBits;
            }
            else
            {
                newMag = new int[magLen + nInts];
            }

            int m = mag[0];
            for (int j = 0; j < magLen - 1; j++)
            {
                int next = mag[j + 1];

                newMag[i++] = (m << nBits) | (next >>> nBits2);
                m = next;
            }

            newMag[i] = mag[magLen - 1] << nBits;
        }

        return newMag;
    }

    /**
     * do a right shift - this does it in place.
     */
    private int[] shiftRight(int start, int[] mag, int n)
    {
        int nInts = (n >>> 5) + start;
        int nBits = n & 0x1f;
        int magLen = mag.length;

        if (nInts != start)
        {
            int delta = (nInts - start);

            for (int i = magLen - 1; i >= nInts; i--)
            {
                mag[i] = mag[i - delta];
            }
            for (int i = nInts - 1; i >= start; i--)
            {
                mag[i] = 0;
            }
        }

        if (nBits != 0)
        {
            int nBits2 = 32 - nBits;
            int m = mag[magLen - 1];

            for (int i = magLen - 1; i >= nInts + 1; i--)
            {
                int next = mag[i - 1];

                mag[i] = (m >>> nBits) | (next << nBits2);
                m = next;
            }

            mag[nInts] >>>= nBits;
        }

        return mag;
    }

    /**
     * do a right shift by one - this does it in place.
     */
    private int[] shiftRightOne(int start, int[] mag)
    {
        int magLen = mag.length;

        int m = mag[magLen - 1];

        for (int i = magLen - 1; i >= start + 1; i--)
        {
            int next = mag[i - 1];

            mag[i] = (m >>> 1) | (next << 31);
            m = next;
        }

        mag[start] >>>= 1;

        return mag;
    }

    /**
     * returns x = x - y - we assume x is >= y
     */
    private int[] subtract(int xStart, int[] x, int yStart, int[] y)
    {
        int iT = x.length - 1;
        int iV = y.length - 1;
        long m;
        int borrow = 0;

        do
        {
            m = (((long)x[iT]) & IMASK) - (((long)y[iV--]) & IMASK) + borrow;

            x[iT--] = (int)m;

            if (m < 0)
            {
                borrow = -1;
            }
            else
            {
                borrow = 0;
            }
        } while (iV >= yStart);

        while (iT >= xStart)
        {
            m = (((long)x[iT]) & IMASK) + borrow;
            x[iT--] = (int)m;

            if (m < 0)
            {
                borrow = -1;
            }
            else
            {
                break;
            }
        }

        return x;
    }

    public BigInteger subtract(BigInteger val)
    {
        if (val.signum == 0 || val.magnitude.length == 0)
        {
            return this;
        }
        if (signum == 0 || magnitude.length == 0)
        {
            return val.negate();
        }
        if (val.signum < 0)
        {
            if (this.signum > 0)
                return this.add(val.negate());
        }
        else
        {
            if (this.signum < 0)
                return this.add(val.negate());
        }

        BigInteger bigun, 
        littlun;
        int compare = compareTo(val);
        if (compare == 0)
        {
            return ZERO;
        }

        if (compare < 0)
        {
            bigun = val;
            littlun = this;
        }
        else
        {
            bigun = this;
            littlun = val;
        }

        int res[] = new int[bigun.magnitude.length];

        System.arraycopy(bigun.magnitude, 0, res, 0, res.length);

        return new BigInteger(this.signum * compare, subtract(0, res, 0, littlun.magnitude));
    }

    public static final BigInteger ZERO = new BigInteger(0, new byte[0]);
    public static final BigInteger ONE = valueOf(1);
    //private static final BigInteger TWO = valueOf(2);

    public static BigInteger valueOf(long val)
    {
        if (val == 0)
        {
            return BigInteger.ZERO;
        }

        // store val into a byte array
        byte[] b = new byte[8];
        for (int i = 0; i < 8; i++)
        {
            b[7 - i] = (byte)val;
            val >>= 8;
        }

        return new BigInteger(b);
    }

    public double doubleValue() {
    	return Double.valueOf(toString()).doubleValue();
    }
    
    public float floatValue() {
    	return Float.valueOf(toString()).floatValue();
    }
    
    public int signum() {
    	return signum;
    }
 
    private int signInt() {
    	return (int) (signum < 0 ? -1 : 0);
    }
 
    private int firstNonzeroIntNum() {
    	/*
    	 * Initialize firstNonzeroIntNum field the first time this method is
    	 * executed. This method depends on the atomicity of int modifies;
    	 * without this guarantee, it would have to be synchronized.
    	 */
    	if (firstNonzeroIntNum == -2) {
    	    // Search for the first nonzero int
    	    int i;
    	    for (i=magnitude.length-1; i>=0 && magnitude[i]==0; i--)
    		;
    	    firstNonzeroIntNum = magnitude.length-i-1;
    	}
    	return firstNonzeroIntNum;
    }
    
    private int getInt(int n) {
        if (n < 0)
            return 0;
        
		if (n >= magnitude.length)
		    return signInt();
	
		int magInt = magnitude[magnitude.length-n-1];
	
		return (int) (signum >= 0 ? magInt :
			       (n <= firstNonzeroIntNum() ? -magInt : ~magInt));
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
    
    static int bitCnt(int val) {
        val -= (0xaaaaaaaa & val) >>> 1;
        val = (val & 0x33333333) + ((val >>> 2) & 0x33333333);
        val = val + (val >>> 4) & 0x0f0f0f0f;
        val += val >>> 8;
        val += val >>> 16;
        return val & 0xff;
    }
    
    public int bitLength() {
    	/*
    	 * Initialize bitLength field the first time this method is executed.
    	 * This method depends on the atomicity of int modifies; without
    	 * this guarantee, it would have to be synchronized.
    	 */
    	if (bitLength == -1) {
    	    if (signum == 0) {
    	    	bitLength = 0;
    	    } else {
    		// Calculate the bit length of the magnitude
    		int magBitLength = ((magnitude.length-1) << 5) + bitLen(magnitude[0]);

    		if (signum < 0) {
    		    // Check if magnitude is a power of two
    		    boolean pow2 = (bitCnt(magnitude[0]) == 1);
    		    for(int i=1; i<magnitude.length && pow2; i++)
    			pow2 = (magnitude[i]==0);

    		    bitLength = (pow2 ? magBitLength-1 : magBitLength);
    		} else {
    		    bitLength = magBitLength;
    		}
    	    }
    	}
    	return bitLength;
        }
    
}
