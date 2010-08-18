package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.builtin.RubyHash;
public class RubyHash_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "fetch", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyHash)receiver).fetch(args, block);}
});
klass.defineMethod( "[]", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyHash)receiver).getValue(arg);}
});
klass.defineMethod( "reject", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyHash)receiver).reject(block);}
});
klass.defineMethod( "has_key?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyHash)receiver).has_key(arg);}
});
klass.aliasMethod("include?","has_key?");
klass.aliasMethod("key?","has_key?");
klass.aliasMethod("member?","has_key?");
klass.defineMethod( "keys", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyHash)receiver).keys();}
});
klass.defineMethod( "has_value?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyHash)receiver).has_value(arg);}
});
klass.defineMethod( "values_at", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyHash)receiver).values_at(args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return ((RubyHash)receiver).values_at();}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyHash)receiver).values_at(arg);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return ((RubyHash)receiver).values_at(args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "size", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyHash)receiver).size();}
});
klass.aliasMethod("length","size");
/*klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyHash)receiver).to_s();}
});*/
klass.defineMethod( "values", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyHash)receiver).values();}
});
klass.defineMethod( "default", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyHash)receiver).getDefaultValue();}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyHash)receiver).getDefaultValue(arg);}
});
klass.defineMethod( "==", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyHash)receiver).equal(arg);}
});
klass.defineMethod( "delete", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyHash)receiver).delete(arg, block);}
});
klass.defineMethod( "default=", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyHash)receiver).setDefaultValue(arg);}
});
klass.defineMethod( "delete_if", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyHash)receiver).delete_if(block);}
});
klass.defineMethod( "shift", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyHash)receiver).shift();}
});
klass.defineMethod( "[]=", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyHash)receiver).setValue(arg0, arg1);}
});
klass.aliasMethod("store","[]=");
klass.getSingletonClass().defineMethod( "[]", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyHash.create(args);}
});
klass.defineMethod( "clear", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyHash)receiver).clear();}
});

klass.defineMethod( "initialize", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyHash)receiver).initialize(args, block);}
});
klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyHash.alloc(receiver);	}} );
}}
