/**
 * Copyright 2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class BEGINBlock extends Statement {
    private CompoundStatement cs_;

    public BEGINBlock(CompoundStatement cs) {
        cs_ = cs;
    }

    public void accept(CodeVisitor visitor) {
        if (null != cs_) {
            cs_.accept(visitor);
        }
    }

}
