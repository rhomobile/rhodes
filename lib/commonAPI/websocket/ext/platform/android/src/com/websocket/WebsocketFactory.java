package com.websocket;

import com.rhomobile.rhodes.api.RhoApiFactory;
import java.util.Map;
import java.util.HashMap;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;


public class WebsocketFactory
        extends RhoApiFactory< Websocket, WebsocketSingleton>
        implements IWebsocketFactory {
            
    private static final String TAG = "WebsocketFactory";

    @Override
    protected WebsocketSingleton createSingleton() {
        Logger.I(TAG,"WebsocketFactory.createSingleton");

        return new WebsocketSingleton(this);
    }
            
    @Override
    protected Websocket createApiObject(String id) {
        Logger.I(TAG,"WebsocketFactory.createApiObject: " + id);
        
        return new Websocket(this,id/*,url*/);
    }
}
