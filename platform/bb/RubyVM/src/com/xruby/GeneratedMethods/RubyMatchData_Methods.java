package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyMatchData;
public class RubyMatchData_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "[]", new RubyOneOrTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyMatchData)receiver).aref(arg);}
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyMatchData)receiver).aref(arg0, arg1);}
});

klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyMatchData)receiver).to_s();}
});
klass.defineMethod( "to_a", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyMatchData)receiver).to_a();}
});
klass.defineMethod( "captures", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyMatchData)receiver).captures();}
});
klass.defineMethod( "post_match", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyMatchData)receiver).post_match();}
});
klass.defineMethod( "pre_match", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyMatchData)receiver).pre_match();}
});

}}
