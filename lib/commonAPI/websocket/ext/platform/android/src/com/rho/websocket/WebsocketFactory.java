package com.rho.websocket;

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
        return new WebsocketSingleton(this);
    }
            
    private int mObjId = 1;
    Map<String,Websocket> mObjectMap = new HashMap<String,Websocket>();
            
    private String getNextId() {
        return String.valueOf(mObjId++);
    }
            
    public String create(String url) {
        String id = getNextId();

        Websocket obj = new Websocket(this,id,url);
        mObjectMap.put(id,obj);
        return id;
    }
            
    public void destroy(Websocket ws) {
        mObjectMap.remove(ws.getId());
    }

    @Override
    protected Websocket createApiObject(String id) {
        return mObjectMap.get(id);
    }
}
