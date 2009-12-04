/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

public class WArrayExpression extends Expression {
	private final String value_;

	public WArrayExpression(String value) {
		value_ = value.trim();
	}

	private ArrayExpression convert2ArrayExpression() {
		ArrayExpression a = new ArrayExpression();
		String[] tokens = value_.split("\\s");
        //RHO
        String item = "";
		for (String s : tokens) {
            if ( s.endsWith("\\") ){
                item += s.subSequence(0,s.length()-1) + " ";
                continue;
            }

            item += s;
			a.addElement(new StringExpression(item, false));
            item = "";
		}
        if ( item.length() > 0 )
		    a.addElement(new StringExpression(item, false));

		return a;
	}
	
	public void accept(CodeVisitor visitor) {
		ArrayExpression a = convert2ArrayExpression();
		a.accept(visitor);
	}
}
