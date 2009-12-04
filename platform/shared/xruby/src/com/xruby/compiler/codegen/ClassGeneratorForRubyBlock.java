/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import org.objectweb.asm.*;
import org.objectweb.asm.commons.*;

import com.xruby.runtime.lang.RubyBinding;

import java.util.*;

class FieldManager {
    private final FieldManager parent_;
    private final ClassGeneratorForRubyBlock cg_;
    private final Set<String> fields_ = new HashSet<String>();//assigned fields are fields as well
    private final Set<String> assigned_fields_ = new HashSet<String>();
    private final Set<String> previous_blocks_ = new HashSet<String>();

    public FieldManager(FieldManager parent, ClassGeneratorForRubyBlock cg) {
        parent_ = parent;
        cg_ = cg;
    }

    public void addPreviousBlock(String block) {
        previous_blocks_.add(block);
    }

    public void addField(String s) {
        fields_.add(s);
        if (null != parent_) {
            parent_.addField(s);
        }
    }

    public void addAssignedField(String s, boolean is_for_in_expression) {
        fields_.add(s);

        if (is_for_in_expression || cg_.isDefinedInOwnerScope(s)) {
            assigned_fields_.add(s);
        }

        if (null != parent_) {
            parent_.addField(s);
            parent_.addAssignedField(s, false);
        }
    }

    public String[] getPreviousBlocks() {
        return previous_blocks_.toArray(new String[previous_blocks_.size()]);
    }

    public String[] getFields() {
        return fields_.toArray(new String[fields_.size()]);
    }

    public String[] getAssignedFields() {
        return assigned_fields_.toArray(new String[assigned_fields_.size()]);
    }

}

class ClassGeneratorForRubyBlock extends ClassGenerator {
    private final SymbolTable symbol_table_of_the_current_scope_;
    private final int argc_;
    private final boolean has_asterisk_parameter_;
    private final int default_argc_;
    private final boolean is_for_in_expression_;//for..in expression does not introduce new scope
    private final RubyBinding binding_;
    private final FieldManager field_manager_;
    private final String file_name_;
    private final ClassGeneratorForRubyBlockHelper helper;
    private final ClassGenerator owner_;

    public ClassGeneratorForRubyBlock(String name,
            String file_name,
            int argc,
            boolean has_asterisk_parameter,
            int default_argc,
            ClassGenerator owner,
            boolean is_for_in_expression,
            boolean has_extra_comma,
            RubyBinding binding) {
        super(name);
        this.helper = ClassGeneratorForRubyBlockHelper.getHelper(argc, has_asterisk_parameter, is_for_in_expression, has_extra_comma);
        this.file_name_ = file_name;
        symbol_table_of_the_current_scope_ = owner.getSymbolTable();
        mg_for_run_method_ = visitRubyBlock();
        argc_ = argc;
        has_asterisk_parameter_ = has_asterisk_parameter;
        default_argc_ = default_argc;
        is_for_in_expression_ = is_for_in_expression;
        binding_ = binding;
        field_manager_ = new FieldManager(
            (owner instanceof ClassGeneratorForRubyBlock) ? ((ClassGeneratorForRubyBlock)owner).field_manager_ : null,
                    this);
        owner_ = owner;
    }

    String getOrginalMethodName() {
        if (owner_ instanceof ClassGeneratorForRubyMethod) {
            return ((ClassGeneratorForRubyMethod)owner_).getOrginalMethodName();
        } else if (owner_ instanceof ClassGeneratorForRubyBlock) {
            return ((ClassGeneratorForRubyBlock)owner_).getOrginalMethodName();
        } else {
            return null;
        }
    }

    private String findAndSetupPreviousBlocks(String var){
        if (owner_ instanceof ClassGeneratorForRubyBlock) {
            String s = ((ClassGeneratorForRubyBlock)owner_).findAndSetupPreviousBlocks(var);
            if (null != s) {
                getSymbolTable().addVariblesAssignedInBlock(s, -1, new String[]{var});
            }
            return s;
        } else {
            if (((SymbolTableForBlock)getSymbolTable()).isDefinedInOwnerScope(var)) {
                field_manager_.addAssignedField(var, false);
                return name_;
            } else {
                return null;
            }
        }
    }

    public void callSuperMethod(boolean has_no_arg, boolean has_one_arg) {
        if (has_one_arg ||
            (has_no_arg && OwnerHasOneArg())) {
            getMethodGenerator().RubyAPI_callSuperOneArgMethod();
        } else if (has_no_arg && OwnerHasNoArg()) {
            getMethodGenerator().RubyAPI_callSuperNoArgMethod();
        } else {
            getMethodGenerator().RubyAPI_callSuperMethod();
        }
    }

