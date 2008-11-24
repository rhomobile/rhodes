package j2me.math;

class HugeIntHelper {
    int[] value;
    int intLen;
    int offset = 0;
    private final static long LONG_MASK = 0xffffffffL;
    
    HugeIntHelper() {
        value = new int[1];
        intLen = 0;
    }
    
    HugeIntHelper(int[] val) {
        value = val;
        intLen = val.length;
    }

    void clear() {
        offset = intLen = 0;
        for (int index=0, n=value.length; index < n; index++)
            value[index] = 0;
    }

    final int compare(HugeIntHelper b) {
        if (intLen < b.intLen)
            return -1;
        if (intLen > b.intLen)
            return 1;

        for (int i=0; i<intLen; i++) {
            int b1 = value[offset+i]     + 0x80000000;
            int b2 = b.value[b.offset+i] + 0x80000000;
            if (b1 < b2)
                return -1;
            if (b1 > b2)
                return 1;
        }
        return 0;
    }

    final void normalize() {
        if (intLen == 0) {
            offset = 0;
            return;
        }

        int index = offset;
        if (value[index] != 0)
            return;

        int indexBound = index+intLen;
        do {
            index++;
        } while(index < indexBound && value[index]==0);

        int numZeros = index - offset;
        intLen -= numZeros;
        offset = (intLen==0 ?  0 : offset+numZeros);
    }

    void setValue(int[] val, int length) {
        value = val;
        intLen = length;
        offset = 0;
    }

    void copyValue(HugeIntHelper val) {
        int len = val.intLen;
        if (value.length < len)
            value = new int[len];

        for(int i=0; i<len; i++)
            value[i] = val.value[val.offset+i];
        intLen = len;
        offset = 0;
    }

    void rightShift(int n) {
        if (intLen == 0)
            return;
        int nInts = n >>> 5;
        int nBits = n & 0x1F;
        this.intLen -= nInts;
        if (nBits == 0)
            return;
        int bitsInHighWord = HugeInt.bitLen(value[offset]);
        if (nBits >= bitsInHighWord) {
            this.primitiveLeftShift(32 - nBits);
            this.intLen--;
        } else {
            primitiveRightShift(nBits);
        }
    }

    void leftShift(int n) {
        if (intLen == 0)
           return;
        int nInts = n >>> 5;
        int nBits = n&0x1F;
        int bitsInHighWord = HugeInt.bitLen(value[offset]);
        
        if (n <= (32-bitsInHighWord)) {
            primitiveLeftShift(nBits);
            return;
        }

        int newLen = intLen + nInts +1;
        if (nBits <= (32-bitsInHighWord))
            newLen--;
        if (value.length < newLen) {
            int[] result = new int[newLen];
            for (int i=0; i<intLen; i++)
                result[i] = value[offset+i];
            setValue(result, newLen);
        } else if (value.length - offset >= newLen) {
            for(int i=0; i<newLen - intLen; i++)
                value[offset+intLen+i] = 0;
        } else {
            for (int i=0; i<intLen; i++)
                value[i] = value[offset+i];
            for (int i=intLen; i<newLen; i++)
                value[i] = 0;
            offset = 0;
        }
        intLen = newLen;
        if (nBits == 0)
            return;
        if (nBits <= (32-bitsInHighWord))
            primitiveLeftShift(nBits);
        else
            primitiveRightShift(32 -nBits);
    }

    private int divadd(int[] a, int[] result, int offset) {
        long carry = 0;

        for (int j=a.length-1; j >= 0; j--) {
            long sum = (a[j] & LONG_MASK) + 
                       (result[j+offset] & LONG_MASK) + carry;
            result[j+offset] = (int)sum;
            carry = sum >>> 32;
        }
        return (int)carry;
    }

    private int mulsub(int[] q, int[] a, int x, int len, int offset) {
        long xLong = x & LONG_MASK;
        long carry = 0;
        offset += len;

        for (int j=len-1; j >= 0; j--) {
            long product = (a[j] & LONG_MASK) * xLong + carry;
            long difference = q[offset] - product;
            q[offset--] = (int)difference;
            carry = (product >>> 32)
                     + (((difference & LONG_MASK) >
                         (((~(int)product) & LONG_MASK))) ? 1:0);
        }
        return (int)carry;
    }

    private final void primitiveRightShift(int n) {
        int[] val = value;
        int n2 = 32 - n;
        for (int i=offset+intLen-1, c=val[i]; i>offset; i--) {
            int b = c;
            c = val[i-1];
            val[i] = (c << n2) | (b >>> n);
        }
        val[offset] >>>= n;
    }

    private final void primitiveLeftShift(int n) {
        int[] val = value;
        int n2 = 32 - n;
        for (int i=offset, c=val[i], m=i+intLen-1; i<m; i++) {
            int b = c;
            c = val[i+1];
            val[i] = (b << n) | (c >>> n2);
        }
        val[offset+intLen-1] <<= n;
    }

