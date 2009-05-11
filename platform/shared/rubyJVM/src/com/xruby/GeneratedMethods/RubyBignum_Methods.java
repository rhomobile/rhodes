package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyBignum;
public class RubyBignum_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "to_f", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyBignum)receiver).to_f();}
});
klass.defineMethod( "[]", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).aref(arg);}
});
klass.defineMethod( "-@", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyBignum)receiver).uminus();}
});
klass.defineMethod( "&", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).op_band(arg);}
});
klass.defineMethod( "coerce", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).coerce(arg);}
});
klass.defineMethod( "%", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).op_mod(arg);}
});
klass.defineMethod( "*", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).op_mul(arg);}
});
klass.defineMethod( "+", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).op_add(arg);}
});
klass.defineMethod( "/", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).op_div(arg);}
});
klass.aliasMethod("fdiv","/");
klass.defineMethod( "-", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).op_sub(arg);}
});
klass.defineMethod( "size", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyBignum)receiver).rubySize();}
});
klass.defineMethod( "<=>", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).cmp(arg);}
});
klass.defineMethod( "==", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).equal(arg);}
});
klass.defineMethod( "eql?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).eql(arg);}
});
klass.defineMethod( "to_s", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyBignum)receiver).to_s();}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).to_s(arg);}
});
klass.defineMethod( "quo", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).quo(arg);}
});
klass.defineMethod( ">>", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).rshift(arg);}
});
klass.defineMethod( "<<", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).lshift(arg);}
});
klass.defineMethod( "~", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyBignum)receiver).op_neg();}
});
klass.defineMethod( "|", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).op_bor(arg);}
});
klass.defineMethod( "^", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).op_bxor(arg);}
});
klass.defineMethod( "**", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyBignum)receiver).pow(arg);}
});
}}
