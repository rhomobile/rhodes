/** 
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codedom;

import antlr.RecognitionException;
import java.util.ArrayList;

public class MethodCallExpression extends Expression {

	private Expression receiver_; 
	private String methodName_;
	private Block block_;
	private MethodCallArguments arguments_;

	public MethodCallExpression(Expression receiver, String methodName, MethodCallArguments arguments, Block block) throws RecognitionException {
		if (null != block && null != arguments && null != arguments.getBlockArgument()) {
			throw new RecognitionException("both block arg and actual block given");
		}

		receiver_ = receiver;
		methodName_ = methodName;
		arguments_ = arguments;
		block_ = block;
	}

	boolean isElementSet() {
		return methodName_.equals("[]") || (null != receiver_);
	}
	
	Expression convertElementAccessToElementSet(Expression value) throws RecognitionException {
		MethodCallArguments  args;
		if (null != arguments_) {
			args = arguments_.clone();
		} else {
			args = new MethodCallArguments();
		}
		
		args.addArgument(value);
		MethodCallExpression r = new MethodCallExpression(receiver_, methodName_ + "=", args, null);
		return r;
	}

	Expression getReceiver() {
		return receiver_;
	}

	public final String getName() {
		return methodName_;
	}

	public MethodCallArguments getArguments() {
		return arguments_;
	}

	public Block getBlock() {
		return block_;
	}

	//Optimization for "lambda {...}.call"
	//TODO, handle "x = lambda {...}; x.call"
	private boolean isLambdaCall() {
		if (null == receiver_) {
			return false;
		}

		if (!methodName_.equals("call")) {
			return false;
		}

		if (!(receiver_ instanceof MethodCallExpression)) {
			return false;
		}

		MethodCallExpression m = (MethodCallExpression)receiver_;
		if (null == m.block_) {
			return false;
		}

		return m.methodName_.equals("lambda") || m.methodName_.equals("proc");
	}

	public void acceptMultipleArrayAssign(CodeVisitor visitor) {
		arguments_.getFirstExpression().accept(visitor);
		receiver_.accept(visitor);
		visitor.visitMultipleArrayAssign();
	}

	public void acceptAsLambdaCall(CodeVisitor visitor) {
		MethodCallExpression m = (MethodCallExpression)receiver_;
		m.block_.accept(visitor);

		visitor.visitSpecialLambdaCallBegin();

		if (null == arguments_) {
			visitor.visitNoParameter();
		} else {
			arguments_.accept(visitor);
		}

		visitor.visitSpecialLambdaCallEnd();
	}
	
	public void accept(CodeVisitor visitor) {
		if (isLambdaCall()) {
			acceptAsLambdaCall(visitor);
			return;
		}
		
		boolean is_eval = false;
		if (null == receiver_ && (methodName_.equals("eval") || methodName_.equals("binding"))) {
			is_eval = true;
		}
		
		visitor.visitMethodCallBegin();
		
		if (null != receiver_) {
			receiver_.accept(visitor);
			if (methodName_.equals("call")) {
				//maybe this is Proc#call, need to setup context information
				visitor.visitPotentialProcCall();
			}
		} else {
			visitor.visitSelfExpression();
		}
		
		int argc = is_eval ? -1 : getArgc();

		if (null == arguments_) {
			if (is_eval) {
				visitor.visitBinding(true);
				argc = 1;
			}
		} else {
			switch (argc) {
			case 0:
				break;
			case 1:
				arguments_.getFirstExpression().accept(visitor);
				break;
			case 2:
				arguments_.getFirstExpression().accept(visitor);
				arguments_.getSecondExpression().accept(visitor);
				break;
			default:
				arguments_.accept(visitor);
				if (is_eval && arguments_.size() <= 1) {
					visitor.visitBinding(false);
				}
				break;
			}
		}
			
		String name = null;
		if (null != block_) {
			name = block_.accept(visitor);
		} else if (null != arguments_ && null != arguments_.getBlockArgument()) {
			arguments_.getBlockArgument().accept(visitor);
			visitor.visitBlockArgument();
		} else {
			//TODO Give block_given?/iterator? a special treatment
			//This is a hack, and break the alias to those functions. In the future we need to add
			//another parameter to RubyMethod#run()
			boolean is_block_given_call = false;
			if (null == receiver_ && 
				(methodName_.equals("block_given?") || methodName_.equals("iterator?"))) {
				is_block_given_call = true;
			}
			
			visitor.visitNoBlock(is_block_given_call);
		}

		visitor.visitMethodCallEnd(methodName_,
							(null != receiver_),
							name,
							argc);
	}
	
	private int getArgc() {
		if (arguments_ == null) {
			return 0;
		}
		
		if (arguments_.getAsteriskArgument() == null) {
			return arguments_.size();
		}
		
		return -1;
	}
	
	public void getFrequentlyUsedIntegers(ArrayList<Integer> result) {
		if (null != receiver_) {
			receiver_.getFrequentlyUsedIntegers(result);
		}
		if (null != arguments_) {
			arguments_.getFrequentlyUsedIntegers(result);
		}
	}

    void pullBlock(ArrayList<Block> result) {
        if (null != block_) {
            result.add(block_);
        }
    }

    void getNewlyAssignedVariables(ISymbolTable symboltable, ArrayList<String> result) {
        if (null != receiver_) {
            receiver_.getNewlyAssignedVariables(symboltable, result);
        }
        if (null != arguments_) {
			arguments_.getNewlyAssignedVariables(symboltable, result);
		}
    }

}
