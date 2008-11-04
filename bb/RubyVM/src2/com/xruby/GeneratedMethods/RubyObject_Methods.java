package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.RubyObject;
public class RubyObject_Methods{
public static void initMethods( RubyClass klass){

klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyObject.alloc(receiver);	}} );
}}
