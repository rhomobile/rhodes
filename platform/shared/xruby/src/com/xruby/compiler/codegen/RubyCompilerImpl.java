/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng, Yu Su
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import com.xruby.compiler.codedom.CodeVisitor;
import com.xruby.compiler.codedom.Program;
import com.xruby.runtime.lang.RubyBinding;

import org.objectweb.asm.Label;
import org.objectweb.asm.Opcodes;
import org.objectweb.asm.Type;
import org.objectweb.asm.commons.GeneratorAdapter;

import java.math.BigInteger;
import java.util.Stack;

public class RubyCompilerImpl implements CodeVisitor {

    private ClassGenerator cg_;
    private final Stack<ClassGenerator> suspended_cgs_ = new Stack<ClassGenerator>();
    private final CompilationResults compilation_results_;
    private final String extra_;
    private final String script_name_;
    private RubyBinding binding_;

    private Label currentLineLabel;
    private boolean enableDebug = false;

    public RubyCompilerImpl(String extra, String script_name) {
        extra_ = extra;
        script_name_ = script_name;
        compilation_results_ = new CompilationResults();
    }

    private boolean isInGlobalScope() {
        return suspended_cgs_.empty() && !isInClassBuilder();
    }

    private boolean isInBlock() {
        return (cg_ instanceof ClassGeneratorForRubyBlock);
    }

    private boolean isInSingletonMethod() {
        if (cg_ instanceof ClassGeneratorForRubyMethod) {
            return ((ClassGeneratorForRubyMethod)cg_).isSingletonMethod();
        }

        return false;
    }

    private boolean isInClassBuilder() {
        return (cg_ instanceof ClassGeneratorForClassModuleBuilder);
    }

    public void enableDebug() {
        enableDebug = true;
    }

    private void switchToNewClassGenerator(ClassGenerator cg) {
        suspended_cgs_.push(cg_);
        cg_ = cg;
    }

    private void switchToPreviousClassGenerator(boolean last_statement_has_return_value) {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (!last_statement_has_return_value) {
            mg.loadNil();
        }

        mg.returnValue();
        mg.endMethod();
        cg_.visitEnd();

        compilation_results_.add(cg_.getCompilationResult());
        cg_ = suspended_cgs_.pop();
    }

    public CompilationResults compile(Program program, RubyBinding binding) {
        binding_ = binding;
        RubyIDClassGenerator.initScript(extra_, script_name_);
        String className = NameFactory.createClassName(extra_, script_name_, null);
        cg_ = new ClassGeneratorForRubyProgram(className, script_name_, binding, false, false);

        // Start compiling
        program.accept(this);

        MethodGenerator mg = cg_.getMethodGenerator();
        // Record the local variables' range, if user enables debug
        if (enableDebug) {
            mg.writeLocalVariableInfo();
        }
        mg.endMethod();
        cg_.visitEnd();
        compilation_results_.add(RubyIDClassGenerator.getCompilationResult());
//		RubyIDClassGenerator.clear();
        compilation_results_.add(cg_.getCompilationResult());
        return compilation_results_;
    }

    public void visitClassDefinition1(String className, boolean has_scope) {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (!isInGlobalScope() && !has_scope) {
            mg.loadCurrentClass(isInBlock());
        } else if (has_scope) {
            mg.checkCast(Types.RUBY_MODULE_TYPE);
        }

        mg.push(className);
        //super class will be pushed next, then visitSuperClass() will be called
    }

    public void visitClassDefinition2(String className, boolean has_scope) {
        //TODO optimizing aceess to builtin class (use them directly)
        MethodGenerator mg = cg_.getMethodGenerator();
        if (has_scope) {
            mg.checkCast(Types.RUBY_MODULE_TYPE);
            mg.RubyModule_defineClass();
        } else if (isInGlobalScope()) {
            mg.RubyAPI_defineClass();
        } else {
            mg.RubyModule_defineClass();
        }

        //The class body may refer the constant, so save it before class builder starts.
        callClassModuleBuilder(className, false);
    }

    private void callClassModuleBuilder(String name, boolean is_singleton) {
        MethodGenerator mg = cg_.getMethodGenerator();
        int i = mg.newLocal(Types.RUBY_VALUE_TYPE);
        mg.storeLocal(i);

        String uniqueName = NameFactory.createClassnameForClassModuleBuilder(extra_, script_name_, name);
        Type builder = Type.getType("L" + uniqueName + ";");
        mg.newInstance(builder);
        mg.dup();
        mg.invokeConstructor(builder, CgUtil.CONSTRUCTOR);
        mg.loadLocal(i);
        mg.pushNull();
        mg.pushNull();
        mg.loadLocal(i);
        mg.invokeVirtual(builder,
                CgUtil.getMethod("invoke", Types.RUBY_VALUE_TYPE, Types.RUBY_VALUE_TYPE, Types.RUBY_ARRAY_TYPE, Types.RUBY_BLOCK_TYPE, Types.RUBY_MODULE_TYPE));

        switchToNewClassGenerator(new ClassGeneratorForClassModuleBuilder(uniqueName, script_name_, null, is_singleton));
    }

