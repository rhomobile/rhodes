/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng, Jie Li, Yu Su
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.*;
//import com.xruby.runtime.lang.annotation.RubyLevelClass;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;

import j2me.util.*;
import j2me.lang.Iterable;
import j2me.lang.AssertMe;

////@RubyLevelClass(name="Array", modules="Enumerable")
public class RubyArray extends RubyBasic implements Iterable/*<RubyValue>*/ {
    private List/*<RubyValue>*/ array_;

    //MRHS will be converted to a RubyArray, but its original form need to be saved
    //to handle multiple assignment
    private final int rhs_size_;
    private final boolean has_single_asterisk_or_lambda_call_;

    public RubyArray() {
        this(0);
    }

    public RubyArray(int size) {
        this(size, 0, false);
    }

    public RubyArray(RubyValue v) {
        this(1);
        this.array_.add(v);
    }

    public RubyArray(RubyValue value1, RubyValue value2) {
        this(2);
        this.array_.add(value1);
        this.array_.add(value2);
    }

    RubyArray(int size, RubyValue default_value) {
        this(size);
        for (int i = 0; i < size; ++i) {
            array_.add(default_value);
        }
    }

    RubyArray(int size, int rhs_size, boolean has_single_asterisk_or_lambda_call) {
        super(RubyRuntime.ArrayClass);
        array_ = new ArrayList/*<RubyValue>*/(size);

        rhs_size_ = rhs_size;
        has_single_asterisk_or_lambda_call_ = has_single_asterisk_or_lambda_call;
    }

    private RubyArray(List/*<RubyValue>*/ array) {
        super(RubyRuntime.ArrayClass);
        this.array_ = array;

        rhs_size_ = 0;
        has_single_asterisk_or_lambda_call_ = false;
    }

    //@SuppressWarnings("unchecked")
    public /*RubyArray*/RubyValue clone() {
        RubyArray v = new RubyArray(0, this.rhs_size_, this.has_single_asterisk_or_lambda_call_);//(RubyArray) super.clone();
        v.array_ = new ArrayList/*<RubyValue>*/(this.array_);
        v.doClone(this);
        return v;
    }

    public RubyArray toAry() {
        return this;
    }

    public boolean isSingleAsterisk() {
        return (0 == rhs_size_) && has_single_asterisk_or_lambda_call_;
    }

    public boolean isSingleRhs() {
        return (rhs_size_ <= 1) && !has_single_asterisk_or_lambda_call_;
    }

    //@RubyLevelMethod(name="to_a")
    public RubyValue to_a() {
        if( this.getRubyClass() != RubyRuntime.ArrayClass)
            return clone();
        
        return this;
    }
    
    //@RubyLevelMethod(name="[]")
    public static RubyValue create(RubyValue receiver) {
    	RubyArray a = new RubyArray();
    	a.setRubyClass((RubyClass) receiver);
    	return a;
    }
    
    //@RubyLevelMethod(name="[]")
    public static RubyValue create(RubyValue receiver, RubyArray args) {
        RubyArray a = args.copy();
        a.setRubyClass((RubyClass) receiver);
        return a;
    }

    public RubyArray add(/*RubyValue*/Object v) {
        array_.add(v);
        return this;
    }
    public RubyArray add(RubyValue v) {
        array_.add(v);
        return this;
    }

    private RubyArray insert(int index, RubyArray a) {
        for (int i = array_.size(); i < index; ++i) {
            array_.add(RubyConstant.QNIL);
        }
        array_.addAll(index, a.array_);
        return this;
    }

    public RubyArray insert(int index, RubyValue v) {
        array_.add(index, v);
        return this;
    }

    public int size() {
        return array_.size();
    }

    public RubyValue delete_at(int index) {
        if (index >= size()) {
            return RubyConstant.QNIL;
        }

        if(index < 0){
            index += size();
        }

        if(index < 0){
            return RubyConstant.QNIL;
        }

        return (RubyValue)array_.remove(index);
    }

