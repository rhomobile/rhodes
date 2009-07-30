package com.xruby.runtime.lang;
//import com.xruby.runtime.lang.*;
//import com.xruby.runtime.builtin.*;
//import com.xruby.runtime.lang.RubyMarshalModule;
public class RubyMarshalModule_Methods{
public static void initMethods( RubyModule klass){

klass.defineModuleMethod( "dump", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMarshalModule.dump(arg);}
});
klass.defineModuleMethod( "load", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMarshalModule.load(arg);}
});
klass.defineModuleMethod( "load", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMarshalModule.load(arg);}
});
}}
