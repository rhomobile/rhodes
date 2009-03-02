package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.builtin.RubyRegexp;
public class RubyRegexp_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "=~", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyRegexp)receiver).opMatch(arg);}
});
klass.defineMethod( "===", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyRegexp)receiver).caseEqual(arg);}
});
klass.defineMethod( "source", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyRegexp)receiver).source();}
});
klass.getSingletonClass().defineMethod( "escape", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyRegexp.quote(receiver, arg);}
});
klass.getSingletonClass().aliasMethod("quote","escape");
klass.defineMethod( "match", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyRegexp)receiver).match(arg);}
});
klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyRegexp)receiver).to_s();}
});
klass.defineMethod( "initialize", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyRegexp)receiver).initialize(args);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyRegexp)receiver).initialize(arg);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyRegexp)receiver).initialize(arg0, arg1);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `"+ this.getID() + "': wrong number of arguments");}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyRegexp.alloc(receiver);	}} );
}}
