/**
 * Copyright 2006-2007 Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import j2me.util.HashMap;
import j2me.util.Map;
import j2me.util.concurrent.atomic.AtomicLong;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyFixnum;

public class RubyID {
    private static Map/*<String, RubyID>*/ symbolMap = new HashMap/*<String, RubyID>*/();
//	private static ConcurrentMap<String, RubyID> symbolMap = new ConcurrentHashMap<String, RubyID>();

    public static RubyID intern(String name) {
        RubyID id = (RubyID)symbolMap.get(name);
        if (id != null) {
            return id;
        }

        // FIXME: set id flag;

        id = nextId(name);
        synchronized (symbolMap) {
            if (symbolMap.get(name) == null) {
                symbolMap.put(name, id);
            }
        }
        return id;
    }

    // ID_ALLOCATOR = 1
    private static AtomicLong lastId = new AtomicLong(2);

    public static RubyID nextId(String name) {
        return new RubyID(lastId.getAndIncrement(), name);
    }

    public static RubyID ID_ALLOCATOR = new RubyID(1);

    private long id;
    private String name;
    private RubySymbol symbol;

    // internal ID
    static final RubyID attachedID = intern("__attached__");
    static final RubyID classidID = intern("__classid__");

    public static final RubyID ltID = intern("<");
    public static final RubyID leID = intern("<=");
    public static final RubyID gtID = intern(">");
    public static final RubyID geID = intern(">=");
    public static final RubyID unequalID = intern("<=>");
    public static final RubyID longEqualID = intern("===");
    public static final RubyID equalID = intern("==");
    public static final RubyID matchID = intern("=~");
    public static final RubyID subID = intern("-");
    public static final RubyID plusID = intern("+");
    public static final RubyID mulID = intern("*");
    public static final RubyID divID = intern("/");
    public static final RubyID modID = intern("%");
    public static final RubyID powID = intern("**");
    public static final RubyID quoID = RubyID.intern("quo");

    public static final RubyID toFID = intern("to_f");
    public static final RubyID toIID = intern("to_i");
    public static final RubyID toSID = intern("to_s");
    public static final RubyID toStrID = intern("to_str");
    public static final RubyID toIntID = intern("to_int");
    public static final RubyID toAryID = intern("to_ary");
    public static final RubyID toTimeID = intern("to_time");

    public static final RubyID methodAddedID = intern("method_added");
    public static final RubyID methodMissingId = intern("method_missing");
    public static final RubyID extendObjectID = intern("extend_object");

    public static final RubyID RESPOND_TO_P = intern("respond_to?");
    public static final RubyID writeID = intern("write");
    public static final RubyID lengthID = intern("length");
    public static final RubyID eachID = intern("each");
    public static final RubyID inspectID = intern("inspect");
    public static final RubyID initializeId = intern("initialize");
    public static final RubyID inheritedID = intern("inherited");
    public static final RubyID coerceID = intern("coerce");
    public static final RubyID succID = RubyID.intern("succ");
    public static final RubyID hashID = RubyID.intern("hash");
    public static final RubyID joinID = RubyID.intern("join");
    public static final RubyID append_featuresID = RubyID.intern("append_features");
    public static final RubyID includedID = RubyID.intern("included");

    private RubyID(long id) {
        this(id, null);
    }

    private RubyID(long id, String name) {
        this.id = id;
        this.name = name;
    }

    public long getId() {
        return id;
    }

    public int hashCode() {
        return (int)this.id;
    }

    public String toString() {
        return this.name;
    }

    public RubySymbol toSymbol() {
        if (this.symbol == null) {
            this.symbol = new RubySymbol(this);
        }

        return this.symbol;
    }

    public RubyFixnum toFixnum() {
        return ObjectFactory.createFixnum(this.id);
    }

    //used by the compiler to use common ruby id
    static String name2Field(String name) {
        //TODO resue more
        if (name.equals("+")) {
            return "plusID";
        } else  if (name.equals("-")) {
            return "subID";
        } else {
            return null;
        }
    }
}
