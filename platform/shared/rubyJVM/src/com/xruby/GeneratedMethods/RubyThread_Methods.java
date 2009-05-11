package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.builtin.RubyThread;
public class RubyThread_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "alive?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThread)receiver).alive();}
});
klass.defineMethod( "[]", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyThread)receiver).getVariable(arg);}
});
klass.getSingletonClass().defineMethod( "list", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyThread.list(receiver);}
});
klass.defineMethod( "kill", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThread)receiver).kill();}
});
klass.aliasMethod("exit","kill");
klass.defineMethod( "run", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThread)receiver).run();}
});
klass.defineMethod( "priority=", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyThread)receiver).set_priority(arg);}
});
klass.defineMethod( "wakup", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThread)receiver).wakup();}
});
klass.defineMethod( "stop?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThread)receiver).getStoped();}
});
klass.getSingletonClass().defineMethod( "main", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyThread.getMainThread(receiver);}
});
klass.defineMethod( "key?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyThread)receiver).is_key(arg);}
});
klass.defineMethod( "==", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyThread)receiver).equal(arg);}
});
klass.defineMethod( "priority", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThread)receiver).priority();}
});
klass.defineMethod( "value", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThread)receiver).value();}
});
klass.getSingletonClass().defineMethod( "new", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyThread.newThread(receiver, args, block);}
});
klass.getSingletonClass().aliasMethod("fork","new");
klass.getSingletonClass().aliasMethod("start","new");
klass.getSingletonClass().defineMethod( "pass", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyThread.pass(receiver);}
});
klass.defineMethod( "[]=", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyThread)receiver).setVariable(arg0, arg1);}
});
klass.defineMethod( "status", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThread)receiver).status();}
});
klass.getSingletonClass().defineMethod( "current", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyThread.current(receiver);}
});
klass.defineMethod( "keys", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThread)receiver).keys();}
});
klass.defineMethod( "join", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyThread)receiver).join(args, block);}
});
klass.getSingletonClass().defineMethod( "critical", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyThread.critical(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "critical=", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyThread.set_critical(receiver, arg);}
});
klass.getSingletonClass().defineMethod( "stop", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return RubyThread.stop(receiver);}
});
klass.defineMethod( "group", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThread)receiver).getThreadGroup();}
});
klass.defineMethod( "inspect", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyThread)receiver).rubyInspect();}
});
}}
