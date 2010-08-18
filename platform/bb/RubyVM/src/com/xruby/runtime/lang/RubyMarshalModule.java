package com.xruby.runtime.lang;

import j2me.math.HugeInt;
import j2me.util.Iterator;

/*import com.xruby.runtime.lang.RubyAPI;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubySymbol;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.annotation.RubyLevelMethod;
import com.xruby.runtime.lang.annotation.RubyLevelModule;*/

import com.xruby.runtime.builtin.*;
/*
 * @see http://headius.com/rubyspec/index.php/Marshaling
 */
class MarshalDumper {
    private static void packInteger(RubyFixnum v, StringBuffer sb) {
        sb.append('i');
        packInteger(v.toInt(), sb);
    }

    private static void packInteger(int v, StringBuffer sb) {
        if (0 == v) {
            sb.append((char) 0);
        } else {
            sb.append((char) (v + 5));
        }
    }

    private static void packString(RubyString v, StringBuffer sb) {
        sb.append('"');
        packInteger(v.length(), sb);
        sb.append(v);
    }

    private static void packSymbol(RubySymbol v, StringBuffer sb) {
        sb.append(':');
        packInteger(v.toString().length(), sb);
        sb.append(v.toString());
    }

    private static void packArray(RubyArray v, StringBuffer sb) {
        sb.append('[');
        packInteger(v.size(), sb);
//        for (RubyValue a : v) {
        for (Iterator iter = v.iterator(); iter.hasNext();) {
        	RubyValue a = (RubyValue)iter.next();
            packValue(a, sb);
        }
    }

    private static void packHash(RubyHash v, StringBuffer sb) {
        sb.append('{');
        packInteger(v.size().toInt(), sb);
        RubyArray keys = v.keys();
//        for (RubyValue a : keys) {
        for (Iterator iter = keys.iterator(); iter.hasNext();) {
        	RubyValue a = (RubyValue)iter.next();
            packValue(a, sb);
            packValue(v.get(a), sb);
        }
    }

    private static void packFloat(RubyFloat v, StringBuffer sb) {
        sb.append('f');
        String s = Double.toString(v.doubleValue());
        packInteger(s.length(), sb);
        sb.append(s);
    }

    private static void packBignum(RubyBignum v, StringBuffer sb) {
        sb.append('l');
        HugeInt biginteger = v.getInternal();

        if (biginteger.compareTo(HugeInt.ZERO) > 0) {
            sb.append('+');
        } else {
            sb.append('-');
        }

        int size = v.size();
        sb.append((char) size);

        byte[] bytes = new byte[size];
        byte[] orig = biginteger.toByteArray();
        System.arraycopy(orig, 0, bytes, size - orig.length, orig.length);
        for (int i = size - 1; i >= 0; --i) {
            int c = bytes[i];
            sb.append((char) (c & 0xff));
        }
    }

    private static void packObject(RubyValue v, StringBuffer sb) {
        sb.append("C:");
        String s = v.getRubyClass().getName();
        packInteger(s.length(), sb);
        sb.append(s);

        if (v instanceof RubyString) {
            packString((RubyString) v, sb);
        } else {
            throw new RubyException("not implemented");
        }
    }

    private static void packValue(RubyValue v, StringBuffer sb) {
        if (v == RubyConstant.QNIL) {
            sb.append((char) 0);
        } else if (v == RubyConstant.QTRUE) {
            sb.append('T');
        } else if (v == RubyConstant.QFALSE) {
            sb.append('F');
        } else {
            RubyClass klass = v.getRubyClass().getRealClass();
            if (klass == RubyRuntime.StringClass) {
                packString((RubyString) v, sb);
            } else if (klass == RubyRuntime.SymbolClass) {
                packSymbol((RubySymbol) v, sb);
            } else if (klass == RubyRuntime.FixnumClass) {
                packInteger((RubyFixnum) v, sb);
            } else if (klass == RubyRuntime.ArrayClass) {
                packArray((RubyArray) v, sb);
            } else if (klass == RubyRuntime.HashClass) {
                packHash((RubyHash) v, sb);
            } else if (klass == RubyRuntime.FloatClass) {
                packFloat((RubyFloat) v, sb);
            } else if (klass == RubyRuntime.BignumClass) {
                packBignum((RubyBignum) v, sb);
            } else {
                packObject(v, sb);
            }
        }
    }

