package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyFixnum;
import com.xruby.runtime.builtin.RubyFloat;
public class RubyFloat_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "coerce", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFloat)receiver).coerce(arg);}
});
klass.defineMethod( "to_i", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFloat)receiver).to_i();}
});
klass.defineMethod( "ceil", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFloat)receiver).ceil();}
});
klass.defineMethod( "%", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFloat)receiver).opMod(arg);}
});
klass.defineMethod( "*", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFloat)receiver).opMul(arg);}
});
klass.defineMethod( "+", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFloat)receiver).opPlus(arg);}
});
klass.defineMethod( "round", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFloat)receiver).round();}
});
klass.defineMethod( "/", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFloat)receiver).opDiv(arg);}
});
klass.defineMethod( "<=>", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFloat)receiver).cmp(arg);}
});
klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFloat)receiver).to_s();}
});
klass.defineMethod( "-", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFloat)receiver).opMinus(arg);}
});
klass.defineMethod( "abs", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFloat)receiver).abs();}
});
klass.defineMethod( "floor", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFloat)receiver).floor();}
});
klass.defineMethod( "truncate", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFloat)receiver).truncate();}
});
klass.defineMethod( "nan?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFloat)receiver).nan_p();}
});
klass.defineMethod( "finite?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFloat)receiver).finite_p();}
});
klass.defineMethod( "infinite?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFloat)receiver).infinite_p();}
});
klass.defineMethod( "**", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFloat)receiver).pow(arg);}
});

}}
