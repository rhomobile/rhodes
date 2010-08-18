package com.rho;

public interface IRhoLogSink {
    void writeLogMessage( String strMsg );
    int  getCurPos();
    void close();
    void clear();
}
