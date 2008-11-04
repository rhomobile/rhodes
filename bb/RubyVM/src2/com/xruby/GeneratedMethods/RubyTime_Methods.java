package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.builtin.RubyTime;
public class RubyTime_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "localtime", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).localtime();}
});
klass.defineMethod( "to_f", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).to_f();}
});
klass.defineMethod( "to_i", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).to_i();}
});
klass.aliasMethod("tv_sec","to_i");
klass.getSingletonClass().defineMethod( "now", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyTime.now(receiver, args, block);}
});
klass.defineMethod( "+", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyTime)receiver).plus(arg);}
});
klass.defineMethod( "gmt?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).gmt_question();}
});
klass.getSingletonClass().defineMethod( "mktime", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyTime.local(receiver, args);}
});
klass.getSingletonClass().aliasMethod("local","mktime");
klass.defineMethod( "usec", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).usec();}
});
klass.aliasMethod("tv_usec","usec");
klass.getSingletonClass().defineMethod( "utc", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyTime.utc(receiver, args);}
});
klass.getSingletonClass().aliasMethod("gm","utc");
klass.defineMethod( "<=>", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyTime)receiver).cmp(arg);}
});
klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).to_s();}
});
klass.defineMethod( "-", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyTime)receiver).minus(arg);}
});
klass.getSingletonClass().defineMethod( "at", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyTime.at(receiver, arg);}
});
klass.defineMethod( "month", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).month();}
});
klass.defineMethod( "year", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).year();}
});
klass.defineMethod( "day", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).day();}
});
klass.defineMethod( "zone", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).zone();}
});
klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyTime.alloc(receiver);	}} );
}}
