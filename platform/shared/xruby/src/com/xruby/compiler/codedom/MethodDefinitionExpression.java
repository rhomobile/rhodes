/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import java.util.*;

public class MethodDefinitionExpression extends Expression {
    private String methodName_;
	private Expression method_owner_;
	private BodyStatement bodyStatement_ = null;
	private ArrayList<String> parameters_ = new ArrayList<String>();
	private String asterisk_parameter_ = null;
	private String block_parameter_ = null;
	private ArrayList<Expression> default_parameters_ = new ArrayList<Expression>();

	public MethodDefinitionExpression(String methodName) {
		methodName_ = methodName;
		method_owner_ = null;
	}

	public MethodDefinitionExpression(String methodName, Expression method_owner) {
		methodName_ = methodName;
		method_owner_ = method_owner;
	}

	public void setBody(BodyStatement bodyStatement) {
		bodyStatement_ = bodyStatement;
    }

	public void addParameter(String name, Expression default_value) {
		parameters_.add(name);
		if (null != default_value) {
			default_parameters_.add(default_value);
		}
	}

	public void setAsteriskParameter(String name) {
		if (null == name) {
			asterisk_parameter_ = "$unused";
		} else {
			asterisk_parameter_ = name;
		}
	}

	public void setBlockParameter(String name) {
		assert(null == block_parameter_);
		block_parameter_ = name;
	}

	public void accept(CodeVisitor visitor) {
		if (null != method_owner_) {
			method_owner_.accept(visitor);
		}

		String uniqueMethodName = visitor.visitMethodDefinition(methodName_,
						parameters_.size(),
						(null != asterisk_parameter_),
						default_parameters_.size(),
						(null != method_owner_));

		for (String p : parameters_) {
			visitor.visitMethodDefinitionParameter(p);
		}

		if (null != asterisk_parameter_) {
			visitor.visitMethodDefinitionAsteriskParameter(asterisk_parameter_, parameters_.size());
		}

		if (null != block_parameter_) {
			visitor.visitMethodDefinitionBlockParameter(block_parameter_);
		}

		int i = parameters_.size() - default_parameters_.size();
		if (!default_parameters_.isEmpty()) {
			visitor.visitMethodDefinitionDefaultParameters(default_parameters_.size());

			for (Expression e : default_parameters_) {
				Object next_label = visitor.visitMethodDefinitionDefaultParameterBegin(i);
				e.accept(visitor);
				visitor.visitMethodDefinitionDefaultParameterEnd(next_label);
				++i;
			}
		}

		if (null != bodyStatement_) {
			bodyStatement_.accept(visitor);
		}

		visitor.visitMethodDefinitionEnd((null != bodyStatement_) ?
										bodyStatement_.lastStatementHasReturnValue() : false);

        int firstLine = this.getPosition();
        int lastLine = firstLine;
        if(bodyStatement_ != null) {
            lastLine = bodyStatement_.getLastLine();
        }

        String scriptName = extractScriptName(uniqueMethodName);
        BlockFarm.markMethod(scriptName, uniqueMethodName, new int[]{firstLine, lastLine});
    }

    // Extract script file's name, as same as the one in the Block.java
    // TODO: so, we do need a global variable  to keep current script name
    private static String extractScriptName(String uniqueMethodName) {
        if(uniqueMethodName.startsWith(com.xruby.compiler.codegen.NameFactory.name_classprefix_ + "/")) {
            uniqueMethodName = uniqueMethodName.substring(6);
        }
        StringTokenizer st = new StringTokenizer(uniqueMethodName, "/");

        if(st.hasMoreTokens()) {
            return st.nextToken() + ".rb";
        } else {
            return null;
        }
    }
}
