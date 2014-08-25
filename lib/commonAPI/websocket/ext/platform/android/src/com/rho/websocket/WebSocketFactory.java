package com.rho.websocket;

import com.rhomobile.rhodes.api.RhoApiFactory;
import java.util.Map;
import java.util.HashMap;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;


public class WebSocketFactory
        extends RhoApiFactory< WebSocket, WebSocketSingleton>
        implements IWebSocketFactory {
            
    private static final String TAG = "WebSocketFactory";

    @Override
    protected WebSocketSingleton createSingleton() {
        Logger.I(TAG,"WebSocketFactory.createSingleton");

        return new WebSocketSingleton(this);
    }
            
    @Override
    protected WebSocket createApiObject(String id) {
        Logger.I(TAG,"WebSocketFactory.createApiObject: " + id);
        
        return new WebSocket(this,id/*,url*/);
    }
}
