package com.websocket;

import java.util.LinkedList;
import java.util.List;

import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.Logger;


class WebsocketSingleton extends WebsocketSingletonBase implements IWebsocketSingleton {
    private static final String TAG = "WebsocketSingleton";
    
    private WebsocketFactory mFactory;
    
    public WebsocketSingleton(WebsocketFactory factory) {
        mFactory = factory;
    }

}