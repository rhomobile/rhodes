/**
 * Copyright 2005-2007 Xue Yong Zhi, Jie Li, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Formatter;
import java.util.StringTokenizer;

import com.xruby.runtime.lang.*;
import com.xruby.runtime.lang.annotation.RubyAllocMethod;
import com.xruby.runtime.lang.annotation.RubyLevelClass;
import com.xruby.runtime.lang.annotation.RubyLevelMethod;

@RubyLevelClass(name="String")
public class RubyString extends RubyBasic {
    private StringBuilder sb_;

    RubyString(RubyClass c, String s) {
        super(c);
        sb_ = new StringBuilder(s);
    }

    RubyString(String s) {
        super(RubyRuntime.StringClass);
        sb_ = new StringBuilder(s);
    }

    RubyString(StringBuilder sb) {
        super(RubyRuntime.StringClass);
        sb_ = sb;
    }

    public RubyString clone() {
        RubyString s = (RubyString)super.clone();
        s.sb_ = new StringBuilder(sb_);
        return s;
    }

    public String toString() {
        return sb_.toString();
    }

    public int toInt() {
        return Integer.valueOf(sb_.toString());
    }

    public RubyInteger toRubyInteger() {
        return RubyBignum.bignorm(this.toInt());
    }

    public double toFloat() {
		try {
        	return Double.parseDouble(this.sb_.toString());
		} catch (NumberFormatException e) {
			return 0.0;
		}
    }

    public RubyString toRubyString() {
        return this;
    }

    public String toStr() {
        return this.sb_.toString();
    }

    public String asString() {
        return this.sb_.toString();
    }

    public RubyID toID() {
        return RubyID.intern(this.sb_.toString());
    }

    public boolean equals(Object obj) {
        if (obj == this) {
            return true;
        } else if (obj instanceof RubyString) {
            RubyString s = (RubyString)obj;
            return this.sb_.toString().equals(s.sb_.toString());
        } else {
            return super.equals(obj);
        }
    }

    public int hashCode() {
        return sb_.toString().hashCode();
    }

    public int length() {
        return sb_.length();
    }

    public RubyString appendString(String v) {
        sb_.append(v);
        return this;
    }

    private RubyString appendString(RubyString v) {
        sb_.append(v.sb_);
        return this;
    }

    public RubyString appendString(RubyValue v) {
        if (v instanceof RubyString) {
            return appendString((RubyString)v);
        } else {
            RubyValue r = RubyAPI.callPublicNoArgMethod(v, null, RubyID.toSID);
            return appendString((RubyString)r);
        }
    }

    @RubyAllocMethod
    public static RubyString alloc(RubyValue receiver) {
        return ObjectFactory.createString((RubyClass)receiver, "");
    }

    @RubyLevelMethod(name="initialize", alias="initialize_copy")
    public RubyString initialize(RubyValue v) {
        this.setString(v.toStr());
        return this;
    }

    @RubyLevelMethod(name="initialize", alias="initialize_copy")
    public RubyString initialize() {
        return this;
    }

    @RubyLevelMethod(name="to_f")
    public RubyFloat toRubyFloat() {
        double d;
        try {
            d = this.toFloat();
        } catch (NumberFormatException e) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, e.toString());
        }
        return ObjectFactory.createFloat(d);
    }

    @RubyLevelMethod(name="to_s")
    public RubyString to_s() {
        return ObjectFactory.createString(this.sb_.toString());
    }

    @RubyLevelMethod(name="length")
    public RubyFixnum rubyLength() {
        return ObjectFactory.createFixnum(sb_.length());
    }

    @RubyLevelMethod(name="intern", alias="to_sym")
    public RubySymbol intern() {
        if (this.sb_.length() <= 0) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "interning empty string");
        }

        RubyID id = RubyID.intern(this.sb_.toString());
        return id.toSymbol();
    }

    @RubyLevelMethod(name="+")
    public RubyString plus(RubyValue v) {
        StringBuilder sb = new StringBuilder();
        sb.append(this.sb_);
        sb.append(v.toRubyString().sb_);
        return ObjectFactory.createString(sb);
    }

    @RubyLevelMethod(name="concat", alias="<<")
    public RubyString concat(RubyValue v) {
        if (v instanceof RubyFixnum) {
            int i = v.toInt();
            if (i >= 0 && i <= 0xff) {
                this.sb_.append((char)i);
                return this;
            }
        }

        this.sb_.append(v.toRubyString().sb_);
        return this;
    }

    public RubyString setString(String s) {
        sb_.replace(0, sb_.length(), s);
        return this;
    }

    private int cmp(RubyString s) {
        int result = this.sb_.toString().compareTo(s.sb_.toString());
        if (result == 0) {
            return 0;
        } else if (result > 0) {
            return 1;
        } else {
            return -1;
        }
    }

    private String replace(String source, int start, int end, String replacement) {
        assert(start <= source.length() - 1);

        if (end < start) {
            end = start + 1;
        }

        StringBuffer result = new StringBuffer(source.substring(0, start));
        result.append(replacement);
        result.append(source.substring(end));
        return result.toString();
    }

    private RubyValue substring(String string, int begin, int end, boolean isExcludeEnd) {

        if (begin < 0) {
            begin = string.length() + begin;
        }

        if (end < 0) {
            end = string.length() + end;
        }

        if (!isExcludeEnd) {
            ++end;
        }

        if (begin < 0 || end < 0 || begin > end || begin > string.length() || end > string.length()) {
            return RubyConstant.QNIL;
        }

        return ObjectFactory.createString(string.substring(begin, end));
    }

    private RubyString gsub(RubyString g, RubyArray args) {
        if (null == args || args.size() != 2) {
            int actual_argc = (null == args ) ? 0 : args.size();
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `gsub': wrong number of arguments (" + actual_argc + " for 2)");
        }

        if (!(args.get(1) instanceof RubyString)) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "can't convert " + args.get(1).getRubyClass().getName() + " into String");
        }

        RubyString s = (RubyString) args.get(1);

        if (args.get(0) instanceof RubyRegexp) {
            RubyRegexp r = (RubyRegexp) args.get(0);
            return r.gsub(g, s);
        } else if (args.get(0) instanceof RubyString) {
            RubyString r = (RubyString) args.get(0);
            String result = g.toString().replaceAll(r.toString(), s.toString());
            return ObjectFactory.createString(result);
        } else {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong argument type " + args.get(0).getRubyClass().getName() + " (expected Regexp)");
        }
    }

    private RubyString sub(RubyString g, RubyArray args) {
        if (null == args || args.size() != 2) {
            int actual_argc = (null == args ) ? 0 : args.size();
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `sub': wrong number of arguments (" + actual_argc + " for 2)");
        }

        if (!(args.get(1) instanceof RubyString)) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "can't convert " + args.get(1).getRubyClass().getName() + " into String");
        }

        RubyString s = (RubyString) args.get(1);

        if (args.get(0) instanceof RubyRegexp) {
            RubyRegexp r = (RubyRegexp) args.get(0);
            return r.sub(g, s);
        } else if (args.get(0) instanceof RubyString) {
            RubyString r = (RubyString) args.get(0);
            String result = g.toString().replaceFirst(r.toString(), s.toString());
            return ObjectFactory.createString(result);
        } else {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong argument type " + args.get(0).getRubyClass().getName() + " (expected Regexp)");
        }
    }

    private Collection<String> split(RubyString s, String delimiter) {
        StringTokenizer t = new StringTokenizer(s.toString(), delimiter);
        int total = t.countTokens();
        Collection<String> r = new ArrayList<String>(total);
        for (int i = 0; i < total; ++i) {
            r.add(t.nextToken());
        }
        return r;
    }

    private Collection<String> split(RubyString g, RubyRegexp r, RubyArray args) {
        if (args.size() <= 1) {
            return r.split(g.toString(), 0);
        } else {
            RubyFixnum i = (RubyFixnum) args.get(1);
            return r.split(g.toString(), i.toInt());
        }
    }

    @RubyLevelMethod(name="==")
    public RubyValue opEqual(RubyValue v) {
        if (this == v) {
            return RubyConstant.QTRUE;
        }

        if (v instanceof RubyString) {
            RubyString str = ((RubyString)v);
            if ((this.sb_.length() == str.sb_.length() && this.cmp(str) == 0)) {
                return RubyConstant.QTRUE;
            } else {
                return RubyConstant.QFALSE;
            }
        }

        if (v.respondTo(RubyID.toStrID)) {
            return ObjectFactory.createBoolean(v.equals(this));
        } else {
            return RubyConstant.QFALSE;
        }
    }


    @RubyLevelMethod(name="strip")
    public RubyString strip() {
        return ObjectFactory.createString(sb_.toString().trim());
    }

    @RubyLevelMethod(name="lstrip!")
    public RubyValue lstripBang() {
        int i = 0;
        while (i < sb_.length() && Character.isWhitespace(sb_.charAt(i))) {
            i++;
        }

        if (0 == i) {
            //No change
            return RubyConstant.QNIL;
        }

        sb_.delete(0, i);
        return this;
    }

    @RubyLevelMethod(name="strip!")
    public RubyValue stripBang() {
        String str = this.sb_.toString();
        int orgSize = str.length();
        str = str.trim();
        this.sb_ = new StringBuilder(str);
        if (str.length() == orgSize) {
            return RubyConstant.QNIL;
        }

        return this;
    }

    @RubyLevelMethod(name="capitalize")
    public RubyString capitalize() {
        int length = this.sb_.length();
        if (length == 0) {
            return ObjectFactory.createString();
        }

        char[] ca = new char[length];
        this.sb_.getChars(0, length, ca, 0);

        ca[0] = Character.toUpperCase(ca[0]);
        for (int i = 1; i < length; i++) {
            ca[i] = Character.toLowerCase(ca[i]);
        }

        return ObjectFactory.createString(new StringBuilder().append(ca));
    }

    @RubyLevelMethod(name="capitalize!")
    public RubyValue capitalizeBang() {
        int length = this.sb_.length();
        if (length == 0) {
            return RubyConstant.QNIL;
        }

        boolean modify = false;
        char[] ca = new char[length];
        this.sb_.getChars(0, length, ca, 0);

        if (Character.isLowerCase(ca[0])) {
            ca[0] = Character.toUpperCase(ca[0]);
            modify = true;
        }

        for (int i = 1; i < ca.length; i++) {
            char c = ca[i];
            if (Character.isUpperCase(c)) {
                ca[i] = Character.toLowerCase(c);
                modify = true;
            }
        }

        this.sb_.delete(0, ca.length).append(ca);
        return modify ? this : RubyConstant.QNIL;
    }

    @RubyLevelMethod(name="upcase")
    public RubyString upcase() {
        int length = this.sb_.length();
        if (length == 0) {
            return ObjectFactory.createString();
        }

        char[] ca = new char[length];
        this.sb_.getChars(0, length, ca, 0);

        for (int i = 0; i < ca.length; i++) {
            ca[i] = Character.toUpperCase(ca[i]);
        }

        return ObjectFactory.createString(new StringBuilder().append(ca));
    }

    @RubyLevelMethod(name="upcase!")
    public RubyValue upcaseBang() {
        boolean modify = false;

        int length = this.sb_.length();
        char[] ca = new char[length];
        this.sb_.getChars(0, length, ca, 0);

        for (int i = 0; i < length; i++) {
            char c = ca[i];
            if (Character.isLowerCase(c)) {
                ca[i] = Character.toUpperCase(c);
                modify = true;
            }
        }
        this.sb_.delete(0, ca.length).append(ca);
        return modify ? this : RubyConstant.QNIL;
    }

    @RubyLevelMethod(name="downcase")
    public RubyString downcase() {
        int length = this.sb_.length();
        if (length == 0) {
            return ObjectFactory.createString();
        }

        char[] ca = new char[length];
        this.sb_.getChars(0, length, ca, 0);

        for (int i = 0; i < ca.length; i++) {
            ca[i] = Character.toLowerCase(ca[i]);
        }

        return ObjectFactory.createString(new StringBuilder().append(ca));
    }

    @RubyLevelMethod(name="downcase!")
    public RubyValue downcaseBang() {
        boolean modify = false;
        int length = this.sb_.length();
        char[] ca = new char[length];
        this.sb_.getChars(0, length, ca, 0);
        for (int i = 0; i < ca.length; i++) {
            char c = ca[i];
            if (Character.isUpperCase(c)) {
                ca[i] = Character.toLowerCase(c);
                modify = true;
            }
        }
        this.sb_.delete(0, ca.length).append(ca);
        return modify ? this : RubyConstant.QNIL;
    }

    @RubyLevelMethod(name="reverse!")
    public RubyString reverse_danger() {
        sb_.reverse();
        return this;
    }

    private boolean chomp(String seperator) {
        if (!sb_.toString().endsWith(seperator)) {
            return false;
        }

        int start = sb_.length() - seperator.length();
        int end = sb_.length();
        sb_.delete(start, end);
        return true;
    }

    @RubyLevelMethod(name="scan")
    public RubyValue scan(RubyValue arg, RubyBlock block) {
        RubyRegexp regex = (RubyRegexp)arg;
        if (null != block) {
            regex.scan(sb_.toString(), block);
            return this;
        } else {
            return regex.scan(sb_.toString());
        }
    }

    /// @return false if no change made
    // TODO handle more situations
    private boolean transform(String from, String to, boolean remove_duplicate) {
        String oldString = sb_.toString();

        if (from.length() == 3 && to.length() == 3 && from.charAt(1) == '-' && to.charAt(1) == '-') {
            char from_start = from.charAt(0);
            char from_end = from.charAt(2);
            char to_start = to.charAt(0);
            char to_end = to.charAt(2);

            char last_char = 0;
            for (int i = 0; i < sb_.length(); ++i) {
                char current_char = sb_.charAt(i);
                if (current_char >= from_start && current_char <= from_end) {
                    if (remove_duplicate && last_char == current_char) {
                        sb_.deleteCharAt(i);
                        --i;
                    } else {
                        int replace_char = (current_char - from_start) + to_start;
                        sb_.setCharAt(i, replace_char < to_end ? (char)replace_char : to_end);
                        last_char = current_char;
                    }
                }
            }
        }else {
            char last_char = 0;
            for (int i = 0; i < sb_.length(); ++i) {
                char current_char = sb_.charAt(i);
                int index = from.indexOf(current_char);
                if (index >= 0) {
                    if (remove_duplicate && last_char == current_char) {
                            sb_.deleteCharAt(i);
                            --i;
                        } else {
                            char replace_char = to.charAt(index < to.length() ? index : to.length());
                            sb_.setCharAt(i, replace_char);
                            last_char = current_char;
                        }
                }
            }
        }

        return !oldString.equals(sb_.toString());
    }

    private boolean tr(String from, String to) {
        return transform(from, to, false);
    }

    private boolean tr_s(String from, String to) {
        return transform(from, to, true);
    }

    private boolean squeeze(String from) {
        if (null != from && from.length() == 3 && from.charAt(1) == '-' ) {
            char from_start = from.charAt(0);
            char from_end = from.charAt(2);
            char last_char = 0;
            for (int i = 0; i < sb_.length(); ++i) {
                char current_char = sb_.charAt(i);
                if (current_char >= from_start && current_char <= from_end) {
                    if (last_char == current_char) {
                        sb_.deleteCharAt(i);
                        --i;
                    } else {
                        last_char = current_char;
                    }
                }
            }
            return true;
        }

        //TODO handle more situations
        return false;
    }

    private boolean delete(String from) {
        if (null != from && from.length() == 3 && from.charAt(1) == '-' ) {
            char from_start = from.charAt(0);
            char from_end = from.charAt(2);
            for (int i = 0; i < sb_.length(); ++i) {
                char current_char = sb_.charAt(i);
                if (current_char >= from_start && current_char <= from_end) {
                    sb_.deleteCharAt(i);
                    --i;
                }
            }
            return true;
        } else {
            boolean changed = false;
            for (;;) {
                int index = sb_.indexOf(from);
                if (index < 0) {
                    return changed;
                }

                sb_.delete(index, index + from.length());
                changed = true;
            }
        }
    }

    private int count(String s) {
        int n = 0;
        for (int i = 0; i < sb_.length(); ++i) {
            if (s.indexOf(sb_.charAt(i)) >= 0) {
                ++n;
            }
        }
        return n;
    }

    @RubyLevelMethod(name="swapcase")
    public RubyString swapcase() {
        int length = this.sb_.length();
        if (length == 0) {
            return ObjectFactory.createString();
        }

        char[] ca = new char[length];
        this.sb_.getChars(0, length, ca, 0);

        for (int i = 0; i < length; i++) {
            char c = ca[i];
            if (Character.isUpperCase(c)) {
                ca[i] = Character.toLowerCase(c);
            } else if (Character.isLowerCase(c)) {
                ca[i] = Character.toUpperCase(c);
            }
        }

        return ObjectFactory.createString(new StringBuilder().append(ca));
    }

    @RubyLevelMethod(name="swapcase!")
    public RubyValue swapcaseBang() {
        int length = this.sb_.length();
        if (length == 0) {
            return RubyConstant.QNIL;
        }

        char[] ca = new char[length];
        this.sb_.getChars(0, length, ca, 0);
        boolean modify = false;

        for (int i = 0; i < length; i++) {
            char c = ca[i];
            if (Character.isUpperCase(c)) {
                ca[i] = Character.toLowerCase(c);
                modify = true;
            } else if (Character.isLowerCase(c)) {
                ca[i] = Character.toUpperCase(c);
                modify = true;
            }
        }

        this.sb_.delete(0, ca.length).append(ca);
        return modify ? this : RubyConstant.QNIL;
    }

    @RubyLevelMethod(name="chop")
    public RubyValue chop() {
        RubyString rs = this.clone();
        rs.chopBang();
        return rs;
    }

    @RubyLevelMethod(name="chop!")
    public RubyValue chopBang() {
        int length = this.sb_.length();
        if (length > 0) {
            int orgLength = length;
            length--;
            if (this.sb_.charAt(length) == '\n') {
                if (length > 0 && this.sb_.charAt(length - 1) == '\r') {
                    length--;
                }
            }

            this.sb_.delete(length, orgLength);
        }

        return RubyConstant.QNIL;
    }

    private boolean isEvstr(char c, int current, int end) {
        return (current < end) && (c == '$') && (c == '@') && (c == '{');
    }

    private boolean isAscii(char c) {
        return c <= 0x7F;
    }

    private boolean isPrint(char c) {
        return isAscii(c) && c > 0x1F;
    }

    private static Formatter formatter;

    private String formatForDump(String format, char c) {
        if (RubyString.formatter == null) {
            RubyString.formatter = new Formatter();
        }

        return RubyString.formatter.format(format, c).toString();
    }

    public String dump() {
        int length = this.sb_.length();
        StringBuilder buf = new StringBuilder();
        buf.append('"');

        for (int i = 0; i < length; i++) {
            char c = this.sb_.charAt(i);

            if (c == '"' || c == '\\') {
                buf.append('\\');
                buf.append(c);
            } else if (c == '#') {
                if (isEvstr(c, i, length - 1)) {
                    buf.append('\\');
                }
                buf.append('#');
            } else if (isPrint(c)) {
                buf.append(c);
            } else if (c == '\n') {
                buf.append('\\');
                buf.append('n');
            } else if (c == '\r') {
                buf.append('\\');
                buf.append('r');
            } else if (c == '\t') {
                buf.append('\\');
                buf.append('t');
            } else if (c == '\f') {
                buf.append('\\');
                buf.append('f');
            } else if (c == '\013') {
                buf.append('\\');
                buf.append('v');
            } else if (c == '\010') {
                buf.append('\\');
                buf.append('b');
            } else if (c == '\007') {
                buf.append('\\');
                buf.append('a');
            } else if (c == '\033') {
                buf.append('\\');
                buf.append('e');
            } else {
                buf.append('\\');
                buf.append(formatForDump("%03o", c));
            }
        }

        buf.append('"');

        return buf.toString();
    }

    @RubyLevelMethod(name="dump")
    public RubyString rubyDump() {
        return ObjectFactory.createString(this.dump());
    }

    @RubyLevelMethod(name="each", alias="each_line")
    public RubyValue each(RubyBlock block) {
        // FIXME: for each line
        block.invoke(this, this);
        return this;
    }

    @RubyLevelMethod(name="to_i")
    public RubyValue to_i() {
        return to_i(10);
    }

    @RubyLevelMethod(name="to_i")
    public RubyValue to_i(RubyValue arg) {
        return to_i(arg.toInt());
    }

    private RubyValue to_i(int radix) {
        String value = toString();

        value = value.replaceAll("[^+\\-a-zA-Z0-9]", "");
        int end = value.indexOf('+', 1);
        int end1 = value.indexOf('-', 1);

        if (end < 0) {
            if (end1 < 0) {
                end = value.length();
            } else {
                end = end1;
            }
        } else {
            if (end1 >= 0) {
                end = Math.min(end, end1);
            }
        }

        value = value.substring(0, end);

        if (radix >= 2 && radix <= 36) {
            BigInteger bigint;
            try {
                bigint = new BigInteger(value, radix);
            } catch (NumberFormatException e) {
                return ObjectFactory.FIXNUM0;
            }
            return RubyBignum.bignorm(bigint);
        }
        throw new RubyException(RubyRuntime.ArgumentErrorClass, "illegal radix " + radix);
    }

    @RubyLevelMethod(name="hex")
    public RubyValue hex() {
        String s = toString();
        if (s.startsWith("0x")) {
            s = s.substring("0x".length());
        }

        try {
            return ObjectFactory.createFixnum(Integer.valueOf(s, 16));
        } catch (NumberFormatException e) {
            return ObjectFactory.FIXNUM0;
        }

    }

    @RubyLevelMethod(name="gsub")
    public RubyValue gsub(RubyArray args, RubyBlock block) {
        if (null == block) {
            return gsub(this, args);
        } else {
            if (null == args || args.size() != 1) {
                int actual_argc = (null == args ) ? 0 : args.size();
                throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `gsub': wrong number of arguments (" + actual_argc + " for 1)");
            }
            if (!(args.get(0) instanceof RubyRegexp)) {
                throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong argument type " + args.get(0).getRubyClass().getName() + " (expected Regexp)");
            }

            RubyRegexp r = (RubyRegexp) args.get(0);
            return r.gsub(this, block);
        }
    }

    @RubyLevelMethod(name="gsub!")
    public RubyValue gsub_danger(RubyArray args, RubyBlock block) {
        if (null == block) {
            RubyString result = gsub(this, args);

            if (result == this) {
                return RubyConstant.QNIL;
            } else {
                return setString(result.toString());
            }
        } else {
            if (null == args || args.size() != 1) {
                int actual_argc = (null == args ) ? 0 : args.size();
                throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `gsub!': wrong number of arguments (" + actual_argc + " for 1)");
            }
            if (!(args.get(0) instanceof RubyRegexp)) {
                throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong argument type " + args.get(0).getRubyClass().getName() + " (expected Regexp)");
            }

            RubyRegexp r = (RubyRegexp) args.get(0);
            return setString(r.gsub(this, block).toString());
        }
    }

    @RubyLevelMethod(name="sub")
    public RubyValue sub(RubyArray args, RubyBlock block) {
        if (null == block) {
            return sub(this, args);
        } else {
            if (null == args || args.size() != 1) {
                int actual_argc = (null == args ) ? 0 : args.size();
                throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `sub': wrong number of arguments (" + actual_argc + " for 1)");
            }
            if (!(args.get(0) instanceof RubyRegexp)) {
                throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong argument type " + args.get(0).getRubyClass().getName() + " (expected Regexp)");
            }

            RubyRegexp r = (RubyRegexp) args.get(0);
            return r.sub(this, block);
        }
    }

    @RubyLevelMethod(name="sub!")
    public RubyValue sub_danger(RubyArray args, RubyBlock block) {
        if (null == block) {
            RubyString result = sub(this, args);
            if (result == this) {
                return RubyConstant.QNIL;
            } else {
                return setString(result.toString());
            }
        } else {
            if (null == args || args.size() != 1) {
                int actual_argc = (null == args ) ? 0 : args.size();
                throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `sub!': wrong number of arguments (" + actual_argc + " for 1)");
            }
            if (!(args.get(0) instanceof RubyRegexp)) {
                throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong argument type " + args.get(0).getRubyClass().getName() + " (expected Regexp)");
            }

            RubyRegexp r = (RubyRegexp) args.get(0);
            return setString(r.sub(this, block).toString());
        }
    }

    @RubyLevelMethod(name="split")
    public RubyValue split(RubyArray args) {
        RubyValue r = (null == args) ? GlobalVariables.get("$;") : args.get(0);

        Collection<String> splitResult;
        if (r == RubyConstant.QNIL) {
            splitResult = split(this, " ");
        } else if (r instanceof RubyRegexp) {
            splitResult = split(this, (RubyRegexp) r, args);
        } else if (r instanceof RubyString) {
            splitResult = split(this, ((RubyString) r).toString());
        } else {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong argument type " + r.getRubyClass() + " (expected Regexp)");
        }

        RubyArray a = new RubyArray(splitResult.size());
        int i = 0;
        for (String str : splitResult) {
            if (0 != i || !str.equals("")) {
                //To conform ruby's behavior, discard the first empty element
                a.add(ObjectFactory.createString(str));
            }
            ++i;
        }
        return a;
    }

    @RubyLevelMethod(name="<=>")
    public RubyValue operator_compare(RubyValue arg) {
        if (!(arg instanceof RubyString)) {
            return RubyConstant.QNIL;
        }

        RubyString value2 = (RubyString) arg;
        int compare = toString().compareTo(value2.toString());
        if (compare > 0) {
            compare = 1;
        } else if (compare < 0) {
            compare = -1;
        }
        return ObjectFactory.createFixnum(compare);
    }

    @RubyLevelMethod(name="casecmp")
    RubyValue run(RubyValue arg) {
        if (!(arg instanceof RubyString)) {
            return RubyConstant.QNIL;
        }

        RubyString value2 = (RubyString) arg;
        int compare = toString().toUpperCase().compareTo(value2.toString().toUpperCase());
        if (compare > 0) {
            compare = 1;
        } else if (compare < 0) {
            compare = -1;
        }
        return ObjectFactory.createFixnum(compare);
    }

    @RubyLevelMethod(name="=~")
    public RubyValue operator_match(RubyValue arg) {
        if (arg instanceof RubyRegexp) {
            RubyRegexp reg = (RubyRegexp) arg;
            int p = reg.matchPosition(toString());
            if (p >= 0) {
                return ObjectFactory.createFixnum(p);
            } else {
                return RubyConstant.QNIL;
            }
        } else {
            return RubyAPI.callPublicOneArgMethod(arg, this, null, RubyID.matchID);
        }
    }

    @RubyLevelMethod(name="[]")
    public RubyValue array_access(RubyArray args) {
        String string = toString();
        if (args.size() == 1) {
            RubyValue arg = args.get(0);
            if (arg instanceof RubyString) {
                String str = ((RubyString) arg).toString();
                if (string.indexOf(str) >= 0) {
                    return ObjectFactory.createString(str);
                } else {
                    return RubyConstant.QNIL;
                }
            } else if (arg instanceof RubyRange) {
                RubyRange range = (RubyRange) arg;
                int start = range.getLeft().toInt();
                int end = range.getRight().toInt();
                return substring(string, start, end, range.isExcludeEnd());
            } else if (arg instanceof RubyRegexp) {
                RubyRegexp regexp = (RubyRegexp) arg;
                RubyMatchData match = regexp.match(string);
                if (match != null) {
                    return ObjectFactory.createString(match.toString());
                } else {
                    return RubyConstant.QNIL;
                }
            } else {
                int index = arg.toInt();
                if (index < 0) {
                    index = string.length() + index;
                }

                if (index < 0 || index >= string.length()) {
                    return RubyConstant.QNIL;
                } else {
                    return ObjectFactory.createFixnum(string.charAt(index));
                }
            }
        } else {
            int start = args.get(0).toInt();
            int length = args.get(1).toInt() - 1;

            return substring(string, start, start + length, false);
        }
    }

    @RubyLevelMethod(name="[]=")
    public RubyValue array_set(RubyArray args) {
        String string = toString();
        String replacement;

        int start, end;

        if (args.size() == 2) {
            RubyValue arg = args.get(0);
            replacement = ((RubyString) args.get(1)).toString();

            if (arg instanceof RubyString) {
                String str = ((RubyString) arg).toString();
                start = string.indexOf(str);
                if (start >= 0) {
                    end = start + str.length();
                } else {
                    throw new RubyException(RubyRuntime.IndexErrorClass, "string not matched");
                }
            } else if (arg instanceof RubyRange) {
                RubyRange range = (RubyRange) arg;
                start = range.getLeft().toInt();
                end = range.getRight().toInt();
                if (start >= string.length()) {
                    throw new RubyException(RubyRuntime.RangeClass, range.toString() + " out of range");
                }
            } else if (arg instanceof RubyRegexp) {
                RubyRegexp regexp = (RubyRegexp) arg;
                RubyMatchData match = regexp.match(string);
                if (match != null) {
                    String matched = match.toString();
                    start = string.indexOf(matched);
                    end = matched.length() + start;
                } else {
                    throw new RubyException(RubyRuntime.IndexErrorClass, "regexp not matched");
                }
            } else {
                start = arg.toInt();
                end = start + 1;
            }
        } else {
            replacement = ((RubyString) args.get(2)).toString();

            start = args.get(0).toInt();
            end = args.get(1).toInt() + start;
            if (start >= string.length()) {
                throw new RubyException(RubyRuntime.RangeClass, String.format("index %d out of string", start));
            }
        }
        setString(replace(string, start, end, replacement));
        return ObjectFactory.createString(replacement);
    }

    @RubyLevelMethod(name="%")
    public RubyValue format(RubyValue arg) {
        String format = toString();
        String s;
        if (arg instanceof RubyArray) {
            s = String.format(format, RubyKernelModule.buildFormatArg((RubyArray)arg, 0));
        } else {
            s = String.format(format, RubyKernelModule.buildFormatArg(new RubyArray(arg), 0));
        }
        return ObjectFactory.createString(s);
    }

    @RubyLevelMethod(name="*")
    public RubyValue operator_star(RubyValue arg) {
        String string = toString();
        int count = arg.toInt();
        if (count < 0) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "negative argument");
        }
        StringBuilder result = new StringBuilder();
        for (int i = 0; i < count; ++i) {
            result.append(string);
        }
        return ObjectFactory.createString(result);
    }

    @RubyLevelMethod(name="each_byte")
    public RubyValue each_byte(RubyBlock block) {
        String string = toString();
        for (int i = 0; i < string.length(); ++i) {
            char c = string.charAt(i);
            block.invoke(this, ObjectFactory.createFixnum((int) c));
        }
        return this;
    }

    @RubyLevelMethod(name="reverse")
    public RubyValue reverse() {
        RubyString string = ObjectFactory.createString(toString());
        return string.reverse_danger();
    }

    @RubyLevelMethod(name="chomp")
    public RubyValue chomp(RubyArray args) {
        RubyString string = ObjectFactory.createString(toString());
        RubyValue separator = (null != args) ? args.get(0) : GlobalVariables.get("$/");
        string.chomp(((RubyString) separator).toString());
        return string;
    }

    @RubyLevelMethod(name="chomp!")
    public RubyValue chomp_danger(RubyArray args) {
        RubyValue separator = (null == args) ? GlobalVariables.get("$/") : args.get(0);
        if (chomp(((RubyString) separator).toString())) {
            return this;
        } else {
            return RubyConstant.QNIL;
        }
    }

    @RubyLevelMethod(name="tr!")
    public RubyValue tr_danger(RubyValue arg1, RubyValue arg2) {
        RubyString from = (RubyString) arg1;
        RubyString to = (RubyString) arg2;
        return tr(from.toString(), to.toString()) ? this : RubyConstant.QNIL;
    }

    @RubyLevelMethod(name="tr")
    public RubyValue tr(RubyValue arg1, RubyValue arg2) {
        RubyString from = (RubyString) arg1;
        RubyString to = (RubyString) arg2;
        tr(from.toString(), to.toString());
        return this;
    }

    @RubyLevelMethod(name="tr_s!")
    public RubyValue trs_danger(RubyValue arg1, RubyValue arg2) {
        RubyString from = (RubyString) arg1;
        RubyString to = (RubyString) arg2;
        return tr_s(from.toString(), to.toString()) ? this : RubyConstant.QNIL;
    }

    @RubyLevelMethod(name="tr_s")
    public RubyValue tr_s(RubyValue arg1, RubyValue arg2) {
        RubyString string = ObjectFactory.createString(toString());
        RubyString from = (RubyString) arg1;
        RubyString to = (RubyString) arg2;
        string.tr_s(from.toString(), to.toString());
        return string;
    }

    @RubyLevelMethod(name="squeeze!")
    public RubyValue squeeze_danger(RubyArray args) {
        String arg = ((null == args) ? null : ((RubyString) args.get(0)).toString());
        return squeeze(arg) ? this : RubyConstant.QNIL;
    }

    @RubyLevelMethod(name="squeeze")
    public RubyValue squeeze(RubyArray args) {
        RubyString string = ObjectFactory.createString(toString());
        String arg = ((null == args) ? null : ((RubyString) args.get(0)).toString());
        string.squeeze(arg);
        return string;
    }

    @RubyLevelMethod(name="delete!")
    public RubyValue delete_danger(RubyArray args) {
        if (null == args) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong number of arguments");
        }

        String arg = ((RubyString) args.get(0)).toString();
        return delete(arg) ? this : RubyConstant.QNIL;
    }

    @RubyLevelMethod(name="delete")
    public RubyValue delete(RubyArray args) {
        if (null == args) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong number of arguments");
        }

        RubyString string = ObjectFactory.createString(toString());
        String arg = ((RubyString) args.get(0)).toString();
        string.delete(arg);
        return string;
    }

    @RubyLevelMethod(name="count")
    public RubyValue count(RubyArray args) {
        if (null == args) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong number of arguments");
        }

        //TODO incomplete
        int n = 0;
        for (RubyValue v : args) {
            RubyString other_str = (RubyString) v;
            n += count(other_str.toString());
        }
        return ObjectFactory.createFixnum(n);
    }

    @RubyLevelMethod(name="unpack")
    public RubyValue unpack(RubyValue arg) {
        RubyString format = ((RubyString) arg);
        return ArrayPacker.unpack(toString(), format.toString());
    }

}
