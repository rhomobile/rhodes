package com.rhomobile.rhodes;

abstract class IRhoRubyObject
{
    private RhoRubyTypes type;
    private Object value;

    private IRhoRubyObject()
    {
        type = RhoRubyTypes.None;
        value = null;
    }

    IRhoRubyObject(RhoRubyTypes t, Object v)
    {
        type = t;
        value = v;
    }

    IRhoRubyObject(int t, Object v)
    {
        switch (t) {
            case 0: type = RhoRubyTypes.None; break;
            case 1: type = RhoRubyTypes.RubyNil; break;
            case 2: type = RhoRubyTypes.Class; break;
            case 3: type = RhoRubyTypes.Object; break;
            case 4: type = RhoRubyTypes.Boolean; break;
            case 5: type = RhoRubyTypes.Integer; break;
            case 6: type = RhoRubyTypes.Float; break;
            case 7: type = RhoRubyTypes.String; break;
            case 8: type = RhoRubyTypes.Array; break;
            case 9: type = RhoRubyTypes.Hash; break;
            default: throw new RuntimeException("Unknown type!");
        }
        value = v;
    }

    private void setVlaue(RhoRubyTypes t, Object v)
    {
        type = t;
        value = v;
    }

    public Object getValue() { return value; }
    public int getType()
    {
        switch (type) {
            case None: return 0;
            case RubyNil: return 1;
            case Class: return 2;
            case Object: return 3;
            case Boolean: return 4;
            case Integer: return 5;
            case Float: return 6;
            case String: return 7;
            case Array: return 8;
            case Hash: return 9;
            default: throw new RuntimeException("Unknown type!");
        }

    }
}


public class RhoRubyObject extends IRhoRubyObject {

    public RhoRubyObject(RhoRubyTypes t, Object v)
    {
        super(t, v);
        if(t == RhoRubyTypes.Array) throw new IllegalArgumentException("Cannot directly create an Array. Use RhoRubyArraysObject");
        if(t == RhoRubyTypes.Hash) throw new IllegalArgumentException("Cannot directly create an Hash. Use RhoRubyHashObject");
        //if(t == RhoRubyTypes.Class) throw new IllegalArgumentException("Cannot directly create an Class. Use RhoRubyClassObject");
    }

    public RhoRubyObject(int t, Object v)
    {
        super(t, v);
        if(t == 8) throw new IllegalArgumentException("Cannot directly create an Array. Use RhoArrayObjects");
        if(t == 9) throw new IllegalArgumentException("Cannot directly create an Hash. Use RhoRubyHashObject");
        //if(t == 2) throw new IllegalArgumentException("Cannot directly create an Class. Use RhoRubyClassObject");
    }

    public int getIntValue() throws ClassCastException
    {
        if(super.getValue() instanceof Integer)
            return (Integer)super.getValue();
        else
            throw new ClassCastException("This value is not integer");
    }

    public String getStringValue() throws ClassCastException
    {
        if(super.getValue() instanceof String)
            return (String)super.getValue();
        else
            throw new ClassCastException("This value is not string");
    }

    public boolean getBooleanValue() throws ClassCastException
    {
        if(super.getValue() instanceof Boolean)
            return (Boolean) super.getValue();
        else
            throw new ClassCastException("This value is not boolean");
    }

    public float getFloatValue() throws ClassCastException
    {
        if(super.getValue() instanceof Float)
            return (Float) super.getValue();
        else
            throw new ClassCastException("This value is not float");
    }

}