/**
 * Copyright 2005-2007 Xue Yong Zhi
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyIO;
import com.xruby.runtime.builtin.RubyProc;

import j2me.util.Iterator;
import j2me.util.List;
//import j2me.util.Map;
import j2me.lang.AssertMe;
import j2me.util.concurrent.ConcurrentHashMap;
import j2me.util.ArrayList;

class MultipleMap/*<T>*/ {
	
	//class T{}
    private ConcurrentHashMap/*<String, ArrayList<T>>*/ values_ = new ConcurrentHashMap/*<String, ArrayList<T>>*/();

    public void put(String s, Object proc) {
        if (!values_.containsKey(s)) {
            values_.put(s, new ArrayList/*<T>*/());
        }

        ArrayList/*<T>*/ set = (ArrayList)values_.get(s);
        set.add(0, proc);
    }

    public List/*<T>*/ get(String name) {
        return (ArrayList)values_.get(name);
    }

    public void remove(String s, Object proc) {
    	ArrayList/*<T>*/ set = (ArrayList)values_.get(s);
        if (null == set) {
            return;
        }

        set.remove(proc);
    }

    public void removeAll(String s) {
    	ArrayList/*<T>*/ set = (ArrayList)values_.get(s);
        if (null != set) {
            set.clear();
        }
    }
}

class DefaultGlobalVariable implements GlobalVariable {
	private RubyValue v;
	
	public DefaultGlobalVariable(RubyValue v) {
		this.v = v;
	}
	public RubyValue get() {
		return v;
	}

	public void set(RubyValue v) {
		this.v = v;
	}
}

interface GlobalVariable {
	void set(RubyValue v);
	
	RubyValue get();
}

/*
$* $ARGV
$" $LOADED_FEATURES
$? $CHILD_STATUS
$& $MATCH
$< $DEFAULT_INPUT
$. $NR
$> $DEFAULT_OUTPUT
$, $OFS
$! $ERROR_INFO
$\ $ORS
$@ $ERROR_POSITION
$, $OUTPUT_FIELD_SEPARATOR
$; $FIELD_SEPARATOR
$\ $OUTPUT_RECORD_SEPARATOR
$; $FS
$$ $PID
$= $IGNORECASE
$' $POSTMATCH
$. $INPUT_LINE_NUMBER
$` $PREMATCH
$/ $INPUT_RECORD_SEPARATOR
$$ $PROCESS_ID
$~ $LAST_MATCH_INFO
$0 $PROGRAM_NAME
$+ $LAST_PAREN_MATCH
$/ $RS
$_ $LAST_READ_LINE
*/
public class GlobalVariables {
	// $,
	public static RubyValue OUTPUT_FS = RubyConstant.QNIL;
	// $\
	public static RubyValue OUTPUT_RS = RubyConstant.QNIL;
	
    private static ConcurrentHashMap/*<String, GlobalVariable>*/ values_ = new ConcurrentHashMap/*<String, GlobalVariable>*/();
    private static MultipleMap/*<RubyProc>*/ traces_procs_ = new MultipleMap/*<RubyProc>*/();

    private static boolean in_tracing_ = false;//TODO should this be global or per variable?

    public static void initialize() {
    	values_.put("$,", new GlobalVariable() {
			public RubyValue get() {
				return OUTPUT_FS;
			}

			public void set(RubyValue v) {
				OUTPUT_FS = v;
			}
    	});
    	
    	values_.put("$\\", new GlobalVariable() {
			public RubyValue get() {
				return OUTPUT_RS;
			}

			public void set(RubyValue v) {
				OUTPUT_RS = v;
			}
    	});
    	
    	values_.put("$stdout", new GlobalVariable() {
    		public RubyValue get() {
				return RubyIO.STDOUT;
			}

			public void set(RubyValue v) {
				RubyIO.STDOUT = (RubyIO)v;
			}
    	});
    	
        values_.put("$/", new DefaultGlobalVariable(ObjectFactory.createString("\n")));
        values_.put("$$", new DefaultGlobalVariable(ObjectFactory.FIXNUM0));//no way to get pid in java
    }

    //e.g. 'ruby -s filename -xxx -yyy=555'
    //dash has been removed
    public static void importValuesFromCommandLine(String[] values) {
//        for (String s : values) {
    	for(int k=0;k<values.length;k++){
    		String s = values[k];
            int i = s.indexOf('=');
            if (0 == i) {
                throw new Error("bad format!");
            } else if (i < 0) {
                //'-xxx'
                values_.put("$" + s, new DefaultGlobalVariable(RubyConstant.QTRUE));
            } else {
                String name = "$" + s.substring(0, i);
                String value = s.substring(i + 1);
                values_.put(name, new DefaultGlobalVariable(ObjectFactory.createString(value)));
            }
        }
    }

    public static RubyValue get(String name) {
        AssertMe.rho_assert('$' == name.charAt(0));

        GlobalVariable v = (GlobalVariable)values_.get(name);
        if (null != v) {
            return v.get();
        } else {
            return RubyConstant.QNIL;
        }
    }

    public static boolean isReadOnly(String name) {
        if (name.equals("$$")) {
            return true;
        }

        //TODO more

        return false;
    }

    public static void throwNameError(String name) {
        throw new RubyException(RubyRuntime.NameErrorClass, name + " is a read-only variable");
    }

    //TODO '$! = 2'  should raise exception: assigning non-exception to $! (TypeError)
    public static RubyValue set(RubyValue value, String name) {
        AssertMe.rho_assert('$' == name.charAt(0));
        GlobalVariable gv = (GlobalVariable)values_.get(name);
        if (gv == null) {
        	values_.put(name, new DefaultGlobalVariable(value));
        } else {
        	gv.set(value);
        }

        if (!in_tracing_) {
            in_tracing_ = true;

            try {
                List/*<RubyProc>*/ set = traces_procs_.get(name);
                if (null != set) {
//                    for (RubyProc p : set) {
                    for (Iterator iter = set.iterator(); iter.hasNext();) {
                    	RubyProc p = (RubyProc)iter.next();
                	
                        p.getBlock().invoke(value, value);//TODO What the receiver should be?
                    }
                }
            } finally {
                in_tracing_ = false;
            }
        }

        return value;
    }

    public static void alias(String newName, String oldName) {
        AssertMe.rho_assert('$' == newName.charAt(0));
        AssertMe.rho_assert('$' == oldName.charAt(0));

        GlobalVariable v = (GlobalVariable)values_.get(oldName);
        if (null != v) {
            values_.put(newName, v);
        }
    }

    private static boolean isDefined(String name) {
        return (values_.get(name) != null);
    }

    public static void addTraceProc(String name, RubyProc proc) {
        traces_procs_.put(name, proc);
    }

    public static void removeTraceProc(String name, RubyProc proc) {
        if (!isDefined(name)) {
            throw new RubyException(RubyRuntime.NameErrorClass, "undefined global variable " + name);
        }

        traces_procs_.remove(name, proc);
    }

    public static void removeAllTraceProc(String name) {
        if (!isDefined(name)) {
            throw new RubyException(RubyRuntime.NameErrorClass, "undefined global variable " + name);
        }

        traces_procs_.removeAll(name);
    }

    public static void setProgramName(String filename) {
        set(ObjectFactory.createString(filename), "$0");
    }
}

