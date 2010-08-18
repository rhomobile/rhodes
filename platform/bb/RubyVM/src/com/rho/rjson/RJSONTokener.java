package com.rho.rjson;

import org.json.me.*;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.lang.*;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyHash;

public class RJSONTokener extends RhoJSONTokener
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("JSON");
	
    public RJSONTokener(String s) {
        super(s);
    }
    
    public Object nextValue() throws RhoJSONException {
    	throw new RhoJSONException("RJSONTokener should not call nextValue.");    	
    }
    /**
     * Get the next value. The value can be a Boolean, Double, Integer,
     * JSONArray, JSONObject, Long, or String, or the JSONObject.NULL object.
     * @throws RhoJSONException If syntax error.
     *
     * @return An object.
     */
    public RubyValue nextRValue() throws RhoJSONException {
        char c = nextClean();
        String s;

        switch (c) {
            case '"':
            case '\'':
                return ObjectFactory.createString(nextString(c));
            case '{':
                back();
                return parseObject(this);
            case '[':
                back();
                return parseArray(this);
        }

        /*
         * Handle unquoted text. This could be the values true, false, or
         * null, or it can be a number. An implementation (such as this one)
         * is allowed to also accept non-standard forms.
         *
         * Accumulate characters until we reach the end of the text or a
         * formatting character.
         */

        StringBuffer sb = new StringBuffer();
        char b = c;
        while (c >= ' ' && ",:]}/\\\"[{;=#".indexOf(c) < 0) {
            sb.append(c);
            c = next();
        }
        back();

        /*
         * If it is true, false, or null, return the proper value.
         */

        s = sb.toString().trim();
        if (s.equals("")) {
            throw syntaxError("Missing value.");
        }
        if (s.equalsIgnoreCase("true")) {
            return ObjectFactory.createBoolean(true);
        }
        if (s.equalsIgnoreCase("false")) {
        	return ObjectFactory.createBoolean(false);
        }
        if (s.equalsIgnoreCase("null")) {
        	return RubyConstant.QNIL;
        }

        /*
         * If it might be a number, try converting it. We support the 0- and 0x-
         * conventions. If a number cannot be produced, then the value will just
         * be a string. Note that the 0-, 0x-, plus, and implied string
         * conventions are non-standard. A JSON parser is free to accept
         * non-JSON forms as long as it accepts all correct JSON forms.
         */

        if ((b >= '0' && b <= '9') || b == '.' || b == '-' || b == '+') {
            if (b == '0') {
                if (s.length() > 2 &&
                        (s.charAt(1) == 'x' || s.charAt(1) == 'X')) {
                    try {
                        return ObjectFactory.createInteger(Integer.parseInt(s.substring(2),
                                16));
                    } catch (Exception e) {
                        /* Ignore the error */
                    }
                } else {
                    try {
                        return ObjectFactory.createInteger(Integer.parseInt(s, 8));
                    } catch (Exception e) {
                        /* Ignore the error */
                    }
                }
            }
            try {
                return ObjectFactory.createInteger(Integer.parseInt(s));
            } catch (Exception e) {
                try {
                    return ObjectFactory.createInteger(Long.parseLong(s));
                } catch (Exception f) {
                    try {
                        return ObjectFactory.createFloat(Double.parseDouble(s));
                    }  catch (Exception g) {
                        return ObjectFactory.createString(s);
                    }
                }
            }
        }
        return ObjectFactory.createString(s);
    }
    
    RubyValue parseArray(RJSONTokener x)throws RhoJSONException
    {
    	RubyArray ar = new RubyArray();
        if (x.nextClean() != '[') {
            throw x.syntaxError("A JSONArray text must start with '['");
        }
        if (x.nextClean() == ']') {
            return ar;
        }
        x.back();
        for (;;) {
            if (x.nextClean() == ',') {
                x.back();
                ar.add(RubyConstant.QNIL);
            } else {
                x.back();
                ar.add(x.nextRValue());
            }
            switch (x.nextClean()) {
            case ';':
            case ',':
                if (x.nextClean() == ']') {
                    return ar;
                }
                x.back();
                break;
            case ']':
                return ar;
            default:
                throw x.syntaxError("Expected a ',' or ']'");
            }
        }
    }
    
    RubyValue parseObject(RJSONTokener x)throws RhoJSONException
    {
    	RubyHash hash = ObjectFactory.createHash();
    	
        char c;
        RubyValue key;

        if (x.nextClean() != '{') {
            throw x.syntaxError("A JSONObject text must begin with '{'");
        }
        for (;;) {
            c = x.nextClean();
            switch (c) {
            case 0:
                throw x.syntaxError("A JSONObject text must end with '}'");
            case '}':
                return hash;
            default:
                x.back();
                key = x.nextRValue();
            }

            /*
             * The key is followed by ':'. We will also tolerate '=' or '=>'.
             */

            c = x.nextClean();
            if (c == '=') {
                if (x.next() != '>') {
                    x.back();
                }
            } else if (c != ':') {
                throw x.syntaxError("Expected a ':' after a key");
            }
            hash.add(key, x.nextRValue());

            /*
             * Pairs are separated by ','. We will also tolerate ';'.
             */

            switch (x.nextClean()) {
            case ';':
            case ',':
                if (x.nextClean() == '}') {
                    return hash;
                }
                x.back();
                break;
            case '}':
                return hash;
            default:
                throw x.syntaxError("Expected a ',' or '}'");
            }
        }
    }
    
	public static void initMethods(RubyModule klass) {
		klass.getSingletonClass().defineMethod("parse", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
				try {
					String str = arg.toStr();
					RJSONTokener parser = new RJSONTokener(str);
					return parser.nextRValue();
				} catch(Exception e) {
					LOG.ERROR("parse failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
		klass.getSingletonClass().defineMethod("quote_value", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
				try {
					String str = arg.toStr();
					
					String strRes = RhoJSONObject.quote(str);
					
					return ObjectFactory.createString(strRes);
				} catch(Exception e) {
					LOG.ERROR("parse failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});		
		
	}    
}