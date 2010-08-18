/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng, Jie Li
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyID;
import com.xruby.runtime.lang.RubyMethod;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubySymbol;
import com.xruby.runtime.lang.RubyValue;
import java.util.Hashtable;

import j2me.math.HugeInt;
import org.apache.oro.text.regex.MatchResult;

public class ObjectFactory {
    //For preformance reason, some frequently used numbers are predefined here.
    public static final RubyFixnum FIXNUM_NEGATIVE_ONE = new RubyFixnum(-1);
    public static final RubyFixnum FIXNUM0 = new RubyFixnum(0);
    public static final RubyFixnum FIXNUM1 = new RubyFixnum(1);
    public static final RubyFixnum FIXNUM2 = new RubyFixnum(2);
    public static final RubyFixnum FIXNUM3 = new RubyFixnum(3);
    public static final RubyFixnum FIXNUM4 = new RubyFixnum(4);
    public static final RubyFixnum FIXNUM5 = new RubyFixnum(5);
    public static final RubyFixnum FIXNUM6 = new RubyFixnum(6);
    public static final RubyFixnum FIXNUM7 = new RubyFixnum(7);
    public static final RubyFixnum FIXNUM8 = new RubyFixnum(8);
    public static final RubyFixnum FIXNUM9 = new RubyFixnum(9);
    public static final RubyFixnum FIXNUM10 = new RubyFixnum(10);

    private static final int CACHE_SIZE = 1024;/*cache 0...CACHE_SIZE*/

    private static class FixnumCache {
        private FixnumCache() {
        }

        static final RubyFixnum cache[] = new RubyFixnum[CACHE_SIZE];

        static {
            for (int i = 0; i < CACHE_SIZE; i++) {
                cache[i] = new RubyFixnum(i);
            }
        }
    }

    public static RubyFixnum createFixnum(long value) {
        if (value >= 0 && value < CACHE_SIZE) {
            return FixnumCache.cache[(int)value];
        }

        return new RubyFixnum(value);
    }

    public static RubyFixnum createFixnum(int value) {
        if (value >= 0 && value < CACHE_SIZE) {
            return FixnumCache.cache[(int)value];
        }

        return new RubyFixnum(value);
    }
    
    public static RubyInteger createInteger(long value) {
        return RubyBignum.bignorm(value);
    }

    public static RubyString createString(String value) {
        return new RubyString(value);
    }

    public static RubyString createString() {
        return new RubyString("");
    }

    public static RubyString createString(StringBuffer value) {
        return new RubyString(value);
    }

    public static RubyString createString(RubyClass c, String s) {
        return new RubyString(c, s);
    }

    private static Hashtable m_hashRegex = new Hashtable(); 
    public static synchronized RubyRegexp createRegexp(String value, String option) 
    {
    	String strKey = value+option;
    	Object obj = m_hashRegex.get(strKey);
    	if ( obj == null )
    	{
    		obj = new RubyRegexp(value, option);
    		m_hashRegex.put(strKey, obj);
    	}
        return (RubyRegexp)obj;
    }

    public static RubyRegexp createRegexp() {
        return new RubyRegexp();
    }

    public static RubySymbol createSymbol(String value) {
        RubyID id = RubyID.intern(value);
        return id.toSymbol();
    }

    public static RubySymbol createSymbol(RubyID id) {
        return id.toSymbol();
    }

    public static RubyFloat createFloat(double value) {
        return new RubyFloat(value);
    }

    public static RubyArray createArray(int size, RubyValue default_value) {
        return new RubyArray(size, default_value);
    }

    public static RubyArray createArray(int size, int rhs_size, boolean has_single_asterisk_or_lambda_call) {
        return new RubyArray(size, rhs_size, has_single_asterisk_or_lambda_call);
    }

    public static RubyHash createHash() {
        return new RubyHash();
    }

    public static RubyIO createFile(String filename, String mode) {
        return new RubyFile(filename, mode);
    }

    //RHO_COMMENT
    public static RubyIO createResourceFile(String filename, String mode) 
    {
    	InputStreamExecutor ise = new InputStreamExecutor(filename, mode);
    	if ( ise.getInputStream() == null )
    		return null;
        return new RubyFile( ise, RubyRuntime.FileClass );
    }
    //RHO_COMMENT
    
    public static RubyMethodValue createMethod(RubyValue r, String s, RubyMethod m) {
        return new RubyMethodValue(r, s, m);
    }

    public static RubyProc createProc(RubyBlock block) {
        return new RubyProc(block);
    }

    public static RubyTime createTime(long v) {
        return new RubyTime(v);
    }

    public static RubyTime createTime() {
        return new RubyTime();
    }

    public static RubyRange createRange() {
        return new RubyRange();
    }

    public static RubyRange createRange(RubyValue left, RubyValue right, boolean isExclusive) {
        RubyRange r = new RubyRange();
        r.setValue(left, right, isExclusive);
        return r;
    }

    public static RubyMatchData createMatchData(MatchResult m,String input) {
        return new RubyMatchData(m,input);
    }

    public static RubyBignum createBignum(HugeInt value) {
        return new RubyBignum(value);
    }

    public static RubyBignum createBignum(String value) {
        return new RubyBignum(new HugeInt(value));
    }

    public static RubyValue createBoolean(boolean is_true) {
        return is_true ? RubyConstant.QTRUE : RubyConstant.QFALSE;
    }

    public static RubyThread createThread(RubyBlock block) {
        return new RubyThread(block);
    }
}
