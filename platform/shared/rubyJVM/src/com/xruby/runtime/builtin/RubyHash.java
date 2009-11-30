/**
 * Copyright 2005-2007 Xue Yong Zhi, Yu Su, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.*;

import j2me.util.HashMap;
import j2me.util.Iterator;

////@RubyLevelClass(name="Hash", modules="Enumerable")
public class RubyHash extends RubyBasic {
    private HashMap/*<RubyValue, RubyValue>*/ map_ = new HashMap/*<RubyValue, RubyValue>*/();
    private RubyValue default_value_ = RubyConstant.QNIL;
    private RubyBlock default_value_as_block_ = null;

    public RubyHash() {
        super(RubyRuntime.HashClass);
    }

    public RubyValue clone() {
    	RubyHash cl = new RubyHash();
	  	cl.doClone(this);
	  	return cl;
    }
    
    //@SuppressWarnings("unchecked")
    protected void doClone(RubyValue orig) {
    	RubyHash h = (RubyHash)orig;
        map_ = (HashMap/*<RubyValue, RubyValue>*/) h.map_.clone();
        default_value_ = h.default_value_;
        default_value_as_block_ = h.default_value_as_block_; 
        super.doClone(orig);
    }

    //@RubyLevelMethod(name="default")
    public RubyValue getDefaultValue() {
        return default_value_;
    }

    //@RubyLevelMethod(name="default")
    public RubyValue getDefaultValue(RubyValue value) {
        return default_value_;
    }

    //@RubyLevelMethod(name="default=")
    public RubyValue setDefaultValue(RubyValue defaultValue) {
        default_value_as_block_ = null;
        default_value_ = defaultValue;
        return this;
    }

    public RubyBlock getDefaultValueAsBlock() {
        return default_value_as_block_;
    }

    public void setDefaultValueAsBlock(RubyBlock b) {
        default_value_as_block_ = b;
    }

    public RubyHash add(RubyValue k, RubyValue v) {
        map_.put(k, v);
        return this;
    }

    //@RubyLevelMethod(name="size", alias="length")
    public RubyFixnum size() {
        return ObjectFactory.createFixnum(map_.size());
    }

    //RHO_COMMENT
    //@RubyLevelMethod(name="to_s")
