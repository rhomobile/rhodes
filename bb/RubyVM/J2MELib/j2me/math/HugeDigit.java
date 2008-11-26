package j2me.math;

import j2me.lang.Number;
import j2me.lang.Comparable;

public class HugeDigit extends Number implements Comparable {
    private HugeInt intVal;
    private int scale = 0;
    private volatile transient int precision = 0;
    private static final long serialVersionUID = 6108874887143696463L;
    public static final HugeDigit ZERO = new HugeDigit(HugeInt.ZERO, 0);
    public static final HugeDigit ONE = new HugeDigit(HugeInt.ONE, 0);
    public static final HugeDigit TEN = new HugeDigit(HugeInt.TEN, 0);
    
    public HugeDigit(char[] in, int offset, int len) {
        try {
            boolean isneg = false;          
            if (in[offset] == '-') {
                isneg = true;               
                offset++;
                len--;
            } else if (in[offset] == '+') { 
                offset++;
                len--;
            }

            int dotoff = -1;                 
            int cfirst = offset;             
            long exp = 0;                    
            if (len > in.length)             
                throw new NumberFormatException();
            char coeff[] = new char[len];    
            char c;                          

            for (; len > 0; offset++, len--) {
                c = in[offset];
                if ((c >= '0' && c <= '9') || Character.isDigit(c)) {
                    // have digit
                    coeff[precision] = c;
                    precision++;             
                    continue;
                }
                if (c == '.') {
                    // have dot
                    if (dotoff >= 0)         
                        throw new NumberFormatException();
                    dotoff = offset;
                    continue;
                }
                // exponent expected
                if ((c != 'e') && (c != 'E'))
                    throw new NumberFormatException();
                offset++;
                c = in[offset];
                len--;
                boolean negexp = false;
                // optional sign
                if (c == '-' || c == '+') {
                    negexp = (c == '-');
                    offset++;
                    c = in[offset];
                    len--;
                }
                if (len <= 0 || len > 10)    // no digits, or too long
                    throw new NumberFormatException();
                // c now holds first digit of exponent
                for (;; len--) {
                    int v;
                    if (c >= '0' && c <= '9') {
                        v = c - '0';
                    } else {
                        v = Character.digit(c, 10);
                        if (v < 0)            // not a digit
                            throw new NumberFormatException();
                    }
                    exp = exp * 10 + v;
                    if (len == 1)
                        break;               // that was final character
                    offset++;
                    c = in[offset];
                }
                if (negexp)                  // apply sign
                    exp = -exp;
                if ((int)exp != exp)         // overflow
                    throw new NumberFormatException();
                break;                       // [saves a test]
                }
            if (precision == 0)              // no digits found
                throw new NumberFormatException();

            if (dotoff >= 0) {               // had dot; set scale
                scale = precision - (dotoff - cfirst);
                // [cannot overflow]
            }
            if (exp != 0) {                  // had significant exponent
                try {
                    scale = checkScale(-exp + scale); // adjust
                } catch (ArithmeticException e) { 
                    throw new NumberFormatException("Scale out of range.");
                }
            }
            // Remove leading zeros from precision (digits count)
            int first = 0;
            for (; coeff[first] == '0' && precision > 1; first++)
                precision--;

            char quick[];
            if (!isneg) {
                quick = new char[precision];
                System.arraycopy(coeff, first, quick, 0, precision);
            } else {
                quick = new char[precision+1];
                quick[0] = '-';
                System.arraycopy(coeff, first, quick, 1, precision);
            }
            intVal = new HugeInt(quick);
        } catch (ArrayIndexOutOfBoundsException e) {
            throw new NumberFormatException();
        } catch (NegativeArraySizeException e) {
            throw new NumberFormatException();
        }
    }

    public HugeDigit(String val) {
        this(val.toCharArray(), 0, val.length());
    }

    public HugeDigit(HugeInt unscaledVal, int scale) {
        intVal = unscaledVal;
        this.scale = scale;
    }

    public static HugeDigit valueOf(double val) {
        return new HugeDigit(Double.toString(val));
    }

