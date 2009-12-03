/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug;

import com.sun.jdi.ReferenceType;
import com.sun.jdi.event.ClassPrepareEvent;
import com.sun.jdi.request.ClassPrepareRequest;

import java.util.List;
import java.util.ArrayList;

/**
 * Event Request Handler, for ClassPrepareRequest
 * Instructions like stop gonna need extend this 
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public abstract class EventRequestHandler {
    protected ClassPrepareRequest prepareRequest;
    private String classId;

    public void preSolved() {
        prepareRequest = DebugContext.getEventRequestManager().createClassPrepareRequest();
        prepareRequest.addClassFilter(classId);
        prepareRequest.addCountFilter(1);
        prepareRequest.enable();
    }

    public abstract Result resolveEventRequest(ReferenceType refType);

    public Result solveEvent(ClassPrepareEvent event) {
        ReferenceType refType = event.referenceType();
        Result result = resolveEventRequest(refType);

        if(result.isSuccessful()) {
            // Disable this class's prepare event and delete it
            prepareRequest.disable();
            DebugContext.getEventRequestManager().deleteEventRequest(prepareRequest);
        }

        return result;
    }

    protected ReferenceType isClassPrepared() {
        List<ReferenceType> clazzs = DebugContext.getJVM().allClasses();
        for(ReferenceType refType: clazzs) {
            if(refType.isPrepared() && refType.name().equals(classId)) {
                return refType;
            }
        }

        return null;
    }

    protected String getClassId() {
        return classId;
    }

    protected void setClassId(String classId) {
        this.classId = classId;
    }

    public ClassPrepareRequest getPrepareRequest() {
        return prepareRequest;
    }

    public void setPrepareRequest(ClassPrepareRequest prepareRequest) {
        this.prepareRequest = prepareRequest;
    }


    /**
     * Execute all the commands deferred,
     * because they're all waiting for the ReferenceType.
     * These commands used to be comitted before JVM starts
     *
     * @param event ClassPrepareEvent
     */
    public static void resolveAllDeferred(ClassPrepareEvent event) {
        List<Instruction> deferredInsns = DebugContext.getDeferredInsns();
        List<Instruction> recycleBin = new ArrayList<Instruction>();
        for (Instruction insn : deferredInsns) {
            if (insn instanceof EventRequestHandler) {
                EventRequestHandler handler = (EventRequestHandler) insn;
                Result result = handler.solveEvent(event);

                if (result.isSuccessful()) {
                    recycleBin.add(insn);
                }

            }
        }

        // Remove the commands who have been executed successfully
        for(Instruction insn: recycleBin) {
            deferredInsns.remove(insn);
        }
    }

    /**
     * Pre-Solve the deferred commands
     */
    public static void presolveAllDeferred() {
        List<Instruction> deferredInsns = DebugContext.getDeferredInsns();
        for (Instruction insn : deferredInsns) {
            if (insn instanceof EventRequestHandler) {
                EventRequestHandler handler = (EventRequestHandler) insn;
                handler.preSolved();
            }
        }
    }
}
