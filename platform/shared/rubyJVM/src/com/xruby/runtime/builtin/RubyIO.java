/**
 * Copyright 2005-2007 Xue Yong Zhi, Jie Li, Ye Zheng, Michael Chen
 * Distributed under the BSD License
 */

package com.xruby.runtime.builtin;

import java.io.IOException;
//import java.nio.channels.Pipe;
import j2me.util.Iterator;

import com.rho.RhoClassFactory;
import com.rho.RhoRuby;
import com.xruby.runtime.lang.GlobalVariables;
import com.xruby.runtime.lang.RubyAPI;
import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyID;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;
//import com.xruby.runtime.lang.annotation.RubyLevelClass;
//import com.xruby.runtime.lang.annotation.RubyLevelMethod;

import java.io.InputStream;
//import java.io.ByteArrayOutputStream;
//import javolution.io.UTF8StreamReader;
import com.rho.net.NetRequest;

////@RubyLevelClass(name="IO")
public class RubyIO extends RubyBasic {
	public static RubyIO STDOUT;
	
	private static final RubyString DEFAULT_RS = new RubyString("\n");

    private RubyIOExecutor executor;
    private boolean is_closed_ = false;

    public RubyIO(RubyIOExecutor executor) {
        super(RubyRuntime.IOClass);
        this.executor = executor;
    }
    
    public RubyIO(RubyIOExecutor executor, RubyClass klass) {
    	super(klass);
    	this.executor = executor;
    }

    public void initIO(RubyIO io)
    {
    	executor = io.executor;
    	is_closed_ = io.is_closed_;
    }
    
    public RubyValue clone(){
    	RubyIO cl = new RubyIO( this.executor, this.class_);
    	cl.is_closed_ = is_closed_; 
    	cl.doClone(this);
    	return cl;
    }
    
    public InputStream getInputStream()
    {
    	if ( executor == null )
    		return null;
    	
    	return executor.getInputStream();
    }
    
    public void print(String s) {
        executor.print(s);
    }

    public String read() {
        return executor.read();
    }

    public String read(int length, int offset) {
        return executor.read(length, offset);
    }

    public String read(long length) {
        return executor.read(length);
    }
    
    public void truncate(int length) {
        this.executor.truncate(length);
    }

    //@RubyLevelMethod(name="write")
    public RubyInteger write(RubyValue arg) {
    	String value = arg.asString();
        
        if (this instanceof RubyFile) {
        	if (value.length() == 0) {
        		return ObjectFactory.FIXNUM0;
        	}
        	
        	this.executor.print(value);
        } else {
        	RubyAPI.callOneArgMethod(this, arg, null, RubyID.writeID);
        }

        return ObjectFactory.createInteger(value.length());
    }
    
    public void writeInvocation(RubyValue value) {
    	RubyAPI.callOneArgMethod(this, value, null, RubyID.writeID);
    }
    
    //@RubyLevelMethod(name="puts")
    public RubyValue puts() {
    	/* if no argument given, print newline. */
    	this.writeInvocation(DEFAULT_RS);
    	return RubyConstant.QNIL;
    }
    
    //@RubyLevelMethod(name="puts")
    public RubyValue puts(RubyArray args) {
    	String line;

//    	for (RubyValue arg : args) {
        for (Iterator iter = args.iterator(); iter.hasNext();) {
        	RubyValue arg = (RubyValue)iter.next();
    		if (arg == RubyConstant.QNIL) {
    			line = "nil";
    		} else {
    			line = arg.asString();
    		}

    		this.writeInvocation(ObjectFactory.createString(line));

/*    		int len = line.length();
    		if (len == 0 || line.charAt(len - 1) != '\n') {
    			this.writeInvocation(DEFAULT_RS);
    		}*/
    	}

    	return RubyConstant.QNIL;
    }
    
    //@RubyLevelMethod(name="print")
    public RubyValue print() {
    	return this.print(new RubyArray(GlobalVariables.get("$_")));
    }
    
