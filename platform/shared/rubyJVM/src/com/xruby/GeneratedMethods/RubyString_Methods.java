package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
public class RubyString_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "downcase!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).downcaseBang();}
});
klass.defineMethod( "count", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).count(args);}
});
klass.defineMethod( "tr", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyString)receiver).tr(arg0, arg1);}
});
klass.defineMethod( "swapcase!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).swapcaseBang();}
});
klass.defineMethod( "<=>", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).operator_compare(arg);}
});
klass.defineMethod( "include?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).include_p(arg);}
});
klass.defineMethod( "=~", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).operator_match(arg);}
});
klass.defineMethod( "match", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).match(arg);}
});

klass.defineMethod( "squeeze", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).squeeze(args);}
});
klass.defineMethod( "hex", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).hex();}
});
klass.defineMethod( "downcase", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).downcase();}
});
klass.defineMethod( "initialize", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).initialize();}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).initialize(arg);}
});
klass.aliasMethod("initialize_copy","initialize");
klass.defineMethod( "delete!", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).delete_danger(args);}
});
klass.defineMethod( "%", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).format(arg);}
});
klass.defineMethod( "*", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).operator_star(arg);}
});
klass.defineMethod( "+", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).plus(arg);}
});
klass.defineMethod( "capitalize", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).capitalize();}
});
klass.defineMethod( "strip", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).strip();}
});
klass.defineMethod( "upcase", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).upcase();}
});
klass.defineMethod( "split", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).split(args);}
});
klass.defineMethod( "each_byte", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).each_byte(block);}
});
klass.aliasMethod("bytes","each_byte");
klass.defineMethod( "each_char", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).each_char(block);}
});
klass.aliasMethod("chars","each_char");

klass.defineMethod( "each", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).each(block);}
});
klass.aliasMethod("each_line","each");
klass.defineMethod( "chop", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).chop();}
});
klass.defineMethod( "sub!", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).sub_danger(args, block);}
});
klass.defineMethod( "unpack", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).unpack(arg);}
});
klass.defineMethod( "sub", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).sub(args, block);}
});
klass.defineMethod( "to_f", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).toRubyFloat();}
});
klass.defineMethod( "[]", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).array_access(args);}
});
klass.aliasMethod("slice","[]");

klass.defineMethod( "dump", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).rubyDump();}
});
klass.defineMethod( "to_i", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).to_i();}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).to_i(arg);}
});
klass.defineMethod( "gsub!", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).gsub_danger(args, block);}
});
klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).to_s();}
});
klass.defineMethod( "swapcase", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).swapcase();}
});
klass.defineMethod( "upcase!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).upcaseBang();}
});
klass.defineMethod( "==", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).opEqual(arg);}
});
klass.defineMethod( "eql?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).opEql(arg);}
});
klass.defineMethod( "delete", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).delete(args);}
});
klass.defineMethod( "length", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).rubyLength();}
});
klass.defineMethod( "bytesize", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).rubyBytesize();}
});

klass.defineMethod( "capitalize!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).capitalizeBang();}
});
klass.defineMethod( "tr_s!", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyString)receiver).trs_danger(arg0, arg1);}
});
klass.defineMethod( "[]=", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).array_set(args);}
});
klass.defineMethod( "gsub", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).gsub(args, block);}
});
klass.defineMethod( "reverse", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).reverse();}
});
klass.defineMethod( "chomp", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).chomp(args);}
});
klass.defineMethod( "tr!", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyString)receiver).tr_danger(arg0, arg1);}
});
klass.defineMethod( "reverse!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).reverse_danger();}
});
klass.defineMethod( "tr_s", new RubyTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyString)receiver).tr_s(arg0, arg1);}
});
klass.defineMethod( "concat", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).concat(arg);}
});
klass.aliasMethod("<<","concat");
klass.defineMethod( "strip!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).stripBang();}
});
klass.defineMethod( "squeeze!", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).squeeze_danger(args);}
});
klass.defineMethod( "lstrip!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).lstripBang();}
});
klass.defineMethod( "chomp!", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyString)receiver).chomp_danger(args);}
});
klass.defineMethod( "intern", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).intern();}
});
klass.aliasMethod("to_sym","intern");
klass.defineMethod( "scan", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).scan(arg, block);}
});
klass.defineMethod( "chop!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).chopBang();}
});
klass.defineMethod( "encoding", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).getEncoding();}
});
klass.defineMethod( "force_encoding", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).force_encoding(arg);}
});
klass.defineMethod( "end_with?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyString)receiver).opEndWith(arg);}
});
klass.defineMethod( "end_with?", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray arg, RubyBlock block ){
		return ((RubyString)receiver).opEndWith(arg);}
});
klass.defineMethod( "ord", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyString)receiver).ord();}
});

klass.defineAllocMethod(new RubyNoArgMethod(){
	protected RubyValue run(RubyValue receiver, RubyBlock block )	{
		return RubyString.alloc(receiver);	}} );
}}

