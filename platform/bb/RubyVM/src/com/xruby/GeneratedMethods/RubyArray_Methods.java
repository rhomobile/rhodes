package com.xruby.GeneratedMethods;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.*;
public class RubyArray_Methods{
public static void initMethods( RubyClass klass){

klass.defineMethod( "last", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).last();}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).last(arg);}
});
klass.defineMethod( "pop", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).pop();}
});
klass.defineMethod( "pack", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).pack(arg);}
});
klass.defineMethod( "<=>", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).compare(arg);}
});
klass.defineMethod( "reverse_each", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).reverse_each(block);}
});
klass.defineMethod( "each_index", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).each_index(block);}
});
klass.defineMethod( "compact", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).compact();}
});
klass.defineMethod( "include?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).include(arg);}
});
klass.defineMethod( "rindex", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).rindex(arg);}
});
klass.getSingletonClass().defineMethod( "new", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyArray.newArray(receiver, args, block);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return RubyArray.newArray(receiver, block);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(1, 0, block != null? block.createdByLambda() : false );
		args.add(arg);
		return RubyArray.newArray(receiver, args, block);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return RubyArray.newArray(receiver, args, block);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "rassoc", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).rassoc(arg);}
});
klass.defineMethod( "index", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).index(arg);}
});
klass.defineMethod( "assoc", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).assoc(arg);}
});
klass.defineMethod( "empty?", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).empty_p();}
});
klass.defineMethod( "&", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).and(arg);}
});
klass.defineMethod( "hash", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).hash();}
});
klass.defineMethod( "*", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).run(arg, block);}
});
klass.defineMethod( "indexes", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyArray)receiver).indexes(args);}
});
klass.aliasMethod("indices","indexes");
klass.defineMethod( "+", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).plus(arg);}
});
klass.defineMethod( "-", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).minus(arg);}
});
klass.defineMethod( "slice!", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyArray)receiver).slice_danger(args);}
});
klass.defineMethod( "each", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).each(block);}
});
klass.defineMethod( "flatten!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).flatten_danger();}
});
klass.defineMethod( "[]", new RubyOneOrTwoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).aref(arg);}
	protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyArray)receiver).aref(arg0, arg1);}
});
klass.aliasMethod("slice","[]");
klass.defineMethod( "nitems", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).nitems();}
});
klass.defineMethod( "sort", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).sort(block);}
});
klass.defineMethod( "delete_at", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).deleteAt(arg);}
});
klass.defineMethod( "collect!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).collect_danger(block);}
});
klass.aliasMethod("map!","collect!");
klass.defineMethod( "push", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyArray)receiver).multiPush(args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).multiPush();}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).multiPush(arg);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return ((RubyArray)receiver).multiPush(args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
/*klass.defineMethod( "to_s", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).to_s();}
});*/
klass.defineMethod( "clear", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).clear();}
});
klass.defineMethod( "compact!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).compact_danger();}
});
klass.defineMethod( "flatten", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).flatten();}
});
klass.defineMethod( "==", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).opEquals(arg);}
});
klass.defineMethod( "eql?", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).opEql(arg);}
});
klass.defineMethod( "length", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).length();}
});
klass.defineMethod( "delete", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).delete(arg, block);}
});
klass.defineMethod( "uniq!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).uniq_danger();}
});
klass.defineMethod( "delete_if", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).delete_if(block);}
});
klass.defineMethod( "unshift", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyArray)receiver).unshift(args);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).unshift(arg);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `"+ this.getID() + "': wrong number of arguments");}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return ((RubyArray)receiver).unshift(args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "[]=", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyArray)receiver).aset(args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).aset();}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).aset(arg);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		return ((RubyArray)receiver).aset(arg0, arg1);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "reverse", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).reverse();}
});
klass.defineMethod( "sort!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).sort_danger(block);}
});
klass.defineMethod( "at", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).at(arg);}
});
klass.defineMethod( "reverse!", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).reverseBang();}
});
klass.getSingletonClass().defineMethod( "[]", new RubyVarArgMethod(){ 
	public RubyValue invoke(RubyValue receiver, RubyArray args, RubyBlock block ){
		return RubyArray.create(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyBlock block ){
		return RubyArray.create(receiver);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(1, 0, block != null? block.createdByLambda() : false );
		args.add(arg);
		return RubyArray.create(receiver, args);}
	public RubyValue invoke(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block ){
		RubyArray args = ObjectFactory.createArray(2, 0, block != null? block.createdByLambda() : false );
		args.add(arg0);		args.add(arg1);
		return RubyArray.create(receiver, args);}
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
    	throw new Error("we overided invoke, so this method should never be called");}
});
klass.defineMethod( "concat", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).concat(arg);}
});
klass.defineMethod( "replace", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).replace(arg);}
});
klass.defineMethod( "uniq", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).uniq();}
});
klass.defineMethod( "<<", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).push(arg);}
});
klass.defineMethod( "|", new RubyOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).or(arg);}
});
klass.defineMethod( "insert", new RubyVarArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block ){
		return ((RubyArray)receiver).insert(args);}
});
klass.defineMethod( "first", new RubyNoOrOneArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).first();}
	protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block ){
		return ((RubyArray)receiver).first(arg);}
});
klass.defineMethod( "shift", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).shift();}
});
klass.defineMethod( "to_a", new RubyNoArgMethod(){ 
	protected RubyValue run(RubyValue receiver, RubyBlock block ){
		return ((RubyArray)receiver).to_a();}
});

}}