    public void visitSingletonClassDefinition() {
        MethodGenerator mg = cg_.getMethodGenerator();      
        mg.loadCurrentScope(isInClassBuilder(), isInSingletonMethod(), isInGlobalScope(), isInBlock());
        mg.RubyValue_getSingletonClass();
        callClassModuleBuilder("SINGLETON", true);
    }

    public void visitClassDefinitionEnd(boolean last_statement_has_return_value) {
        switchToPreviousClassGenerator(last_statement_has_return_value);
    }

    public void visitModuleDefinition(String moduleName, boolean has_scope) {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (!mg.RubyRuntime_getBuiltinModule(moduleName)) {
            if (has_scope) {
                mg.checkCast(Types.RUBY_MODULE_TYPE);
                mg.RubyModule_defineModule(moduleName);
            } else if (isInGlobalScope()) {
                mg.RubyAPI_defineModule(moduleName);
            } else {
                mg.loadArg(3);
                mg.RubyModule_defineModule(moduleName);
            }
        }

        callClassModuleBuilder(moduleName, false);
    }

    public void visitModuleDefinitionEnd(boolean last_statement_has_return_value) {
        switchToPreviousClassGenerator(last_statement_has_return_value);
    }

    public int visitBlockBegin(StringBuilder name, boolean pulled) {
        String method_name = null;
        if (cg_ instanceof ClassGeneratorForRubyMethod) {
            method_name = ((ClassGeneratorForRubyMethod)cg_).getOrginalMethodName();
        } else if (cg_ instanceof ClassGeneratorForRubyBlock) {
            method_name = ((ClassGeneratorForRubyBlock)cg_).getOrginalMethodName();
        }
        String uniqueBlockName = NameFactory.createClassNameForBlock(extra_, script_name_, method_name);

        MethodGenerator mg = cg_.getMethodGenerator();
        mg.new_BlockClass(cg_, uniqueBlockName, isInClassBuilder(), isInSingletonMethod(), isInGlobalScope(), isInBlock());

        assert(name.length() == 0);
        name.insert(0, uniqueBlockName);

        if (pulled) {
            //saved the result to a local variable to retrieve later
            int i = mg.newLocal(Type.getType("L" + uniqueBlockName + ";"));
            mg.storeLocal(i);
            return i;
        }

        return -1;
    }

    public void visitBlockBodyBegin(String uniqueBlockName,
            int argc,
            boolean has_asterisk_parameter,
            int num_of_default_args,
            boolean is_for_in_expression,
            boolean has_extra_comma,
            boolean has_body) {

        //Save the current state and sart a new class file to write.
        switchToNewClassGenerator(new ClassGeneratorForRubyBlock(uniqueBlockName, script_name_,
                    argc,
                    has_asterisk_parameter,
                    num_of_default_args,
                    cg_,
                    is_for_in_expression,
                    has_extra_comma,
                    binding_));
        if (has_body) {
            if (argc >= 1 || has_asterisk_parameter || is_for_in_expression) {
                // has_extra_comma == (argc >= 1)
                cg_.getMethodGenerator().loadArg(1);
            } else {
                cg_.getMethodGenerator().loadNil();
            }
        }

        cg_.getMethodGenerator().getLabelManager().openNewScope();
    }

    public void visitBlockBodyEnd(String uniqueBlockName, boolean last_statement_has_return_value, int saved_as_pulled) {

        ClassGeneratorForRubyBlock block_cg = (ClassGeneratorForRubyBlock)cg_;
        String[] commons = block_cg.getFields();
        String[] assigned_commons = block_cg.getAssignedFields();
        String[] blocks = block_cg.getPreviousBlocks();
        block_cg.createFieldsAndConstructorOfRubyBlock(commons, blocks);

        cg_.getMethodGenerator().getLabelManager().closeCurrentScope();

        switchToPreviousClassGenerator(last_statement_has_return_value);

        if (saved_as_pulled >= 0) {
            cg_.getMethodGenerator().loadLocal(saved_as_pulled);
        }

        for (int i = 0; i < commons.length; ++i) {
            cg_.getMethodGenerator().dup();
            cg_.loadVariable(commons[i]);
            cg_.getMethodGenerator().putField(Type.getType("L" + uniqueBlockName + ";"), ClassGenerator.decorateName(commons[i]), Types.RUBY_VALUE_TYPE);
        }

        for (int i = 0; i < blocks.length; ++i) {
            cg_.getMethodGenerator().dup();
            cg_.getSharedBlock(blocks[i]);;
            cg_.getMethodGenerator().putField(Type.getType("L" + uniqueBlockName + ";"), ClassGeneratorForRubyBlock.getNameFromFullpath(blocks[i]), Type.getType("L" + blocks[i] + ";"));
        }

        cg_.getMethodGenerator().storeVariablesAssignedInBlock(uniqueBlockName, assigned_commons, saved_as_pulled);
    }

    public void visitBlockBody() {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.mark(mg.getLabelManager().getCurrentRedo());
    }

