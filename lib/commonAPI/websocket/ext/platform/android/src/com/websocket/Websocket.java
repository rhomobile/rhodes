package com.websocket;

import java.util.Map;
import java.util.HashMap;

import java.net.URI;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.api.MethodResult;
import com.rhomobile.rhodes.Logger;

public class Websocket extends WebsocketBase implements IWebsocket {

    private static final int STATE_CONNECTING   = 0;
    private static final int STATE_OPEN         = 1;
    private static final int STATE_CLOSING      = 2;
    private static final int STATE_CLOSED       = 3;
    
    private int mState = -1;
    
    private static final String TAG = "Websocket";
    
    private IMethodResult mOnOpen;
    private IMethodResult mOnMessage;
    private IMethodResult mOnError;
    private IMethodResult mOnClose;
    
    private WebSocketClient mClient;
    private WebsocketFactory mFactory;
    private String mURL;
    
    public Websocket(WebsocketFactory factory, String id) {
        super(id);
        
        Logger.I(TAG,"Websocket ctor, id: " + id);
        mFactory = factory;
    }
    
    public void setOnopen(IMethodResult result) {
        Logger.I(TAG,"Websocket setOnopen");
        mOnOpen = result;
    }
    
    public void setOnmessage(IMethodResult result) {
        Logger.I(TAG,"Websocket setOnmessage");
        mOnMessage = result;
    }
    
    public void setOnerror(IMethodResult result) {
        Logger.I(TAG,"Websocket setOnerror");
        mOnError = result;
    }
    
    public void setOnclose(IMethodResult result) {
        Logger.I(TAG,"Websocket setOnclose");
        mOnClose = result;
    }
    
    public void getUrl(IMethodResult result) {
        result.set(mURL);
    }
    
    public void getReadyState(IMethodResult result) {
        result.set(mState);
    }
    
    public void getExtensions(IMethodResult result) {
        result.set("");
    }
    
    public void getProtocol(IMethodResult result) {
        result.set("");
    }
    
    @Override
    public void create(String url, String protocols, IMethodResult result) {
        Logger.I(TAG,"Websocket create, url:" + url);

        if ( url.length() == 0 ) {
          result.setError("Websocket URL is empty.");
          return;
        }
        
        mURL = url;

        mClient = new WebSocketClient(URI.create(url),new WebSocketClient.Listener() {
            @Override
            public void onConnect() {
                mState = STATE_OPEN;
            
                if ( mOnOpen != null ) {
                    Map<String,Object> result = new HashMap<String,Object>();
                    mOnOpen.set(result);
                }
            }
            
            @Override
            public void onMessage(String message) {
                if ( mOnMessage != null ) {
                    Map<String,Object> result = new HashMap<String,Object>();
                    result.put("data",message);
                    mOnMessage.set(result);
                }
            }
            
            @Override
            public void onMessage(byte[] data) {
                if ( mOnMessage != null ) {
                    Map<String,Object> result = new HashMap<String,Object>();
                    result.put("data",new String(data));
                    mOnMessage.set(result);
                }
            }
                      
            @Override
            public void onDisconnect(int code, String reason) {
                mState = STATE_CLOSED;
                
                if ( mOnClose != null ) {
                    Map<String,Object> result = new HashMap<String,Object>();
                    result.put("code",code);
                    result.put("reason",reason);
                    mOnClose.set(result);
                }
            }
                      
            @Override
            public void onError(Exception error) {
                reportError(error);
            }
            },
            null
        );
        
        connect(result);
    }

    public void connect( IMethodResult result ) {
        mState = STATE_CONNECTING;

        try {
            mClient.connect();
        } catch (Exception e) {
            result.setError( e.getMessage() );
        }
    }
    

    @Override
    public void close(IMethodResult result) {
        mState = STATE_CLOSING;
        
        try {
            mClient.disconnect();
        } catch (Exception e) {
            result.setError( e.getMessage() );
        }
    }
    
    @Override
    public void send( String message, IMethodResult result) {
        try {
            mClient.send(message);
        } catch (Exception e) {
            result.setError( e.getMessage() );
        }
    }
    
    private void reportError(Exception e) {
        Logger.E(TAG,"Websocket error: " + e.getMessage());

        if ( mOnError != null ) {
            Map<String,Object> result = new HashMap<String,Object>();
            result.put("message",e.getMessage());
            mOnError.set(result);
        }
    }
}