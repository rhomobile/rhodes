package com.rho;

import org.apache.oro.text.regex.*;

import com.rho.db.PerstLiteAdapter;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;

import j2me.lang.AssertMe;

//@RubyLevelClass(name="StringScanner")
public class StringScanner extends RubyBasic {

	String str;
	PatternMatcherInput input;
	PatternMatcher matcher;
	MatchResult result;
    int scannerFlags = 0;
    private static final int MATCHED_STR_SCN_F = 1 << 11;     
	
	StringScanner(RubyClass c) {
		super(c);
	}

    //@RubyAllocMethod
    public static StringScanner alloc(RubyValue receiver) {
        return new StringScanner(RubyRuntime.StringScannerClass);
    }
	
    //@RubyLevelMethod(name="initialize")
    public StringScanner initialize(RubyValue v) {
    	str = v.toString();
    	input = new PatternMatcherInput(str);
    	matcher = new Perl5Matcher();
   	
        return this;
    }

    private void check() {
        if (str == null) 
        	throw new RubyException("uninitialized StringScanner object");
    }
    
    //@RubyLevelMethod(name="eos?")
    public RubyValue isEos() {
        check();
        return input.endOfInput() ? RubyConstant.QTRUE : RubyConstant.QFALSE;
    }
    
    //@RubyLevelMethod(name="scan")
    public RubyValue scan(RubyValue regex) {
        return scan(regex, true, true, true);
    }

    //@RubyLevelMethod(name="[]")
    public RubyValue getResult(RubyValue arg) {
    	int nIndex = arg.toInt();
    	if ( result == null )
    		return RubyConstant.QNIL;
    	
		if ( nIndex < 0 || nIndex >= result.groups() )
			throw new RubyException("Invalid argument");
		
        String s = result.group(nIndex);
        if ( null == s )
        	return RubyConstant.QNIL;
        
		return  ObjectFactory.createString( s );
    }
    
    private RubyValue scan(RubyValue regex, boolean succptr, boolean getstr, boolean headonly) {
        if (!(regex instanceof RubyRegexp)) 
        	throw new RubyException("wrong argument type " + regex.getRubyClass().toString() + " (expected Regexp)");
        check();
        
        clearMatched();
        
        RubyRegexp re = ((RubyRegexp)regex);
        if ( !matcher.contains(input, re.getPattern() ) )
        	return RubyConstant.QNIL;
        
        result = matcher.getMatch();
        setMatched();
        
        return  getstr ? getResult(ObjectFactory.createInteger(0)) : RubyConstant.QTRUE;
    }
    
    private RubyValue extractBegLen(int beg, int len) {
        assert( len >= 0 );
        int size = str.length();
        if (beg > size) 
        	return RubyConstant.QNIL;
        if (beg + len > size) 
        	len = size - beg;
        //TODO: optimize memory allocation
        return ObjectFactory.createString(str.toString().substring(beg, len));
    }
    
    private void clearMatched() {
        scannerFlags &= ~MATCHED_STR_SCN_F;
    }

    private void setMatched() {
        scannerFlags |= MATCHED_STR_SCN_F;
    }
    
	public static void initMethods( RubyClass klass){
		klass.defineMethod( "initialize", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((StringScanner)receiver).initialize(arg);}
		});
		klass.defineAllocMethod(new RubyNoArgMethod(){
			protected RubyValue run(RubyValue receiver, RubyBlock block )	{
				return StringScanner.alloc(receiver);}
		});
		klass.defineMethod( "eos?", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return ((StringScanner)receiver).isEos();}
		});
		klass.defineMethod( "scan", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((StringScanner)receiver).scan(arg);}
		});
		klass.defineMethod( "[]", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((StringScanner)receiver).getResult(arg);}
		});
		
	}
    
}
