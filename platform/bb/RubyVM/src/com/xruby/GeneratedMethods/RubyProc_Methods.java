package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyProc;
public class RubyProc_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "arity", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyProc)receiver).arity();}
});
klass.defineMethod( "==", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyProc)receiver).equal(arg);}
});
klass.aliasMethod("==","eql?");
klass.getSingletonClass().defineMethod( "new", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyProc.newProc(receiver, block);}
});
klass.defineMethod( "call", new RubyProc.Invoke() );
klass.aliasMethod("[]","call");
klass.aliasMethod("===","call");
klass.aliasMethod("yield","call");
}}
