package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.lang.RubyExceptionValue;
public class RubyExceptionValue_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyExceptionValue)receiver).to_s();}
});
klass.aliasMethod("message","to_s");
klass.aliasMethod("to_str","to_s");
klass.defineMethod( "initialize", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyExceptionValue)receiver).initialize();}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyExceptionValue)receiver).initialize(arg);}
});
klass.defineMethod( "backtrace", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyExceptionValue)receiver).backtrace();}
});
klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyExceptionValue.alloc(receiver);	}} );
}}
