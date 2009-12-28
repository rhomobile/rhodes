/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.*;

import j2me.util.ArrayList;
import j2me.util.Collection;

import org.apache.oro.text.perl.Perl5Util;
import org.apache.oro.text.regex.*;

import j2me.lang.*;;

/*
 * TODO:
 * Regexp and global variables:
 *   $& receives the part of the string that was matched by the pattern
 *   $` receives the part of the string that preceded the match
 *   $' receives the string after the match
 *   $~ is a MatchData object that holds everything you may want to know about the match
 *   $1, and so on, hold the values of parts of the match
 *
 * Options:
 *   i Case Insensitive.
 *   o Substitute Once.
 *   m Multiline Mode.
 *   x ExtendedMode.
 *   n no encoding (ASCII)
 *   e EUC
 *   s SJIS
 *   u UTF-8
 */
//@RubyLevelClass(name="Regexp")
public class RubyRegexp extends RubyBasic {

    private Pattern pattern_;

    RubyRegexp(String v) {
        super(RubyRuntime.RegexpClass);
        this.setValue(v, Perl5Compiler.MULTILINE_MASK);
    }

    RubyRegexp(String v, String option) {
        super(RubyRuntime.RegexpClass);
        int mode = Perl5Compiler.DEFAULT_MASK;
        if (option.indexOf('m') > -1) {
            mode |= Perl5Compiler.SINGLELINE_MASK;
        } else {
            mode |= Perl5Compiler.MULTILINE_MASK;
        }

        if (option.indexOf('i') > -1) {
            mode |= Perl5Compiler.CASE_INSENSITIVE_MASK;
        }

        if (option.indexOf('x') > -1) {
            mode |= Perl5Compiler.EXTENDED_MASK;
        }

        this.setValue(v, mode);
    }

    RubyRegexp() {
        super(RubyRuntime.RegexpClass);
    }

    protected void doClone(RubyValue orig){
    	pattern_ = ((RubyRegexp)orig).pattern_;
    	super.doClone(orig);
    }
    
    private void setValue(String v, int mode) {
        PatternCompiler compiler = new Perl5Compiler();

        //'\z' works for ruby, but ORO only supports '\Z'
        v = StringMe.replaceAll(v, "\\z", "\\Z");

        try {
            pattern_ = compiler.compile(v, mode);
        } catch (MalformedPatternException e) {
            throw new Error(e.toString());
        }
    }

    //@RubyAllocMethod
    public static RubyRegexp alloc(RubyValue receiver) {
        return ObjectFactory.createRegexp();
    }

    //@RubyLevelMethod(name="initialize")
    public RubyValue initialize(RubyValue arg) {
        RubyValue pattern = arg;
        if (pattern instanceof RubyRegexp) {
            pattern_ = ((RubyRegexp)pattern).pattern_;
        } else {
            setValue(pattern.toStr(), Perl5Compiler.MULTILINE_MASK);
        }

        return this;
    }

    //@RubyLevelMethod(name="initialize")
    public RubyValue initialize(RubyValue pattern, RubyValue mode) {
        if (pattern instanceof RubyRegexp) {
            pattern_ = ((RubyRegexp)pattern).pattern_;
        } else {
            setValue(pattern.toStr(), getFlag(mode));
        }

        return this;
    }

    //@RubyLevelMethod(name="initialize")
    public RubyValue initialize(RubyArray args) {
        //TODO right now we just ignore the third parameter
        return initialize(args.get(0), args.get(1));
    }

    public static final int RE_OPTION_IGNORECASE = 1;
    public static final int RE_OPTION_EXTENDED   = 0x0020;
    public static final int RE_OPTION_MULTILINE   = 0x0008;
    
    private int getFlag(RubyValue mode) {
        if (mode instanceof RubyFixnum) {
            return mode.toInt();
        }

        // 0 is default flag
        return mode.isTrue() ? RE_OPTION_IGNORECASE : 0;
    }

    //@RubyLevelMethod(name="===")
    public RubyValue caseEqual(RubyValue arg) {
        if (!(arg instanceof RubyString)) {
            //not comparable
            return RubyConstant.QFALSE;
        }

        if (this.caseEqual(arg.toStr())) {
            return RubyConstant.QTRUE;
        } else {
            return RubyConstant.QFALSE;
        }
    }

    //@RubyLevelMethod(name="match")
    public RubyValue match(RubyValue arg) {
        /*if (!(arg instanceof RubyString)) {
            //not comparable
            return RubyConstant.QFALSE;
        }*/
    	if ( arg == RubyConstant.QNIL )
    		return RubyConstant.QNIL; 
    	
        RubyMatchData m = this.match(arg.toStr());
        if (null == m) {
            return RubyConstant.QNIL;
        } else {
            return m;
        }
    }