    //@RubyLevelMethod(name="print")
    public RubyValue print(RubyValue arg) {
    	if (arg == RubyConstant.QNIL) {
        	this.writeInvocation(ObjectFactory.createString("nil"));
        } else {
        	this.writeInvocation(arg);
        }
    	
    	// if the output record separator($\) is not nil, it will be appended to the output.
		if (GlobalVariables.OUTPUT_RS != RubyConstant.QNIL) {
        	this.writeInvocation(GlobalVariables.OUTPUT_RS);
        }

        return RubyConstant.QNIL;
    }
    
    //@RubyLevelMethod(name="print")
    public RubyValue print(RubyArray args) {
    	int size = args.size();
		for (int i = 0; i < size; ++i) {
			// insert the output field separator($,) if not nil
            if (i > 0 && GlobalVariables.OUTPUT_FS != RubyConstant.QNIL) {
            	this.writeInvocation(GlobalVariables.OUTPUT_FS);
            }

            if (args.get(i) == RubyConstant.QNIL) {
            	this.writeInvocation(ObjectFactory.createString("nil"));
            } else {
            	this.writeInvocation(args.get(i));
            }
        }
        
		// if the output record separator($\) is not nil, it will be appended to the output.
		if (GlobalVariables.OUTPUT_RS != RubyConstant.QNIL) {
        	this.writeInvocation(GlobalVariables.OUTPUT_RS);
        }

        return RubyConstant.QNIL;
    }
    
    //@RubyLevelMethod(name="close")
    public RubyValue close() {
    	this.executor.close();
        return RubyConstant.QNIL;
    }
    
    //@RubyLevelMethod(name="flush")
    public RubyValue flush() {
        this.executor.flush();
        return RubyConstant.QNIL;
    }
    
    //@RubyLevelMethod(name="seek")
    public RubyFixnum seek(RubyValue arg) {
        long pos = RubyTypesUtil.convertToJavaLong(arg);
        this.executor.seek(pos);
        return ObjectFactory.FIXNUM0;
    }
    
    //@RubyLevelMethod(name="closed?")
    public RubyValue closedP(RubyValue receiver) {
        return ObjectFactory.createBoolean(is_closed_);
    }

    //@RubyLevelMethod(name="eof", alias="eof?")
	public RubyValue eof() {
		return ObjectFactory.createBoolean(this.executor.eof());
	}
	
	//@RubyLevelMethod(name="read")
	public RubyValue read(RubyArray args) {
        RubyString buffer = null;
        if (null != args && args.size() > 1) {
            buffer = (RubyString)args.get(1);
        }

        if ((null == args) || (RubyConstant.QNIL == args.get(0))) {
            return buildResult(this.executor.read(), buffer);
        } else {
            return buildResult(this.executor.read(args.get(0).toInt()), buffer);
        }
    }
	
	//@RubyLevelMethod(name="readpartial")
	public RubyValue readpartial(RubyArray args) {
        RubyValue v = read(args);
        if (this.executor.eof()) {
            throw new RubyException(RubyRuntime.EOFErrorClass, "end of file reached");
        }
        return v;
    }

	
    private static RubyValue buildResult(String s, RubyString buffer) {
        if (null == s) {
            if (null != buffer) {
                buffer.setString("");
            }
            return RubyConstant.QNIL;
        } else if (null != buffer) {
            buffer.setString(s);
            return buffer;
        } else {
            return ObjectFactory.createString(s);
        }
    }
    
    //@RubyLevelMethod(name="gets")
    public RubyValue gets(RubyArray args) {
    	RubyValue seperator = (null == args) ? GlobalVariables.get("$/") : args.get(0);
    	String s = this.executor.gets(seperator);
    	//RubyValue v = (null == s) ? RubyConstant.QNIL : ObjectFactory.createString(s);
    	RubyValue v = RubyConstant.QNIL;
    	if ( null != s )
    		v = ObjectFactory.createString(s);
    	
    	GlobalVariables.set(v, "$_");

    	return GlobalVariables.get("$_");
    }

