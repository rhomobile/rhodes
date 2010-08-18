package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyENV;
public class RubyENV_Methods{
public static void initMethods( RubyClass klass){

klass.getSingletonClass().defineMethod( "__os_ev_index__", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyENV.osEvIndex(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "__get_os_ev__", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyENV.getOSEv(receiver, arg);}
});
}}
