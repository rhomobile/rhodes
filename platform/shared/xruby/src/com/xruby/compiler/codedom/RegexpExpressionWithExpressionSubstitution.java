/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class RegexpExpressionWithExpressionSubstitution extends ExpressionWithExpressionSubstitution {

    private String option_;

    public RegexpExpressionWithExpressionSubstitution(String s) {
        addString(s);
    }

    public void addLastString(String value) {
        //A regexp "/abc/o" will look like "abc/o" now
        int index_of_back_slash = value.lastIndexOf("/");
        assert(index_of_back_slash >= 0);
        addString(value.substring(0, index_of_back_slash));
        option_ = value.substring(index_of_back_slash + 1);
    }

    public void accept(CodeVisitor visitor) {

        visitor.visitStringExpressionWithExpressionSubstitutionBegin();

        for (Object o : stmts_) {
            if (o instanceof String) {
                visitor.visitStringExpressionWithExpressionSubstitution((String)o);
            } else if (o instanceof InstanceVariableExpression) {
                ((InstanceVariableExpression)o).accept(visitor);
                visitor.visitStringExpressionWithExpressionSubstitution();
            } else if (o instanceof ClassVariableExpression) {
                ((ClassVariableExpression)o).accept(visitor);
                visitor.visitStringExpressionWithExpressionSubstitution();
            } else if (o instanceof GlobalVariableExpression) {
                ((GlobalVariableExpression)o).accept(visitor);
                visitor.visitStringExpressionWithExpressionSubstitution();
            } else if (o instanceof CompoundStatement){
                ((CompoundStatement)o).accept(visitor);
                visitor.visitStringExpressionWithExpressionSubstitution();
            } else {
                assert(false);
            }
        }

        visitor.visitRegexpExpressionWithExpressionSubstitutionEnd(option_);
    }

}
