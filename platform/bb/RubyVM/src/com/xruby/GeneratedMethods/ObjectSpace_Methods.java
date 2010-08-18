package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.lang.ObjectSpace;
public class ObjectSpace_Methods{
public static void initMethods( RubyModule klass){

klass.getSingletonClass().defineMethod( "each_object", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ObjectSpace.rubyEachObject(receiver, block);}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ObjectSpace.rubyEachObject(receiver, arg, block);}
});
}}
