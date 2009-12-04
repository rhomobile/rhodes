/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.math.BigInteger;
import java.util.ArrayList;
import antlr.RecognitionException;

public class IntegerExpression extends Expression {
	private BigInteger value_;
	private static final BigInteger FIXNUM_MAX = BigInteger.valueOf(Integer.MAX_VALUE);
	private static final BigInteger FIXNUM_MIN = BigInteger.valueOf(Integer.MIN_VALUE);

	public IntegerExpression(BigInteger value) {
		value_ = value;
	}
	
	public IntegerExpression(int value){
		value_ = BigInteger.valueOf(value);
	}

	public IntegerExpression(String value, int radix) throws RecognitionException {
		try {
			value_ = new BigInteger(value, radix);
		} catch (NumberFormatException e) {
			throw new RecognitionException(e.toString());
		}
	}

	public void accept(CodeVisitor visitor) {
		if (value_.compareTo(FIXNUM_MIN) >= 0 && value_.compareTo(FIXNUM_MAX) <= 0) {
			visitor.visitFixnumExpression(value_.intValue());
		} else {
			visitor.visitBignumExpression(value_);
		}
	}

	public void getFrequentlyUsedIntegers(ArrayList<Integer> result) {
		if (value_.compareTo(FIXNUM_MIN) >= 0 && value_.compareTo(FIXNUM_MAX) <= 0) {
			result.add(value_.intValue());
		}
	}
}
