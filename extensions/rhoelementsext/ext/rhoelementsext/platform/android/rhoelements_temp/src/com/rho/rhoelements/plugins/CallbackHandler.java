package com.rho.rhoelements.plugins;

import java.lang.reflect.Method;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

/**
 * @author Geoff Day (XFH386, Initial Creation)
 *
 */
public class CallbackHandler extends Handler
{
    private Plugin callingObject; 
    
    public CallbackHandler(Plugin callingObject)
    {
	this.callingObject = callingObject;
    }
    
    public void handleMessage(Message msg) 	
    {
		Class<? extends Plugin> cls = callingObject.getClass();
		Method mymethod = null;
	
		// Just for Debugging Purposes
		//Method[] ms = cls.getDeclaredMethods();
		//for (int i = 0; i < ms.length; i++)     Log.d("ListMethos",ms[i].toString());
	
		try 
		{
		    mymethod = cls.getDeclaredMethod(msg.getData().getString("CALLBACK_METHOD_NAME"),Bundle.class);
		    mymethod.invoke(callingObject, msg.getData());
		}
		catch (Exception e1)
		{
		    e1.printStackTrace();
		}
    }    
}
