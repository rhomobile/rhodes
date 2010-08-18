/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import com.xruby.runtime.lang.*;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;
//import com.xruby.runtime.lang.annotation.RubyLevelClass;

//@RubyLevelClass(name="Struct")
public class RubyStruct {

    //@RubyLevelMethod(name="new", singleton=true)
    public static RubyValue alloc(RubyValue receiver, RubyArray args, RubyBlock block) {
        if (!(args.get(0) instanceof RubyString)) {
            RubyClass c = RubyRuntime.ObjectClass.defineClass(null, RubyRuntime.ObjectClass);
            addMethods(c, args, 0);
            return c;
        } else {
            RubyClass c = RubyRuntime.StructClass.defineClass(args.get(0).toString(), RubyRuntime.ObjectClass);
            addMethods(c, args, 1);
            return c;
        }
    }

    private static void addMethods(RubyClass c, final RubyArray super_args, final int offset) {
        for (int i = offset; i < super_args.size(); ++i) {
            String s = super_args.get(i).toStr();
            c.defineMethod(s, new AttrReader(s));
            c.defineMethod(s + "=", new AttrWriter(s));
        }

        c.getSingletonClass().defineMethod("new", new RubyVarArgMethod(super_args.size() - offset, false, 0) {
            protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {
                RubyValue v = new RubyObject((RubyClass) receiver);
                for (int i = 0; i < args.size(); ++i) {
                    RubyID id = RubyID.intern("@" + super_args.get(i + offset).toStr());
                    v.setInstanceVariable(args.get(i), id);
                }
                return v;
            }

        }
        );
        
        c.defineMethod("[]", new RubyOneArgMethod() {
            protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block) {
                String name;
                if (arg instanceof RubyString) {
                    name = arg.toString();
                } else if (arg instanceof RubySymbol) {
                    name = arg.toString();
                } else if (arg instanceof RubyFixnum) {
                    int i = ((RubyFixnum) arg).toInt();
                    name = super_args.get(i + 1).toStr();
                } else {
                    throw new RubyException(RubyRuntime.TypeErrorClass, "can't convert " + arg.getRubyClass().getName() + " into Integer");
                }

                return receiver.getInstanceVariable(RubyID.intern("@" + name));
            }
        }
        );

        c.defineMethod("[]=", new RubyTwoArgMethod() {
            protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) {
                String name;
                if (arg1 instanceof RubyString) {
                    name = arg1.toString();
                } else if (arg1 instanceof RubySymbol) {
                    name = arg1.toString();
                } else if (arg1 instanceof RubyFixnum) {
                    int i = ((RubyFixnum) arg1).toInt();
                    name = super_args.get(i + 1).toStr();
                } else {
                    throw new RubyException(RubyRuntime.TypeErrorClass, "can't convert " + arg1.getRubyClass().getName() + " into Integer");
                }

                return receiver.setInstanceVariable(arg2, RubyID.intern("@" + name));
            }
        }
        );
        
        c.defineMethod("to_a", new RubyNoArgMethod() {
            protected RubyValue run(RubyValue receiver, RubyBlock block) {
                RubyArray a = new RubyArray(super_args.size() - 1);
                for (int i = 1; i < super_args.size(); ++i) {
                    String name = super_args.get(i).toStr();
                    a.add(receiver.getInstanceVariable(RubyID.intern("@" + name)));
                }
                return a;
            }
        }
        );
    }
}
