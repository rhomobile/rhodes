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
    
    private boolean mOpenEventPending = false;
    private boolean mErrorEventPending = false;
    private Exception mErrorPending;
    private boolean mCloseEventPending = false;
    private int mCloseCodePending;
    private String mCloseReasonPending;
    
    public Websocket(WebsocketFactory factory, String id) {
        super(id);
        
        Logger.I(TAG,"Websocket ctor, id: " + id);
        mFactory = factory;
    }
    
    synchronized public void setOnopen(IMethodResult result) {
        Logger.I(TAG,"Websocket setOnopen");
        mOnOpen = result;
        if ( mOpenEventPending && ( mOnOpen != null ) && ( mOnOpen.hasCallback() ) ) {
            Logger.I(TAG,"Pending open event, calling callback immediately");
            mOpenEventPending = false;
            dispatchOpenEvent();
        }
    }
    
    synchronized public void setOnmessage(IMethodResult result) {
        Logger.I(TAG,"Websocket setOnmessage");
        mOnMessage = result;
    }
    
    synchronized public void setOnerror(IMethodResult result) {
        Logger.I(TAG,"Websocket setOnerror");
        mOnError = result;
        if ( mErrorEventPending && ( mOnError != null ) && ( mOnError.hasCallback() ) ) {
            Logger.I(TAG,"Pending error event, calling callback immediately");

            mErrorEventPending = false;
            dispatchErrorEvent(mErrorPending);
        }
    }
    
    synchronized public void setOnclose(IMethodResult result) {
        Logger.I(TAG,"Websocket setOnclose");
        mOnClose = result;
        if ( mCloseEventPending && ( mOnClose != null ) && ( mOnClose.hasCallback() ) ) {
            Logger.I(TAG,"Pending close event, calling callback immediately");

            mCloseEventPending = false;
            dispatchCloseEvent(mCloseCodePending,mCloseReasonPending);
        }
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
            
                synchronized( Websocket.this ) {
                    mState = STATE_OPEN;
                    
                    if ( (mOnOpen != null) && (mOnOpen.hasCallback() ) ) {
                        dispatchOpenEvent();
                    } else {
                        mOpenEventPending = true;
                    }
                }
            }
            
            @Override
            public void onMessage(String message) {
                synchronized( Websocket.this ) {
                    dispatchMessageEvent(message);
                }
            }
            
            @Override
            public void onMessage(byte[] data) {
                synchronized( Websocket.this ) {
                    dispatchMessageEvent(new String(data));
                }
            }
                      
            @Override
            public void onDisconnect(int code, String reason) {
                synchronized( Websocket.this ) {
                    mState = STATE_CLOSED;
                    
                    if ( ( mOnClose != null ) && ( mOnClose.hasCallback() ) ) {
                        dispatchCloseEvent(code,reason);
                    } else {
                        mCloseEventPending = true;
                        mCloseCodePending = code;
                        mCloseReasonPending = reason;
                    }
                }
            }
                      
            @Override
            public void onError(Exception error) {
                synchronized( Websocket.this ) {
                    if ( ( mOnError != null ) && ( mOnError.hasCallback() ) ) {
                        dispatchErrorEvent(error);
                    } else {
                        mErrorEventPending = true;
                        mErrorPending = error;
                    }
                }
            }
            },
            null
        );
        
        connect(result);
    }

    private void connect( IMethodResult result ) {
        mState = STATE_CONNECTING;

        try {
            mClient.connect();
        } catch (Exception e) {
            result.setError( e.getMessage() );
        }
    }
    

    @Override
    synchronized public void close(IMethodResult result) {
        mState = STATE_CLOSING;
        
        try {
            mClient.disconnect("User request.");
        } catch (Exception e) {
            result.setError( e.getMessage() );
        }
    }
    
    @Override
    synchronized public void send( String message, IMethodResult result) {
        try {
            mClient.send(message);
        } catch (Exception e) {
            result.setError( e.getMessage() );
        }
    }
    
    private void dispatchErrorEvent(Exception e) {
        Logger.E(TAG,"Websocket error: " + e.getMessage());

        if ( mOnError != null ) {
            Map<String,Object> result = new HashMap<String,Object>();
            result.put("message",e.getMessage());
            mOnError.set(result);
        }
    }
    
    private void dispatchOpenEvent() {
        if ( mOnOpen != null ) {
            Map<String,Object> result = new HashMap<String,Object>();
            mOnOpen.set(result);
        }
    }
    
    private void dispatchCloseEvent(int code, String reason) {
        if ( mOnClose != null ) {
            Map<String,Object> result = new HashMap<String,Object>();
            result.put("code",code);
            result.put("reason",reason);
            mOnClose.set(result);
        }
    }
    
    private void dispatchMessageEvent( String message ) {
        if ( mOnMessage != null ) {
            Map<String,Object> result = new HashMap<String,Object>();
            result.put("data",message);
            mOnMessage.set(result);
        }
    }
}