    public HugeDigit divide(HugeDigit divisor, int scale, int roundingMode) {
        if (roundingMode < ROUND_UP || roundingMode > ROUND_UNNECESSARY)
            throw new IllegalArgumentException("Invalid rounding mode");

        HugeDigit dividend;
        if (checkScale((long)scale + divisor.scale) >= this.scale) {
            dividend = this.setScale(scale + divisor.scale);
        } else {
            dividend = this;
            divisor = divisor.setScale(checkScale((long)this.scale - scale));
        }

        HugeInt i[] = dividend.intVal.divideAndRemainder(divisor.intVal);
        HugeInt q = i[0], r = i[1];
        if (r.signum() == 0)
            return new HugeDigit(q, scale);
        if (roundingMode == ROUND_UNNECESSARY)      
            throw new ArithmeticException("Rounding necessary");

        int signum = dividend.signum() * divisor.signum(); 
        boolean increment;
        if (roundingMode == ROUND_UP) {             
            increment = true;
        } else if (roundingMode == ROUND_DOWN) {    
            increment = false;
        } else if (roundingMode == ROUND_CEILING) { 
            increment = (signum > 0);
        } else if (roundingMode == ROUND_FLOOR) {   
            increment = (signum < 0);
        } else { 
            int cmpFracHalf = r.add(r).abs().compareTo(divisor.intVal.abs());
            if (cmpFracHalf < 0) {         /* We're closer to higher digit */
                increment = false;
            } else if (cmpFracHalf > 0) {  /* We're closer to lower digit */
                increment = true;
            } else {                       /* We're dead-center */
                if (roundingMode == ROUND_HALF_UP)
                    increment = true;
                else if (roundingMode == ROUND_HALF_DOWN)
                    increment = false;
                else  
                    increment = q.testBit(0);   
            }
        }
        return (increment
                ? new HugeDigit(q.add(HugeInt.valueOf(signum)), scale)
                : new HugeDigit(q, scale));
    }

    public int signum() {
        return intVal.signum();
    }
    
    public int precision() {
        int result = precision;
        if (result == 0) {
            result = digitLength();
            precision = result;
        }
        return result;
    }

    public final static int ROUND_UP =           0;
    public final static int ROUND_DOWN =         1;
    public final static int ROUND_CEILING =      2;
    public final static int ROUND_FLOOR =        3;
    public final static int ROUND_HALF_UP =      4;
    public final static int ROUND_HALF_DOWN =    5;
    public final static int ROUND_HALF_EVEN =    6;
    public final static int ROUND_UNNECESSARY =  7;
    
    public HugeDigit setScale(int newScale, int roundingMode) {
        if (roundingMode < ROUND_UP || roundingMode > ROUND_UNNECESSARY)
            throw new IllegalArgumentException("Invalid rounding mode");

        if (newScale == this.scale)        // easy case
            return this;
        if (this.signum() == 0)            // zero can have any scale
            return new HugeDigit(HugeInt.ZERO, newScale);
        if (newScale > this.scale) {
            int raise = checkScale((long)newScale - this.scale);
            HugeDigit result = new HugeDigit(
                intVal.multiply(tenToThe(raise)), newScale);
            if (this.precision > 0)
                result.precision = this.precision + newScale - this.scale;
            return result;
        }
        return divide(ONE, newScale, roundingMode);
    }

    public HugeDigit setScale(int newScale) {
        return setScale(newScale, ROUND_UNNECESSARY);
    }

    public int compareTo(Object arg) {
        HugeDigit val = (HugeDigit)arg; 
        int sigDiff = signum() - val.signum();
        if (sigDiff != 0)
            return (sigDiff > 0 ? 1 : -1);

        int aethis = this.precision() - this.scale;    // [-1]
        int aeval  =  val.precision() - val.scale;     // [-1]
        if (aethis < aeval)
            return -this.signum();
        else if (aethis > aeval)
            return this.signum();

        HugeDigit arg2[] = new HugeDigit[2];
        arg2[0] = this;  arg2[1] = val;
        matchScale(arg2);
        return arg2[0].intVal.compareTo(arg2[1].intVal);
    }

    public boolean equals(Object x) {
        if (!(x instanceof HugeDigit))
            return false;
        HugeDigit xDec = (HugeDigit) x;

        return scale == xDec.scale && intVal.equals(xDec.intVal);
    }

    public HugeDigit min(HugeDigit val) {
        return (compareTo(val) <= 0 ? this : val);
    }

    public HugeDigit max(HugeDigit val) {
        return (compareTo(val) >= 0 ? this : val);
    }

    public int hashCode() {
        return 31*intVal.hashCode() + scale;
    }

    public String toString() {
        return new String(layoutChars(true));
    }

    public HugeInt toBigInteger() {
        return this.setScale(0, ROUND_DOWN).intVal;
    }
    
    public long longValue(){
        return toBigInteger().longValue();
    }
    public int intValue() {
        return toBigInteger().intValue();
    }

