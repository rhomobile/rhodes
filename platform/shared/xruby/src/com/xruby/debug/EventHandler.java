/*
 * @(#)ThreadInfo.java	1.24 03/12/19
 *
 * Copyright 2004 Sun Microsystems, Inc. All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
/*
 * Copyright (c) 1997-1999 by Sun Microsystems, Inc. All Rights Reserved.
 *
 * Sun grants you ("Licensee") a non-exclusive, royalty free, license to use,
 * modify and redistribute this software in source and binary code form,
 * provided that i) this copyright notice and license appear on all copies of
 * the software; and ii) Licensee does not utilize the software in a manner
 * which is disparaging to Sun.
 *
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY
 * IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN AND ITS LICENSORS SHALL NOT BE
 * LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN OR ITS
 * LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT,
 * INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF
 * OR INABILITY TO USE SOFTWARE, EVEN IF SUN HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 * This software is not designed or intended for use in on-line control of
 * aircraft, air traffic, aircraft navigation or aircraft communications; or in
 * the design, construction, operation or maintenance of any nuclear
 * facility. Licensee represents and warrants that it will not use or
 * redistribute the Software for such purposes.
 */
/*
 * Copyright 2006-2007
 * Distributed under the BSD License
 *
 * Attention !!!:
 * This implementation is based on the ThreadInfo in com.sun.tools.example.debug.tty package
 * which is implemented by Sun Microsystems.
 * The detail of that is in the bottom of this file.
 */
/*
 * Copyright 2006-2007, current implementation is based on the Sun's EvenHandler
 */
package com.xruby.debug;

import com.sun.jdi.ThreadReference;
import com.sun.jdi.VMDisconnectedException;
import com.sun.jdi.event.BreakpointEvent;
import com.sun.jdi.event.ClassPrepareEvent;
import com.sun.jdi.event.ClassUnloadEvent;
import com.sun.jdi.event.Event;
import com.sun.jdi.event.EventIterator;
import com.sun.jdi.event.EventQueue;
import com.sun.jdi.event.EventSet;
import com.sun.jdi.event.ExceptionEvent;
import com.sun.jdi.event.LocatableEvent;
import com.sun.jdi.event.MethodEntryEvent;
import com.sun.jdi.event.MethodExitEvent;
import com.sun.jdi.event.StepEvent;
import com.sun.jdi.event.ThreadDeathEvent;
import com.sun.jdi.event.ThreadStartEvent;
import com.sun.jdi.event.VMDeathEvent;
import com.sun.jdi.event.VMDisconnectEvent;
import com.sun.jdi.event.VMStartEvent;
import com.sun.jdi.event.WatchpointEvent;
import com.sun.jdi.request.EventRequest;

/**
 * JVM Event Handler
 */
public class EventHandler implements Runnable {
    private boolean connected = true;
    private JVMEventNotifier notifier;
    private boolean completed = false;
    private boolean stopOnVMStart = false;
    private Thread thread;

    EventHandler(JVMEventNotifier notifier, boolean stopOnVMStart) {
        this.notifier = notifier;
        this.stopOnVMStart = stopOnVMStart;
        this.thread = new Thread(this, "event-handler");
        this.thread.start();
    }

    public void run() {
        EventQueue queue = DebugContext.getJVM().eventQueue();
        while (connected) {
            try {
                EventSet eventSet = queue.remove();
                boolean resumeStoppedApp = false;
                EventIterator it = eventSet.eventIterator();
                while (it.hasNext()) {
                    resumeStoppedApp |= !handleEvent(it.nextEvent());
                }

                if (resumeStoppedApp) {
                    eventSet.resume();
                } else if (eventSet.suspendPolicy() == EventRequest.SUSPEND_ALL) {
                    setCurrentThread(eventSet);
                    notifier.vmInterrupted();
                }
            } catch (InterruptedException exc) {
                // Do nothing. Any changes will be seen at top of loop.
            } catch (VMDisconnectedException discExc) {
                //handleDisconnectedException();
                break;
            }
        }
        synchronized (this) {
            completed = true;
            notifyAll();
        }
    }

