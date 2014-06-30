package com.rho.websocket;

import java.util.LinkedList;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.Logger;


class WebSocketSingleton extends WebSocketSingletonBase implements IWebSocketSingleton {
    private static final String TAG = "WebSocketSingleton";
    
    private WebSocketFactory mFactory;
    
    public WebSocketSingleton(WebSocketFactory factory) {
        mFactory = factory;
    }

}