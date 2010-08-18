/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;


//import java.lang.reflect.Constructor;
import j2me.util.ArrayList;
import j2me.util.Iterator;

public class AtExitBlocks {
    private static ArrayList/*<RubyBlock>*/ registgered_blocks_ = new ArrayList/*<RubyBlock>*/();

    public static void registerBlock(RubyBlock block) {
        registgered_blocks_.add(0, block);
    }

    static void invokeAll() {
//        for (RubyBlock block : registgered_blocks_) {
        for (Iterator iter = registgered_blocks_.iterator(); iter.hasNext();) {
        	RubyBlock block = (RubyBlock)iter.next();
    	
            block.invoke(RubyRuntime.TOP_LEVEL_SELF_VALUE);
        }
    }
}
