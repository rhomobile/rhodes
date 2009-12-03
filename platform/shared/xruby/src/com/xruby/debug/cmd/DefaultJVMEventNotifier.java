/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug.cmd;

import com.sun.jdi.AbsentInformationException;
import com.sun.jdi.IncompatibleThreadStateException;
import com.sun.jdi.Location;
import com.sun.jdi.StackFrame;
import com.sun.jdi.event.BreakpointEvent;
import com.sun.jdi.event.ClassPrepareEvent;
import com.sun.jdi.event.Event;
import com.sun.jdi.event.VMStartEvent;
import com.xruby.debug.JVMEventNotifier;
import com.xruby.debug.ThreadInfo;
import com.xruby.debug.SourceCodeMgr;

import static java.lang.System.err;
import static java.lang.System.out;

/**
 * Implementation for JVMEventNotifier
 * It's only for command line, so we don't use MessageCenter
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public class DefaultJVMEventNotifier implements JVMEventNotifier {

    public void vmStartEvent(VMStartEvent e) {
        Thread.yield();  // fetch output
        out.println("VM Started:");
    }

    public void receivedEvent(Event event) {
    }

    public void vmInterrupted() {
        Thread.yield();
        printCurrentLocation();
    }

    public void classPrepareEvent(ClassPrepareEvent e) {
//        System.out.println("ClassPrepared event");
    }

    public void breakpointEvent(BreakpointEvent e) {
        System.out.println("A breakpoint hit");
    }


    private void printCurrentLocation() {
        ThreadInfo threadInfo = ThreadInfo.getCurrentThreadInfo();
        StackFrame frame;
        try {
            frame = threadInfo.getCurrentFrame();
        } catch (IncompatibleThreadStateException exc) {
            err.println("<location unavailable>");
            return;
        }
        if (frame == null) {
            err.println("No frames on the current call stack");
        } else {
            Location loc = frame.location();
            // TODO: Print out source code
            try {
                String sourceFile = loc.sourceName();
                int lineNumber = loc.lineNumber();
                if(lineNumber > 0) {
                    out.println(
                            String.format("Breakpoint: %s:%d", sourceFile, lineNumber));
                    out.println(SourceCodeMgr.sourceLine(sourceFile, lineNumber));
                }
            } catch (AbsentInformationException e) {
                err.println("Debug information lost");
            }
        }
        // Output the current source line, if possible
//            if (loc.lineNumber() != -1) {
//                String line;
//                try {
//                    line = Env.sourceLine(loc, loc.lineNumber());
//                } catch (java.io.IOException e) {
//                    line = null;
//                }
//                if (line != null) {
//                    MessageOutput.println("source line number and line",
//                            new Object[]{new Integer(loc.lineNumber()),
//                                    line});
//                }
//            }
//        }
//        MessageOutput.println();
    }
}
