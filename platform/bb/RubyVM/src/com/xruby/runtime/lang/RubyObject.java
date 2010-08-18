/** 
 * Copyright 2005-2007 Xue Yong Zhi, Yu Zhang
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import j2me.util.HashMap;
import j2me.util.Iterator;
import j2me.util.Map;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyHash;
//import com.xruby.runtime.lang.annotation.RubyAllocMethod;
//import com.xruby.runtime.lang.annotation.RubyLevelClass;

//@RubyLevelClass(name="Object", superclass="", modules="Kernel")
public class RubyObject extends RubyBasic {
	protected Map/*<RubyID, RubyValue>*/ instance_varibles_ = null;
	protected RubyHash m_rhomProps;
	
	public RubyObject(RubyClass c) {
		super(c);
	}
	
    protected void doClone(RubyObject orig)
    {
    	instance_varibles_ = orig.instance_varibles_;
    	super.doClone(orig);
    }
	
	//@RubyAllocMethod
	public static RubyValue alloc(RubyValue receiver) {
		return new RubyObject((RubyClass)receiver);
	}

	public RubyValue getInstanceVariable(RubyID id) {
		if (this.instance_varibles_ != null) {
			RubyValue v = (RubyValue)instance_varibles_.get(id);
			if (v != null) {
				return v;
			}
		}
		
        return RubyConstant.QNIL;
	}

	public RubyValue setInstanceVariable(RubyValue value, RubyID id) {
		if (null == instance_varibles_) {
            instance_varibles_ = new HashMap/*<RubyID, RubyValue>*/();
        }

        instance_varibles_.put(id, value);
        return value;
	}
	
	public String inspect() {
		StringBuffer sb = new StringBuffer();
        sb.append("#<");
        sb.append(this.getRubyClass().getRealClass().getName());
        sb.append(":0x");
        sb.append(Integer.toHexString(this.hashCode()));

        String sep = "";
        Map/*<RubyID, RubyValue>*/ vars = this.instance_varibles_;

        if (vars != null) {
            for (Iterator iter = vars.keySet().iterator(); iter.hasNext();) {
                RubyID id = (RubyID)iter.next();
                sb.append(sep);
                sb.append(" ");
                sb.append(id.toString());
                sb.append("=");
                RubyValue inspectValue = RubyAPI.callPublicNoArgMethod((RubyValue)vars.get(id), null, RubyID.inspectID);
                sb.append(inspectValue.toStr());
                sep = ",";
            }
        }
        sb.append(">");

        return sb.toString();
	}
	
	//RHO_COMMENT
    //@RubyLevelMethod(name="instance_variable_set")
    public RubyValue instance_variable_set(RubyValue arg1, RubyValue arg2){
		RubyID mid = RubyID.intern(arg1.toStr());
		return setInstanceVariable(arg2, mid);
    }

    //@RubyLevelMethod(name="instance_variable_get")
    public RubyValue instance_variable_get(RubyValue arg1){
		RubyID mid = RubyID.intern(arg1.toStr());
		return getInstanceVariable(mid);
    }

    //@RubyLevelMethod(name="instance_variable_defined?")
    public RubyValue instance_variable_defined(RubyValue arg1){
		RubyID mid = RubyID.intern(arg1.toStr());
		if ( getInstanceVariable(mid) != RubyConstant.QNIL )
			return RubyConstant.QTRUE;
		
		return RubyConstant.QFALSE;
    }

    //@RubyLevelMethod(name="remove_instance_variable")
    public RubyValue instance_variable_remove(RubyValue arg1){
		RubyID mid = RubyID.intern(arg1.toStr());
		
		if (this.instance_varibles_ != null) {
			RubyValue v = (RubyValue)(instance_varibles_.remove(mid));
			if (v != null) {
				return v;
			}
		}
		
		throw new RubyException(RubyRuntime.NameErrorClass, "instance variable '" + mid.toString() + "' not defined");
    }

    //@RubyLevelMethod(name="instance_variables")
    public RubyValue instance_variables(){
    	RubyArray ar = new RubyArray();
    	
        for (Iterator iter = instance_varibles_.keySet().iterator(); iter.hasNext();) {
        	RubyID value = (RubyID)iter.next();
            ar.add( ObjectFactory.createString(value.toString()) );
        }
    	
    	return ar;
    }

    //@RubyLevelMethod(name="rhom_init")
    public RubyValue rhom_init(RubyValue arg1){
    	m_rhomProps = (RubyHash)arg1;
    	return RubyConstant.QNIL;
    }
    
    public RubyValue rhom_processProperty(RubyID mid)
    {
    	if ( m_rhomProps == null )
    		return null;
    	
    	return m_rhomProps.getRaw(mid.toSymbol());
    }
}
