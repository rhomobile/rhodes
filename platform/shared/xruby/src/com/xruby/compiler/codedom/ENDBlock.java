/**
 * Copyright 2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class ENDBlock extends Statement {

    private CompoundStatement cs_;

    public ENDBlock(CompoundStatement cs) {
        cs_ = cs;
    }

    public void accept(CodeVisitor visitor) {
        if (null != cs_) {
            cs_.accept(visitor);
        }
    }

}
