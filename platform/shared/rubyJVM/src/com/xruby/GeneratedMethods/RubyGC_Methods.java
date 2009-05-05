package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyGC;
public class RubyGC_Methods{
public static void initMethods( RubyModule klass){

klass.getSingletonClass().defineMethod( "start", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyGC.start(receiver);}
});
}}
