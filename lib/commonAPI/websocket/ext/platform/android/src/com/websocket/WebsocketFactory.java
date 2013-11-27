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
            
//    private int mObjId = 1;
//    Map<String,Websocket> mObjectMap = new HashMap<String,Websocket>();
/*
    private String getNextId() {
        return String.valueOf(mObjId++);
    }
*/            
/*
    public String create(socketFactory.create: " + id );
    
        String id = getNextId();

        Websocket obj = new Websocket(this,id);
        mObjectMap.put(id,obj);
        return id;
    }
    */
/*
    public void destroy(Websocket ws) {
        Logger.I(TAG,"WebsocketFactory.destroy");

        mObjectMap.remove(ws.getId());
    }
*/
    @Override
    protected Websocket createApiObject(String id) {
        Logger.I(TAG,"WebsocketFactory.createApiObject: " + id);
        
        return new Websocket(this,id/*,url*/);
        /*
        if ( !mObjectMap.containsKey(id) ) {
            create(id);
        }
            
        return mObjectMap.get(id);
        */
    }
}