    //@RubyLevelMethod(name="readline")
    public RubyValue readline(RubyArray args) {
    	if ( this.executor.eof() )
    		throw new RubyException(RubyRuntime.EOFErrorClass, "end of file reached");
    	
    	RubyValue seperator = (null == args) ? GlobalVariables.get("$/") : args.get(0);
    	String s = this.executor.gets(seperator);
    	if ( s == null )
    		throw new RubyException(RubyRuntime.EOFErrorClass, "end of file reached");
    	
    	return ObjectFactory.createString(s);
    }
    
    //@RubyLevelMethod(name="pipe")
   /* public static RubyValue pipeSingleton(RubyValue receiver, RubyBlock block) {
        RubyArray array = new RubyArray(2);
        try {
            Pipe pipe = Pipe.open();
            RubyIO r = new RubyIO(new RubyIOPipeSourceExecutor(pipe.source()));
            RubyIO w = new RubyIO(new RubyIOPipeSinkExecutor(pipe.sink()));
            array.add(r);
            array.add(w);
        } catch (IOException e) {
            throw new RubyException(RubyRuntime.IOErrorClass, e.toString());
        }

        return array;
    }*/
    
    //RHO_COMMENT
/*    
    private static char[] buffer = new char[1024];
	public static final RubyValue readFully(InputStream in, boolean bText) throws IOException {
		RubyString str = ObjectFactory.createString();
		UTF8StreamReader reader = new UTF8StreamReader();
		reader.setInput(in);
		while (true) {
			synchronized (buffer) {
				int len = reader.read(buffer);
				if (len < 0) {
					break;
				}
				str.appendChars(buffer,len);
			}
		}
		return str;
	}*/

    static RubyValue loadFromResources(String fileName, boolean bText){
    	InputStream stream = null;
		try {
			//stream = RhoClassFactory.createFile().getResourceAsStream(fileName.getClass(), "/"+fileName);
			stream = RhoRuby.loadFile("/" + fileName);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
    	if ( stream == null )
    		return null;
    	
    	try{
   			String res = NetRequest.readFully(stream, bText ? "=UTF-8" : "");
   			
   			return ObjectFactory.createString(res);
    	}catch( Exception exc ){
    		throw new RubyException(RubyRuntime.RuntimeErrorClass,exc.getMessage());
    	}finally
    	{
			try{ if ( stream != null ) stream.close(); }catch(java.io.IOException exc){}
    	}
    }
    
    //@RubyLevelMethod(name="read")
    public static RubyValue read(RubyValue receiver, RubyArray args, RubyBlock block) {
        String fileName = ((RubyString) args.get(0)).toStr();
        //RHO_COMMENT
        RubyValue r = loadFromResources(fileName, true);
        if ( r != null )
        	return r;
        
        RubyIO io = ObjectFactory.createFile(fileName, "r");
        int offset;
        int length;

        if (args.size() == 1) {
            r = buildResult(io.read(), null);
        } else {
            length = ((RubyFixnum) args.get(1)).toInt();
            if (args.size() == 2) {
                r = buildResult(io.read(length), null);
            } else {
                offset = ((RubyFixnum) args.get(2)).toInt();
                r = buildResult(io.read(length, offset), null);
            }
        }

        io.close();
        return r;
    }
    
    //@RubyLevelMethod(name="read")
    public static RubyValue binread(RubyValue receiver, RubyArray args, RubyBlock block) {
        String fileName = ((RubyString) args.get(0)).toStr();
        //RHO_COMMENT
        RubyValue r = loadFromResources(fileName, false);
        if ( r != null )
        	return r;
        
        RubyIO io = ObjectFactory.createFile(fileName, "r");
        int offset;
        int length;

        if (args.size() == 1) {
            r = buildResult(io.read(), null);
        } else {
            length = ((RubyFixnum) args.get(1)).toInt();
            if (args.size() == 2) {
                r = buildResult(io.read(length), null);
            } else {
                offset = ((RubyFixnum) args.get(2)).toInt();
                r = buildResult(io.read(length, offset), null);
            }
        }

        io.close();
        return r;
    }
    
}