    public void loadNoParameterForSuper() {
        if (OwnerHasOneArg()) {
            getMethodGenerator().RubyBlock_argOfCurrentMethod_();
        } else {
            getMethodGenerator().RubyBlock_argsOfCurrentMethod_();
        }
    }

    private boolean OwnerHasOneArg() {
        if (owner_ instanceof ClassGeneratorForRubyBlock) {
            return ((ClassGeneratorForRubyBlock)owner_).OwnerHasOneArg();
        } else if (owner_ instanceof ClassGeneratorForOneArgRubyMethod) {
            return true;
        } else {
            assert(owner_ instanceof ClassGeneratorForRubyMethod);
            return false;
        }
    }

    private boolean OwnerHasNoArg() {
        if (owner_ instanceof ClassGeneratorForRubyBlock) {
            return ((ClassGeneratorForRubyBlock)owner_).OwnerHasNoArg();
        } else if (owner_ instanceof ClassGeneratorForNoArgRubyMethod) {
            return true;
        } else {
            assert(owner_ instanceof ClassGeneratorForRubyMethod);
            return false;
        }
    }

    private void loadField(String name) {
        mg_for_run_method_.loadThis();
        mg_for_run_method_.getField(Type.getType("L" + name_ + ";"), decorateName(name), Types.RUBY_VALUE_TYPE);
    }

    private void storeField(String name) {
        getMethodGenerator().loadThis();
        getMethodGenerator().swap();
        getMethodGenerator().putField(Type.getType("L" + name_ + ";"), decorateName(name), Types.RUBY_VALUE_TYPE);
    }

    private void addNewFieldToClass(String name) {
        FieldVisitor fv = cv_.visitField(Opcodes.ACC_PUBLIC,
            decorateName(name),
            Type.getDescriptor(Types.RUBY_VALUE_CLASS),
            null,
            null
            );
        fv.visitEnd();
    }

    public String[] getFields() {
        return field_manager_.getFields();
    }

    public String[] getAssignedFields() {
        return field_manager_.getAssignedFields();
    }

    public String[] getPreviousBlocks() {
        return field_manager_.getPreviousBlocks();
    }

    private void addNewBlockFieldToClass(String name) {
        FieldVisitor fv = cv_.visitField(Opcodes.ACC_PUBLIC,
            getNameFromFullpath(name),
            Type.getType("L" + name + ";").getDescriptor(),
            null,
            null
            );
        fv.visitEnd();
    }

    static String getNameFromFullpath(String s) {
        return '$' + s.substring(s.lastIndexOf('/') + 1);
    }

    void getSharedBlock(String name) {
        int i = getSymbolTable().getBlock(name);
        if (i >= 0) {
            getMethodGenerator().loadLocal(i);
        } else {
            getMethodGenerator().loadThis();
            if (name != name_) {
               getMethodGenerator().getField(Type.getType("L" + name_ + ";"), getNameFromFullpath(name), Type.getType("L" + name + ";"));
            }
        }
    }

    public void loadVariable(String name) {
        if (isDefinedInOwnerScope(name)) {
            //findAndSetupPreviousBlocks(name);
            SymbolTable.VariableAssignedInBlock record = getSymbolTable().getVariableAssignedInBlock(name);
            if (null != record) {
                //have been assigned in previous block. e.g. x=1;1.times{x=2};1.times{x=3}
                field_manager_.addPreviousBlock(record.block_name_);
                getSharedBlock(record.block_name_);
                mg_for_run_method_.getField(Type.getType("L" + record.block_name_ + ";"), decorateName(name), Types.RUBY_VALUE_TYPE);
            } else {
                field_manager_.addField(name);
                loadField(name);
            }
        } else {
            super.loadVariable(name);
        }
    }

    public void storeVariable(String name) {
        if (is_for_in_expression_ || isDefinedInOwnerScope(name)) {
            findAndSetupPreviousBlocks(name);
            SymbolTable.VariableAssignedInBlock record = getSymbolTable().getVariableAssignedInBlock(name);
            if (null != record) {
                field_manager_.addPreviousBlock(record.block_name_);
                getSharedBlock(record.block_name_);
                getMethodGenerator().swap();
                getMethodGenerator().putField(Type.getType("L" + record.block_name_ + ";"), decorateName(name), Types.RUBY_VALUE_TYPE);
            } else {
                field_manager_.addAssignedField(name, is_for_in_expression_);
                storeField(name);
                if (is_for_in_expression_) {
                    updateBinding(binding_, name);
                }
            }
        } else {
            super.storeVariable(name);
        }
    }

