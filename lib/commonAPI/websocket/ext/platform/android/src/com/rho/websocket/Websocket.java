package com.rho.websocket;

import java.util.Map;
import java.util.HashMap;

import java.net.URI;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.Logger;
//import com.rhomobile.rhodes.websockets.WebSocketClient;

public class Websocket extends WebsocketBase implements IWebsocket {
    
    private static final String TAG = "Websocket";
    
    private IMethodResult mListener;
    private WebSocketClient mClient;
    private WebsocketFactory mFactory;
    
    public Websocket(WebsocketFactory factory, String id, String url) {
        super(id);
        mListener = null;
        mFactory = factory;
        
        mClient = new WebSocketClient(URI.create(url),new WebSocketClient.Listener() {
            @Override
            public void onConnect() {
                if ( mListener != null ) {
                    Map<String,Object> result = new HashMap<String,Object>();
                    result.put("event","onConnect");
                    mListener.set(result);
                }
            }
            
            @Override
            public void onMessage(String message) {
                if ( mListener != null ) {
                    Map<String,Object> result = new HashMap<String,Object>();
                    result.put("event","onMessage");
                    result.put("message",message);
                    mListener.set(result);
                }
            }
            
            @Override
            public void onMessage(byte[] data) {
                if ( mListener != null ) {
                    Map<String,Object> result = new HashMap<String,Object>();
                    result.put("event","onMessage");
                    result.put("message",new String(data));
                    mListener.set(result);
                }
            }
                      
            @Override
            public void onDisconnect(int code, String reason) {
                if ( mListener != null ) {
                    Map<String,Object> result = new HashMap<String,Object>();
                    result.put("event","onDisconnect");
                    mListener.set(result);
                }
            }
                      
            @Override
            public void onError(Exception error) {
                reportError(error);
            }
            },
            null
        );
        
    }
    
    public void setListener(IMethodResult listener) {
        mListener = listener;
    }
        
    public void destroy(IMethodResult result) {
        mFactory.destroy(this);
    }

    @Override
    public void connect(IMethodResult result) {
        try {
            mClient.connect();
        } catch (Exception e) {
            reportError(e);
        }
    }
    

    @Override
    public void disconnect(IMethodResult result) {
        try {
            mClient.disconnect();
        } catch (Exception e) {
            reportError(e);
        }
    }
    
    @Override
    public void send( String message, IMethodResult result) {
        try {
            mClient.send(message);
        } catch (Exception e) {
            reportError(e);
        }
    }
    
    private void reportError(Exception e) {
        if ( mListener != null ) {
            Map<String,Object> result = new HashMap<String,Object>();
            result.put("event","onError");
            result.put("error",e.getMessage());
            mListener.set(result);
        }
    }
}