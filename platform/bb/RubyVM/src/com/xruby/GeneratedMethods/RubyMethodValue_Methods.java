package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.builtin.RubyMethodValue;
public class RubyMethodValue_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "arity", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyMethodValue)receiver).arity();}
});
klass.defineMethod( "call", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyMethodValue)receiver).call(args, block);}
});
klass.defineMethod( "to_proc", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyMethodValue)receiver).convertToRubyProc();}
});
klass.defineMethod( "unbind", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyMethodValue)receiver).unbind();}
});
klass.defineMethod( "bind", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyMethodValue)receiver).bind(arg);}
});
klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyMethodValue)receiver).to_s();}
});
klass.aliasMethod("inspect","to_s");
}}
