/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug.cmd;

import com.sun.jdi.AbsentInformationException;
import com.sun.jdi.IncompatibleThreadStateException;
import com.sun.jdi.Location;
import com.sun.jdi.Method;
import com.sun.jdi.StackFrame;
import com.xruby.debug.Instruction;
import com.xruby.debug.Result;
import com.xruby.debug.ThreadInfo;

import static java.lang.System.out;
import java.util.List;

/**
 * Commandline only, dump stack
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public class WhereInsn implements Instruction {

    public Result execute() {
        ThreadInfo threadInfo = ThreadInfo.getCurrentThreadInfo();
        List<StackFrame> stack = null;
        try {
            stack = threadInfo.getStack();
        } catch (IncompatibleThreadStateException e) {
            out.println("Current thread isn't suspended.");
            return null;
        }
        if (stack == null) {
            out.println(
                    "No stack is available in current thread, maybe the program is not running");
        } else {
            int nFrames = stack.size();
            for (int i = threadInfo.getCurrentFrameIndex(); i < nFrames; i++) {
                StackFrame frame = stack.get(i);
                dumpFrame(frame);
            }
        }
        return null;
    }

    private void dumpFrame(StackFrame frame) {
        Location location = frame.location();

        Method meth = location.method();

        long lineNumber = location.lineNumber();
        String methodInfo = null;
        if (meth != null && meth.isNative()) {
            methodInfo = "native method";
        } else if (lineNumber != -1) {
            try {
                methodInfo = location.sourceName() + ":" + lineNumber;
            } catch (AbsentInformationException e) {
                methodInfo = "unknown";
            }
        }

        String type = meth.declaringType().name();
        if (type.indexOf("com.xruby") == -1) {
            out.printf(" %s.%s (%s)\n", type, meth.name(), methodInfo);
        }
    }
}
