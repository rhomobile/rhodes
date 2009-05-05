package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.builtin.RubyTopSelf;
public class RubyTopSelf_Methods{
public static void initMethods( RubyClass klass){

klass.getSingletonClass().defineMethod( "include", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyTopSelf.include(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return RubyTopSelf.include(receiver);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyTopSelf.include(receiver, arg);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return RubyTopSelf.include(receiver, args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
}}
