/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class ModuleDefinitionExpression extends Expression {
    private Expression scope_;
    private String moduleName_;
    private BodyStatement bodyStatement_;

    public ModuleDefinitionExpression(Expression exp, String moduleName, BodyStatement bodyStatement) {
        scope_ = exp;
        moduleName_ = moduleName;
        bodyStatement_ = bodyStatement;
    }

    public void accept(CodeVisitor visitor) {
        if (null != scope_) {
            scope_.accept(visitor);
        }

        visitor.visitModuleDefinition(moduleName_, null != scope_);

        if (null != bodyStatement_) {
            bodyStatement_.accept(visitor);
        }

        visitor.visitModuleDefinitionEnd((null == bodyStatement_) ? false : bodyStatement_.lastStatementHasReturnValue());
    }

}
