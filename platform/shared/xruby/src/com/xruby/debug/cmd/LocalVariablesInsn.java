/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug.cmd;

import com.sun.jdi.AbsentInformationException;
import com.sun.jdi.IncompatibleThreadStateException;
import com.sun.jdi.LocalVariable;
import com.sun.jdi.StackFrame;
import com.sun.jdi.Value;
import com.xruby.debug.Instruction;
import com.xruby.debug.Result;
import com.xruby.debug.ThreadInfo;

import static java.lang.System.out;
import java.util.List;
import java.util.Map;

/**
 * Commandline only
 * @author Yu Su (beanworms@gmail.com)
 */
public class LocalVariablesInsn implements Instruction {

    public Result execute() {
        StackFrame frame;
        ThreadInfo threadInfo = ThreadInfo.getCurrentThreadInfo();
        if (threadInfo == null) {
            out.println("No default thread specified:");
            return null;
        }
        try {
            frame = threadInfo.getCurrentFrame();
            if (frame == null) {
                throw new AbsentInformationException();
            }
            List vars = frame.visibleVariables();

            if (vars.size() == 0) {
                out.println("No local variables");
                return null;
            }
            Map<LocalVariable, Value> values = frame.getValues(vars);

            out.println("Method arguments:");
            for (Object variable : vars) {
                LocalVariable var = (LocalVariable) variable;
                if (var.isArgument()) {
                    Value val = values.get(var);
                    printVar(var, val);
                }
            }
            out.println("Local variables:");
            for (Object variable : vars) {
                LocalVariable var = (LocalVariable) variable;
                if (!var.isArgument()) {
                    Value val = values.get(var);
                    printVar(var, val);
                }
            }
        } catch (AbsentInformationException aie) {
            out.println("Local variable information not available.");
        } catch (IncompatibleThreadStateException exc) {
            out.println("Current thread isnt suspended.");
        }

        return null;
    }

    private void printVar(LocalVariable var, Value value) { 
        out.printf("%s: %s", var.name(), value != null? value.toString(): null);
    }
}
