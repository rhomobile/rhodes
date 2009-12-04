/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug;

/**
 * @author Yu Su (beanworms@gmail.com)
 */
public class ContinueInsn implements Instruction {
    public Result execute() {
        Result result = new Result();
        if (DebugContext.isStarted()) {
            DebugContext.getJVM().resume();

            result.setStatus(Result.Status.SUCCESSFUL);
        } else {
            DebugContext.emitMessage("VM hasn't started");
        }

        return result;
    }
}
