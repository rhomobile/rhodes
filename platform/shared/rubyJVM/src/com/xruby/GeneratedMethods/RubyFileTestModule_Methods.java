package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyFileTestModule;
public class RubyFileTestModule_Methods{
public static void initMethods( RubyModule klass){

klass.getSingletonClass().defineMethod( "exist?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyFileTestModule.exist_p(receiver, arg, block);}
});
}}