    //@RubyLevelMethod(name="=~")
    public RubyValue opMatch(RubyValue arg) {
        if (!(arg instanceof RubyString)) {
            //not comparable
            return RubyConstant.QFALSE;
        }

        int p = this.matchPosition(arg.toStr());
        if (p < 0) {
            return RubyConstant.QNIL;
        } else {
            return ObjectFactory.createFixnum(p);
        }
    }

    //@RubyLevelMethod(name="escape", alias="quote")
    public static RubyString quote(RubyValue receiver, RubyValue arg) {
        String input = arg.toStr();
        StringBuffer result = new StringBuffer(input.length() * 2);

        for(int i = 0; i < input.length(); ++i) {
            char c = input.charAt(i);
            switch (c) {
                case '[': case ']': case '{': case '}':
                case '(': case ')': case '|': case '-':
                case '*': case '.': case '\\':
                case '?': case '+': case '^': case '$':
                case '#':
                case ' ':
                    result.append('\\');
                    result.append(c);
                    break;
                case '\n':
                    result.append("\\n");
                    break;
                case '\t':
                    result.append("\\t");
                    break;
                case '\r':
                    result.append("\\r");
                    break;
                case '\f':
                    result.append("\\f");
                    break;
                case 11:
                    result.append("\\v");
                    break;
                default:
                    result.append(c);
                    break;
            }
        }

        return new RubyString(result);
    }

    boolean caseEqual(String v) {
        return match(v) != null;
    }

    public RubyMatchData match(String input) {
        PatternMatcher m = new Perl5Matcher();
        if (m.contains(input, pattern_)) {
            MatchResult r = m.getMatch();
            RubyMatchData mData = ObjectFactory.createMatchData(r,input);
            
            updateGlobalVariables(r,mData);
            
            return mData;
        } else {
            clearGlobalVariables();
            return null;
        }
    }

    public RubyArray scan(String str) {
        RubyArray a = new RubyArray();

        PatternMatcherInput input = new PatternMatcherInput(str);
        PatternMatcher m = new Perl5Matcher();
        while (m.contains(input, pattern_)) {
            MatchResult r = m.getMatch();
            updateGlobalVariables(r, str);

            if (r.groups() == 1) {
                a.add(ObjectFactory.createString(r.group(0)));
            } else {
                RubyArray subarray = new RubyArray();
                for (int i = 1; i < r.groups(); ++i) {
                    String s = r.group(i);
                    if ( null == s )
                    	subarray.add(RubyConstant.QNIL);
                    else
                    	subarray.add(ObjectFactory.createString(s));
                    //subarray.add(null == s ? RubyConstant.QNIL : ObjectFactory.createString(s));
                }
                a.add(subarray);
            }
        }

        return a;
    }

    public void scan(String str, RubyBlock block) {
        PatternMatcherInput input = new PatternMatcherInput(str);
        PatternMatcher m = new Perl5Matcher();
        while (m.contains(input, pattern_)) {
            MatchResult r = m.getMatch();
            updateGlobalVariables(r, str);

            if (r.groups() == 1) {
                block.invoke(this, ObjectFactory.createString(r.group(0)));
            } else {
                RubyArray subarray = new RubyArray();
                for (int i = 1; i < r.groups(); ++i) {
                	
                	String res = r.group(i);
                	if ( res != null )
                		subarray.add(ObjectFactory.createString(res));
                	else
                		subarray.add(RubyConstant.QNIL);
                }
                block.invoke(this, subarray);
            }
        }
    }

    int matchPosition(String input) {
        PatternMatcher m = new Perl5Matcher();
        if (m.contains(input, pattern_)) {
            MatchResult r = m.getMatch();
            updateGlobalVariables(r, input);
            
            return r.beginOffset(0);
        } else {
            clearGlobalVariables();
            return -1;
        }
    }

    private String getReplaceString(String replace_string) {
        //java and oro use $1, $2, ruby uses \1, \2
        replace_string = StringMe.replaceAll(replace_string, "\\&", "$0");

        //%q{location:1 in 'l'}.sub(/\A(.+:\d+).*/, ' [\\1]') is as same as
        //%q{location:1 in 'l'}.sub(/\A(.+:\d+).*/, ' [\1]')
        for (int i = 1; i < 10; ++i) {
            replace_string = StringMe.replaceAll( replace_string, "\\\\" + i, "$" + i);
            replace_string = StringMe.replaceAll(replace_string, "\\" + i, "$" + i);
        }

        return replace_string;
    }

    public RubyString sub(RubyString input, RubyBlock block) {
        return sub(input, block, 0);
    }

    public RubyString gsub(RubyString input, RubyBlock block) {
        return sub(input, block, -1);
    }

