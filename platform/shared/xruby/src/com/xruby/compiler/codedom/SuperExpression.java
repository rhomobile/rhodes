/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class SuperExpression extends Expression {

    private MethodCallArguments arguments_;
    private Block block_;

    public SuperExpression(MethodCallArguments arguments, Block block) {
        arguments_ = arguments;
        block_ = block;
    }

    public void accept(CodeVisitor visitor) {
        if (null == arguments_) {
            acceptSuperWithImplicitParameter(visitor);
        } else {
            acceptSuperWithExplicitParameter(visitor);
        }
    }

    private void acceptSuperWithImplicitParameter(CodeVisitor visitor) {
        visitor.visitSuperBegin();
        visitor.visitNoParameterForSuper();
        acceptBlock(visitor);
        visitor.visitImplicitSuperEnd();
    }

    private void acceptSuperWithExplicitParameter(CodeVisitor visitor) {
        visitor.visitSuperBegin();

        final int argc = (null == arguments_.getAsteriskArgument()) ? arguments_.size() : -1;

        if (0 == argc) {
            //do nothing
        } else if (1 == argc) {
            arguments_.getFirstExpression().accept(visitor);
        } else if (2 == argc) {
			arguments_.getFirstExpression().accept(visitor);
			arguments_.getSecondExpression().accept(visitor);
        } else {
            arguments_.accept(visitor);
        }

        acceptBlock(visitor);

        visitor.visitExplicitSuperEnd(argc);
    }

    private void acceptBlock(CodeVisitor visitor) {
        if (null != block_) {
            block_.accept(visitor);
        } else if (null != arguments_ && null != arguments_.getBlockArgument()) {
            arguments_.getBlockArgument().accept(visitor);
            visitor.visitBlockArgument();
        } else {
            visitor.visitNoBlock(true);
        }
    }

}
