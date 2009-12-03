/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug;

import com.sun.jdi.ThreadReference;
import com.sun.jdi.request.EventRequestManager;
import com.sun.jdi.request.StepRequest;

import java.util.List;

/**
 * Next step
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public class NextInsn implements Instruction {
    private static final String[] excludes = {"java.*", "javax.*", "sun.*", "com.sun.*", "com.xruby.*"};
    
    public Result execute() {
        ThreadInfo threadInfo = ThreadInfo.getCurrentThreadInfo();
        if (threadInfo == null) {
            return null;
        }
        clearPreviousStep(threadInfo.getThread());
        EventRequestManager reqMgr = DebugContext.getEventRequestManager();
        StepRequest request = reqMgr.createStepRequest(threadInfo.getThread(),
                StepRequest.STEP_LINE,
                StepRequest.STEP_OVER);
        addExcludes(request);
        // We want just the next step event and no others
        request.addCountFilter(1);
        request.enable();
        ThreadInfo.invalidateAll();
        DebugContext.getJVM().resume();
        
        return null;
    }

    private void clearPreviousStep(ThreadReference thread) {
        EventRequestManager reqMgr = DebugContext.getEventRequestManager();
        List<StepRequest> requests = reqMgr.stepRequests();
        for (StepRequest request : requests) {
            if (request.thread().equals(thread)) {
                reqMgr.deleteEventRequest(request);
                break;
            }
        }
    }

    private void addExcludes(StepRequest request) {
        for(String pattern: excludes) {
            request.addClassExclusionFilter(pattern);
        }
    }
}
