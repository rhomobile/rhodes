/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import org.objectweb.asm.*;
import org.objectweb.asm.commons.*;
import org.objectweb.asm.util.CheckClassAdapter;
import com.xruby.runtime.lang.RubyBinding;
import java.util.*;

abstract class ClassGenerator {

    private final ClassWriter cw_ = new ClassWriter(ClassWriter.COMPUTE_MAXS);

    //can simply use 'cv_ = cw_'. CheckClassAdapter make compilation slower,
    //but it does lots of verification on the bytecode
    protected final ClassVisitor cv_ = new CheckClassAdapter(cw_);

    protected final String name_;
    protected MethodGenerator mg_for_run_method_ = null;

    public abstract void loadArgOfMethodForBlock();

    protected ClassGenerator(String name) {
        name_ = name;
    }

    public CompilationResult getCompilationResult() {
        return new CompilationResult(name_, cw_.toByteArray());
    }

    SymbolTable getSymbolTable() {
        return getMethodGenerator().getSymbolTable();
    }

    static String decorateName(String name) {
        return "$" + name;
    }

    public void addParameter(String name) {
        getSymbolTable().addMethodParameter(name);
    }

    public void setAsteriskParameter(String name, int argc) {
        //initializeAsteriskParameter() is always called. -- this will makes code generation simpler.
        //But doing it here has a little advantage (optimazation): if the
        //asterisk parameter is not used, we'd better avoid calling initializeAsteriskParameter().
        MethodGenerator mg = getMethodGenerator();
		mg.RubyAPI_initializeAsteriskParameter(argc);
        int i = mg.newLocal(Types.RUBY_VALUE_TYPE);
        getSymbolTable().addAsteriskOrBlockMethodParameter(name, i);
        mg.storeLocal(i);
    }

    public void setBlockParameter(String name) {
        MethodGenerator mg = getMethodGenerator();
		mg.RubyAPI_initializeBlockParameter();
        int i = mg.newLocal(Types.RUBY_VALUE_TYPE);
        getSymbolTable().addAsteriskOrBlockMethodParameter(name, i);
        mg.storeLocal(i);
    }

    public void visitEnd() {
        cv_.visitEnd();
    }

    public int getAnonymousLocalVariable() {
        return getMethodGenerator().newLocal(Types.RUBY_VALUE_TYPE);
    }

    void getSharedBlock(String name) {
        getMethodGenerator().loadLocal(getSymbolTable().getBlock(name));
    }

    public void storeVariable(String name) {
        SymbolTable.VariableAssignedInBlock record = getSymbolTable().getVariableAssignedInBlock(name);
        MethodGenerator mg = getMethodGenerator();
		if (null != record) {
            mg.loadLocal(record.block_var_);
            mg.swap();//TODO aoid using swap, jad does not unserstand this well.
            mg.putField(Type.getType("L" + record.block_name_ + ";"), decorateName(name), Types.RUBY_VALUE_TYPE);
            return;
        }

        if (getSymbolTable().getLocalVariable(name) >= 0) {
            mg.storeRubyLocalVariable(name);
            return;
        }

        int i = getSymbolTable().getAsteriskOrBlockMethodParameter(name);
        if (i >= 0) {
            mg.storeLocal(i);
            return;
        }

        int index = getSymbolTable().getMethodParameter(name);
        if (index >= 0) {
            storeMethodParameter(index);
            return;
        }

        mg.storeNewLocalVariable(name);
    }

    public void loadVariable(String name) {
        SymbolTable.VariableAssignedInBlock record = getSymbolTable().getVariableAssignedInBlock(name);
        MethodGenerator mg = getMethodGenerator();
		if (null != record) {
             mg.loadLocal(record.block_var_);
             mg.getField(Type.getType("L" + record.block_name_ + ";"), decorateName(name), Types.RUBY_VALUE_TYPE);
             return;
        }

        //check if this is local variable
        if (getSymbolTable().getLocalVariable(name) >= 0) {
            mg.loadRubyLocalVariable(name);
            return;
        }

        int i = getSymbolTable().getAsteriskOrBlockMethodParameter(name);
        if (i >= 0) {
            mg.loadLocal(i);
            return;
        }

        //check if this is normal method parameter
        int index = getSymbolTable().getMethodParameter(name);
        if (index >= 0) {
            loadMethodPrameter(index);
            return;
        }

        // never used, for example a = a + 1
        mg.loadNil();
    }

    public void loadMethodPrameter(int index) {
        //signature:
        //run(RubyValue reciever, RubyArray args, RubyBlock block)
        MethodGenerator mg = getMethodGenerator();
		mg.loadArg(1);
        mg.push(index);
        mg.invokeVirtual(Types.RUBY_ARRAY_TYPE,
                    CgUtil.getMethod("get", Types.RUBY_VALUE_TYPE, Type.INT_TYPE));
    }

