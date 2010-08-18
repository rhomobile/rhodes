package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.stdlib.RubyStringIO;
public class RubyStringIO_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "puts", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyStringIO)receiver).puts(arg);}
});
klass.defineMethod( "write", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyStringIO)receiver).write(arg);}
});
klass.aliasMethod("syswrite","write");

klass.defineMethod( "read", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyStringIO)receiver).read(args);}
});
klass.defineMethod( "readline", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyStringIO)receiver).readLine(args);}
});
klass.defineMethod( "eof?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyStringIO)receiver).isEOF();}
});

klass.defineMethod( "string", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyStringIO)receiver).string();}
});
klass.defineMethod( "initialize", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyStringIO)receiver).initialize();}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyStringIO)receiver).initialize(arg);}
});
klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyStringIO.alloc(receiver);	}} );
}}
