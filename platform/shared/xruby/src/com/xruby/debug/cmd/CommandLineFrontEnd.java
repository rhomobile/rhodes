/* 
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug.cmd;

import com.xruby.debug.DebugCommandLineOptions;
import com.xruby.debug.DebugConstant;
import com.xruby.debug.DebugContext;
import com.xruby.debug.FrontEnd;
import com.xruby.debug.Result;
import com.xruby.debug.XRubyDebugException;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import static java.lang.System.out;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Command Line FrontEnd
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public class CommandLineFrontEnd extends FrontEnd {
    // --------------------
    //   Constant Fields
    // --------------------
    private static final String RUN   = "run";
    private static final String STOP  = "stop";
    private static final String LIST  = "list";
    private static final String CLEAR = "clear";
    private static final String CONT  = "cont";
    private static final String SETP_OVER = "next";
    private static final String LOCALS = "locals";
    private static final String WHERE = "where";

    private static final String CMD_PATTERN_STR =
            "run(\\s*)|stop(\\s*)|list(\\s*)|cont(\\s*)|next(\\s*)|clear(\\s*)|locals(\\s*)|where(\\s*)";
    private static final String STOP_PATTERN_STR =
            "(stop){1}\\s*(at|in)\\s*((([\\w\\$]+)(.rb)*){1}:(\\d+){1})*";
    private static final String LIST_PATTERN_STR =
            "(list)\\s+(\\d+)?";
    private static final String CLEAR_PATTERN_STR =
            "(clear)\\s+(\\d+)?";

    private static final String cmdPrompt = "> ";

    private static Pattern commandPattern = Pattern.compile(CMD_PATTERN_STR);
    private static Pattern stopPattern = Pattern.compile(STOP_PATTERN_STR);
    private static Pattern listPattern = Pattern.compile(LIST_PATTERN_STR);
    private static Pattern clearPattern = Pattern.compile(CLEAR_PATTERN_STR);

    public CommandLineFrontEnd(Map<String, String> arguments) throws XRubyDebugException {
        super(arguments);
    }

    protected String getLaunchMode() {
        return "com.sun.jdi.CommandLineLaunch";
    }

    protected void handleResult(Result result) {
    }

    public void start() throws XRubyDebugException {
        out.println("XRuby debugger ...");

        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        while (true) {
            try {                    
                out.print(cmdPrompt);

                String cmd = in.readLine().trim();
                Matcher m = commandPattern.matcher(cmd);
                if (isInstruction(m)) {
                    String cmdName = m.group().trim();
                    String[] arguments = retrieveArgs(cmdName, cmd);

                    execute(cmdName, arguments);
                } else {
                    DebugContext.emitMessage("Invalid instruction");
                }

                // TODO: We should handle VMdisconnect&VMDeath events in EventHandler
                
            } catch (IOException ioe) {
                throw new XRubyDebugException(ioe);
            }
        }

    }

    /**
     * Create instructions, and execute them
     *
     * @param command command's name
     * @param args    its arguments
     * @return Result
     */
    private Result execute(final String command, String[] args) {
        Result result = null;
        if (command.equalsIgnoreCase(RUN)) {             // run
            result = run();
        } else if (command.equalsIgnoreCase(STOP)) {     // stop at filename:postion
            String classId = args[0];
            int lineNumber = Integer.parseInt(args[1]);

            result = stop(classId, lineNumber);
        } else if (command.equalsIgnoreCase(LIST)) {     // list [range]
            int range = -1;
            if(args != null && args[0] != null) {
                range = Integer.parseInt(args[0]);
            }
            result = new ListInsn(range).execute();
        } else if (command.equalsIgnoreCase(CONT)) {     // cont
            result = cont();
        } else if (command.equalsIgnoreCase(SETP_OVER)) {// next
            result = stepOver();
        } else if (command.equalsIgnoreCase(CLEAR)) {    // clear [index]
            int index = -1;
            if(args != null && args[0] != null) {
                index = Integer.parseInt(args[0]);
            }
            result = new ClearInsn(index).execute();
        } else if(command.equalsIgnoreCase(LOCALS)) {
            result = new LocalVariablesInsn().execute();
        } else if(command.equalsIgnoreCase(WHERE)) {
            result = new WhereInsn().execute();
        }

        return result;
    }

    public void onVmShutdown() {
        System.exit(0);
    }

    private String[] retrieveArgs(String insnName, String cmd) {
        String[] args = null;
        if (insnName.equalsIgnoreCase(STOP)) {
            Matcher m = stopPattern.matcher(cmd);

            if(m.find()) {
                args = new String[2];
                args[0] = m.group(4);
                args[1] = m.group(7);
            }
        } else if (insnName.equalsIgnoreCase(LIST)) {
            Matcher m = listPattern.matcher(cmd);

            if(m.find()) {
                args = new String[1];
                args[0] = m.group(2);
            }
        } else if (insnName.equalsIgnoreCase(CLEAR)) {
            Matcher m = clearPattern.matcher(cmd);

            if(m.find()) {
                args = new String[1];
                args[0] = m.group(2);
            }
        }

        return args;
    }

    private boolean isInstruction(Matcher m) {
        return m.find() && m.start() == 0;
    }

    public static void main(String[] args) throws Exception {
        // Parse the arguments: entrance, options
        DebugCommandLineOptions options = new DebugCommandLineOptions(args);
        Map<String, String> arguments = new HashMap<String, String>();

        // Options, this is optional
        String optionsArg;
        if (options.getClassPath() != null) {
            optionsArg = String.format("-classpath %s", options.getClassPath());
            arguments.put(DebugConstant.OPTIONS, optionsArg);
        }

        // Entrance, required
        String mainArg = String.format("%s", options.getEntrance());
        if (mainArg == null || mainArg.equals("")) {
            throw new XRubyDebugException("No entrance");
        }
        arguments.put(DebugConstant.MAIN, mainArg);

        // initiate debug context
        DebugContext.addSourcePath(options.getPathList());
        DebugContext.setClassPath(options.getClassPathList());
        DebugContext.setNotifier(new DefaultJVMEventNotifier());
        DebugContext.setMessageCenter(new DefaultMessageCenter());

        // Create front end
        FrontEnd frontEnd = new CommandLineFrontEnd(arguments);
                
        frontEnd.start();
    }
}