    public void storeMethodParameter(int index) {
        MethodGenerator mg = getMethodGenerator();
		int i = mg.newLocal(Types.RUBY_VALUE_TYPE);
        mg.storeLocal(i);
        mg.loadArg(1);
        mg.push(index);
        mg.loadLocal(i);
        mg.invokeVirtual(Types.RUBY_ARRAY_TYPE,
                CgUtil.getMethod("set", Types.RUBY_VALUE_TYPE, Type.INT_TYPE, Types.RUBY_VALUE_TYPE));
        mg.pop();
    }

    public MethodGenerator getMethodGenerator() {
        return mg_for_run_method_;
    }

    public void createBinding(boolean isInClassBuilder, boolean isInSingletonMethod, boolean isInGlobalScope, boolean isInBlock) {
        int i = getSymbolTable().getInternalBindingVar();
        MethodGenerator mg = getMethodGenerator();
		if (i >= 0) {
            mg.loadLocal(i);
            updateBinding(isInClassBuilder, isInSingletonMethod, isInGlobalScope, isInBlock);
            return;
        }

        newBinding();
        updateBinding(isInClassBuilder, isInSingletonMethod, isInGlobalScope, isInBlock);

        mg.dup();
        i = mg.newLocal(Types.RUBY_BINDING_TYPE);
        mg.storeLocal(i);
        getSymbolTable().setInternalBindingVar(i);
    }

    private void newBinding() {
        MethodGenerator mg = getMethodGenerator();
		mg.newInstance(Types.RUBY_BINDING_TYPE);
        mg.dup();
        mg.invokeConstructor(Types.RUBY_BINDING_TYPE,	CgUtil.CONSTRUCTOR);
    }

    private void updateBinding(boolean isInClassBuilder, boolean isInSingletonMethod, boolean isInGlobalScope, boolean isInBlock) {
        MethodGenerator mg = getMethodGenerator();
		mg.loadSelf(isInBlock);
        mg.invokeVirtual(Types.RUBY_BINDING_TYPE,
        		CgUtil.getMethod("setSelf", Types.RUBY_BINDING_TYPE, Types.RUBY_VALUE_TYPE));

        if (isInClassBuilder) {
            mg.pushNull();
        } else {
            mg.loadBlock(isInBlock);
        }
        mg.invokeVirtual(Types.RUBY_BINDING_TYPE,
        		CgUtil.getMethod("setBlock", Types.RUBY_BINDING_TYPE, Types.RUBY_BLOCK_TYPE));

        if (!isInBlock) {
            mg.loadCurrentScope(isInClassBuilder, isInSingletonMethod, isInGlobalScope, isInBlock);
        } else {
            mg.pushNull();//TODO fix this and loadCurrentClass
        }
        mg.invokeVirtual(Types.RUBY_BINDING_TYPE,
        		CgUtil.getMethod("setScope", Types.RUBY_BINDING_TYPE, Types.RUBY_MODULE_TYPE));

        addVariableToBinding();
    }

    public void addVariableToBinding() {
        Collection<String> vars = getSymbolTable().getLocalVariables();
        Method addVarMethod = CgUtil.getMethod("addVariable", Types.RUBY_BINDING_TYPE, Type.getType(String.class), Types.RUBY_VALUE_TYPE);
        MethodGenerator mg = getMethodGenerator();
		for (String s : vars) {
            mg.push(s);
            loadVariable(s);
			mg.invokeVirtual(Types.RUBY_BINDING_TYPE, addVarMethod);
        }

        Collection<String> params = getSymbolTable().getParameters();
        for (String s : params) {
            mg.push(s);
            loadMethodPrameter(getSymbolTable().getMethodParameter(s));
            mg.invokeVirtual(Types.RUBY_BINDING_TYPE, addVarMethod);
        }
    }

    public boolean isDefinedInCurrentScope(String name) {
        return getSymbolTable().isDefinedInCurrentScope(name);
    }

    protected void updateBinding(RubyBinding binding, String name) {
        if (null != binding && !binding.hasName(name)) {

            binding.addVariableName(name);

            mg_for_run_method_.dup();
            mg_for_run_method_.loadArg(1);
            mg_for_run_method_.swap();
            mg_for_run_method_.invokeVirtual(Types.RUBY_ARRAY_TYPE,
            		CgUtil.getMethod("add", Types.RUBY_ARRAY_TYPE, Types.RUBY_VALUE_TYPE));
            mg_for_run_method_.pop();
        }
    }
}

