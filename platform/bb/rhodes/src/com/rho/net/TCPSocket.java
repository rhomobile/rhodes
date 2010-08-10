package com.rho.net;

import com.rho.RhoClassFactory;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.lang.*;

import java.io.IOException;

import javax.microedition.io.SocketConnection;

public class TCPSocket extends BaseSocket {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("TCPSocket");
	
	public TCPSocket(RubyClass c) {
		super(c);
	}

    public static TCPSocket alloc(RubyValue receiver) {
    	return new TCPSocket((RubyClass)receiver);
    }
	
    public void initialize(String strHost, int nPort) throws IOException
    {
    	NetworkAccess na = (NetworkAccess)RhoClassFactory.getNetworkAccess();
    	SocketConnection conn = na.socketConnect(strHost, nPort);
    	setConnection(conn);
    	super.initialize(strHost, nPort);
    }
    
	public static void initMethods(RubyClass klass) {
		
		klass.defineAllocMethod(new RubyNoArgMethod(){
			protected RubyValue run(RubyValue receiver, RubyBlock block )	{
				return TCPSocket.alloc(receiver);}
		});
		
		klass.defineMethod( "initialize", new RubyTwoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block )
			{
		    	try{
					String strHost = arg1.toStr();
					int nPort = arg2.toInt();
					((TCPSocket)receiver).initialize(strHost, nPort);
			        return receiver;
					
				}catch(Exception e)
				{
					LOG.ERROR("initialize failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
		klass.getSingletonClass().defineMethod("open", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg1, RubyValue arg2, RubyBlock block) 
			{
				try{
					TCPSocket res = TCPSocket.alloc(RubyRuntime.TCPSocketClass);
					String strHost = arg1.toStr();
					int nPort = arg2.toInt();
					res.initialize(strHost, nPort);
					
					return res;
				}catch(Exception e)
				{
					LOG.ERROR("open failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
		klass.defineMethod( "closed?", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
		    	try{
					boolean bRes = ((TCPSocket)receiver).is_closed();
					
					return ObjectFactory.createBoolean(bRes);
				}catch(Exception e)
				{
					LOG.ERROR("closed? failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});

		klass.defineMethod( "flush", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
		    	try{
					((TCPSocket)receiver).flush();
					
					return RubyConstant.QNIL;
				}catch(Exception e)
				{
					LOG.ERROR("close failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
		klass.defineMethod( "close", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
		    	try{
					((TCPSocket)receiver).close();
					
					return RubyConstant.QNIL;
				}catch(Exception e)
				{
					LOG.ERROR("close failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});

		klass.defineMethod( "write", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
		    	try{
					int nRes = ((TCPSocket)receiver).write(arg.toStr());
					
					return ObjectFactory.createInteger(nRes);
				}catch(Exception e)
				{
					LOG.ERROR("write failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});

		klass.defineMethod( "sysread", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
		    	try{
					String strRes = ((TCPSocket)receiver).sysread(arg.toInt());
					
					return ObjectFactory.createString(strRes);
				}catch(Exception e)
				{
					if (e instanceof RubyException )
					{
						RubyException re = (RubyException)e;
						if ( re.getRubyValue().getRubyClass() == RubyRuntime.EOFErrorClass )
							throw re;
					}
					LOG.ERROR("sysread failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		
	}	
}