    private void initialFiledUsingBlockParameter(String name) {
        if (isDefinedInOwnerScope(name)) {
            field_manager_.addAssignedField(name, false);
            getMethodGenerator().loadThis();
            super.loadVariable(name);
            getMethodGenerator().putField(Type.getType("L" + name_ + ";"), decorateName(name), Types.RUBY_VALUE_TYPE);
        }
    }

    public void addParameter(String name) {
        super.addParameter(name);
        initialFiledUsingBlockParameter(name);
    }

    public void setAsteriskParameter(String name, int argc) {
        super.setAsteriskParameter(name, argc);
        initialFiledUsingBlockParameter(name);
    }

    public void setBlockParameter(String name) {
        super.setBlockParameter(name);
        initialFiledUsingBlockParameter(name);
    }

    private MethodGenerator visitRubyBlock() {
        cv_.visit(CgConfig.TARGET_VERSION,
                Opcodes.ACC_PUBLIC,		//need to modify fields when doing Proc#call, see RubyProc.java
                name_,
                null,								// signature
                this.helper.getSuperName(),	// superName
                null								// interface
                );

        // set source file's name, for debug
        if(file_name_ != null) {
            cv_.visitSource(file_name_, null);
        }

        return new MethodGenerator(Opcodes.ACC_PROTECTED,
                Method.getMethod(this.helper.getRunMethodName()),
                cv_,
                null,
                symbol_table_of_the_current_scope_,
                false);
    }

    static String buildContructorSignature() {
        return "void <init> (com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyValue, com.xruby.runtime.builtin.RubyArray, com.xruby.runtime.lang.RubyBlock, com.xruby.runtime.lang.RubyModule, com.xruby.runtime.lang.RubyMethod, boolean)";
    }

    public void createFieldsAndConstructorOfRubyBlock(String[] commons, String[] blocks) {
        createConstructorOfRubyBlock();
        for (String name : commons) {
            addNewFieldToClass(name);
        }
        for (String name : blocks) {
            addNewBlockFieldToClass(name);
        }
    }

    private void createConstructorOfRubyBlock() {

        MethodGenerator mg = new MethodGenerator(Opcodes.ACC_PUBLIC,
                Method.getMethod(buildContructorSignature()),
                cv_,
                null,
                symbol_table_of_the_current_scope_,
                false);

        mg.loadThis();

        this.helper.pushBasciArgForSuperArg(mg, argc_, has_asterisk_parameter_, default_argc_);

        final int FIXED_PARAMETERS = 7;
        for (int i = 0; i < FIXED_PARAMETERS; ++i) {
            mg.loadArg(i);
        }

        mg.invokeConstructor(this.helper.getSuperClassType(),
                Method.getMethod(this.helper.getSuperCtorName()));

        mg.returnValue();
        mg.endMethod();
    }

    private void addVariableToBinding(String s) {
        MethodGenerator mg = getMethodGenerator();
		mg.push(s);
        field_manager_.addField(s);
        loadField(s);
        mg.invokeVirtual(Types.RUBY_BINDING_TYPE,
        		CgUtil.getMethod("addVariable", Types.RUBY_BINDING_TYPE, Type.getType(String.class), Types.RUBY_VALUE_TYPE));
    }

    public void createBinding(boolean isInClassBuilder, boolean isInSingletonMethod, boolean isInGlobalScope, boolean isInBlock) {
        super.createBinding(isInClassBuilder, isInSingletonMethod, isInGlobalScope, isInBlock);

        //TODO one level look up is not enough
        Collection<String> vars = symbol_table_of_the_current_scope_.getLocalVariables();
        for (String s : vars) {
            addVariableToBinding(s);
        }

        Collection<String> params = symbol_table_of_the_current_scope_.getParameters();
        for (String s : params) {
            addVariableToBinding(s);
        }
    }

    public boolean isDefinedInOwnerScope(String name) {
        SymbolTableForBlock st = (SymbolTableForBlock)getSymbolTable();
        return st.isDefinedInOwnerScope(name);
    }

    public boolean isDefinedInCurrentScope(String name) {
        if (isDefinedInOwnerScope(name)) {
            return true;
        } else {
            return super.isDefinedInCurrentScope(name);
        }
    }

    public void loadArgOfMethodForBlock() {
        getMethodGenerator().RubyBlock_argOfCurrentMethod_();
        getMethodGenerator().RubyBlock_argsOfCurrentMethod_();
    }
}

