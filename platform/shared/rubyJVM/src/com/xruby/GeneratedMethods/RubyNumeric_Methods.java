package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyNumeric;
public class RubyNumeric_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "-@", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyNumeric)receiver).uminus();}
});
klass.defineMethod( "coerce", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyNumeric)receiver).coerce(arg);}
});
klass.defineMethod( "+@", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyNumeric)receiver).uplus();}
});
klass.defineMethod( "step", new RubyOneOrTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyNumeric)receiver).step(arg, block);}
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyNumeric)receiver).step(arg0, arg1, block);}
});
}}
