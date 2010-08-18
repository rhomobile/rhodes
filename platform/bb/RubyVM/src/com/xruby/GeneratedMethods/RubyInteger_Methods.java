package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyInteger;
public class RubyInteger_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "times", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyInteger)receiver).times(block);}
});
klass.defineMethod( "chr", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyInteger)receiver).chr();}
});
klass.defineMethod( "ord", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyInteger)receiver).ord();}
});

}}
