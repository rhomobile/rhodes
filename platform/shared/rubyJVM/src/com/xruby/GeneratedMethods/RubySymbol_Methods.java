package com.xruby.GeneratedMethods;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.*;
public class RubySymbol_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "to_sym", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubySymbol)receiver).to_sym();}
});
klass.defineMethod( "to_i", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubySymbol)receiver).to_i();}
});
klass.aliasMethod("to_int","to_i");
klass.defineMethod( "inspect", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubySymbol)receiver).rubyInspect();}
});
klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubySymbol)receiver).to_s();}
});
klass.aliasMethod("id2name","to_s");
klass.aliasMethod("to_str","to_s");

klass.defineMethod( "length", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubySymbol)receiver).rubyLength();}
});

klass.defineMethod( "===", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubySymbol)receiver).sym_eqq(arg);}
});

}}
