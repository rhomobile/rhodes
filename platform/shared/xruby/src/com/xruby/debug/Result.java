/*
 * Copyright 2006-2007 Yu Su
 * Distributed under the BSD License
 */
package com.xruby.debug;

import java.util.HashMap;
import java.util.Map;

/**
 * Result of instrution execution, including status and
 * additional information, status is set to DEFERRED by default
 *
 * @author Yu Su (beanworms@gmail.com)
 */
public class Result {
    /**
     * status
     */
    public enum Status {ERROR, SUCCESSFUL, DEFERRED}
    
    private static final String ERROR_MSG = "ERROR_MSG";

    // status is set to DEFERRED by default
    private Status status = Status.DEFERRED;
    private Map<String, Object> additionalInfo;    

    public Status getStatus() {
        return status;
    }

    public void setStatus(Status status) {
        this.status = status;
    }

    public boolean isSuccessful() {
        return (status == Status.SUCCESSFUL);
    }

    public void addInfo(String key, Object info) {
        if(additionalInfo == null) {
            additionalInfo = new HashMap<String, Object>();
        }

        additionalInfo.put(key, info);
    }

    public Object getInfo(String key) {
        return additionalInfo.get(key);
    }

    public void setErrMsg(String msg) {
        addInfo(ERROR_MSG, msg);
    }

    public String getErrMsg() {
        return (String) getInfo(ERROR_MSG);
    }
}
