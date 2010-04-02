/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyFloat;
import com.xruby.runtime.builtin.RubyInteger;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.builtin.RubyTime;

import j2me.util.HashMap;
import j2me.util.Map;

import j2me.lang.Cloneable;
//import j2me.lang.CloneNotSupportedException;

/*
'return' inside a block will cause return in the method, for example:
    def f
        1.times {return 1}
    end
Or worse (compile-time undetectable):
    def f block; block.call; print 'yyy'; end
    def test_return2; a = Proc.new {return}; f a; end
    test_return2

so we need to check if this happend after each function call.

As java does not support multiple return value, we made RubyValue
subclass of BlockCallStatus. 	Should be replaced with a 'Context' object
in the future.
*/
abstract class BlockCallStatus {
    private boolean returned_in_block_ = false;
    private boolean breaked_in_block_ = false;
    private boolean do_not_allow_break_ = false;

    public void setReturnedInBlock(boolean returned, boolean break_or_returned, boolean do_not_allow_break) {
        returned_in_block_ = returned;
        breaked_in_block_ = break_or_returned && !returned;//just breaked
        do_not_allow_break_ = do_not_allow_break;
    }

    public boolean returnedInBlock() {
        if (do_not_allow_break_ && breaked_in_block_) {
            //TODO should check context!
            throw new RubyException(RubyRuntime.LocalJumpErrorClass, "break from proc-closure");
        }
        return returned_in_block_;
    }

    protected void doClone(BlockCallStatus orig){
    	setReturnedInBlock(this.returned_in_block_, this.breaked_in_block_, this.do_not_allow_break_);
    }
}

public abstract class RubyValue extends BlockCallStatus implements Cloneable {
    private boolean frozen_ = false;
    private boolean taint_ = false;
    private static Map/*<RubyValue, Map<RubyID, RubyValue>>*/ genericIvTbl;

    public abstract void setRubyClass(RubyClass klass);
    public abstract RubyClass getRubyClass();
    
