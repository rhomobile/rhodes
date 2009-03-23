package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.builtin.RubyMatchData;
public class RubyMatchData_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "[]", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyMatchData)receiver).aref(arg);}
});
klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyMatchData)receiver).to_s();}
});
}}
