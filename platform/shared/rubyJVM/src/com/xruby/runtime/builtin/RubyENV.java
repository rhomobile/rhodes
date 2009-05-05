package com.xruby.runtime.builtin;

import j2me.util.Iterator;
import j2me.util.Map;
import j2me.lang.SystemMe;

import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyValue;

////@RubyLevelObject(name="ENV", objectClass="Object")
public class RubyENV {
	////@RubyLevelMethod(name="__get_os_ev__", singleton=true)
	public static RubyValue getOSEv(RubyValue receiver, RubyValue arg) {
        RubyString s = arg.toRubyString();
        String v = SystemMe.getenv(s.toString());
        if ( null == v )
        	return RubyConstant.QNIL;
        return ObjectFactory.createString(v);
//        return (null == v) ? RubyConstant.QNIL : ObjectFactory.createString(v);
    }
	
	////@RubyLevelMethod(name="__os_ev_index__", singleton=true)
	public static RubyValue osEvIndex(RubyValue receiver, RubyValue arg) {
        String s = RubyTypesUtil.convertToJavaString(arg);
        Map/*<String, String>*/ map = SystemMe.getenv();

        if (map.containsValue(s)) {
//            for (String k : map.keySet()) {
            for (Iterator iter = map.keySet().iterator(); iter.hasNext();) {
            	String k = (String)iter.next();
                String v = (String)map.get(k);
                if (s.equals(v)) {
                    return ObjectFactory.createString(k);
                }
            }
        }

        return RubyConstant.QNIL;
    }
}

