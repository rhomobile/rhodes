/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug;

import com.sun.jdi.VirtualMachine;

import java.util.Map;

/**
 * Abstract FrontEnd
 * If you wanna create a new FrontEnd, e.g. a GUI debugger:
 * 1. Initiate FrontEnd using the protected constructor.
 * 2. Retrieve instruction, and pass it to the distributeCommand method
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public abstract class FrontEnd {

    /**
     * Validate arguments, LAUNCH, MAIN, OPTIONS
     *
     * @param arguments arguments
     * @throws XRubyDebugException throw exception if no entrance
     */
    protected FrontEnd(Map<String, String> arguments) throws XRubyDebugException {
        if (arguments.containsKey(DebugConstant.MAIN)) {
            arguments.put(DebugConstant.LAUNCH, getLaunchMode());
        } else {
            throw new XRubyDebugException("No entrance");
        }

        // Initiate Context
        int traceMode = getTraceMode();

        // Maybe monostate is better for DebugContext
        DebugContext.initContext(traceMode, arguments);
        DebugContext.registerFrontEnd(this);
        DebugContext.validateContext();
    }

    protected Result run() {
        return new RunInsn().execute();
    }

    protected Result stop(String classId, int lineNumber) {
        StopInsn insn = null;
        try {
            insn = new StopInsn(classId, lineNumber);
        } catch (XRubyDebugException e) {
            Result result = new Result();
            result.setStatus(Result.Status.ERROR);
            result.setErrMsg(e.getMessage());

            return result;
        }        

        return insn.execute();
    }

    protected Result cont() {
        return new ContinueInsn().execute();
    }

    protected Result stepOver() {
        return new NextInsn().execute();
    }


    // --------------------------------------
    // protected (abstract) methos
    // --------------------------------------

    /**
     * Return lauch mode, e.g.
     * com.sun.jdi.CommandLineLaunch, attach etc.
     *
     * @return mode
     */
    protected abstract String getLaunchMode();

    protected abstract void handleResult(Result result);

    protected int getTraceMode() {
        return VirtualMachine.TRACE_NONE;
    }

    public abstract void start() throws XRubyDebugException ;

    public abstract void onVmShutdown();
}