    public RubyValue clone()
    { 
    	RubyValue cl = null;
    	try{
    		cl = (RubyValue)(this.getClass().newInstance());
    		cl.doClone(this);
    	}catch(Exception e){
    		throw new RubyException(RubyRuntime.ExceptionClass, e.toString());    		
    	}
    	return cl; 
    }
    protected void doClone(RubyValue orig){
        frozen_ = orig.frozen_;
        taint_ = orig.taint_;
        super.doClone(orig);
    }
    public RubyValue rhom_processProperty(RubyID mid)
    {
    	return null;
    }
    //RHO_COMMENT: clone
/*
    public RubyValue clone() {
        RubyValue v;
        try {
            v = (RubyValue) super.clone();
        } catch (CloneNotSupportedException e) {
            throw new RubyException(RubyRuntime.ExceptionClass, e.toString());
        }
        return v;
    }
*/
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        } else if (o instanceof RubyValue) {
            return RubyAPI.callPublicOneArgMethod(this, (RubyValue)o, null, RubyID.equalID).isTrue();
        } else {
            return false;
        }
    }

    public boolean frozen() {
        return frozen_;
    }

    public void freeze() {
        frozen_ = true;
    }

    public boolean tainted() {
        return taint_;
    }

    public void taint() {
        taint_ = true;
    }

    public void untaint() {
    	if ( tainted() )
    	{
    		if ( frozen() )
    		{
    			//TODO raise frozen exception
    		}
    		taint_ = false;
    	}
    }
    
    public RubyValue getInstanceVariable(RubyID id) {
        if (genericIvTbl != null) {
            Map/*<RubyID, RubyValue>*/ table = (Map)genericIvTbl.get(this);
            if (table != null) {
                RubyValue v = (RubyValue)table.get(id);
                if (v != null) {
                    return v;
                }
            }
        }

        return RubyConstant.QNIL;
    }

    public RubyValue setInstanceVariable(RubyValue value, RubyID id) {
        if (genericIvTbl == null) {
            genericIvTbl = new HashMap/*<RubyValue, Map<RubyID, RubyValue>>*/();
        }

        Map/*<RubyID, RubyValue>*/ table = (Map)genericIvTbl.get(this);
        if (table == null) {
            table = new HashMap/*<RubyID, RubyValue>*/();
            genericIvTbl.put(this, table);
        }

        table.put(id, value);
        return value;
    }

    public RubyClass getSingletonClass() {
        return getSingletonClass(null);
    }

    public RubyClass getSingletonClass(RubyModule scope) {
        RubyClass klass = this.getRubyClass();

        if (klass.isSingleton()
                && klass.getInstanceVariable(RubyID.attachedID) == this) {
            klass.setScope(scope);
            klass.setAccessPublic();
            return klass;
        } else {
            return new RubySingletonClass(this, this.getRubyClass(), scope);
        }
    }

    public boolean respondTo(RubyID id) {
        RubyClass klass = this.getRubyClass();
        if (klass.findMethod(RubyID.RESPOND_TO_P) == RubyRuntime.getRespondMethod()) {
            return klass.isMethodBound(id, false);
        } else {
            return RubyAPI.callOneArgMethod(this, id.toSymbol(), null, RubyID.RESPOND_TO_P).isTrue();
        }
    }

    public boolean isTrue() {
        return true;
    }

    public boolean isKindOf(RubyModule m) {
        return m.isKindOf(this.getRubyClass());
    }

    public String toString() {
        return getRubyClass().getName() + super.toString();
    }

    public String inspect() {
        return RubyAPI.callNoArgMethod(this, null, RubyID.toSID).toStr();
    }

    public RubyID toID() {
        throw new RubyException(RubyRuntime.TypeErrorClass, this.inspect() + " is not a symbol");
    }

    public int toInt() {
        return this.convertToInteger().toInt();
    }

    public long toLong() {
        return this.convertToInteger().toLong();
    }
    
    public RubyArray toAry() {
        return this.convertToArray().toAry();
    }

    public double toFloat() {
        return this.convertToFloat().toFloat();
    }

    public String toStr() {
        return this.convertToString().toStr();
    }

    public String asString() {
        RubyValue value = RubyAPI.callPublicNoArgMethod(this, null, RubyID.toSID);
        if (value instanceof RubyString) {
            return value.toStr();
        }

        return "#<" + this.getRubyClass().getName() + ":0x" + Integer.toHexString(this.hashCode()) + "x>";
    }

    public RubyInteger toRubyInteger() {
        return this.convertToInteger().toRubyInteger();
    }

    public RubyFloat toRubyFloat() {
        return this.convertToFloat().toRubyFloat();
    }

    public RubyString toRubyString() {
        return this.convertToString().toRubyString();
    }

    public RubyArray toRubyArray() {
        return toAry();
    }
    
    public RubyTime toRubyTime() {
    	return (RubyTime)this.convertToTime();
    }
    
    private RubyValue convertToInteger() {
        return convertToType(RubyRuntime.IntegerClass, RubyID.toIntID);
    }

    private RubyValue convertToArray() {
        return convertToType(RubyRuntime.ArrayClass, RubyID.toAryID);
    }

    private RubyValue convertToFloat() {
        return convertToType(RubyRuntime.FloatClass, RubyID.toFID);
    }

    private RubyValue convertToString() {
        return convertToType(RubyRuntime.StringClass, RubyID.toStrID);
    }
    
    private RubyValue convertToTime() {
    	return convertToType(RubyRuntime.TimeClass, RubyID.toTimeID);
    }

    private RubyValue convertToType(RubyClass klass, RubyID id) {
        if (this.isKindOf(klass)) {
            return this;
        }

        RubyClass theClass = this.getRubyClass();
        if (!this.respondTo(id)) {
            throw new RubyException("can't convert " + this.getRubyClass().getName() + " into " + klass.getName());
        }

        RubyValue v = RubyAPI.callNoArgMethod(this, null, id);

        if (!v.isKindOf(klass)) {
            throw new RubyException(this.getRubyClass().getName() + "#" + id.toString() + " should return " + klass.getName());
        }

        return v;
    }

    public RubyMethod findPublicMethod(RubyID mid) {
        return getRubyClass().findOwnPublicMethod(mid);
    }

    public RubyMethod findMethod(RubyID mid) {
        return getRubyClass().findOwnMethod(mid);
    }
}

