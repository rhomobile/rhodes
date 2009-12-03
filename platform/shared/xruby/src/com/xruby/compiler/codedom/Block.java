/**
 * Copyright 2005-2007 Xue Yong Zhi, Yu Su, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.*;

public class Block {

    private CompoundStatement bodyStatement_ = null;
    private ArrayList<ParameterVariableExpression> parameters_ = new ArrayList<ParameterVariableExpression>();
    private ParameterVariableExpression asterisk_parameter_ = null;
    private boolean asterisk = false;
    private ArrayList<Expression> default_parameters_ = new ArrayList<Expression>();
    private boolean should_validate_argument_length_ = false;
    private boolean is_for_in_expression_ = false;
    private boolean has_extra_comma_ = false;

    private int saved_as_pulled_ = -1;
    private String name_ = null;
    private int startPosition;
    private int endPosition;

    public void setHasExtraComma() {
        has_extra_comma_ = true;
    }

    public void initAllParametersToNil(CodeVisitor visitor) {
        for (ParameterVariableExpression var : parameters_) {
            var.acceptAsInitializeToNil(visitor);
        }

        if (null != asterisk_parameter_) {
            asterisk_parameter_.acceptAsInitializeToNil(visitor);
        }
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        for (ParameterVariableExpression var : parameters_) {
            if (var instanceof LocalVariableExpression) {
    			String name = ((LocalVariableExpression)var).getValue();
    			if (!symboltable.isDefinedInCurrentScope(name)) {
    				result.add(name);
    			}
    		}
        }

        if (null != asterisk_parameter_) {
            if (asterisk_parameter_ instanceof LocalVariableExpression) {
    			String name = ((LocalVariableExpression)asterisk_parameter_).getValue();
    			if (!symboltable.isDefinedInCurrentScope(name)) {
    				result.add(name);
    			}
    		}
        }
    }

    public void setBody(CompoundStatement bodyStatement) {
        bodyStatement_ = bodyStatement;
    }

    public void addParameter(ParameterVariableExpression name, Expression default_value) {
        parameters_.add(name);
        if (null != default_value) {
            default_parameters_.add(default_value);
        }
    }

    void setIsForInExpression() {
        is_for_in_expression_ = true;
    }

    //{||} -> true
    //{|x,...|} -> true
    //{} -> false
    public void setShouldValidateArgumentLength() {
        should_validate_argument_length_ = true;
    }

    public void setAsteriskParameter(ParameterVariableExpression name) {
        assert(null == asterisk_parameter_);
        asterisk_parameter_ = name;
        asterisk = true;
    }

    public void setAsterisk() {
        this.asterisk = true;
    }

    private ArrayList<Expression> buildLhs() {
        ArrayList<Expression> mlhs = new ArrayList<Expression>();
        for (ParameterVariableExpression var : parameters_) {
            mlhs.add(var);
        }
        return mlhs;
    }

    public void acceptAsPulled(CodeVisitor visitor) {
        if (null != name_) {
            //has been pulled
            return;
        }
        StringBuilder name = new StringBuilder();
        saved_as_pulled_ = visitor.visitBlockBegin(name, true);
        name_ = name.toString();
    }

    public String accept(CodeVisitor visitor) {
        // f{|x, |} = f{|x, *|}
        if (this.asterisk && this.asterisk_parameter_ == null) {
            this.has_extra_comma_ = true;
        }

        if (null == name_) {
            //not pulled
            StringBuilder name = new StringBuilder();
            saved_as_pulled_ = visitor.visitBlockBegin(name, false);
            name_ = name.toString();
        }

        boolean has_body = null != bodyStatement_;
		visitor.visitBlockBodyBegin(name_, getArgc(),
                asterisk,
                default_parameters_.size(),
                is_for_in_expression_,
                has_extra_comma_,
                has_body);

        if (has_body) {
            boolean is_multi_rhs = (parameters_.size() > 1) || asterisk || is_for_in_expression_ || has_extra_comma_;
            MultipleAssignmentStatement.acceptMLhs(visitor, buildLhs(), asterisk_parameter_, has_extra_comma_, is_multi_rhs);
            visitor.visitTerminal();
            //	TODO support default_value
            visitor.visitBlockBody();
            bodyStatement_.accept(visitor);
            setEndPosition(bodyStatement_.getLastLine());
        } else {
            setEndPosition(startPosition);
        }

        visitor.visitBlockBodyEnd(name_,
            (has_body) ? bodyStatement_.lastStatementHasReturnValue() : false,
            saved_as_pulled_);
        if (name_ != null) {
            BlockFarm.markBlock(this);
        }

        return name_;
    }

	private int getArgc() {	
		return should_validate_argument_length_ ? parameters_.size() : -1;
	}


    public int getStartPosition() {
        return startPosition;
    }

    public void setStartPosition(int startPosition) {
        this.startPosition = startPosition;
    }

    public int getEndPosition() {
        return endPosition;
    }

    public String getName() {
        return name_;
    }

    public void setEndPosition(int endPosition) {
        this.endPosition = endPosition;
    }
}
