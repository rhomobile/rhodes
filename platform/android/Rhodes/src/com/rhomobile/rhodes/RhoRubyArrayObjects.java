package com.rhomobile.rhodes;

public class RhoRubyArrayObjects extends IRhoRubyObject {

    public RhoRubyArrayObjects(IRhoRubyObject[] arr) {
        super(RhoRubyTypes.Array, arr);
    }

    public RhoRubyArrayObjects(int c) {
        super(RhoRubyTypes.Array, new IRhoRubyObject[c]);
    }

    public int getCount() {
        return ((IRhoRubyObject[]) getValue()).length;
    }

    public IRhoRubyObject getItem(int index)
    {
        return ((IRhoRubyObject[])getValue())[index];
    }

    public RhoRubyObject getRhoObjectItem(int index)
    {
        return ((RhoRubyObject[])getValue())[index];
    }

    public  void setItem(int index, IRhoRubyObject obj)
    {
        ((IRhoRubyObject[])getValue())[index] = obj;
    }

}

