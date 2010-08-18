package com.rho;

import org.apache.oro.text.regex.*;

import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;

//@RubyLevelClass(name="StringScanner")
public class StringScanner extends RubyBasic {

	RubyString str;
	PatternMatcherInput input;
	Perl5Matcher matcher;
	MatchResult result;
    int scannerFlags = 0;
    private static final int MATCHED_STR_SCN_F = 1 << 11;     
	
	StringScanner(RubyClass c) {
		super(c);
	}

    //@RubyAllocMethod
    public static StringScanner alloc(RubyValue receiver) {
        return new StringScanner((RubyClass)receiver);//RubyRuntime.StringScannerClass);
    }
	
    //@RubyLevelMethod(name="initialize")
    public StringScanner initialize(RubyValue v) {
    	str = v.toRubyString();
    	input = new PatternMatcherInput(str.toString());
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

    public RubyValue skip(RubyValue regex) {
        return scan(regex, true, false, true);
    }
    
  //@RubyLevelMethod(name = "check")
    public RubyValue check(RubyValue regex) {
        return scan(regex, false, true, true);
    }

    //@RubyLevelMethod(name = "match?")
    public RubyValue ismatch(RubyValue regex) {
        return scan(regex, false, false, true);
    }
    
    //@RubyLevelMethod(name = "string")
    public RubyString string() {
        return str;
    }

    //@RubyLevelMethod(name = "peek")
    public RubyString peek(RubyValue val) 
    {
    	int nLen = val.toInt();
    	int nOffset = input.getCurrentOffset();
    	if ( nOffset >= str.length() )
    		return ObjectFactory.createString("");
    	
    	if (nOffset+nLen > str.length() )
    		nLen = str.length()-nOffset;
    	
    	String strRes = str.toStr().substring(nOffset, nOffset+nLen);
        return ObjectFactory.createString(strRes);
    }
    
    //@RubyLevelMethod(name = "string=")
    public RubyValue set_string(RubyValue str) {
    	initialize(str);
        clearMatched();
        return str;
    }

    public RubyValue reset() {
    	//input = new PatternMatcherInput(str.toString());
    	//matcher = new Perl5Matcher();
    	
        //clearMatched();
        return this;
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
        boolean bRes = false;
        int nOffset = input.getCurrentOffset();
       	bRes = matcher.matchesPrefixEx(input, re.getPattern() );//matcher.contains(input, re.getPattern() );
        
        if ( !bRes )
        	return RubyConstant.QNIL;

        if ( !succptr )
        	input.setCurrentOffset(nOffset);
        
        result = matcher.getMatch();
        setMatched();
        if ( getstr )
        	return getResult(ObjectFactory.createInteger(0));
        else
        {
            String s = result != null ? result.group(0) : "";
            return ObjectFactory.createInteger(s != null ? s.length() : 0);
        }
    }
    
    private void clearMatched() {
        scannerFlags &= ~MATCHED_STR_SCN_F;
    }

    private void setMatched() {
        scannerFlags |= MATCHED_STR_SCN_F;
    }
    
	public static void initMethods( RubyClass klass){
		klass.defineMethod( "initialize", new RubyOneOrTwoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((StringScanner)receiver).initialize(arg);}
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyValue arg1, RubyBlock block ){
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
		klass.defineMethod( "skip", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((StringScanner)receiver).skip(arg);}
		});
		klass.defineMethod( "[]", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((StringScanner)receiver).getResult(arg);}
		});
		klass.defineMethod( "string", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return ((StringScanner)receiver).string();}
		});
		klass.defineMethod( "string=", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((StringScanner)receiver).set_string(arg);}
		});
		klass.defineMethod( "check", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((StringScanner)receiver).check(arg);}
		});
		klass.defineMethod( "reset", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block ){
				return ((StringScanner)receiver).reset();}
		});
		klass.defineMethod( "peek", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((StringScanner)receiver).peek(arg);}
		});
		klass.defineMethod( "match?", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
				return ((StringScanner)receiver).ismatch(arg);}
		});
		
	}
    
}
