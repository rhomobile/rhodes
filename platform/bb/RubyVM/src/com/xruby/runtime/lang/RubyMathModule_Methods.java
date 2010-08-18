package com.xruby.runtime.lang;
//import com.xruby.runtime.lang.*;
//import com.xruby.runtime.builtin.*;
//import com.xruby.runtime.builtin.RubyMathModule;
public class RubyMathModule_Methods{
public static void initMethods( RubyModule klass){

klass.defineModuleMethod( "cosh", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.cosh(receiver, arg);}
});
klass.defineModuleMethod( "atan", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.atan(receiver, arg);}
});
klass.defineModuleMethod( "asin", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.asin(receiver, arg);}
});
klass.defineModuleMethod( "cos", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.cos(receiver, arg);}
});
klass.defineModuleMethod( "acosh", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.acosh(receiver, arg);}
});
klass.defineModuleMethod( "sqrt", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.sqrt(receiver, arg);}
});
klass.defineModuleMethod( "cbrt", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.cbrt(receiver, arg);}
});
klass.defineModuleMethod( "sinh", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.sinh(receiver, arg);}
});
klass.defineModuleMethod( "atanh", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.atanh(receiver, arg);}
});
klass.defineModuleMethod( "log", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.log(receiver, arg);}
});
klass.defineModuleMethod( "log10", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.log10(receiver, arg);}
});
klass.defineModuleMethod( "log2", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.log2(receiver, arg);}
});

klass.defineModuleMethod( "tanh", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.tanh(receiver, arg);}
});
klass.defineModuleMethod( "exp", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.exp(receiver, arg);}
});
klass.defineModuleMethod( "frexp", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.frexp(receiver, arg);}
});
klass.defineModuleMethod( "ldexp", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return RubyMathModule.ldexp(receiver, arg0, arg1);}
});

klass.defineModuleMethod( "atan2", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return RubyMathModule.atan2(receiver, arg0, arg1);}
});
klass.defineModuleMethod( "hypot", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return RubyMathModule.hypot(receiver, arg0, arg1);}
});
klass.defineModuleMethod( "asinh", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.asinh(receiver, arg);}
});
klass.defineModuleMethod( "sin", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.sin(receiver, arg);}
});
klass.defineModuleMethod( "tan", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.tan(receiver, arg);}
});
klass.defineModuleMethod( "acos", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.acos(receiver, arg);}
});

klass.defineModuleMethod( "erf", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.erf(receiver, arg);}
});
klass.defineModuleMethod( "erfc", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.erfc(receiver, arg);}
});
klass.defineModuleMethod( "gamma", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.gamma(receiver, arg);}
});
klass.defineModuleMethod( "lgamma", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return RubyMathModule.lgamma(receiver, arg);}
});

}}