    void divideOneWord(int divisor, HugeIntHelper quotient) {
        long divLong = divisor & LONG_MASK;

        if (intLen == 1) {
            long remValue = value[offset] & LONG_MASK;
            quotient.value[0] = (int) (remValue / divLong);
            quotient.intLen = (quotient.value[0] == 0) ? 0 : 1;
            quotient.offset = 0;

            value[0] = (int) (remValue - (quotient.value[0] * divLong));
            offset = 0;
            intLen = (value[0] == 0) ? 0 : 1;
           
            return;
        }

        if (quotient.value.length < intLen)
            quotient.value = new int[intLen];
        quotient.offset = 0;
        quotient.intLen = intLen;

        int shift = 32 - HugeInt.bitLen(divisor);

        int rem = value[offset];
        long remLong = rem & LONG_MASK;
        if (remLong < divLong) {
            quotient.value[0] = 0;
        } else {
            quotient.value[0] = (int)(remLong/divLong);
            rem = (int) (remLong - (quotient.value[0] * divLong));
            remLong = rem & LONG_MASK;
        }

        int xlen = intLen; 
        int[] qWord = new int[2];
        while (--xlen > 0) {
            long dividendEstimate = (remLong<<32) |
                (value[offset + intLen - xlen] & LONG_MASK);
            if (dividendEstimate >= 0) {
                qWord[0] = (int) (dividendEstimate/divLong);
                qWord[1] = (int) (dividendEstimate - (qWord[0] * divLong));
            } else {
                divWord(qWord, dividendEstimate, divisor);
            }
            quotient.value[intLen - xlen] = (int)qWord[0];
            rem = (int)qWord[1];
            remLong = rem & LONG_MASK;
        }
        
        if (shift > 0)
            value[0] = rem %= divisor;
        else
            value[0] = rem;
        intLen = (value[0] == 0) ? 0 : 1;
        
        quotient.normalize();
    }

    void divide(HugeIntHelper b,
                        HugeIntHelper quotient, HugeIntHelper rem) {  
        if (b.intLen == 0)
            throw new ArithmeticException("BigInteger divide by zero");

        // Dividend is zero
        if (intLen == 0) {
            quotient.intLen = quotient.offset = rem.intLen = rem.offset = 0;
            return;
        }
 
        int cmp = compare(b);

        if (cmp < 0) {
            quotient.intLen = quotient.offset = 0;
            rem.copyValue(this);
            return;
        }
        
        if (cmp == 0) {
            quotient.value[0] = quotient.intLen = 1;
            quotient.offset = rem.intLen = rem.offset = 0;
            return;
        }

        quotient.clear();

        if (b.intLen == 1) {
            rem.copyValue(this);
            rem.divideOneWord(b.value[b.offset], quotient);
            return;
        }

        int[] d = new int[b.intLen];
        for(int i=0; i<b.intLen; i++)
            d[i] = b.value[b.offset+i];
        int dlen = b.intLen;

        if (rem.value.length < intLen +1)
            rem.value = new int[intLen+1];

        for (int i=0; i<intLen; i++)
            rem.value[i+1] = value[i+offset];
        rem.intLen = intLen;
        rem.offset = 1;

        int nlen = rem.intLen;

        int limit = nlen - dlen + 1;
        if (quotient.value.length < limit) {
            quotient.value = new int[limit];
            quotient.offset = 0;
        }
        quotient.intLen = limit;
        int[] q = quotient.value;
        
        int shift = 32 - HugeInt.bitLen(d[0]);
        if (shift > 0) {
            HugeInt.primitiveLeftShift(d, dlen, shift);
            rem.leftShift(shift);
        }
       
        if (rem.intLen == nlen) {
            rem.offset = 0;
            rem.value[0] = 0;
            rem.intLen++;
        }

        int dh = d[0];
        long dhLong = dh & LONG_MASK;
        int dl = d[1];
        int[] qWord = new int[2];
        
        for(int j=0; j<limit; j++) {
            int qhat = 0;
            int qrem = 0;
            boolean skipCorrection = false;
            int nh = rem.value[j+rem.offset];
            int nh2 = nh + 0x80000000;
            int nm = rem.value[j+1+rem.offset];

            if (nh == dh) {
                qhat = ~0;
                qrem = nh + nm;
                skipCorrection = qrem + 0x80000000 < nh2;
            } else {
                long nChunk = (((long)nh) << 32) | (nm & LONG_MASK);
                if (nChunk >= 0) {
                    qhat = (int) (nChunk / dhLong);
                    qrem = (int) (nChunk - (qhat * dhLong));
                } else {
                    divWord(qWord, nChunk, dh);
                    qhat = qWord[0];
                    qrem = qWord[1];
                }
            }

            if (qhat == 0)
                continue;
            
            if (!skipCorrection) { // Correct qhat
                long nl = rem.value[j+2+rem.offset] & LONG_MASK;
                long rs = ((qrem & LONG_MASK) << 32) | nl;
                long estProduct = (dl & LONG_MASK) * (qhat & LONG_MASK);

                if (unsignedLongCompare(estProduct, rs)) {
                    qhat--;
                    qrem = (int)((qrem & LONG_MASK) + dhLong);
                    if ((qrem & LONG_MASK) >=  dhLong) {
                        estProduct = (dl & LONG_MASK) * (qhat & LONG_MASK);
                        rs = ((qrem & LONG_MASK) << 32) | nl;
                        if (unsignedLongCompare(estProduct, rs))
                            qhat--;
                    }
                }
            }

            rem.value[j+rem.offset] = 0;
            int borrow = mulsub(rem.value, d, qhat, dlen, j+rem.offset);

            if (borrow + 0x80000000 > nh2) {
                divadd(d, rem.value, j+1+rem.offset);
                qhat--;
            }

            q[j] = qhat;
        }

        if (shift > 0)
            rem.rightShift(shift);

        rem.normalize();
        quotient.normalize();
    }

    private boolean unsignedLongCompare(long one, long two) {
        return (one+Long.MIN_VALUE) > (two+Long.MIN_VALUE);
    }

    private void divWord(int[] result, long n, int d) {
        long dLong = d & LONG_MASK;

        if (dLong == 1) {
            result[0] = (int)n;
            result[1] = 0;
            return;
        }
                
        long q = (n >>> 1) / (dLong >>> 1);
        long r = n - q*dLong;

        while (r < 0) {
            r += dLong;
            q--;
        }
        while (r >= dLong) {
            r -= dLong;
            q++;
        }

        result[0] = (int)q;
        result[1] = (int)r;
    }
}