    public static RubyString dump(RubyValue v) {
        StringBuffer sb = new StringBuffer();

        //version
        sb.append((char) 4);
        sb.append((char) 8);

        packValue(v, sb);

        return ObjectFactory.createString(sb);
    }
}

class MarshalLoader {
    private int current_index_ = 0;

    private int loadInteger(String v) {
        char c = v.charAt(current_index_);
        ++current_index_;
        if (0 == c) {
            return c;
        } else {
            return c - 5;
        }
    }

    private RubyFloat loadFloat(String v) {
        int length = loadInteger(v);
        String s = v.substring(current_index_, current_index_ + length);
        double d = Double.parseDouble(s);
        current_index_ += length;
        return ObjectFactory.createFloat(d);
    }

    private RubySymbol loadSymbol(String v) {
        return ObjectFactory.createSymbol(_loadString(v));
    }

    private RubyString loadString(String v) {
        return ObjectFactory.createString(_loadString(v));
    }

    private String _loadString(String v) {
        int length = loadInteger(v);
        String s = v.substring(current_index_, current_index_ + length);
        current_index_ += length;
        return s;
    }

    private RubyArray loadArray(String v) {
        int length = loadInteger(v);
        RubyArray a = new RubyArray(length);
        for (int i = 0; i < length; ++i) {
            RubyValue value = loadValue(v);
            a.add(value);
        }
        return a;
    }

    private RubyHash loadHash(String v) {
        int length = loadInteger(v);
        RubyHash h = ObjectFactory.createHash();
        for (int i = 0; i < length; ++i) {
            RubyValue k = loadValue(v);
            RubyValue value = loadValue(v);
            h.add(k, value);
        }
        return h;
    }

    private RubyBignum loadBignum(String v) {
        char sign = v.charAt(current_index_++);
        char length = v.charAt(current_index_++);

        HugeInt biginteger = HugeInt.valueOf(0);
        for (int i = 0; i < length; ++i) {
            HugeInt tmp = HugeInt.valueOf(v.charAt(current_index_ + i));
            tmp = tmp.shiftLeft(i * 8);
            biginteger = biginteger.add(tmp);
        }

        current_index_ += length;
        return ObjectFactory.createBignum(biginteger);
    }

    private RubyValue loadObject(String v) {
        ++current_index_;//ignore ':'
        String class_name = _loadString(v);
        RubyValue r = loadValue(v);
        r.setRubyClass((RubyClass) RubyAPI.getConstant(RubyRuntime.ObjectClass, class_name));
        return r;
    }

    private RubyValue loadValue(String v) {
        char c = v.charAt(current_index_);
        ++current_index_;
        switch (c) {
            case '\0':
                return RubyConstant.QNIL;
            case 'T':
                return RubyConstant.QTRUE;
            case 'F':
                return RubyConstant.QFALSE;
            case 'i':
                return ObjectFactory.createFixnum(loadInteger(v));
            case '"':
                return loadString(v);
            case ':':
                return loadSymbol(v);
            case '[':
                return loadArray(v);
            case '{':
                return loadHash(v);
            case 'f':
                return loadFloat(v);
            case 'l':
                return loadBignum(v);
            case 'C':
                return loadObject(v);
            default:
                throw new RubyException("not implemented " + c);
        }
    }

    public RubyValue load(RubyString s) {
        String v = s.toString();
        if (v.length() <= 2) {
            throw new RubyException(RubyRuntime.ArgumentErrorClass, "marshal data too short");
        } else if (v.charAt(0) != 4 && v.charAt(1) != 8) {
            throw new RubyException(RubyRuntime.TypeErrorClass, "incompatible marshal file format (can't be read)");
        } else {
            current_index_ += 2;
            return loadValue(v);
        }
    }
}

//@RubyLevelModule(name="Marshal")
public class RubyMarshalModule {
	//@RubyLevelMethod(name="dump", module=true)
    public static RubyValue dump(RubyValue arg) {
        return MarshalDumper.dump(arg);
    }
	
	//@RubyLevelMethod(name="load", module=true, alias="restore")
	public static RubyValue load(RubyValue arg) {
		MarshalLoader loader = new MarshalLoader();
		return loader.load((RubyString) arg);
	}
}
