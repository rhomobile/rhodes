package com.rhomobile.rhodes.extmanager;

public interface IRhoConfig {
    
    static class ValueNotFoundException extends RuntimeException {
        private static final long serialVersionUID = 6004992824609758591L;
        public ValueNotFoundException(String reason) {
            super(reason);
        }
        public ValueNotFoundException(String reason, Throwable ex) {
            super(reason, ex);
        }
    }
    
    boolean isExist(String name);
    
    String getString(String name);

    boolean getBool(String name);
    int getInt(String name);
    double getDouble(String name);

}
