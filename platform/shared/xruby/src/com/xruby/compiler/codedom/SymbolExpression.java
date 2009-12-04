/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class SymbolExpression extends Expression {
    private String value_;
    private StringExpressionWithExpressionSubstitution str_;//e.g. :"#{1}"

    public SymbolExpression(String value) {
        value_ = value;
    }

    public SymbolExpression(StringExpressionWithExpressionSubstitution str) {
        str_ = str;
    }

    //FIXME value_ may be null, and in that case str_ should be used.
    public String getValue() {
        return value_;
    }

    public void accept(CodeVisitor visitor) {
        if (null != value_) {
            visitor.visitSymbolExpression(value_);
        } else {
            str_.accept(visitor);
            visitor.visitSymbolExpression(null);
        }
    }
}