/*    public RubyString to_s() {
        RubyString r = ObjectFactory.createString();

//        for (RubyValue key : map_.keySet()) {
        for (Iterator iter = map_.keySet().iterator(); iter.hasNext();) {
        	RubyValue key = (RubyValue)iter.next();
            RubyValue value = (RubyValue)map_.get(key);
            r.appendString(key.toString() + value.toString());
        }

        return r;
    }*/

    private static RubyID defaultID = RubyID.intern("default");

    public RubyValue getRaw(RubyValue k) {
        return (RubyValue)map_.get(k);
    }
    
    public RubyValue get(RubyValue k) {
        RubyValue v = (RubyValue)map_.get(k);
        if (null != v) {
            return v;
        } else if (null != default_value_as_block_) {
            return default_value_as_block_.invoke(this, new RubyArray(this, k));
        } else {
            return RubyAPI.callOneArgMethod(this, k, null, defaultID);
        }
    }

    //@RubyLevelMethod(name="has_key?", alias={"include?", "key?", "member?"})
    public RubyValue has_key(RubyValue key) {
        return ObjectFactory.createBoolean(map_.containsKey(key));
    }

    //@RubyLevelMethod(name="delete")
    public RubyValue delete(RubyValue k, RubyBlock block) {
        RubyValue v = (RubyValue)map_.remove(k);
        if ( v == null && block != null )
        	return block.invoke(this, k);
        
        return (null != v) ? v : RubyAPI.callOneArgMethod(this, k, null, defaultID);
    }

    //@RubyLevelMethod(name="delete_if")
    public RubyValue delete_if(RubyBlock block) {
        Iterator/*<RubyValue>*/ ite = map_.keySet().iterator();
        while (ite.hasNext()) {
            RubyValue key = (RubyValue)ite.next();
            RubyValue value = (RubyValue)map_.get(key);
            RubyValue r = block.invoke(this, key, value);
            if (r.isTrue()) {
                ite.remove();
            }
        }

        return this;
    }

    public RubyValue clear() {
        map_.clear();
        return this;
    }
    
    //@RubyLevelMethod(name="reject")
    public RubyValue reject(RubyBlock block) {
        RubyHash dup = (RubyHash)clone();
        dup.delete_if(block);
        return dup;
    }

    //@RubyLevelMethod(name="has_value?")
    public RubyValue has_value(RubyValue value) {
        return ObjectFactory.createBoolean(map_.containsValue(value));
    }

    //@RubyLevelMethod(name="values_at")
    public RubyArray values_at() {
        return new RubyArray();
    }

    //@RubyLevelMethod(name="values_at")
    public RubyArray values_at(RubyValue arg) {
        RubyArray a = new RubyArray();
        RubyValue v = (RubyValue)map_.get(arg);
        a.add((null != v) ? v : RubyConstant.QNIL);
        return a;
    }

    //@RubyLevelMethod(name="values_at")
    public RubyArray values_at(RubyArray keys) {
        RubyArray a = new RubyArray();

//        for (RubyValue key : keys) {
        for (Iterator iter = keys.iterator(); iter.hasNext();) {
        	RubyValue key = (RubyValue)iter.next();
            RubyValue v = (RubyValue)map_.get(key);
            a.add((null != v) ? v : RubyConstant.QNIL);
        }

        return a;
    }

    //@RubyLevelMethod(name="==")
    public RubyValue equal(RubyValue arg) {
        if (!(arg instanceof RubyHash)) {
            return RubyConstant.QFALSE;
        }
        return ObjectFactory.createBoolean(this.equals((RubyHash) arg));
    }

    public boolean equals(RubyHash that) {
        if (this.size() != that.size()) {
            return false;
        }

//        for (RubyValue key : map_.keySet()) {
        for (Iterator iter = map_.keySet().iterator(); iter.hasNext();) {
        	RubyValue key = (RubyValue)iter.next();
            RubyValue v1 = (RubyValue)that.get(key);
            if (null == v1) {
                return false;
            }

            RubyValue v2 = this.get(key);
            if (!RubyAPI.testEqual(v1, v2)) {
                return false;
            }
        }

        return true;
    }

    //@RubyLevelMethod(name="keys")
    public RubyArray keys() {
        RubyArray a = new RubyArray();
//        for (RubyValue key : map_.keySet()) {
        for (Iterator iter = map_.keySet().iterator(); iter.hasNext();) {
        	RubyValue key = (RubyValue)iter.next();
            a.add(key);
        }
        return a;
    }

    //@RubyLevelMethod(name="values")
    public RubyArray values() {
        RubyArray a = new RubyArray();
//        for (RubyValue value : map_.values()) {
        for (Iterator iter = map_.values().iterator(); iter.hasNext();) {
        	RubyValue value = (RubyValue)iter.next();
            a.add(value);
        }
        return a;
    }

    //@RubyLevelMethod(name="shift")
    public RubyValue shift() {
        if (map_.isEmpty()) {
            return default_value_;
        }

        RubyValue k = (RubyValue)map_.keySet().iterator().next();
        RubyValue v = (RubyValue)map_.remove(k);

        RubyArray a = new RubyArray(2);
        a.add(k);
        a.add(v);
        return a;
    }

    //TODO this should be implmented in ruby
    //@RubyLevelMethod(name="fetch")
    public RubyValue fetch(RubyArray args, RubyBlock block) {
        RubyValue key = args.get(0);

        if (args.size() >= 1) {
            RubyValue v = this.get(key);
            if (v != RubyConstant.QNIL) {
                return v;//found
            } else if (args.size() >= 2) {
                return args.get(1);//default_value
            } else if (null != block) {
                return block.invoke(this, key);
            } else {
                throw new RubyException(RubyRuntime.IndexErrorClass, "key not found");
            }
        }

        //TODO
        throw new RubyException("not implemented");
    }

    //@RubyLevelMethod(name="initialize")
    public RubyValue initialize(RubyArray args, RubyBlock block) {
        if (null != block && null != args) { // validation
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "wrong number of arguments");
        }

        if (null != args) {
            RubyValue defaultValue = args.get(0);
            this.setDefaultValue(defaultValue);
        }

        if (null != block) { // Hash.new {...}
            this.setDefaultValueAsBlock(block);
        }

        return this;
    }

    // FIXME: another []
    //@RubyLevelMethod(name="[]")
    public RubyValue getValue(RubyValue arg) {
        return this.get(arg);
    }

    //@RubyLevelMethod(name="[]=")
    public RubyValue setValue(RubyValue arg0, RubyValue arg1) {
        this.add(arg0, arg1);
        return arg1;
    }

    //RHO_COMMENT:
    //@RubyLevelMethod(name="[]")
    public static RubyValue create(RubyArray args) {
        
        if (args.size() == 1 && args.get(0).isKindOf(RubyRuntime.HashClass) ){
            return args.get(0).clone();
        }

        if ((args.size() & 1) != 0) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass,"odd number of args for Hash");
        }

        RubyHash res = ObjectFactory.createHash();
        
        for (int i=0; i < args.size(); i+=2) 
        	res.add(args.get(i), args.get(i+1));
        
        return res;
    }
    
    //@RubyAllocMethod
    public static RubyValue alloc(RubyValue receiver) {
        RubyHash h = ObjectFactory.createHash();
        h.setRubyClass((RubyClass) receiver);
        return h;
    }
    
}
