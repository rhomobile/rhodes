/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import j2me.util.Iterator;
import j2me.util.WeakHashMap;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyFixnum;
//import com.xruby.runtime.builtin.RubyProc;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;
//import com.xruby.runtime.lang.annotation.RubyLevelModule;

//we are using map as list here.
//@RubyLevelModule(name="ObjectSpace")
public class ObjectSpace {
    private static WeakHashMap/*<RubyValue, RubyValue>*/ map_= new WeakHashMap/*<RubyValue, RubyValue>*/();

    public static void add(RubyValue v) {
        map_.put(v, null);
    }
    
    //@RubyLevelMethod(name="each_object", singleton=true)
    public static RubyFixnum rubyEachObject(RubyValue receiver, RubyBlock block) {
    	int n = ObjectSpace.each_object(receiver, block);
        return ObjectFactory.createFixnum(n);
    }
    
    //@RubyLevelMethod(name="each_object", singleton=true)
    public static RubyFixnum rubyEachObject(RubyValue receiver, RubyValue arg, RubyBlock block) {
    	int n = ObjectSpace.each_object(receiver, (RubyModule)arg, block);
        return ObjectFactory.createFixnum(n);
    }
    
    private static int each_object(RubyValue receiver, RubyBlock block) {
        int i = 0;
//        for (RubyValue v : map_.keySet()) {
        for (Iterator iter = map_.keySet().iterator(); iter.hasNext();) {
        	RubyValue v = (RubyValue)iter.next();
        
            block.invoke(receiver, v);
            ++i;
        }
        return i;
    }

    private static int each_object(RubyValue receiver, RubyModule m, RubyBlock block) {
        int i = 0;
//        for (RubyValue v : map_.keySet()) {
        for (Iterator iter = map_.keySet().iterator(); iter.hasNext();) {
        	RubyValue v = (RubyValue)iter.next();
        
            if (RubyAPI.isKindOf(m, v)) {
                block.invoke(receiver, v);
                ++i;
            }
        }
        return i;
    }
    
    
}
