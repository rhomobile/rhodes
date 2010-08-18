package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.RubyFixnum;
public class RubyFixnum_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "[]", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).aref(arg);}
});
klass.defineMethod( "to_f", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFixnum)receiver).convertToFloat();}
});
klass.defineMethod( "-@", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFixnum)receiver).uminus();}
});
klass.defineMethod( "&", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opAnd(arg);}
});
klass.defineMethod( "%", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).mod(arg);}
});
klass.defineMethod( "*", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opMul(arg);}
});
klass.defineMethod( "+", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opPlus(arg);}
});
klass.defineMethod( ">=", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opGe(arg);}
});
klass.defineMethod( "/", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opDiv(arg);}
});
klass.aliasMethod("fdiv","/");
klass.defineMethod( "<=>", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opCmp(arg);}
});
klass.defineMethod( "to_s", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFixnum)receiver).to_s();}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).to_s(arg);}
});
klass.defineMethod( "-", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opMinus(arg);}
});
klass.defineMethod( "quo", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).quo(arg);}
});
klass.defineMethod( ">>", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).rshift(arg);}
});
klass.defineMethod( "==", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opEqual(arg);}
});
klass.defineMethod( "<=", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opLe(arg);}
});
klass.defineMethod( "<<", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).lshift(arg);}
});
klass.defineMethod( "~", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyFixnum)receiver).opRev();}
});
klass.defineMethod( "^", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opXor(arg);}
});
klass.defineMethod( "|", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opOr(arg);}
});
klass.defineMethod( ">", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opGt(arg);}
});
klass.defineMethod( "**", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).pow(arg);}
});
klass.defineMethod( "<", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyFixnum)receiver).opLt(arg);}
});
}}
