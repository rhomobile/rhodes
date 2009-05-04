package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyThreadGroup;
public class RubyThreadGroup_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "enclose", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThreadGroup)receiver).enclose();}
});
klass.getSingletonClass().defineMethod( "new", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyThreadGroup.newThreadGroup(receiver);}
});
klass.defineMethod( "enclosed?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThreadGroup)receiver).enclosed_p();}
});
klass.defineMethod( "list", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThreadGroup)receiver).listAll();}
});
klass.defineMethod( "add", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyThreadGroup)receiver).add(arg);}
});
}}
