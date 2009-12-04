/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;


import java.util.ArrayList;

public class AtExitBlocks {
    private static ArrayList<RubyBlock> registgered_blocks_ = new ArrayList<RubyBlock>();

    public static void registerBlock(RubyBlock block) {
        registgered_blocks_.add(0, block);
    }

    static void invokeAll() {
        for (RubyBlock block : registgered_blocks_) {
            block.invoke(RubyRuntime.TOP_LEVEL_SELF_VALUE);
        }
    }
}