    private boolean handleEvent(Event event) {
        notifier.receivedEvent(event);

        if (event instanceof ExceptionEvent) {
            return true; //exceptionEvent(event);
        } else if (event instanceof BreakpointEvent) {
            return breakpointEvent(event);
        } else if (event instanceof WatchpointEvent) {
            return true; //fieldWatchEvent(event);
        } else if (event instanceof StepEvent) {
            return true; //stepEvent(event);
        } else if (event instanceof MethodEntryEvent) {
            return true; //methodEntryEvent(event);
        } else if (event instanceof MethodExitEvent) {
            return true; //methodExitEvent(event);
        } else if (event instanceof ClassPrepareEvent) {
            return classPrepareEvent(event);
        } else if (event instanceof ClassUnloadEvent) {
            return true; //classUnloadEvent(event);
        } else if (event instanceof ThreadStartEvent) {
            return threadStartEvent(event);
        } else if (event instanceof ThreadDeathEvent) {
            return threadDeathEvent(event);
        } else if (event instanceof VMStartEvent) {
            return vmStartEvent(event);
        } else {
            return handleExitEvent(event);
        }
    }

    private boolean handleExitEvent(Event event) {
        if (event instanceof VMDeathEvent) {
            DebugContext.emitMessage("Application existed");
        } else if (event instanceof VMDisconnectEvent) {
            DebugContext.emitMessage("VM disconneted");
            DebugContext.shutdown();
        }
        
        return false;
    }

    private boolean threadDeathEvent(Event event) {
        ThreadDeathEvent threadDeathEvent = (ThreadDeathEvent) event;
        
        return false;
    }

    private boolean threadStartEvent(Event event) {
        ThreadStartEvent threadStartEvent = (ThreadStartEvent) event;
        ThreadInfo.addThread(threadStartEvent.thread());

        return false;
    }

    private boolean breakpointEvent(Event event) {
        BreakpointEvent be = (BreakpointEvent) event;
        notifier.breakpointEvent(be);
        return true;
    }

    private boolean vmStartEvent(Event event) {
        VMStartEvent se = (VMStartEvent) event;
        notifier.vmStartEvent(se);
        return stopOnVMStart;
    }

    private boolean classPrepareEvent(Event event) {
        ClassPrepareEvent cle = (ClassPrepareEvent) event;
        notifier.classPrepareEvent(cle);
        EventRequestHandler.resolveAllDeferred(cle);

        // One way or another, the debugger should go on even some deferred command
        // couldn't be executed correctly
        // TODO: What's the failed execution bring us? Should we consider that?
        return false;
    }

    private void setCurrentThread(EventSet set) {
        ThreadReference thread;
        if (set.size() > 0) {
            /*
             * If any event in the set has a thread associated with it,
             * they all will, so just grab the first one.
             */
            Event event = set.iterator().next(); // Is there a better way?
            thread = eventThread(event);
        } else {
            thread = null;
        }
        setCurrentThread(thread);
    }

    private void setCurrentThread(ThreadReference thread) {
        ThreadInfo.invalidateAll();
        ThreadInfo.setCurrentThread(thread);
    }

    private ThreadReference eventThread(Event event) {
        if (event instanceof ClassPrepareEvent) {
            return ((ClassPrepareEvent) event).thread();
        } else if (event instanceof LocatableEvent) {
            return ((LocatableEvent) event).thread();
        } else if (event instanceof ThreadStartEvent) {
            return ((ThreadStartEvent) event).thread();
        } else if (event instanceof ThreadDeathEvent) {
            return ((ThreadDeathEvent) event).thread();
        } else if (event instanceof VMStartEvent) {
            return ((VMStartEvent) event).thread();
        } else {
            return null;
        }
    }
}
