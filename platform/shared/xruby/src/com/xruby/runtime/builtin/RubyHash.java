/**
 * Copyright 2005-2007 Xue Yong Zhi, Yu Su, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.*;
import com.xruby.runtime.lang.annotation.RubyLevelClass;
import com.xruby.runtime.lang.annotation.RubyLevelMethod;
import com.xruby.runtime.lang.annotation.RubyAllocMethod;

import java.util.HashMap;
import java.util.Iterator;

@RubyLevelClass(name="Hash", modules="Enumerable")
public class RubyHash extends RubyBasic {
    private HashMap<RubyValue, RubyValue> map_ = new HashMap<RubyValue, RubyValue>();
    private RubyValue default_value_ = RubyConstant.QNIL;
    private RubyBlock default_value_as_block_ = null;

    RubyHash() {
        super(RubyRuntime.HashClass);
    }

    @SuppressWarnings("unchecked")
    public RubyHash clone() {
        RubyHash h = (RubyHash) super.clone();
        h.map_ = (HashMap<RubyValue, RubyValue>) map_.clone();
        return h;
    }

    @RubyLevelMethod(name="default")
    public RubyValue getDefaultValue() {
        return default_value_;
    }

    @RubyLevelMethod(name="default")
    public RubyValue getDefaultValue(RubyValue value) {
        return default_value_;
    }

    @RubyLevelMethod(name="default=")
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

    @RubyLevelMethod(name="size", alias="length")
    public RubyFixnum size() {
        return ObjectFactory.createFixnum(map_.size());
    }

    @RubyLevelMethod(name="to_s")
    public RubyString to_s() {
        RubyString r = ObjectFactory.createString();

        for (RubyValue key : map_.keySet()) {
            RubyValue value = map_.get(key);
            r.appendString(key.toString() + value.toString());
        }

        return r;
    }

    private static RubyID defaultID = RubyID.intern("default");

    public RubyValue get(RubyValue k) {
        RubyValue v = map_.get(k);
        if (null != v) {
            return v;
        } else if (null != default_value_as_block_) {
            return default_value_as_block_.invoke(this, new RubyArray(this, k));
        } else {
            return RubyAPI.callOneArgMethod(this, k, null, defaultID);
        }
    }

    @RubyLevelMethod(name="has_key?", alias={"include?", "key?", "member?"})
    public RubyValue has_key(RubyValue key) {
        return ObjectFactory.createBoolean(map_.containsKey(key));
    }

    @RubyLevelMethod(name="delete")
    public RubyValue delete(RubyValue k) {
        RubyValue v = map_.remove(k);
        return (null != v) ? v : RubyAPI.callOneArgMethod(this, k, null, defaultID);
    }

    @RubyLevelMethod(name="delete_if")
    public RubyValue delete_if(RubyBlock block) {
        Iterator<RubyValue> ite = map_.keySet().iterator();
        while (ite.hasNext()) {
            RubyValue key = ite.next();
            RubyValue value = map_.get(key);
            RubyValue r = block.invoke(this, key, value);
            if (r.isTrue()) {
                ite.remove();
            }
        }

        return this;
    }

    @RubyLevelMethod(name="reject")
    public RubyValue reject(RubyBlock block) {
        RubyHash dup = clone();
        dup.delete_if(block);
        return dup;
    }

    @RubyLevelMethod(name="has_value?")
    public RubyValue has_value(RubyValue value) {
        return ObjectFactory.createBoolean(map_.containsValue(value));
    }

    @RubyLevelMethod(name="values_at")
    public RubyArray values_at() {
        return new RubyArray();
    }

    @RubyLevelMethod(name="values_at")
    public RubyArray values_at(RubyValue arg) {
        RubyArray a = new RubyArray();
        RubyValue v = map_.get(arg);
        a.add((null != v) ? v : RubyConstant.QNIL);
        return a;
    }

    @RubyLevelMethod(name="values_at")
    public RubyArray values_at(RubyArray keys) {
        RubyArray a = new RubyArray();

        for (RubyValue key : keys) {
            RubyValue v = map_.get(key);
            a.add((null != v) ? v : RubyConstant.QNIL);
        }

        return a;
    }

    @RubyLevelMethod(name="==")
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

        for (RubyValue key : map_.keySet()) {
            RubyValue v1 = that.get(key);
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

    @RubyLevelMethod(name="keys")
    public RubyArray keys() {
        RubyArray a = new RubyArray();
        for (RubyValue key : map_.keySet()) {
            a.add(key);
        }
        return a;
    }

    @RubyLevelMethod(name="values")
    public RubyArray values() {
        RubyArray a = new RubyArray();
        for (RubyValue value : map_.values()) {
            a.add(value);
        }
        return a;
    }

    @RubyLevelMethod(name="shift")
    public RubyValue shift() {
        if (map_.isEmpty()) {
            return default_value_;
        }

        RubyValue k = map_.keySet().iterator().next();
        RubyValue v = map_.remove(k);

        RubyArray a = new RubyArray(2);
        a.add(k);
        a.add(v);
        return a;
    }

    //TODO this should be implmented in ruby
    @RubyLevelMethod(name="fetch")
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

    @RubyLevelMethod(name="initialize")
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
    @RubyLevelMethod(name="[]")
    public RubyValue getValue(RubyValue arg) {
        return this.get(arg);
    }

    @RubyLevelMethod(name="[]=")
    public RubyValue setValue(RubyValue arg0, RubyValue arg1) {
        this.add(arg0, arg1);
        return arg1;
    }

    @RubyAllocMethod
    public static RubyValue alloc(RubyValue receiver) {
        RubyHash h = ObjectFactory.createHash();
        h.setRubyClass((RubyClass) receiver);
        return h;
    }
}
