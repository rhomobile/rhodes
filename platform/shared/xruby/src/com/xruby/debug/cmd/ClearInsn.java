package com.xruby.debug.cmd;

import com.sun.jdi.request.EventRequest;
import com.xruby.debug.Instruction;
import com.xruby.debug.Result;
import com.xruby.debug.StopInsn;
import com.xruby.debug.DebugContext;

import java.util.List;

/**
 * Clear/List breakpoint
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public class ClearInsn implements Instruction {

    private int index;
    private String position;

    public ClearInsn(int index) {
        this.index = index;
    }

    public ClearInsn(String position) {
        this.position = position;
    }

    public ClearInsn() {
        this(-1);
    }

    public Result execute() {
        Result result = new Result();
        result.setStatus(Result.Status.SUCCESSFUL);
        
        List<String> list = StopInsn.allBreakpoints();
        
        if(index > 0 && index <= list.size()) {
            position = list.get(index - 1);
        }

        if(position != null) {
            EventRequest request = StopInsn.getBreakpointReq(position);
            if(request != null) {
                DebugContext.getEventRequestManager().deleteEventRequest(request);
            }
            // TODO: A bug, if breakpoint is not resolved

            StopInsn.removeBreakpoint(position);
        } else {
            StringBuffer tmp = new StringBuffer();
            for(int i = 0; i < list.size(); i ++) {
                tmp.append(i + 1).append(" ").append(list.get(i)).append("\n");
            }

            DebugContext.emitMessage(tmp.toString());
        }

        return result;
    }
}
