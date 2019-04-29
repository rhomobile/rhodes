package com.rhomobile.rhodes;

public class RhoRubyClassObject extends IRhoRubyObject {
    private String klass_name;
    public RhoRubyClassObject(String name) {
        super(RhoRubyTypes.Class, null);
        klass_name = name;

        if (!NativeMakeRubyClassObject(klass_name))
            throw new RuntimeException("Could not create class from Ruby!");
    }

    public IRhoRubyObject ExecuteRubyObjectMethod(String name_function, RhoRubyArrayObjects parameters)
    {
        return NativeExecuteRubyObjectMethod(klass_name, name_function, parameters);
    }

    native boolean NativeMakeRubyClassObject(String name);
    native IRhoRubyObject NativeExecuteRubyObjectMethod(String class_name, String name_function, RhoRubyArrayObjects parameters);
}
