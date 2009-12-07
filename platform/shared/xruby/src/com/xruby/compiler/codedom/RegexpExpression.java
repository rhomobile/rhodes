/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class RegexpExpression extends StringExpression {

    private String option_;

    public RegexpExpression(String value) {
        super("", false);

        // A regexp "/abc/o" will look like "abc/o" now
        int index_of_back_slash = value.lastIndexOf("/");
        assert(index_of_back_slash >= 0);
        appendRegExString(value.substring(0, index_of_back_slash));
        option_ = value.substring(index_of_back_slash + 1);
    }

    public void accept(CodeVisitor visitor) {
        visitor.visitRegexpExpression(getValue(), option_);
    }
}