    public String visitMethodDefinition(String methodName, int num_of_args, boolean has_asterisk_parameter, int num_of_default_args, boolean is_singleton_method) {

        String uniqueMethodName = NameFactory.createClassName(extra_, script_name_, methodName);

        MethodGenerator mg = cg_.getMethodGenerator();
        if (!is_singleton_method) {
            mg.loadCurrentScope(isInClassBuilder(), isInSingletonMethod(), isInGlobalScope(), isInBlock());
        } else {
            mg.pushNull();
            mg.RubyValue_getSingletonClass();
        }

        mg.RubyModule_defineMethod(methodName, uniqueMethodName);

        //Save the current state and sart a new class file to write.
        switchToNewClassGenerator(ClassGeneratorForRubyMethodFactory.createClassGeneratorForRubyMethod(methodName,
                                script_name_,
                                uniqueMethodName,
                                num_of_args,
                                has_asterisk_parameter,
                                num_of_default_args,
                                is_singleton_method || mg.isSingleton()));

        return uniqueMethodName;
    }

    public void visitMethodDefinitionParameter(String name) {
        cg_.addParameter(name);
    }

    public void visitMethodDefinitionAsteriskParameter(String name, int argc) {
        cg_.setAsteriskParameter(name, argc);
    }

    public void visitMethodDefinitionBlockParameter(String name) {
        cg_.setBlockParameter(name);
    }

    public void visitMethodDefinitionEnd(boolean last_statement_has_return_value) {
        switchToPreviousClassGenerator(last_statement_has_return_value);
    }

    public void visitMethodDefinitionDefaultParameters(int size) {
        assert(size > 0);
        //create a empty array if arg is null (avoid null reference)
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadArg(1);
        Label label = new Label();
        mg.ifNonNull(label);
        mg.ObjectFactory_createArray(size, 0, false);
        mg.storeArg(1);
        mg.mark(label);
    }

    public Object visitMethodDefinitionDefaultParameterBegin(int index) {
        Label next_label = new Label();

        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadMethodPrameterLength();
        mg.push(index);
        mg.ifICmp(GeneratorAdapter.GT, next_label);

        mg.loadArg(1);

        return next_label;
    }

