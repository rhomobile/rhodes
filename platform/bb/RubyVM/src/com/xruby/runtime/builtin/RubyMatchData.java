/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import org.apache.oro.text.regex.MatchResult;
import com.xruby.runtime.lang.*;

//@RubyLevelClass(name="MatchData")
public class RubyMatchData extends RubyBasic {
    private MatchResult result_;
    private String      str_;
    
    RubyMatchData(MatchResult m,String input) {
        super(RubyRuntime.MatchDataClass);
        result_ = m;
        str_ = input;
    }

    public RubyValue clone(){
    	RubyMatchData cl = new RubyMatchData(result_, str_);
    	cl.doClone(this);
    	return cl;
    }
    
    //@RubyLevelMethod(name="to_s")
    public RubyString to_s() {
        return ObjectFactory.createString(result_.toString());
    }

    public String toString() {
        return result_.group(0);
    }

    //@RubyLevelMethod(name="[]")
    public RubyValue aref(RubyValue arg) 
    {
    	if ( arg instanceof RubyFixnum )
    	{
	        int index = arg.toInt();
	        if (index < 0)
	        	index += result_.groups();
	        
	        String res = result_.group(index);
	        if ( res == null )
	        	return RubyConstant.QNIL;
	        
	        return ObjectFactory.createString(res);
    	}else if ( arg instanceof RubyString )
    	{
    		//TODO: implement m = /(?<foo>a+)b/.match("ccaaab"); m["foo"]   #=> "aaa"
    		throw new RuntimeException("Not implemented");
    	}
    	
    	RubyArray ar = (RubyArray)to_a();
    	return ar.aref(arg);
    }

    public RubyValue aref(RubyValue arg1, RubyValue arg2) 
    {
    	RubyArray ar = (RubyArray)to_a();
    	return ar.aref(arg1, arg2);
    }
    
    public RubyValue to_a() {
        RubyArray ar = new RubyArray();
    	for (int i = 0; i < result_.groups(); i++)
    	{
    		String str = result_.group(i);
    		if ( str != null )
    			ar.add( ObjectFactory.createString(str) );
    		else
    			ar.add( RubyConstant.QNIL );
    	}
    	
    	return ar;
    }

    public RubyValue captures() 
    {
        RubyArray ar = new RubyArray();
    	for (int i = 1; i < result_.groups(); i++)
    	{
    		String res = result_.group(i);
	        if ( res == null )
	        	ar.add(RubyConstant.QNIL);
	        else
	        	ar.add(ObjectFactory.createString(res));
    	}
    	
    	return ar;
    }

    public RubyValue post_match() 
    {
    	String res = "";
    	int nMatchEnd = result_.endOffset(result_.groups()-1);
    	if ( nMatchEnd >= 0 && nMatchEnd+1 <= str_.length() )
    		res = str_.substring(nMatchEnd); 

        if ( res == null )
        	return RubyConstant.QNIL;
    	
		return ObjectFactory.createString(res);
    }

    public RubyValue pre_match() 
    {
    	String res = "";
    	int nMatchEnd = result_.beginOffset(0);
    	if ( nMatchEnd >= 0 && nMatchEnd < str_.length() )
    		res = str_.substring(0,nMatchEnd); 
    			
        if ( res == null )
        	return RubyConstant.QNIL;
    	
		return ObjectFactory.createString(res);
    }
    
}
