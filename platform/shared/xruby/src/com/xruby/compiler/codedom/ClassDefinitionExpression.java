/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class ClassDefinitionExpression extends Expression {
    private Expression scope_exp_;//e.g. class A::B
	private String className_;
	private Expression exp_;//e.g. class << self
	private Expression superClass_;
	private BodyStatement bodyStatement_;
	
	public ClassDefinitionExpression(Expression scope, String className, Expression superClass, BodyStatement bodyStatement) {
        scope_exp_ = scope;
        className_ = className;
		exp_ = null;
		superClass_ = superClass;
		bodyStatement_ = bodyStatement;
	}

	public ClassDefinitionExpression(Expression exp, Expression superClass, BodyStatement bodyStatement) {
		scope_exp_ = null;
        className_ = null;
		exp_ = exp;
		superClass_ = superClass;
		bodyStatement_ = bodyStatement;
	}

	public void accept(CodeVisitor visitor) {
		if (null != className_) {
            if (null != scope_exp_) {
                scope_exp_.accept(visitor);
            }
			visitor.visitClassDefinition1(className_, null != scope_exp_);
			if (null != superClass_) {
				superClass_.accept(visitor);
            } else {
				visitor.visitNoSuperClass();
			}
			visitor.visitClassDefinition2(className_, null != scope_exp_);
		} else {
			exp_.accept(visitor);
			visitor.visitSingletonClassDefinition();
		}
		
		if (null != bodyStatement_) {
			bodyStatement_.accept(visitor);
		}
		
		visitor.visitClassDefinitionEnd((null == bodyStatement_) ? false : bodyStatement_.lastStatementHasReturnValue());
	}

}
