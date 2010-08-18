/**
 * Copyright 2005-2007 Xue Yong Zhi, Ye Zheng
 * Distributed under the BSD License
 */

package com.xruby.runtime.lang;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyFixnum;
import com.xruby.runtime.builtin.RubyString;

//@RubyLevelClass(name="Symbol")
public class RubySymbol extends RubyValue {
    private RubyID id;

	public void setRubyClass(RubyClass klass) {
		throw new RubyException(RubyRuntime.TypeErrorClass, this.getRubyClass().getName() + " can't be set class");
	}
	
	public RubyClass getSingletonClass() {
    	throw new RubyException(RubyRuntime.TypeErrorClass, this.getRubyClass().getName() + " can't define singleton");
    }

	public RubyValue clone() {
		throw new RubyException(RubyRuntime.TypeErrorClass, "can't clone " + this.getRubyClass().getName());
	}
    
    public RubySymbol(RubyID id) {
        this.id = id;
    }

    public RubyClass getRubyClass() {
        return RubyRuntime.SymbolClass;
    }

    public String toString() {
        return id.toString();
    }

    public String toStr() {
        return id.toString();
    }

    public String asString() {
    	return id.toString();
    }

    public int hashCode() {
        return this.id.hashCode();
    }

    public RubyID toID() {
        return this.id;
    }

    //@RubyLevelMethod(name="to_i", alias="to_int")
    public RubyFixnum to_i() {
        return this.id.toFixnum();
    }

    public boolean respondTo(RubyID id) {
    	return super.respondTo(id);
    }
    
    //@RubyLevelMethod(name="to_s", alias="id2name")
    public RubyString to_s() {
        return ObjectFactory.createString(id.toString());
    }

    //@RubyLevelMethod(name="to_sym")
    public RubySymbol to_sym() {
        return this;
    }

    //@RubyLevelMethod(name="length")
    public RubyFixnum rubyLength() {
        return ObjectFactory.createFixnum(id.toString().length());
    }

    //@RubyLevelMethod(name="[]")
    public RubyValue array_access(RubyArray args) {
    	return to_s().array_access(args);
    }
    
    //@RubyLevelMethod(name="inspect")
    public RubyString rubyInspect() {
        String value = this.toString();
        RubyString str = ObjectFactory.createString(":" + value);
        if (!isSymname(value)) {
            String s = str.dump();
            s = ":\"" + s.substring(2);
            str = ObjectFactory.createString(s);
        }

        return str;
    }

    //RHO_COMMENT
    //@RubyLevelMethod(name="===")
    public RubyValue sym_eqq(RubyValue v) {
    	if ( this == v )
    		return RubyConstant.QTRUE;
    	
    	if ( v.toStr().equals(this.toStr()) )
    		return RubyConstant.QTRUE;
    		
    	return RubyConstant.QFALSE;
    }
    
    private boolean isSymname(String name) {
        if (name == null) {
            return false;
        }

        int current = 0;
        boolean id = false;
        boolean localid = false;

        switch (getChar(name, current)) {
            case '\0':
                return false;
            case '$':
                if (isSpecialGlobalName(name.substring(++current))) {
                    return true;
                }
                id = true;
                break;
            case '@':
                current++;
                if (getChar(name, current) == '@') {
                    current++;
                }
                id = true;
            case '<':
                current++;
                switch (getChar(name, current)) {
                    case '<':
                        current++;
                        break;
                    case '=':
                        current++;
                        if (getChar(name, current) == '>') {
                            current++;
                        }
                        break;
                }
                break;
            case '>':
                current++;
                switch (getChar(name, current)) {
                    case '>':
                    case '=':
                        current++;
                        break;
                }
                break;
            case '=':
                current++;
                switch (getChar(name, current)) {
                    case '~':
                        current++;
                        break;
                    case '=':
                        current++;
                        if (getChar(name, current) == '=') {
                            current++;
                        }
                        break;
                    default:
                        return false;
                }
            case '*':
                current++;
                if (getChar(name, current) == '*') {
                    current++;
                }
                break;
            case '+':
            case '-':
                current++;
                if (getChar(name, current) == '@') {
                    current++;
                }
                break;
            case '|':
            case '^':
            case '&':
            case '/':
            case '%':
            case '~':
            case '`':
                current++;
                break;
            case '[':
                current++;
                if (getChar(name, current) != ']') {
                    return false;
                }

                current++;
                if (getChar(name, current) == '=') {
                    current++;
                }
                break;
            default:
                localid = !isUpper(getChar(name, current));
                id = true;
                break;
        }

        if (id) {
            // FIXME: TO BE COMPLETE
            int c = getChar(name, current);
            if (c != '_' && !isAlpha(c)) {
                return false;
            }
            // FIXME: TO BE COMPLETE
            while (isIdentChar(getChar(name, current))) {
                current++;
            }

            if (localid) {
                switch (getChar(name, current)) {
                    case '!':
                    case '?':
                    case '=':
                        current++;
                }
            }
        }

        return getChar(name, current) <= 0;
    }

    private int getChar(String name, int index) {
        if (index >= name.length()) {
            return -1;
        }

        return name.charAt(index);
    }

    private boolean isUpper(int c) {
        return c >= 'A' && c <= 'Z';
    }

    private boolean isAlpha(int c) {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }

    private boolean isDigit(int c) {
        return c >= '0' && c <= '9';
    }

    private boolean isAlnum(int c) {
        return isAlpha(c) || isDigit(c);
    }

    // FIXME: TO BE COMPLETED
    private boolean isIdentChar(int c) {
        return isAlnum(c) || c == '_';
    }

    private boolean isSpecialGlobalName(String name) {
        int current = 0;
        switch (getChar(name, current)) {
            case '~':
            case '*':
            case '$':
            case '?':
            case '!':
            case '@':
            case '/':
            case '\\':
            case ';':
            case ',':
            case '.':
            case '=':
            case ':':
            case '<':
            case '>':
            case '\"':
            case '&':
            case '`':
            case '\'':
            case '+':
            case '0':
                current++;
                break;
            case '-':
                current++;
                // FIXME: TO BE COMPLETED
                // if (is_identchar(*m)) m += mbclen(*m);
                break;
            default:
                if (!isDigit(getChar(name, current))) {
                    return false;
                }

                do {
                    current++;
                } while (isDigit(getChar(name, current)));
        }

        return getChar(name, current) <= 0;
    }
    
}
