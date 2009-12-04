/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug;

import com.sun.jdi.Bootstrap;
import com.sun.jdi.VirtualMachine;
import com.sun.jdi.connect.Connector;
import com.sun.jdi.connect.IllegalConnectorArgumentsException;
import com.sun.jdi.connect.LaunchingConnector;
import com.sun.jdi.connect.VMStartException;
import com.sun.jdi.request.EventRequestManager;
import com.sun.jdi.request.ThreadDeathRequest;
import com.sun.jdi.request.ThreadStartRequest;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import static java.lang.System.err;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Connection to JVM
 */
public class JVMConnection {

    private VirtualMachine jvm;
    private Process process;

    // Read only attributes
    private final Connector connector;
    private final int traceFlag;
    private final Map<String, Connector.Argument> connectorArgs;

    JVMConnection(int traceFlag, Map<String, String> arguments) {
        String launchMethod = arguments.get(DebugConstant.LAUNCH);
        connector = findConnector(launchMethod);
        if (connector == null) {
            throw new RuntimeException("Wrong connector: " + launchMethod);
        }

        this.traceFlag = traceFlag;
        connectorArgs = initConnectArgs(connector, arguments);
    }

    public VirtualMachine getJvm() {
        return jvm;
    }

    String connectorArg(String name) {
        Connector.Argument argument = connectorArgs.get(name);
        if (argument == null) {
            return "";
        }
        return argument.value();
    }

    // Open JVM
    void start() {
        if (connector instanceof LaunchingConnector) {
            jvm = launchTarget();
        }

        jvm.setDebugTraceMode(traceFlag);

        if(jvm.canBeModified()) {
            setEventRequests(jvm);
            EventRequestHandler.presolveAllDeferred();
        }

        // TODO: AttachLauncher
        // TODO: Listen...
        // TODO: Handle suspending commands, e.g. stop ...
    }

    void shutdown() {
        jvm.dispose();
        jvm = null;
    }

    private Map<String, Connector.Argument> initConnectArgs(Connector connector, Map<String, String> arguments) {
        Map<String, Connector.Argument> defaultArgs = connector.defaultArguments();
        Set<String> keys = arguments.keySet();

        for (String key : keys) {
            String value = arguments.get(key);
            if (defaultArgs.containsKey(key)) {
                Connector.Argument argument = defaultArgs.get(key);
                argument.setValue(value);
            }
        }

        return defaultArgs;
    }

    private VirtualMachine launchTarget() {
        LaunchingConnector launcher = (LaunchingConnector) connector;
        try {
            VirtualMachine vm = launcher.launch(connectorArgs);
            process = vm.process();
            displayRemoteOutput(process.getErrorStream());
            displayRemoteOutput(process.getInputStream());
            return vm;
        } catch (IOException ioe) {
            ioe.printStackTrace();
            err.println("Unable to launch target VM.");
        } catch (IllegalConnectorArgumentsException icae) {
            icae.printStackTrace();
            err.println("Internal debugger error.");
        } catch (VMStartException vmse) {
            err.println("vmstartexception: " + vmse.getMessage());
            err.println();
            dumpFailedLaunchInfo(vmse.process());
            err.println("Target VM failed to initialize.");
        }
        return null; // Shuts up the compiler
    }

    private void setEventRequests(VirtualMachine vm) {
        EventRequestManager erm = vm.eventRequestManager();

        ThreadStartRequest tsr = erm.createThreadStartRequest();
        tsr.enable();
        ThreadDeathRequest tdr = erm.createThreadDeathRequest();
        tdr.enable();
    }

    private Connector findConnector(String specName) {
        List<Connector> connectors = Bootstrap.virtualMachineManager().allConnectors();

        for (Connector connector : connectors) {
            if (connector.name().equals(specName)) {
                return connector;
            }
        }

        return null;
    }

    // Start a thread responsible for input stream
    // read characters and print them out. (dumpStream)
    private void displayRemoteOutput(final InputStream stream) {
        Thread thr = new Thread("output reader") {
            public void run() {
                try {
                    dumpStream(stream);
                } catch (IOException ex) {
                    err.println("Failed reading output");
                } finally {
                    notifyOutputComplete();
                }
            }
        };
        thr.setPriority(Thread.MAX_PRIORITY - 1);
        thr.start();
    }

    private void dumpFailedLaunchInfo(Process process) {
        try {
            dumpStream(process.getErrorStream());
            dumpStream(process.getInputStream());
        } catch (IOException e) {
            err.println("Unable to display process output:" + e.getMessage());
        }
    }

    // Read every character from stream, output them
    private void dumpStream(InputStream stream) throws IOException {
        BufferedReader in =
                new BufferedReader(new InputStreamReader(stream));
        int i;
        try {
            while ((i = in.read()) != -1) {
                err.print((char) i);// Special case: use
                //   printDirect()
            }
        } catch (IOException ex) {
            String s = ex.getMessage();
            if (!s.startsWith("Bad file number")) {
                throw ex;
            }
            // else we got a Bad file number IOException which just means
            // that the debuggee has gone away.  We'll just treat it the
            // same as if we got an EOF.
        }
    }

    private int outputCompleteCount = 0;

    synchronized void waitOutputComplete() {
        // Wait for stderr and stdout
        if (process != null) {
            while (outputCompleteCount < 2) {
                try {
                    wait();
                } catch (InterruptedException e) {
                }
            }
        }
    }

    synchronized void notifyOutputComplete() {
        outputCompleteCount++;
        notifyAll();
    }
}
