/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug;

/**
 * Run instruction, only for "launch target"
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public class RunInsn implements Instruction {
    private static final String MESSAGE = "JVM has started already.";
    /**
     * Execute Run instruction, general process is listed as below:
     * 1. Start/Launch the process, pass the arguments to debugee
     * 2. Set Debug track mode
     * 3. Construct&attache the event handler
     * 4. Start threads handling in/error/out streams
     */
    public Result execute() {
        Result result = new Result();
        if(DebugContext.isStarted()) {
            result.setStatus(Result.Status.ERROR);
            result.setErrMsg(MESSAGE);

            return result;
        }

        // Start JVM, program start running
        JVMConnection connection = DebugContext.getJvmConnection();
        connection.start();            

        if ((DebugContext.getHandler() == null) &&
             DebugContext.isStarted())
        {
            DebugContext.setHandler(new EventHandler(DebugContext.getNotifier(), false));
        }

        result.setStatus(Result.Status.SUCCESSFUL);

        return result;
    }
}
