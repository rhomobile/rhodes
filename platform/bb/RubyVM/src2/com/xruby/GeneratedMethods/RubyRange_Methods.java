package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.builtin.RubyRange;
public class RubyRange_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "exclude_end?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyRange)receiver).excludeEndP();}
});
klass.defineMethod( "hash", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyRange)receiver).hash();}
});
klass.defineMethod( "each", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyRange)receiver).each(block);}
});
klass.defineMethod( "to_a", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyRange)receiver).to_a();}
});
klass.defineMethod( "end", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyRange)receiver).getRight();}
});
klass.defineMethod( "begin", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyRange)receiver).getLeft();}
});
klass.defineMethod( "initialize", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyRange)receiver).initialize(args);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyRange)receiver).initialize(arg0, arg1);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `"+ this.getID() + "': wrong number of arguments");}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(1, 0, block != null? block.createdByLambda() : false );
		args.add(arg);
		return ((RubyRange)receiver).initialize(args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "eql?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyRange)receiver).eql(arg);}
});

klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyRange.alloc(receiver);	}} );
}}
