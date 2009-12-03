package com.xruby.runtime.builtin;

import java.util.Map;

import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.annotation.RubyLevelMethod;
import com.xruby.runtime.lang.annotation.RubyLevelObject;

@RubyLevelObject(name="ENV", objectClass="Object")
public class RubyENV {
	@RubyLevelMethod(name="__get_os_ev__", singleton=true)
	public static RubyValue getOSEv(RubyValue receiver, RubyValue arg) {
        RubyString s = arg.toRubyString();
        String v = System.getenv(s.toString());
        return (null == v) ? RubyConstant.QNIL : ObjectFactory.createString(v);
    }
	
	@RubyLevelMethod(name="__os_ev_index__", singleton=true)
	public static RubyValue osEvIndex(RubyValue receiver, RubyValue arg) {
        String s = RubyTypesUtil.convertToJavaString(arg);
        Map<String, String> map = System.getenv();

        if (map.containsValue(s)) {
            for (String k : map.keySet()) {
                String v = map.get(k);
                if (s.equals(v)) {
                    return ObjectFactory.createString(k);
                }
            }
        }

        return RubyConstant.QNIL;
    }
}
