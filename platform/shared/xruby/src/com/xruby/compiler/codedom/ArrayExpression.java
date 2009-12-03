/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.*;

public class ArrayExpression extends Expression {
	private ArrayList<Expression> elements_;
	private Expression asterisk_element_;

	int rhs_size_;
	boolean has_single_asterisk_;
	
	public ArrayExpression() {
		elements_ = new ArrayList<Expression>();
		asterisk_element_ = null;

		rhs_size_ = 0;
		has_single_asterisk_ = false;
	}

	ArrayExpression(ArrayList<Expression> elements, Expression asterisk_element) {
		elements_ = elements;
		asterisk_element_ = asterisk_element;

		rhs_size_ = elements.size();
		has_single_asterisk_ = (null != asterisk_element);
	}
	
	public void addElement(Expression e) {
		elements_.add(e);
	}

	public void setAsteriskElement(Expression e) {
		asterisk_element_ = e;
	}

	public ArrayList<Expression> getElements() {
		return elements_;
	}
	
	public void accept(CodeVisitor visitor) {
		visitor.visitArrayBegin(elements_.size(), rhs_size_, has_single_asterisk_);
		
		for (Expression e : elements_) {
			e.accept(visitor);
			visitor.visitArrayElement(false, e instanceof MethodCallExpression || e instanceof YieldExpression);
		}

		if (null != asterisk_element_) {
			asterisk_element_.accept(visitor);
			visitor.visitArrayElement(true, asterisk_element_ instanceof MethodCallExpression || asterisk_element_ instanceof YieldExpression);
		}
		
		visitor.visitArrayEnd();
	}
	
}