    public void visitMethodDefinitionDefaultParameterEnd(Object next_label) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.RubyArray_add(false);
        mg.pop();
        mg.mark((Label)next_label);
    }

    public void visitNoParameter() {
        cg_.getMethodGenerator().pushNull();
    }

    public void visitNoParameterForSuper() {
        if (cg_ instanceof ClassGeneratorForRubyBlock) {
            ((ClassGeneratorForRubyBlock)cg_).loadNoParameterForSuper();
        } else {
            cg_.getMethodGenerator().loadMethodArg();
        }
    }

    public void visitNoBlock(boolean is_super_or_block_given_call) {
        if (is_super_or_block_given_call) {
            if (isInGlobalScope()) {
                cg_.getMethodGenerator().pushNull();
            } else {
                cg_.getMethodGenerator().loadBlock(isInBlock());
            }
        } else {
            cg_.getMethodGenerator().pushNull();
        }
    }

    public void visitNoSuperClass() {
        cg_.getMethodGenerator().pushNull();
    }

    public void visitBlockArgument() {
        cg_.getMethodGenerator().RubyAPI_convertRubyValue2RubyBlock();
    }

    public void visitMethodCallBegin() {
        cg_.getMethodGenerator().addCurrentVariablesOnStack(Types.RUBY_VALUE_CLASS);
    }

    public void visitMethodCallEnd(String methodName, boolean hasReceiver,
            String blockName, int argc) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.removeCurrentVariablesOnStack();

        if (hasReceiver) {
            switch (argc) {
            case 0:
                mg.RubyAPI_callPublicNoArgMethod(methodName);
                break;
            case 1:
                mg.RubyAPI_callPublicOneArgMethod(methodName);
                break;
            case 2:
                mg.RubyAPI_callPublicTwoArgMethod(methodName);
                break;
            default:
                mg.RubyAPI_callPublicMethod(methodName);
                break;
            }
        } else {
            switch (argc) {
            case 0:
                mg.RubyAPI_callNoArgMethod(methodName);
                break;
            case 1:
                mg.RubyAPI_callOneArgMethod(methodName);
                break;
            case 2:
                mg.RubyAPI_callTwoArgMethod(methodName);
                break;
            default:
                mg.RubyAPI_callMethod(methodName);
                break;
            }
        }

        mg.returnIfBlockReturned();
    }

    public void visitBinaryOperator(String operator) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.pushNull();
        if (operator.equals("!=")) {
            mg.RubyAPI_callPublicOneArgMethod("==");
            mg.RubyAPI_operatorNot();
        } else if (operator.equals("!~")) {
            mg.RubyAPI_callPublicOneArgMethod("=~");
            mg.RubyAPI_operatorNot();
        } else {
            //operator as method call
            mg.RubyAPI_callPublicOneArgMethod(operator);
        }
    }

    public Object visitAndBinaryOperatorLeft() {
        //The and and && operators evaluate their first operand. If false,
        //the expression returns false; otherwise, the expression returns
        //the value of the second operand.
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.dup();
        Label label = (Label)visitAfterIfCondition();
        mg.pop();//discard the current value;
        return label;
    }

    public void visitAndBinaryOperatorRight(Object label) {
        cg_.getMethodGenerator().mark((Label)label);
    }

    public Object visitOrBinaryOperatorLeft() {
        //The or and || operators evaluate their first operand. If true,
        //the expression returns true; otherwise, the expression returns
        //the value of the second operand.
        cg_.getMethodGenerator().dup();
        Label label = (Label)visitAfterUnlessCondition();
        cg_.getMethodGenerator().pop();//discard the current value;
        return label;
    }

    public void visitOrBinaryOperatorRight(Object label) {
        visitAndBinaryOperatorRight(label);
    }

    public void visitUnaryOperator(String operator) {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (operator.equals("!")) {
            mg.RubyAPI_operatorNot();
        } else {
            mg.pushNull();
            mg.pushNull();
            mg.RubyAPI_callPublicMethod(operator);
        }
    }

    public void visitGlobalVariableAssignmentOperator(String var, boolean rhs_is_method_call, boolean is_multiple_assign) {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (rhs_is_method_call) {
            mg.RubyAPI_expandArrayIfThereIsZeroOrOneValue();
        }
        mg.GlobalVatiables_set(var);
        if (is_multiple_assign) {
            mg.pop();
        }
    }

    public void visitLocalVariableAssignmentOperator(String var, boolean rhs_is_method_call, boolean is_multiple_assign) {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (rhs_is_method_call) {
            mg.RubyAPI_expandArrayIfThereIsZeroOrOneValue();
        }
        if (!is_multiple_assign) {
            mg.dup();//do not pop off empty stack
        }

        cg_.storeVariable(var);
        SymbolTable table = mg.getSymbolTable();

        // Record the start label of this variable
        if(enableDebug && table.isNewLocalVar(var)) {
            table.setVarLineNumberInfo(var, currentLineLabel);
        }
    }

    public void visitFloatExpression(double value) {
        cg_.getMethodGenerator().ObjectFactory_createFloat(value);
    }

    public void visitFixnumExpression(int value) {
        cg_.getMethodGenerator().ObjectFactory_createFixnum(value);
    }

    public void visitBignumExpression(BigInteger value) {
        cg_.getMethodGenerator().ObjectFactory_createBignum(value);
    }

    public void visitStringExpression(String value) {
        cg_.getMethodGenerator().ObjectFactory_createString(value);
    }

    public void visitStringExpressionWithExpressionSubstitutionBegin() {
        cg_.getMethodGenerator().ObjectFactory_createString();
    }

    public void visitStringExpressionWithExpressionSubstitution(String value) {
        cg_.getMethodGenerator().RubyString_append(value);
    }

    public void visitStringExpressionWithExpressionSubstitution() {
        cg_.getMethodGenerator().RubyString_append();
    }

    public void visitStringExpressionWithExpressionSubstitutionEnd() {
    }

    public void visitRegexpExpressionWithExpressionSubstitutionEnd(String option) {
        cg_.getMethodGenerator().ObjectFactory_createRegexp(option);
    }

    public void visitCommandOutputExpressionWithExpressionSubstitutionEnd() {
        cg_.getMethodGenerator().RubyAPI_runCommandAndCaptureOutput();
    }

    public void visitRegexpExpression(String value, String option) {
        cg_.getMethodGenerator().ObjectFactory_createRegexp(value, option);
    }

    public void visitSymbolExpression(String value) {
        cg_.getMethodGenerator().ObjectFactory_createSymbol(value);
    }

    public void visitTerminal() {
        cg_.getMethodGenerator().pop();
    }

    public void visitEof(boolean last_statement_has_return_value) {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (!last_statement_has_return_value) {
            mg.loadNil();
        }
        mg.returnValue();
    }

    public void visitLocalVariableExpression(String value) {
        cg_.loadVariable(value);
    }

    public void visitNilExpression() {
        cg_.getMethodGenerator().loadNil();
    }

    public Object visitAfterIfCondition() {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.RubyValue_isTrue();
        Label label = new Label();
        mg.ifZCmp(GeneratorAdapter.EQ, label);
        return label;
    }

    public void visitWhileConditionBegin(boolean do_first) {
        MethodGenerator mg = cg_.getMethodGenerator();
        LabelManager lm = mg.getLabelManager();
        lm.openNewScope();
        if (do_first) {
            mg.goTo(lm.getCurrentRedo());
        }
        mg.mark(lm.getCurrentNext());
    }

    public void visitWhileConditionEnd(boolean always_true, boolean is_until) {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (always_true) {
            mg.push(true);
        } else {
            mg.RubyValue_isTrue();
        }

        if (is_until) {
            mg.ifZCmp(GeneratorAdapter.NE, mg.getLabelManager().getCurrentX());
        } else {
            mg.ifZCmp(GeneratorAdapter.EQ, mg.getLabelManager().getCurrentX());
        }

        mg.mark(mg.getLabelManager().getCurrentRedo());
    }

    public void visitWhileBodyEnd(boolean has_body) {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (has_body) {
            mg.pop();
        }

        LabelManager lm = mg.getLabelManager();
        mg.goTo(lm.getCurrentNext());
        mg.mark(lm.getCurrentX());
        visitNilExpression();//this is the value of while expression if no break is called.

        mg.mark(lm.getCurrentBreak());
        lm.closeCurrentScope();
    }

    public Object visitAfterIfBody(Object next_label, Object end_label) {
        if (null == end_label) {
            end_label = new Label();
        }

        MethodGenerator mg = cg_.getMethodGenerator();
        if (null != next_label) {
            mg.goTo((Label)end_label);
            mg.mark((Label)next_label);
        } else {
            mg.mark((Label)end_label);
        }

        return end_label;
    }

    public Object visitAfterCaseCondition() {
        int i = cg_.getAnonymousLocalVariable();
        cg_.getMethodGenerator().storeLocal(i);
        return i;
    }

    public Object visitAfterWhenCondition(Object case_value, boolean mrhs) {
        int i = (Integer)case_value;
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadLocal(i);
        if (!mrhs) {
            mg.RubyAPI_testCaseEqual();
        } else {
            mg.RubyAPI_testCaseEqual2();
        }
        Label label = new Label();
        mg.ifZCmp(GeneratorAdapter.EQ, label);
        return label;
    }

    public Object visitAfterWhenConditionNotNil(Object case_value) {
        int i = (Integer)case_value;
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadLocal(i);
        mg.RubyAPI_testCaseEqualNotNil();
        
        Label label = new Label();
        mg.ifZCmp(GeneratorAdapter.EQ, label);
        return label;
    }
    
    public Object visitAfterWhenBody(Object next_label, Object end_label) {
        return visitAfterIfBody(next_label, end_label);
    }

    public void visitTrueExpression() {
        cg_.getMethodGenerator().loadTrue();
    }

    public void visitFalseExpression() {
        cg_.getMethodGenerator().loadFalse();
    }

    public Object visitAfterUnlessCondition() {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.RubyValue_isTrue();
        Label label = new Label();
        mg.ifZCmp(GeneratorAdapter.NE, label);
        return label;
    }

    public Object visitAfterUnlessBody(Object next_label, Object end_label) {
        return visitAfterIfBody(next_label, end_label);
    }

    public Object visitBodyBegin(boolean has_ensure) {
        //once exceptio is thrown, everything already on the stack will be destoried. so if we have begin..end
        //in the method parameter. e.g. f(..., begin ..end, ...), the method receiver and parameter is already on the list.
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.saveCurrentVariablesOnStack();

        mg.getEnsureLabelManager().openNewScope();
        if (has_ensure) {
            mg.getEnsureLabelManager().setCurrentFinally(new Label());
        }
        mg.mark(mg.getEnsureLabelManager().getCurrentRetry());
        return mg.getEnsureLabelManager().getCurrentRetry();
    }

    public Object visitBodyAfter() {
        return cg_.getMethodGenerator().mark();
    }

    public void visitBodyEnd(Object label) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.mark((Label)label);
        mg.getEnsureLabelManager().closeCurrentScope();

        mg.restoreCurrentVariablesOnStack();
    }

    public int visitEnsureBodyBegin() {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.mark(mg.getEnsureLabelManager().getCurrentFinally());
        mg.getEnsureLabelManager().setCurrentFinally(null);//finished using it

        int var = mg.newLocal(Type.getType(Object.class));
        mg.storeLocal(var);
        return var;
    }

    public void visitEnsureBodyEnd(int var) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.pop();
        mg.ret(var);
    }

    public Object visitPrepareEnsure1() {
        Label label = new Label();
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.visitJumpInsn(Opcodes.JSR, mg.getEnsureLabelManager().getCurrentFinally());
        return label;
    }

    public void visitEnsure(int exception_var) {
        if (exception_var >= 0) {
            MethodGenerator mg = cg_.getMethodGenerator();
            Label l = mg.getEnsureLabelManager().getCurrentFinally();
            if (null != l) {
                mg.visitJumpInsn(Opcodes.JSR, l);
            }
            mg.loadLocal(exception_var);
            mg.throwException();
        } else {
            invokeFinallyIfExist();
        }
    }

    public Object visitPrepareEnsure() {
        Label after_exception = new Label();
        cg_.getMethodGenerator().goTo(after_exception);
        return after_exception;
    }

    public int visitRescueBegin(Object begin, Object end) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.catchRubyException((Label)begin, (Label)end);

        int exception_variable = cg_.getAnonymousLocalVariable();
        mg.storeLocal(exception_variable);

        return exception_variable;
    }

    public void visitRescueEnd(int exception_variable, boolean has_ensure) {
        if (!has_ensure) {
            MethodGenerator mg = cg_.getMethodGenerator();
            mg.loadLocal(exception_variable);
            mg.throwException();
        }
    }

    public Object visitRescueVariable(String name, int exception_variable) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadLocal(exception_variable);
        mg.RubyAPI_testExceptionType();
        Label label = new Label();
        mg.ifZCmp(GeneratorAdapter.EQ, label);

        if (null != name) {
            mg.loadLocal(exception_variable);
            mg.RubyAPI_convertRubyException2RubyValue();
            cg_.storeVariable(name);
        }

        return label;
    }

    public void visitAfterRescueBody(Object next_label, Object end_label) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.goTo((Label)end_label);
        mg.mark((Label)next_label);
    }

    public void visitArrayBegin(int size, int rhs_size, boolean has_single_asterisk) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.ObjectFactory_createArray(size, rhs_size, has_single_asterisk);
        mg.addCurrentVariablesOnStack(Types.RUBY_ARRAY_CLASS);
    }

    public void visitHashBegin() {
        //TODO use addCurrentVariablesOnStack/removeCurrentVariablesOnStack
        cg_.getMethodGenerator().ObjectFactory_createHash();
    }

    public void visitArrayElement(boolean asterisk, boolean is_method_call) {
        if (asterisk) {
            cg_.getMethodGenerator().RubyArray_expand(is_method_call);
        } else {
            cg_.getMethodGenerator().RubyArray_add(is_method_call);
        }
    }

    public void visitBinding(boolean single_arg) {
        cg_.createBinding(isInClassBuilder(), isInSingletonMethod(), isInGlobalScope(), isInBlock());
        if (!single_arg) {
            cg_.getMethodGenerator().RubyArray_add(false);
        }
    }

    public void visitHashElement() {
        cg_.getMethodGenerator().RubyHash_addValue();
    }

    public void visitArrayEnd() {
        cg_.getMethodGenerator().removeCurrentVariablesOnStack();
    }

    public void visitHashEnd() {
    }

    public void visitYieldBegin() {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadBlock(isInBlock());
        mg.dup();//will be used to call breakOrReturned().
        visitSelfExpression();
    }

    public void visitYieldEnd(int argc) {
        MethodGenerator mg = cg_.getMethodGenerator();
        switch (argc) {
        case 0:
            mg.RubyBlock_invokeNoArg(isInBlock());
            break;
        case 1:
            mg.RubyBlock_invokeOneArg(isInBlock());
            break;
        default:
            mg.RubyBlock_invoke(isInBlock());
            break;
        }

        mg.checkBreakedOrReturned(isInBlock());
    }

    public void visitSuperBegin() {
        cg_.getMethodGenerator().loadArg(0);//TODO error checking: super called outside of method (NoMethodError)
    }

    public void visitImplicitSuperEnd() {
        if (cg_ instanceof ClassGeneratorForRubyMethod) {
            ((ClassGeneratorForRubyMethod)cg_).callSuperMethod(true, false);
        } else {
            ((ClassGeneratorForRubyBlock)cg_).callSuperMethod(true, false);
        }
    }

    public void visitExplicitSuperEnd(int argc) {
        if (cg_ instanceof ClassGeneratorForRubyMethod) {
            MethodGenerator mg = cg_.getMethodGenerator();
            switch (argc) {
            case 0:
                mg.RubyAPI_callSuperNoArgMethod();
                break;
            case 1:
                mg.RubyAPI_callSuperOneArgMethod();
                break;
            case 2:
                mg.RubyAPI_callSuperTwoArgMethod();
                break;
            default:
                mg.RubyAPI_callSuperMethod();
                break;
            }
        } else {
            ((ClassGeneratorForRubyBlock)cg_).callSuperMethod(false, 1 == argc);
        }
    }

    public void visitGlobalVariableExpression(String value) {
        cg_.getMethodGenerator().GlobalVatiables_get(value);
    }

    public void visitCommandOutputExpression(String value) {
        cg_.getMethodGenerator().RubyAPI_runCommandAndCaptureOutput(value);
    }

    private void invokeFinallyIfExist() {
        MethodGenerator mg = cg_.getMethodGenerator();
        Label l = mg.getEnsureLabelManager().getCurrentFinally();
        if (null != l) {
            int tmp = cg_.getAnonymousLocalVariable();
            mg.storeLocal(tmp);//store then load to make stack size always equals 1
            mg.visitJumpInsn(Opcodes.JSR, l);
            mg.loadLocal(tmp);
        }
    }

    public void visitReturn() {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (isInBlock()) {
            invokeFinallyIfExist();
            mg.RubyBlock__return__();
            mg.returnValue();
        } else {
            invokeFinallyIfExist();
            mg.returnValue();
        }
    }

    public void visitAliasGlobalVariable(String newName, String oldName) {
        cg_.getMethodGenerator().GlobalVariables_alias(newName, oldName);
    }

    public void visitAliasMethod(String newName, String oldName) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadCurrentClass(isInBlock());
        mg.RubyModule_aliasMethod(newName, oldName);
    }

    public void visitUndef(String name) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadCurrentClass(isInBlock());
        mg.RubyModule_undefMethod(name);
    }

    public void visitSelfExpression() {
        cg_.getMethodGenerator().loadSelf(isInBlock());
    }

    public void visitClassVariableExpression(String name) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadCurrentScope(isInClassBuilder(), isInSingletonMethod(), isInGlobalScope(), isInBlock());
        mg.RubyModule_getClassVariable(name);
    }

    public void visitClassVariableAssignmentOperator(String name, boolean rhs_is_method_call, boolean is_multiple_assign) {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (rhs_is_method_call) {
            mg.RubyAPI_expandArrayIfThereIsZeroOrOneValue();
        }

        mg.loadCurrentScope(isInClassBuilder(), isInSingletonMethod(), isInGlobalScope(), isInBlock());
        mg.swap();
        mg.RubyModule_setClassVariable(name);
        if (is_multiple_assign) {
            mg.pop();
        }
    }

    public void visitInstanceVariableExpression(String name) {
        visitSelfExpression();
        cg_.getMethodGenerator().RubyValue_getInstanceVariable(name);
    }

    public void visitInstanceVariableAssignmentOperator(String name, boolean rhs_is_method_call, boolean is_multiple_assign) {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (rhs_is_method_call) {
            mg.RubyAPI_expandArrayIfThereIsZeroOrOneValue();
        }

        visitSelfExpression();
        mg.swap();
        mg.RubyValue_setInstanceVariable(name);
        if (is_multiple_assign) {
            mg.pop();
        }
    }

    public void visitMrhs(int var, int index, boolean asterisk) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadLocal(var);
        if (asterisk) {
            mg.RubyArray_collect(index);
        } else {
            mg.RubyArray_get(index);
        }
    }

    public int visitMultipleAssignment(boolean single_lhs, boolean has_mlhs, boolean has_mrhs) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.dup();

        if (single_lhs && !has_mrhs) {
            return 0;
        }

        if (single_lhs) {
            mg.RubyAPI_expandArrayIfThereIsZeroOrOneValue2();
            return 0;
        } else if (has_mlhs) {
            mg.RubyAPI_expandArrayIfThereIsOnlyOneRubyArray();
            return mg.saveRubyArrayAsLocalVariable();
        } else {
            return mg.saveRubyArrayAsLocalVariable();
        }
    }

    public int visitNestedVariable(boolean single_lhs, boolean has_mlhs) {
        if (single_lhs) {
            return 0;
        } else {
            MethodGenerator mg = cg_.getMethodGenerator();
            mg.RubyAPI_convertToArrayIfNotYet();
            if (has_mlhs) {
                mg.RubyAPI_expandArrayIfThereIsOnlyOneRubyArray();
            }
            return mg.saveRubyArrayAsLocalVariable();
        }
    }

    public void visitBreak() {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (isInBlock() && mg.getLabelManager().isAtTopLevel()) {
            invokeFinallyIfExist();
            mg.RubyBlock__break__();
            mg.returnValue();
        } else {
            invokeFinallyIfExist();
            mg.goTo(mg.getLabelManager().getCurrentBreak());
        }
    }

    public void visitNext() {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (isInBlock() && mg.getLabelManager().isAtTopLevel()) {
            mg.returnValue();
        } else {
            mg.pop();
            mg.goTo(mg.getLabelManager().getCurrentNext());
        }
    }

    public void visitRedo() {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.goTo(mg.getLabelManager().getCurrentRedo());
    }

    public void visitRetry() {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (isInBlock() && mg.getLabelManager().isAtTopLevel()) {
            mg.RubyBlock__retry__();
            mg.loadNil();
            mg.returnValue();
        } else {
            mg.goTo(mg.getEnsureLabelManager().getCurrentRetry());
        }
    }

    public void visitExclusiveRangeOperator() {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.push(true);
        mg.ObjectFactory_createRange();
    }

    public void visitInclusiveRangeOperator() {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.push(false);
        mg.ObjectFactory_createRange();
    }

    public void visitCurrentNamespaceConstant(String name) {
        if (isInGlobalScope()) {
            visitTopLevelConstant(name);
            return;
        }

        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadCurrentScope(isInClassBuilder(), isInSingletonMethod(), isInGlobalScope(), isInBlock());
        mg.RubyAPI_getCurrentNamespaceConstant(name);
    }

    public void visitConstant(String name) {
        cg_.getMethodGenerator().RubyAPI_getConstant(name);
    }

    public void visitTopLevelConstant(String name) {
        //quick access for builtin
        MethodGenerator mg = cg_.getMethodGenerator();
        if (mg.RubyRuntime_getBuiltinClass(name)) {
            return;
        } else if (mg.RubyRuntime_getBuiltinModule(name)) {
            return;
        }

        loadTopScope();
        mg.RubyAPI_getCurrentNamespaceConstant(name);
    }

    private void loadTopScope() {
        MethodGenerator mg = cg_.getMethodGenerator();
        if (isInGlobalScope()) {
            mg.loadCurrentClass(false);
        } else {
            mg.RubyRuntime_GlobalScope();
        }
    }

    public void visitCurrentNamespaceConstantAssignmentOperator(String name, boolean rhs_is_method_call, boolean is_multiple_assign) {
        if (isInGlobalScope()) {
            visitTopLevelConstantAssignmentOperator(name, rhs_is_method_call, is_multiple_assign);
            return;
        }

        cg_.getMethodGenerator().loadCurrentScope(isInClassBuilder(), isInSingletonMethod(), isInGlobalScope(), isInBlock());
        visitConstantAssignmentOperator(name, rhs_is_method_call, is_multiple_assign);
    }

    public void visitConstantAssignmentOperator(String name, boolean rhs_is_method_call, boolean is_multiple_assignment) {
        //TODO handle rhs_is_method_call and is_multiple_assignment
        cg_.getMethodGenerator().RubyAPI_setConstant(name);
    }

    public void visitTopLevelConstantAssignmentOperator(String name, boolean rhs_is_method_call, boolean is_multiple_assignment) {
        //TODO handle rhs_is_method_call and is_multiple_assignment
        //TODO work with eval/binding
        cg_.getMethodGenerator().RubyAPI_setTopLevelConstant(name);
    }

    public void visitDefinedPublicMethod(String name) {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadCurrentScope(isInClassBuilder(), isInSingletonMethod(), isInGlobalScope(), isInBlock());
        mg.RubyAPI_isDefinedPublicMethod(name);
    }

    public void visitDefinedCurrentNamespaceConstant(String name) {
        if (Types.isBuiltinClass(name) || Types.isBuiltinModule(name)) {
            visitStringExpression("constant");
            return;
        }

        MethodGenerator mg = cg_.getMethodGenerator();
        mg.loadCurrentScope(isInClassBuilder(), isInSingletonMethod(), isInGlobalScope(), isInBlock());
        mg.RubyAPI_isDefinedCurrentNamespaceConstant(name);
    }

    public void visitDefinedTopLevelConstant(String name) {
        if (Types.isBuiltinClass(name) || Types.isBuiltinModule(name)) {
            visitStringExpression("constant");
            return;
        }

        loadTopScope();
        cg_.getMethodGenerator().RubyAPI_isDefinedCurrentNamespaceConstant(name);
    }

    public void visitDefinedConstant(String name) {
        cg_.getMethodGenerator().RubyAPI_isDefinedCurrentNamespaceConstant(name);
    }

    public void visitDefinedMethod(String name) {
        cg_.getMethodGenerator().RubyAPI_isDefinedMethod(name);
    }

    public void visitDefinedSuperMethod() {
        if (cg_ instanceof ClassGeneratorForRubyMethod) {
            visitSelfExpression();
            cg_.getMethodGenerator().RubyAPI_isDefinedSuperMethod(((ClassGeneratorForRubyMethod)cg_).getOrginalMethodName());
        } else {
            visitNilExpression();
        }
    }

    public void visitDefinedLocalVariable(String name) {
        if (cg_.getSymbolTable().isDefinedInCurrentScope(name)) {
            visitStringExpression("local-variable");
        } else {
            visitNilExpression();
        }
    }

    public void visitDefinedInstanceVariable(String name) {
        if (cg_ instanceof ClassGeneratorForRubyMethod) {
            visitSelfExpression();
            cg_.getMethodGenerator().RubyAPI_isDefinedInstanceVariable(name);
        } else {
            visitNilExpression();
        }
    }
    
    public void visitDefinedYield() {
        if (cg_ instanceof ClassGeneratorForRubyMethod) {
            MethodGenerator mg = cg_.getMethodGenerator();
            mg.loadCurrentBlock();//.loadArg(2);
            mg.RubyAPI_isDefinedYield();
        } else {
            visitNilExpression();
        }
    }

    public boolean isDefinedInCurrentScope(String name) {
        return cg_.isDefinedInCurrentScope(name);
    }

    public void visitSpecialLambdaCallBegin() {
        visitSelfExpression();
    }

    public void visitSpecialLambdaCallEnd() {
        cg_.getMethodGenerator().RubyBlock_invoke(isInBlock());
    }

    public void visitPotentialProcCall() {
        MethodGenerator mg = cg_.getMethodGenerator();
        mg.dup();
        mg.instanceOf(Types.RUBY_PROC_TYPE);

        Label label1 = new Label();
        mg.ifZCmp(GeneratorAdapter.EQ, label1);

        mg.dup();
        mg.checkCast(Types.RUBY_PROC_TYPE);

        //check if in the right context
        //TODO have not considered all the situations
        mg.dup();
        mg.RubyProc_isDefinedInAnotherBlock();
        Label label2 = new Label();
        mg.ifZCmp(GeneratorAdapter.NE, label2);

        cg_.addVariableToBinding();//TODO should we use updateBinding()?
        mg.mark(label2);
        mg.pop();

        mg.mark(label1);
    }

    public void visitMultipleArrayAssign() {
        cg_.getMethodGenerator().RubyAPI_callArraySet();
    }

    // ---------------------------
    //   Interfaces for debugger
    // ---------------------------
    public Label visitLineLabel(int lineNumber) {
        if(enableDebug) {
            // store the current line, if debug is enabled
            currentLineLabel = cg_.getMethodGenerator().mark();
            cg_.getMethodGenerator().visitLineNumber(lineNumber, currentLineLabel);

            return currentLineLabel;
        }

        return null;
    }

}