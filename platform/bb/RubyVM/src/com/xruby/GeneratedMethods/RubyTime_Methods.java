package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
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
klass.aliasMethod("utc?","gmt?");
klass.aliasMethod("gmtime?","gmt?");
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

klass.defineMethod( "utc", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).utc();}
});
klass.aliasMethod("gm","utc");

klass.defineMethod( "wday", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).wday();}
});

klass.defineMethod( "hour", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).hour();}
});
klass.defineMethod( "min", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).min();}
});
klass.defineMethod( "sec", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).sec();}
});
klass.defineMethod( "nsec", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).nsec();}
});

klass.defineMethod( "<=>", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyTime)receiver).cmp(arg);}
});
klass.defineMethod( "eql?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyTime)receiver).eql(arg);}
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
klass.aliasMethod("mon","month");

klass.defineMethod( "year", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).year();}
});
klass.defineMethod( "day", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).day();}
});
klass.aliasMethod("mday","day");

klass.defineMethod( "gmt_offset", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).gmt_offset();}
});
klass.aliasMethod("gmtoff","gmt_offset");
klass.aliasMethod("utc_offset","gmt_offset");

klass.defineMethod( "zone", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyTime)receiver).zone();}
});

klass.defineMethod( "initialize", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyTime)receiver).initialize(args);}
});
klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyTime.alloc(receiver);	}} );
}}
