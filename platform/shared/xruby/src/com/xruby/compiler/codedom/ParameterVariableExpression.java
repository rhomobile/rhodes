/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.ArrayList;

public abstract class ParameterVariableExpression extends VariableExpression {
	abstract public void acceptAsInitializeToNil(CodeVisitor visitor);
	abstract void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result);
}
