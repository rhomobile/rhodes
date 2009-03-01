package com.rhomobile.rhodes.http;

import java.net.InetAddress;

public class InetAddrPort {
	
	private InetAddress addr=null;
    private int port=0;

    public InetAddrPort(){
    }

    public InetAddrPort(int port)
    {
        this.port=port;
    }
    
    public InetAddrPort(InetAddress addr, int port)
    {
        this.addr=addr;
        this.port=port;
    }
    
    public InetAddrPort(String inetAddrPort) throws java.net.UnknownHostException
    {
        int c = inetAddrPort.indexOf(":");
        if (c>=0)
        {
            String addr=inetAddrPort.substring(0,c);
            inetAddrPort=inetAddrPort.substring(c+1);
        
            if (addr.length()>0)
                this.addr=InetAddress.getByName(addr);
        }
        
        this.port = Integer.parseInt(inetAddrPort); 
    }
    
    public InetAddress getInetAddress()
    {
        return this.addr;
    }
    
    public void setInetAddress(InetAddress addr)
    {
        this.addr=addr;
    }

    public int getPort()
    {
        return this.port;
    }
    
    public void setPort(int port)
    {
        this.port=port;
    }
    
    public String toString()
    {
        if (this.addr==null)
            return "127.0.0.1:"+this.port;
        return this.addr.toString()+':'+this.port;
    }
}