    private RubyString sub(RubyString str, RubyBlock block, int limit) {
        RubyString result = new RubyString("");
        PatternMatcherInput input = new PatternMatcherInput(str.toString());
        PatternMatcher matcher = new Perl5Matcher();
        int end = 0;
        boolean matched = false;
        while (matcher.contains(input, pattern_)) {
            matched = true;
            MatchResult r = matcher.getMatch();
            updateGlobalVariables(r, str.toString());

            int begin = r.beginOffset(0);
            if (begin > end) {
                //append unmatched
                result.appendString(str.toString().substring(end, begin));
            }

            end = r.endOffset(0);

            RubyString match = new RubyString(r.group(0));
            RubyValue v = block.invoke(this, match);
            result.appendString(v.toString());

            if (0 == limit) {
                break;
            }
        }

        if (!matched) {
            return str;
        }

        //append unmatched
        if (end < str.length()) {
            result.appendString(str.toString().substring(end, str.length()));
        }

        return result;
    }

    public RubyString gsub(RubyString input, RubyString sub) {
        return sub(input, getReplaceString(sub.toString()), Util.SUBSTITUTE_ALL);
    }

    public RubyString sub(RubyString input, RubyString sub) {
        return sub(input, getReplaceString(sub.toString()), 1);
    }

    private RubyString sub(RubyString input, String sub, int limit) {
        StringBuffer result = new StringBuffer();
        final String strInput = input.toString();
        int nmatch = Util.substitute(
                result,
                new Perl5Matcher(),
                pattern_,
                new Perl5Substitution(sub, Perl5Substitution.INTERPOLATE_ALL) {
                    public void appendSubstitution(java.lang.StringBuffer appendBuffer,
                        MatchResult match,
                        int substitutionCount,
                        PatternMatcherInput originalInput,
                        PatternMatcher matcher,
                        Pattern pattern) {
                        super.appendSubstitution(appendBuffer, match, substitutionCount, originalInput, matcher, pattern);
                        updateGlobalVariables(match, strInput);
                    }
                },
                input.toString(),
                limit);
        if (nmatch > 0) {
            return ObjectFactory.createString(result.toString());
        } else {
            return input;
        }
    }

    public Collection/*<String>*/ split(String input, int limit) {
        Perl5Util util = new Perl5Util();
        Collection/*<String>*/ result = new ArrayList/*<String>*/();
        String regx = "/" + pattern_.getPattern() + "/";
        util.split(result, regx, input, limit);
        return result;
    }

    //@RubyLevelMethod(name="to_s")
    public RubyString to_s() {
        return ObjectFactory.createString(pattern_.getPattern());
    }

    //@RubyLevelMethod(name="source")
    public RubyString source() {
        return ObjectFactory.createString(pattern_.getPattern());
    }

    private void updateGlobalVariables(MatchResult r, String strInput) 
    {
        RubyMatchData mData = ObjectFactory.createMatchData(r,strInput);
        updateGlobalVariables(r, mData);
    }
    
    private void updateGlobalVariables(MatchResult r, RubyMatchData mData) 
    {
    	GlobalVariables.set(mData, "$~");
    	GlobalVariables.set(mData.post_match(), "$'");
    	GlobalVariables.set(mData.pre_match(), "$`");
    	
        GlobalVariables.set(ObjectFactory.createString(r.group(0)), "$&");
        for (int i = 1; i < r.groups(); ++i) {
            String s = r.group(i);
            if ( null == s )
            	GlobalVariables.set(RubyConstant.QNIL, "$" + i );
            else
            	GlobalVariables.set(ObjectFactory.createString(s), "$" + i );
            //GlobalVariables.set(null == s ? RubyConstant.QNIL: ObjectFactory.createString(s), "$" + i);
        }
    }

    private void clearGlobalVariables() {
        GlobalVariables.set(RubyConstant.QNIL, "$~");
        GlobalVariables.set(RubyConstant.QNIL, "$&");
        GlobalVariables.set(RubyConstant.QNIL, "$1");
        GlobalVariables.set(RubyConstant.QNIL, "$2");
        GlobalVariables.set(RubyConstant.QNIL, "$3");
        GlobalVariables.set(RubyConstant.QNIL, "$4");
        GlobalVariables.set(RubyConstant.QNIL, "$5");
        GlobalVariables.set(RubyConstant.QNIL, "$6");
        GlobalVariables.set(RubyConstant.QNIL, "$7");
        GlobalVariables.set(RubyConstant.QNIL, "$8");
        GlobalVariables.set(RubyConstant.QNIL, "$9");
    }
    
    //RHO_COMMENT
    public Pattern getPattern(){ return pattern_; }    
}
