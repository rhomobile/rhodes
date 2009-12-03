/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug;

import com.sun.jdi.AbsentInformationException;
import com.sun.jdi.Location;
import com.sun.jdi.ReferenceType;
import com.sun.jdi.request.EventRequest;
import com.sun.jdi.request.EventRequestManager;
import com.sun.tools.example.debug.tty.AmbiguousMethodException;
import com.sun.tools.example.debug.tty.LineNotFoundException;

import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.ArrayList;

/**
 * Stop instruction, there are two way to use this instruction
 * . Add a breakpoint in a specified position of a file/class
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public class StopInsn extends EventRequestHandler implements Instruction {

    private static Map<String, EventRequest> resolved;   // resolved break points
    private static List<String> all;                     // all break points

    static {
        resolved = new HashMap<String, EventRequest>();
        all = new ArrayList<String>();
    }

    private int lineNumber = -1;

    /**
     * Constructor
     *
     * @param classId    Class identifier
     * @param lineNumber Line number
     * @throws XRubyDebugException exception
     */
    public StopInsn(String classId, int lineNumber) throws XRubyDebugException {
        this.lineNumber = lineNumber;
        setClassId(SourceCodeMgr.getRealClass(classId, lineNumber));

        String position = String.format("%s:%d", classId, lineNumber);
        all.add(position);
    }

    /**
     * Implement instruction interface
     * @return result
     */
    public Result execute() {
        Result result = new Result();
        if(DebugContext.isStarted()) {
            ReferenceType refType = isClassPrepared();
            if(refType != null) {
                return resolveEventRequest(refType);
            }
            else {
                preSolved();
                result.setStatus(Result.Status.SUCCESSFUL);
                return result;
            }
        }
        else {
            result.setStatus(Result.Status.DEFERRED);
            DebugContext.pushCommand(this);
            return result;
        }
    }
        

    /**
     * Implements EventRequestHandler
     */
    public Result resolveEventRequest(ReferenceType refType) {
        Result result = new Result();
        try {
            Location location = location(refType);
            if (location == null) {
                throw new XRubyDebugException();
            }
            EventRequestManager em = refType.virtualMachine().eventRequestManager();
            EventRequest bp = em.createBreakpointRequest(location);
            bp.setSuspendPolicy(EventRequest.SUSPEND_ALL);
            bp.enable();
            result.setStatus(Result.Status.SUCCESSFUL);

            // store the EventRequest
            String position =
                    String.format("%s:%d", location.sourceName(), location.lineNumber());
            StopInsn.resolved.put(position, bp);
        } catch (Exception e) {
            result.setStatus(Result.Status.ERROR);
            result.setErrMsg(e.getMessage());
        }
        
        return result;
    }

    // --------------------------
    //     static methods
    // --------------------------
    public static List<String> allBreakpoints() {
        return StopInsn.all;
    }

    public static EventRequest getBreakpointReq(String position) {
        return StopInsn.resolved.get(position);
    }

    public static void removeBreakpoint(String position) {
        StopInsn.all.remove(position);
        StopInsn.resolved.remove(position);
    }

    // ----------------------
    //        Readers
    // ----------------------
    public int getLineNumber() {
        return lineNumber;
    }

    // Private methods

    // Validate class's id, return false if it's illegal, otherwise true.
    private boolean validateClassId(String id) {
        // TODO: Check class's id
        return true;
    }

    private Location location(ReferenceType refType) throws
            AmbiguousMethodException,
            AbsentInformationException,
            NoSuchMethodException,
            LineNotFoundException {
        Location location = null;
        // let AbsentInformationException be thrown
        List locs = refType.locationsOfLine(lineNumber);
        if (locs.size() == 0) {
            throw new LineNotFoundException();
        }
        
        location = (Location) locs.get(0);
        if (location.method() == null) {
            throw new LineNotFoundException();
        }
        
        return location;
    }
}
