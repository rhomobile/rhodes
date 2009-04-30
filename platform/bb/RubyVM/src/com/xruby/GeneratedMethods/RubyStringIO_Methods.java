package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.stdlib.RubyStringIO;
public class RubyStringIO_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "puts", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyStringIO)receiver).puts(arg);}
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
