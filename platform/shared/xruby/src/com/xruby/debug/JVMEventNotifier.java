package com.xruby.debug;

import com.sun.jdi.event.BreakpointEvent;
import com.sun.jdi.event.ClassPrepareEvent;
import com.sun.jdi.event.Event;
import com.sun.jdi.event.VMStartEvent;

/**
 * @author Yu Su (beanworms@gmail.com)
 */
public interface JVMEventNotifier {
    public void vmStartEvent(VMStartEvent e);

    public void receivedEvent(Event event);

    public void vmInterrupted();

    public void classPrepareEvent(ClassPrepareEvent e);

    public void breakpointEvent(BreakpointEvent e);
}
