package com.xruby.runtime.lang;
//import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
//import com.xruby.runtime.lang.RubyClass;
public class RubyClass_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "new", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyClass)receiver).newInstance(args, block);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return ((RubyClass)receiver).newInstance(block);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyClass)receiver).newInstance(arg, block);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return ((RubyClass)receiver).newInstance(args, block);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "superclass", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyClass)receiver).superclass();}
});
klass.defineMethod( "allocate", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyClass)receiver).allocate();}
});
klass.defineMethod( "initialize", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyClass)receiver).initialize(block);}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyClass)receiver).initialize(arg, block);}
});
klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyClass.allocClass(receiver);	}} );
}}