    private char[] layoutChars(boolean sci) {
        if (scale == 0)                      // zero scale is trivial
            return intVal.toString().toCharArray();
        char coeff[] = intVal.abs().toString().toCharArray();
        StringBuffer buf=new StringBuffer(coeff.length+14);
        if (intVal.signum() < 0)             // prefix '-' if negative
            buf.append('-');
        long adjusted = -(long)scale + (coeff.length-1);
        if ((scale >= 0) && (adjusted >= -6)) { // plain number
            int pad = scale - coeff.length;  // count of padding zeros
            if (pad >= 0) {                  // 0.xxx form
                buf.append('0');
                buf.append('.');
                for (; pad>0; pad--) {
                    buf.append('0');
                }
                buf.append(coeff);
            } else {                         // xx.xx form
                buf.append(coeff, 0, -pad);
                buf.append('.');
                buf.append(coeff, -pad, scale);
            }
        } else { // E-notation is needed
            if (sci) {                       // Scientific notation
                buf.append(coeff[0]);        // first character
                if (coeff.length > 1) {      // more to come
                    buf.append('.');
                    buf.append(coeff, 1, coeff.length-1);
                }
            } else {                         // Engineering notation
                int sig = (int)(adjusted % 3);
                if (sig < 0)
                    sig += 3;                // [adjusted was negative]
                adjusted -= sig;             // now a multiple of 3
                sig++;
                if (intVal.signum() == 0) {
                    switch (sig) {
                    case 1:
                        buf.append('0'); // exponent is a multiple of three
                        break;
                    case 2:
                        buf.append("0.00");
                        adjusted += 3;
                        break;
                    case 3:
                        buf.append("0.0");
                        adjusted += 3;
                        break;
                    default:
                        throw new RuntimeException("Unexpected sig value " + sig);
                    }
                } else if (sig >= coeff.length) {   // significand all in integer
                    buf.append(coeff, 0, coeff.length);
                    // may need some zeros, too
                    for (int i = sig - coeff.length; i > 0; i--)
                        buf.append('0');
                } else {                     // xx.xxE form
                    buf.append(coeff, 0, sig);
                    buf.append('.');
                    buf.append(coeff, sig, coeff.length-sig);
                }
            }
            if (adjusted != 0) {             // [!sci could have made 0]
                buf.append('E');
                if (adjusted > 0)            // force sign for positive
                    buf.append('+');
                buf.append(adjusted);
            }
        }
        char result[] = new char[buf.length()];
        buf.getChars(0, result.length,  result, 0);
        return result;
    }

    private static HugeInt tenToThe(int n) {
        if (n < TENPOWERS.length)     // use value from constant array
            return TENPOWERS[n];
        char tenpow[] = new char[n + 1];
        tenpow[0] = '1';
        for (int i = 1; i <= n; i++) tenpow[i] = '0';
        return new HugeInt(tenpow);
    }
    
    private static HugeInt TENPOWERS[] = {HugeInt.ONE,
        HugeInt.valueOf(10),       HugeInt.valueOf(100),
        HugeInt.valueOf(1000),     HugeInt.valueOf(10000),
        HugeInt.valueOf(100000),   HugeInt.valueOf(1000000),
        HugeInt.valueOf(10000000), HugeInt.valueOf(100000000),
        HugeInt.valueOf(1000000000)};

    private static void matchScale(HugeDigit[] val) {
        if (val[0].scale < val[1].scale)
            val[0] = val[0].setScale(val[1].scale);
        else if (val[1].scale < val[0].scale)
            val[1] = val[1].setScale(val[0].scale);
    }

    private int digitLength() {
        if (intVal.signum() == 0)       // 0 is one decimal digit
            return 1;
        HugeInt work = intVal;
        int digits = 0;                 // counter
        for (;work.mag.length>1;) {
            work = work.divide(TENPOWERS[9]);
            digits += 9;
            if (work.signum() == 0)     // the division was exact
                return digits;          // (a power of a billion)
        }
        digits += intLength(work.mag[0]);
        return digits;
    }

    private int intLength(int i) {
        int digits;
        if (i < 0) {            // 'negative' is 10 digits unsigned
            digits = 10;
        } else {                // positive integer
            if (i < 10000) {
                if (i < 100) {
                    if (i < 10) digits = 1;
                    else digits = 2;
                } else {
                    if (i < 1000) digits = 3;
                    else digits = 4;
                }
            } else {
                if (i < 1000000) {
                    if (i < 100000) digits = 5;
                    else digits = 6;
                } else {
                    if (i < 100000000) {
                        if (i < 10000000) digits = 7;
                        else digits = 8;
                    } else {
                        if (i < 1000000000) digits = 9;
                        else digits = 10;
                    }
                }
            }
        }
        return digits;
    }

    private int checkScale(long val) {
        if ((int)val != val) {
            if ((this.intVal != null &&  this.signum() != 0) || 
                this.intVal == null ) {
                if (val > Integer.MAX_VALUE)
                    throw new ArithmeticException("Underflow");
                if (val < Integer.MIN_VALUE)
                    throw new ArithmeticException("Overflow");
            } else {
                return (val > Integer.MAX_VALUE)?Integer.MAX_VALUE:Integer.MIN_VALUE;
            }
        }
        return (int)val;
    }

}
