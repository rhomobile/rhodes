/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.*;

public class HashExpression extends Expression {
	
	class ExpressionPair implements Visitable {
		private Expression key;
		private Expression value;

		ExpressionPair(Expression k, Expression v) {
			key = k;
			value = v;
		}

		public void accept(CodeVisitor visitor) {
			key.accept(visitor);
			value.accept(visitor);
			visitor.visitHashElement();
		}
	}
	
	private ArrayList<ExpressionPair> elements_ = new ArrayList<ExpressionPair>();
	private Expression last_exp_ = null;

	//Hash may look like "{1, 2, 2, 4, 3, 6}", it should be converted to "{1=>2, 2=>4, 3 => 6}"
	public void addElement(Expression k, Expression v) {
		if (null != v) {
			elements_.add(new ExpressionPair(k, v));
		} else {
			if (null == last_exp_) {
				last_exp_ = k;
			} else {
				elements_.add(new ExpressionPair(last_exp_, k));
				last_exp_ = null;
			}
		}
		//TODO throw exception if "odd number list for Hash"
	}

	public void accept(CodeVisitor visitor) {
		visitor.visitHashBegin();
		
		int i = 0;
		for (ExpressionPair e : elements_) {
			e.accept(visitor);
			++i;
		}
		
		visitor.visitHashEnd();
	}
}