    private RubyArray delete_at(int begin, int length) {
        final int arraySize = array_.size();
        if (begin > arraySize) {
            return null;
        }

        if (length < 0) {
            return null;
        }

        if (begin < 0) {
            begin += arraySize;
        }

        if (begin + length > arraySize) {
            length = arraySize - begin;
            if (length < 0) {
                length = 0;
            }
        }

        while(length > 0){
            array_.remove(begin);
            length--;
        }

        return this;
    }

    private RubyArray plus(RubyArray v) {
        int size = array_.size() + v.size();
        RubyArray resultArray = new RubyArray(size);
        resultArray.array_.addAll(array_);
        resultArray.array_.addAll(v.array_);
        return resultArray;
    }

    private RubyArray minus(RubyArray other) {
        RubyArray a = this.copy();
//        for (RubyValue v : other) {
        for (Iterator iter = other.iterator(); iter.hasNext();) {
        	RubyValue v = (RubyValue)iter.next();
            a.remove(v);
        }
        return a;
    }

    private boolean remove(RubyValue v) {
        boolean r = false;
        while (array_.remove(v)) {
            r = true;
        }
        return r;
    }

    public RubyArray times(int times) {
        if (times < 0) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "negative argument");
        }

        int size = array_.size() * times;
        RubyArray resultArray = new RubyArray(size);
        for (int i = 0; i < times; i++) {
            resultArray.array_.addAll(array_);
        }

        return resultArray;
    }

    public RubyArray expand(RubyValue v) {
        if (v instanceof RubyArray) {
            //[5,6,*[1, 2]]
            array_.addAll(((RubyArray) v).array_);
        } else {
            //[5,6,*1], [5,6,*nil]
            array_.add(v);
        }

        return this;
    }

    //create a new Array containing every element from index to the end
    public RubyValue collect(int index) {
        AssertMe.rho_assert(index >= 0);

        final int size = array_.size() - index;
        if (size < 0) {
            return new RubyArray();
        }

        RubyArray a = new RubyArray(size);
        for (int i = index; i < array_.size(); ++i) {
            a.add(array_.get(i));
        }

        return a;
    }

    public Iterator/*<RubyValue>*/ iterator() {
        return array_.iterator();
    }

    private int getRealIndex(int i) {
        int index = i;
        if (index < 0) {
            index = array_.size() + index;
        }

        if (index < 0) {
            throw new RubyException(RubyRuntime.IndexErrorClass, "index " + i + " out of array");
        }

        return index;
    }

    public RubyValue set(int start, RubyValue value) {
        int index = getRealIndex(start);

        if (index < array_.size()) {
            array_.set(index, value);
        } else {
            for (int i = array_.size(); i < index; ++i) {
                array_.add(RubyConstant.QNIL);
            }
            array_.add(value);
        }

        return value;
    }

    public RubyValue replace(int start, int length, RubyValue value) {
        int index = getRealIndex(start);
        if (value == RubyConstant.QNIL) {
            for(int i=0;i<length;i++) {
                array_.remove(index);
            }
            return value;
        }
        if (length < 0) {
            throw new RubyException(RubyRuntime.IndexErrorClass, "negative length (" + length + ")");
        } else if (0 == length) {
            if (value instanceof RubyArray) {
                array_.addAll(index, ((RubyArray) value).array_);
            } else {
                array_.add(index, value);
            }
        } else {
            for (int i = 0; i < length -1; ++i) {
                array_.remove(index);
            }
            if (value instanceof RubyArray) {
                array_.remove(index);
                array_.addAll(index, ((RubyArray) value).array_);
            } else {
                array_.set(index, value);
            }
        }
        return value;
    }

    public RubyValue get(int index) {
        if (index < 0) {
            index = array_.size() + index;
        }

        if (index < 0 || index >= size()) {
            return RubyConstant.QNIL;
        } else {
            return (RubyValue)array_.get(index);
        }
    }

    public RubyArray copy() {
        RubyArray resultArray = new RubyArray(array_.size());
//        for (RubyValue v : array_) {
        for (Iterator iter = array_.iterator(); iter.hasNext();) {
        	RubyValue v = (RubyValue)iter.next();
            resultArray.add(v);
        }

        return resultArray;
    }

    private RubyValue compare(RubyArray other_array) {
        int length = (size() <= other_array.size()) ? size() : other_array.size();
        for (int i = 0; i < length; ++i) {
            RubyValue v = RubyAPI.callPublicOneArgMethod(get(i), other_array.get(i), null, RubyID.unequalID);
            if (!RubyAPI.testEqual(v, ObjectFactory.FIXNUM0)) {
                return v;
            }
        }

        if (size() == other_array.size()) {
            return ObjectFactory.FIXNUM0;
        } else if (size() > other_array.size()) {
            return ObjectFactory.FIXNUM1;
        } else {
            return ObjectFactory.FIXNUM_NEGATIVE_ONE;
        }
    }

    public RubyArray subarray(int begin, int length) {
        final int arraySize = array_.size();
        if (begin > arraySize) {
            return null;
        }

        if (length < 0) {
            return null;
        }

        if (begin < 0) {
            begin += arraySize;
        }

        if (begin + length > arraySize) {
            length = arraySize - begin;
            if (length < 0) {
                length = 0;
            }
        }

        if (length == 0) {
            return new RubyArray(0);
        }

        return new RubyArray(array_.subList(begin, begin + length));
    }

    //RHO_COMMENT
    public boolean equals(Object o, boolean bConvToAry) {
        if (this == o) {
            return true;
        }

        if (o instanceof RubyArray) {
            RubyArray that = (RubyArray)o;
            int size = array_.size();
            if (size != that.size()) {
                return false;
            }

            for (int i = 0; i < size; ++i) {
                if (!this.get(i).equals(that.get(i))) {
                    return false;
                }
            }

            return true;
        } else if (o instanceof RubyValue && bConvToAry ) {
            RubyValue v = (RubyValue)o;
            if (!v.respondTo(RubyID.toAryID)) {
                return false;
            } else {
                return v.equals(this);
            }
        } else {
            return false;
        }
    }
    
    private void sort() {
        Collections.sort(array_, new Comparator/*<RubyValue>*/() {
            public int compare(Object/*RubyValue*/ arg0, Object/*RubyValue*/ arg1) {
                RubyValue v = RubyAPI.callPublicOneArgMethod((RubyValue)arg0, (RubyValue)arg1, null, RubyID.unequalID);
                return ((RubyFixnum) v).toInt();
            }
        }
        );
    }

    private void sort_with_block(final RubyBlock block) {
        final RubyArray self = this;
        Collections.sort(array_, new Comparator/*<RubyValue>*/() {

            public int compare(Object/*RubyValue*/ arg0, Object/*RubyValue*/ arg1) {
                //TODO can not check if block return/break occured.
                RubyValue v = block.invoke(self, (RubyValue)arg0, (RubyValue)arg1);
                return ((RubyFixnum) v).toInt();
            }
        }
        );
    }

    private int getRealIndex(int size,int i) {
        int index = i;
        if (index < 0) {
            index = size + index;
        }

        if (index < 0) {
            throw new RubyException(RubyRuntime.IndexErrorClass, "index " + i + " out of array");
        }

        return index;
    }

    private boolean recursiveAdd(RubyArray receiver,RubyArray array){
        boolean modified = false;
        for (int i=0;i<array.size();i++) {
            RubyValue val = array.get(i);
            if (val instanceof RubyArray) {
                modified = true;
                recursiveAdd(receiver,(RubyArray)val);
            } else {
                receiver.add(val);
            }
        }
        return modified;
    }
    
    //@RubyLevelMethod(name="new", singleton=true)
    public static RubyValue newArray(RubyValue receiver, RubyBlock block) {
    	RubyArray a = new RubyArray();
    	a.setRubyClass((RubyClass) receiver);
        return a;
    }

    //@RubyLevelMethod(name="new", singleton=true)
    public static RubyValue newArray(RubyValue receiver, RubyArray args, RubyBlock block) {
        RubyArray a;
        if (null == args) {
            a = new RubyArray();
        } else if (null == block) {
            if (args.get(0) instanceof RubyArray) {
                a = (RubyArray)args.get(0).clone();
            } else {
                RubyFixnum size = (RubyFixnum) args.get(0);
                RubyValue default_value = args.get(1);
                a = ObjectFactory.createArray(size.toInt(), default_value);
            }
        } else {
            RubyFixnum size = (RubyFixnum) args.get(0);
            a  = new RubyArray();
            for (int i=0; i<size.toFloat(); i++) {
                RubyValue return_value = block.invoke(receiver, ObjectFactory.createFixnum(i));
                a.add(return_value);
            }
        }
        a.setRubyClass((RubyClass) receiver);
        return a;
    }

    //@RubyLevelMethod(name="empty?")
    public RubyValue empty_p() {
        return ObjectFactory.createBoolean(this.array_.isEmpty());
    }

    //RHO_COMMENT
    //@RubyLevelMethod(name="to_s")
    /*public RubyValue to_s() {
        RubyString r = ObjectFactory.createString();

//        for (RubyValue v : array_) {
        for (Iterator iter = array_.iterator(); iter.hasNext();) {
        	RubyValue v = (RubyValue)iter.next();
            r.appendString(v);

            // TODO: The output of to_s is not as the same as cruby, we should solve this issue
            // TODO: and change the corresponding testcases in RubyCompilerTest, such as test_array_expand.
        }

        return r;
    }*/

    //@RubyLevelMethod(name="length")
    public RubyFixnum length() {
        return ObjectFactory.createFixnum(this.array_.size());
    }

    //@RubyLevelMethod(name="clear")
    public RubyArray clear() {
        array_.clear();
        return this;
    }

    //@RubyLevelMethod(name="[]", alias="slice")
    public RubyValue aref(RubyValue arg) {
        if (arg instanceof RubyFixnum) {
            return this.get(arg.toInt());
        }

        if (arg instanceof RubySymbol) {
            throw new RubyException(RubyRuntime.TypeErrorClass, "Symbol as array index");
        }

        if (arg instanceof RubyRange) {
            RubyRange range = (RubyRange)arg;
            int begin = range.getLeft().toInt();
            int end = range.getRight().toInt();
            if (begin < 0) {
                begin = this.size() + begin;
            }
            if (end < 0) {
                end = this.size() + end;
            }

            if (!range.isExcludeEnd()) {
                ++end;
            }

            RubyArray resultValue = this.subarray(begin, end - begin);
            if ( null == resultValue )
            	return RubyConstant.QNIL;
            return resultValue;
            //return (null == resultValue ? RubyConstant.QNIL : resultValue);
        }

        return this.get(arg.toInt());
    }

    //@RubyLevelMethod(name="[]", alias="slice")
    public RubyValue aref(RubyValue begin, RubyValue length) {
        if (begin instanceof RubySymbol) {
            throw new RubyException(RubyRuntime.TypeErrorClass, "Symbol as array index");
        }

        RubyArray resultValue = this.subarray(begin.toInt(), length.toInt());
        if ( null == resultValue )
        	return RubyConstant.QNIL;
        return resultValue;
//        return (null == resultValue ? RubyConstant.QNIL : resultValue);
    }

    //@RubyLevelMethod(name="insert")
    public RubyArray insert(RubyArray ary) {
        int argc = ary.size();
        if (argc == 1) {
            return this;
        }

        if (argc < 1) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass,  "wrong number of arguments (at least 1)");
        }

        int pos = ary.get(0).toInt();
        if (pos < 0) {
            pos += this.array_.size() + 1;
        }

        return this.insert(pos, ary.subarray(1, ary.size() - 1));
    }

    //@RubyLevelMethod(name="first")
    public RubyValue first() {
        if (this.array_.size() == 0) {
            return RubyConstant.QNIL;
        } else {
            return (RubyValue)this.array_.get(0);
        }
    }

    //@RubyLevelMethod(name="first")
    public RubyValue first(RubyValue v) {
        int n = v.toInt();
        int size = this.array_.size();
        if (n > size) {
            n = size;
        }

        return new RubyArray(this.array_.subList(0, n));
    }

    //@RubyLevelMethod(name="last")
    public RubyValue last() {
        if (this.array_.size() == 0) {
            return RubyConstant.QNIL;
        } else {
            return (RubyValue)this.array_.get(this.size() - 1);
        }
    }

    //@RubyLevelMethod(name="last")
    public RubyValue last(RubyValue v) {
        int n = v.toInt();
        int size = this.array_.size();
        if (n > size) {
            n = size;
        }

        return new RubyArray(this.array_.subList(size - n, size));
    }

    //@RubyLevelMethod(name="at")
    public RubyValue at(RubyValue value) {
        return this.get(value.toInt());
    }

    //@RubyLevelMethod(name="<<")
    public RubyArray push(RubyValue v) {
        this.array_.add(v);
        return this;
    }
    
    //@RubyLevelMethod(name="push")
    public RubyArray multiPush() {
        return this;
    }
    
    //@RubyLevelMethod(name="push")
    public RubyArray multiPush(RubyValue v) {
        this.array_.add(v);
        return this;
    }

    //@RubyLevelMethod(name="push")
    public RubyArray multiPush(RubyArray args) {
//    	for (RubyValue v : args) {
        for (Iterator iter = args.iterator(); iter.hasNext();) {
        	RubyValue v = (RubyValue)iter.next();
    		this.array_.add(v);
    	}
    	return this;
    }

    //@RubyLevelMethod(name="pop")
    public RubyValue pop() {
        int size = this.array_.size();
        if (0 == size) {
            return RubyConstant.QNIL;
        }
        return (RubyValue)this.array_.remove(size - 1);
    }

    //@RubyLevelMethod(name="delete_at")
    public RubyValue deleteAt(RubyValue v) {
        return this.delete_at(v.toInt());
    }

    //@RubyLevelMethod(name="shift")
    public RubyValue shift() {
    	return (RubyValue)((this.array_.size() != 0) ? this.array_.remove(0) : RubyConstant.QNIL);
    }

    //@RubyLevelMethod(name="delete_if")
    public RubyValue delete_if(RubyBlock block) {
		for (int i = 0; i < array_.size();) {
            RubyValue r = block.invoke(this, (RubyValue)array_.get(i));
            if (r.isTrue()) {
                array_.remove(i);
            } else {
                ++i;
            }
        }
        return this;
    }

    //@RubyLevelMethod(name="delete")
    public RubyValue delete(RubyValue item, RubyBlock block) {
        boolean found = false;
        while (array_.remove(item)) {
            found = true;
        }

        if (block != null && !found) {
            return block.invoke(item);
        } else {
            return found ? item : RubyConstant.QNIL;
        }
    }

    //@RubyLevelMethod(name="<=>")
    public RubyValue compare(RubyValue v) {
        return this.compare(v.toAry());
    }

    //RHO_COMMENT
    //@RubyLevelMethod(name="==")
    public RubyValue opEquals(RubyValue v) {
        return ObjectFactory.createBoolean(equals(v,true));
    }

    //@RubyLevelMethod(name="eql?")
    public RubyValue opEql(RubyValue v) {
        return ObjectFactory.createBoolean(equals(v,false));
    }
    
    //@RubyLevelMethod(name="concat")
    public RubyArray concat(RubyValue v) {
        RubyArray ary = v.toAry();
        array_.addAll(ary.array_);
        return this;
    }

    //@RubyLevelMethod(name="+")
    public RubyArray plus(RubyValue v) {
        return this.plus(v.toAry());
    }

    //@RubyLevelMethod(name="-")
    public RubyArray minus(RubyValue v) {
        return this.minus(v.toAry());
    }

    //@RubyLevelMethod(name="include?")
    public RubyValue include(RubyValue v) {
//        for (RubyValue value : array_) {
        for (Iterator iter = array_.iterator(); iter.hasNext();) {
        	RubyValue value = (RubyValue)iter.next();
            if (RubyAPI.testEqual(value, v)) {
                return RubyConstant.QTRUE;
            }
        }

        return RubyConstant.QFALSE;
    }

    //@RubyLevelMethod(name="each")
    public RubyValue each(RubyBlock block) 
    {
    	if ( block == null )
    		return this;
    	
//        for (RubyValue item : array_) {
        for (Iterator iter = array_.iterator(); iter.hasNext();) {
        	RubyValue item = (RubyValue)iter.next();
            RubyValue v = block.invoke(this, item);
            if (block.breakedOrReturned()) {
                return v;
            }
        }
        return this;
    }

    //@RubyLevelMethod(name="each_index")
    public RubyValue each_index(RubyBlock block) 
    {
    	if ( block == null )
    		return this;
    	
        for (int i=0;i<size();i++) {
            RubyValue v = block.invoke(this, new RubyFixnum(i));
            if (block.breakedOrReturned()) {
                return v;
            }
        }
        return this;
    }

    //@RubyLevelMethod(name="reverse_each")
    public RubyValue reverse_each(RubyBlock block) 
    {
    	if ( block == null )
    		return this;
    	
        ListIterator/*<RubyValue>*/ ite = array_.listIterator(array_.size());
        while (ite.hasPrevious()) {
            RubyValue v = block.invoke(this, (RubyValue)ite.previous());
            if (block.breakedOrReturned()) {
                return v;
            }
        }
        return this;
    }

    //@RubyLevelMethod(name="unshift")
    public RubyArray unshift(RubyValue value) {
        array_.add(0, value);
        return this;
    }

    //@RubyLevelMethod(name="unshift")
    public RubyArray unshift(RubyArray value) {
        array_.addAll(0, value.array_);
        return this;
    }

    public int hashCode() {
        int hash = 0;
//        for (RubyValue v : array_) {
        for (Iterator iter = array_.iterator(); iter.hasNext();) {
        	RubyValue v = (RubyValue)iter.next();
            hash += v.hashCode();
        }
        return hash;
    }

    //@RubyLevelMethod(name="hash")
    public RubyFixnum hash() {
        return ObjectFactory.createFixnum(hashCode());
    }

    //@RubyLevelMethod(name="&")
    public RubyArray and(RubyValue value) {
        RubyArray other = value.toAry();

        RubyArray a = new RubyArray();
//        for (RubyValue v : array_) {
        for (Iterator iter = array_.iterator(); iter.hasNext();) {
        	RubyValue v = (RubyValue)iter.next();
            if (other.include(v) == RubyConstant.QTRUE &&
                    a.include(v) == RubyConstant.QFALSE) {
                a.add(v);
            }
        }
        return a;
    }

    //@RubyLevelMethod(name="|")
    public RubyArray or(RubyValue value) {
        RubyArray other = value.toAry();
        RubyArray a = new RubyArray();
//        for (RubyValue v : array_) {
        for (Iterator iter = array_.iterator(); iter.hasNext();) {
        	RubyValue v = (RubyValue)iter.next();
            if (a.include(v)  == RubyConstant.QFALSE) {
                a.add(v);
            }
        }
//        for (RubyValue v : other) {
        for (Iterator iter = other.iterator(); iter.hasNext();) {
        	RubyValue v = (RubyValue)iter.next();
            if (a.include(v) == RubyConstant.QFALSE) {
                a.add(v);
            }
        }
        return a;
    }

    //@RubyLevelMethod(name="compact")
    public RubyValue compact() {
    	//RHO_COMMENT
        //return copy().compact_danger();
    	RubyArray arRes = copy();
    	arRes.compact_danger();//If No nil inside it return NIL
    	return arRes;
    }

    //@RubyLevelMethod(name="compact!")
    public RubyValue compact_danger() {
        if (remove(RubyConstant.QNIL)) {
            return this;
        } else {
            return RubyConstant.QNIL;
        }
    }

    //@RubyLevelMethod(name="uniq")
    public RubyValue uniq() 
    {
    	RubyArray res = copy();
        res.uniq_danger();
        
        return res;
    }

    //@RubyLevelMethod(name="uniq!")
    public RubyValue uniq_danger() {
        boolean b = false;
        for (int i = 0; i < array_.size(); ++i) {
            for (int j = i + 1; j < array_.size();) {
                if (RubyAPI.testEqual((RubyValue)array_.get(i), (RubyValue)array_.get(j))) {
                    array_.remove(j);
                    b = true;
                } else {
                    ++j;
                }
            }
        }
        if ( !b )
        	return RubyConstant.QNIL;
        
        return this;
        //return b ? this : RubyConstant.QNIL;
    }

    //@RubyLevelMethod(name="reverse")
    public RubyArray reverse() {
        RubyArray array = this.copy();
        Collections.reverse(array.array_);
        return array;
    }

    //@RubyLevelMethod(name="reverse!")
    public RubyArray reverseBang() {
        Collections.reverse(array_);
        return this;
    }

    //@RubyLevelMethod(name="index")
    public RubyValue index(RubyValue arg) {
        for(int i=0; i<size(); i++){
            if(get(i).equals(arg))
                return ObjectFactory.createFixnum(i);
        }
        return RubyConstant.QNIL;
    }

    //@RubyLevelMethod(name="rindex")
    public RubyValue rindex(RubyValue arg) {
        for(int i=size()-1; i>=0; i--){
            if(get(i).equals(arg))
                return ObjectFactory.createFixnum(i);
        }
        return RubyConstant.QNIL;
    }

    //@RubyLevelMethod(name="replace")
    public RubyValue replace(RubyValue arg) {
        RubyArray anotherArray = (RubyArray)arg;
        if(this == anotherArray)
            return this;

        clear();
        for(int i=0;i<anotherArray.size();i++){
            add(anotherArray.get(i));
        }
        return this;
    }

    //@RubyLevelMethod(name="indexes", alias="indices")
    public RubyValue indexes(RubyArray args) {
        RubyArray a = new RubyArray(args.size());
        for(int i=0;i<size();i++){
            RubyFixnum n = (RubyFixnum) args.get(i);
            a.add(get(n.toInt()));
        }
        return a;
    }

    //@RubyLevelMethod(name="nitems")
    public RubyValue nitems() {
        int n = 0;
        for(int i=0;i<size();i++){
            if(get(i) != RubyConstant.QNIL)
                n++;
        }
        return ObjectFactory.createFixnum(n);
    }

    //@RubyLevelMethod(name="flatten")
    public RubyValue flatten() {
        RubyArray a = new RubyArray();
        recursiveAdd(a, this);
        return a;
    }

    //@RubyLevelMethod(name="flatten!")
    public RubyValue flatten_danger() {
        RubyArray copy = copy();
        clear();
        boolean hasModified = recursiveAdd(this, copy);
        if (hasModified){
            return this;
        } else {
            return RubyConstant.QNIL;
        }
    }

    //@RubyLevelMethod(name="collect!")
    public RubyValue collect_danger(RubyBlock block) {
        RubyArray a = (RubyArray)RubyAPI.callPublicNoArgMethod(this, block, RubyID.intern("collect"));
        clear();
        for(int i=0;i<a.size();i++){
            add(a.get(i));
        }
        return this;
    }

    //@RubyLevelMethod(name="assoc")
    public RubyValue assoc(RubyValue arg) {
        RubyValue val = null;
        for(int i=0;i<size();i++){
            val = get(i);
            if(val instanceof RubyArray){
                if(((RubyArray)val).size() > 0){
                    RubyValue tmp = ((RubyArray)val).get(0);
                    if(arg.equals(tmp)){
                        return val;
                    }
                }
            }
        }

        return RubyConstant.QNIL;
    }

    //@RubyLevelMethod(name="rassoc")
    public RubyValue rassoc(RubyValue arg) {
        int size = this.array_.size();
		for(int i=0; i < size; i++){
			RubyValue val = get(i);
            if(val instanceof RubyArray){
                if(((RubyArray)val).size() > 1){
                    RubyValue tmp = ((RubyArray)val).get(1);
                    if(arg.equals(tmp)){
                        return val;
                    }
                }
            }
        }

        return RubyConstant.QNIL;
    }

    //@RubyLevelMethod(name="*")
    public RubyValue run(RubyValue arg, RubyBlock block) {
        if (arg instanceof RubyFixnum) {
            RubyFixnum times = (RubyFixnum) arg;
            return times(times.toInt());
        } else if (arg instanceof RubyString) {
            return RubyAPI.callOneArgMethod(this, arg, block, RubyID.joinID);
        }

        throw new RubyException(RubyRuntime.TypeErrorClass, "no implicit conversion from " + arg + " to integer");
    }

    //@RubyLevelMethod(name="sort")
    public RubyValue sort(RubyBlock block) {
        return copy().sort_danger(block);
    }

    //@RubyLevelMethod(name="sort!")
    public RubyValue sort_danger(RubyBlock block) {
        if (null == block) {
            sort();
        } else {
            sort_with_block(block);
        }
        return this;
    }
    
    //@RubyLevelMethod(name="[]=")
    public RubyValue aset() {
        throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong number of arguments (0 for 2)");
    }
    
    //@RubyLevelMethod(name="[]=")
    public RubyValue aset(RubyValue arg) {
        throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong number of arguments (1 for 2)");
    }
    
    //@RubyLevelMethod(name="[]=")
    public RubyValue aset(RubyValue arg0, RubyValue arg1) {
    	if (arg0 instanceof RubyRange) {
            RubyRange range = (RubyRange)arg0;
            int left = range.getLeft().toInt();
            int right = range.getRight().toInt();
            int l_index = getRealIndex(size(), left);
            int r_index = getRealIndex(size(), right);
            int length = r_index - l_index + 1;
            return replace(l_index, length, arg1);
        } else {
            return set(arg0.toInt(), arg1);
        }
    }

    //@RubyLevelMethod(name="[]=")
    public RubyValue aset(RubyArray args) {
        if (3 == args.size()) {
            int index = args.get(0).toInt();
            int length = args.get(1).toInt();
            return replace(index, length, args.get(2));
        }

        throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong number of arguments (" + args.size() + " for 2)");
    }

    //@RubyLevelMethod(name="slice!")
    public RubyValue slice_danger(RubyArray args) {
        if (1 == args.size()) {
            Object argValue = args.get(0);
            if (argValue instanceof RubyFixnum) {
                RubyFixnum index = (RubyFixnum) argValue;
                return delete_at(index.toInt());
            } else if (args.get(0) instanceof RubyRange) {
                int begin = ((RubyFixnum) ((RubyRange) args.get(0)).getLeft()).toInt();
                int end = ((RubyFixnum) ((RubyRange) args.get(0)).getRight()).toInt();
                if (begin < 0) {
                    begin = size() + begin;
                }
                if (end < 0) {
                    end = size() + end;
                }

                if (!((RubyRange) args.get(0)).isExcludeEnd()) {
                    ++end;
                }

                RubyArray resultValue = delete_at(begin, end - begin);
                if ( null == resultValue )
                	return RubyConstant.QNIL;
                return resultValue;
//                return (null == resultValue ? RubyConstant.QNIL : resultValue);
            }
        } else if (2 == args.size()) {
            Object arg0Value = args.get(0);
            Object arg1Value = args.get(1);
            if (arg0Value instanceof RubyFixnum && arg1Value instanceof RubyFixnum) {
                int begin = ((RubyFixnum) arg0Value).toInt();
                int length = ((RubyFixnum) arg1Value).toInt();
                RubyArray resultValue = delete_at(begin, length);
                if ( null == resultValue )
                	return RubyConstant.QNIL;
                return resultValue;
//                return (null == resultValue ? RubyConstant.QNIL : resultValue);
            }
        }

        //TODO
        throw new RubyException("not implemented");
    }

    //@RubyLevelMethod(name="pack")
    public RubyValue pack(RubyValue arg) {
        String format = ((RubyString) arg).toString();
        return ObjectFactory.createString(ArrayPacker.pack(this, format));
    }
    
}

