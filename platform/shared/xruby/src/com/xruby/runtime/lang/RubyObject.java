/** 
 * Copyright 2005-2007 Xue Yong Zhi, Yu Zhang
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import com.xruby.runtime.lang.annotation.RubyAllocMethod;
import com.xruby.runtime.lang.annotation.RubyLevelClass;

@RubyLevelClass(name="Object", superclass="", modules="Kernel")
public class RubyObject extends RubyBasic {
	private Map<RubyID, RubyValue> instance_varibles_ = null;
	
	public RubyObject(RubyClass c) {
		super(c);
	}
	
	@RubyAllocMethod
	public static RubyValue alloc(RubyValue receiver) {
		return new RubyObject((RubyClass)receiver);
	}

	public RubyValue getInstanceVariable(RubyID id) {
		if (this.instance_varibles_ != null) {
			RubyValue v = instance_varibles_.get(id);
			if (v != null) {
				return v;
			}
		}
		
        return RubyConstant.QNIL;
	}

	public RubyValue setInstanceVariable(RubyValue value, RubyID id) {
		if (null == instance_varibles_) {
            instance_varibles_ = new HashMap<RubyID, RubyValue>();
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
        Map<RubyID, RubyValue> vars = this.instance_varibles_;

        if (vars != null) {
            for (Iterator iter = vars.keySet().iterator(); iter.hasNext();) {
                RubyID id = (RubyID)iter.next();
                sb.append(sep);
                sb.append(" ");
                sb.append(id.toString());
                sb.append("=");
                RubyValue inspectValue = RubyAPI.callPublicNoArgMethod(vars.get(id), null, RubyID.inspectID);
                sb.append(inspectValue.toStr());
                sep = ",";
            }
        }
        sb.append(">");

        return sb.toString();
	}
}
