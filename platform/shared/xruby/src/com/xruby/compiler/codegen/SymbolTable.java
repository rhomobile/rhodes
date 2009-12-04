/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.compiler.codegen;

import org.objectweb.asm.Label;

import java.util.*;

class SymbolTable {

    class VariableAssignedInBlock {
        final String block_name_;
        final int block_var_;

        VariableAssignedInBlock(String block_name, int block_var) {
            block_name_ = block_name;
            block_var_ = block_var;
        }
    }

    private final Map<String, Integer> local_variables_ = new HashMap<String, Integer>();
    //all keys in local_variables_assigned_in_block_ also exist in local_variables_
    private final Map<String, VariableAssignedInBlock> local_variables_assigned_in_block_ = new HashMap<String, VariableAssignedInBlock>();
    private final Map<String, Label> localVariableRange = new HashMap<String, Label>();
    private final List<String> declarationSeq = new ArrayList<String>();
    private final List<String> method_parameters_;
    private final Map<String, Integer> asterisk_or_block_method_parameter_ = new HashMap<String, Integer>();

    private static final String NAME_FOR_INTERNAL_BINDING_VAR = "binding$";

    // SymbolTable may have preloaded values (eval, commandline etc)
    public SymbolTable(List<String> binding) {
        if (null == binding) {
            method_parameters_ = new ArrayList<String>();
        } else {
            method_parameters_ = binding;
        }
    }

    public void addVariblesAssignedInBlock(String block_name, int block_var, String[] assigned_commons) {
        for (String s : assigned_commons) {
            if (!local_variables_assigned_in_block_.containsKey(s)) {
                local_variables_assigned_in_block_.put(s, new VariableAssignedInBlock(block_name, block_var));
            }
        }
    }

    public VariableAssignedInBlock getVariableAssignedInBlock(String name) {
        return local_variables_assigned_in_block_.get(name);
    }

    public void setInternalBindingVar(int i) {
        addLocalVariable(NAME_FOR_INTERNAL_BINDING_VAR, i);
    }

    public int getInternalBindingVar() {
        return getLocalVariable(NAME_FOR_INTERNAL_BINDING_VAR);
    }

    public Collection<String> getLocalVariables() {
        Collection<String> r = local_variables_.keySet();
        //r.remove(NAME_FOR_INTERNAL_BINDING_VAR);
        return r;
    }

    public Collection<String> getParameters() {
        return method_parameters_;
    }

    public void addLocalVariable(String name, int i) {
        local_variables_.put(name, i);
        localVariableRange.put(name, null);
    }

    public void addAsteriskOrBlockMethodParameter(String name, int i) {
        asterisk_or_block_method_parameter_.put(name, i);
    }

    public int getLocalVariable(String name) {
        Integer i = local_variables_.get(name);
        if (null == i) {
            return -1;
        } else {
            return i.intValue();
        }
    }

    public int getAsteriskOrBlockMethodParameter(String name) {
        Integer i = asterisk_or_block_method_parameter_.get(name);
        if (null == i) {
            return -1;
        } else {
            return i.intValue();
        }
    }

    public void addMethodParameter(String name) {
        method_parameters_.add(name);
    }

    public int getMethodParameter(String name) {
        return method_parameters_.indexOf(name);
    }

    public boolean isDefinedInCurrentScope(String name) {
        if (getLocalVariable(name) >= 0) {
            return true;
        } else if (getAsteriskOrBlockMethodParameter(name) >= 0) {
            return true;
        } else if (getMethodParameter(name) >= 0) {
            return true;
        } else {
            return false;
        }
    }

    public boolean isNewLocalVar(String var) {
        return localVariableRange.get(var) == null;
    }

    public void setVarLineNumberInfo(String var, Label label) {
        localVariableRange.put(var, label);
        declarationSeq.add(var);
    }

    public Map<String, Label> getLocalVariableRange() {
        return localVariableRange;
    }

    public List<String> getDeclarationSeq() {
        return declarationSeq;
    }

    public int getBlock(String name) {
        for (VariableAssignedInBlock record : local_variables_assigned_in_block_.values()) {
            if (name.equals(record.block_name_)) {
                return record.block_var_;
            }
        }

        return -1;
    }
}


class SymbolTableForBlock extends SymbolTable {
    private SymbolTable owner_;

    public SymbolTableForBlock(List<String> binding, SymbolTable owner) {
        super(binding);
        owner_ = owner;
    }

    public boolean isDefinedInCurrentScope(String s) {
        if (super.isDefinedInCurrentScope(s)) {
            return true;
        } else {
            return owner_.isDefinedInCurrentScope(s);
        }
    }

    public VariableAssignedInBlock getVariableAssignedInBlock(String name) {
        VariableAssignedInBlock v = super.getVariableAssignedInBlock(name);
        if (null != v) {
            return v;
        } else {
            return owner_.getVariableAssignedInBlock(name);
        }
    }

    public boolean isDefinedInOwnerScope(String s) {
        return owner_.isDefinedInCurrentScope(s);
    }
}

