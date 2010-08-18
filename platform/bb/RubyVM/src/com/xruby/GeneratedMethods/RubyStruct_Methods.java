package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.builtin.RubyStruct;
public class RubyStruct_Methods{
public static void initMethods( RubyClass klass){

klass.getSingletonClass().defineMethod( "new", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyStruct.alloc(receiver, args, block);}
});
}}
