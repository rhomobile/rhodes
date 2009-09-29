package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
//import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.RubyObject;
public class RubyObject_Methods{
public static void initMethods( RubyClass klass){

klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyObject.alloc(receiver);	}} );

klass.defineMethod( "instance_variable_get", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyObject)receiver).instance_variable_get(arg);}
});
klass.defineMethod( "instance_variable_defined?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyObject)receiver).instance_variable_defined( arg);}
});
klass.defineMethod( "instance_variable_set", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyObject)receiver).instance_variable_set( arg0, arg1);}
});
klass.defineMethod( "remove_instance_variable", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyObject)receiver).instance_variable_remove( arg);}
});
klass.defineMethod( "instance_variables", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyObject)receiver).instance_variables();}
});
klass.defineMethod( "rhom_init", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyObject)receiver).rhom_init( arg);}
});

}}
