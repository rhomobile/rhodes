/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes.phonebook;


import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import com.rhomobile.rhodes.Logger;

public class Contact {
	
	private static final String TAG = "Contact";	
	//private static final boolean logging_enable = false;
	
	private String id = null;
	
	private Map<String, String> mFields = new HashMap<String, String>();
	
	public Contact() { }

    public Contact(ContactAccessor accessor, String id, String displayName) {
        this.id = id;
        mFields.put(Phonebook.PB_ID, id);
        mFields.put(Phonebook.PB_DISPLAY_NAME, displayName);
    }

    void reset(String id, String displayName) {
        mFields.clear();
        this.id = id;
        mFields.put(Phonebook.PB_ID, id);
        mFields.put(Phonebook.PB_DISPLAY_NAME, displayName);
    }

    boolean checkConditions(Map<String, Object> conditions)
    {
        if (conditions == null)
            return true;
        
        Set<String> cond_keys = conditions.keySet();
        for (String key: cond_keys) {
            Object condObj = conditions.get(key);
            String cond = null;
            boolean isNullFields = true;

            if (condObj instanceof String) {
                cond = (String)condObj;
            }
            //TODO: if (condObj instanceof Map<String, String>)

            Logger.I(TAG, "Check " + key + " " + cond + ", contact: " + id());
            
            
            if (key.equalsIgnoreCase("phone")) {
                isNullFields = getField(Phonebook.PB_BUSINESS_NUMBER) == null
                            && getField(Phonebook.PB_HOME_NUMBER) == null
                            && getField(Phonebook.PB_MOBILE_NUMBER) == null;
            }
            else if (key.equalsIgnoreCase("email")) {
                isNullFields = getField(Phonebook.PB_EMAIL_ADDRESS) == null;
            }

            if (cond.equalsIgnoreCase("not_nil")) {
                if (isNullFields)
                    return false;
            }
            else if (cond.equalsIgnoreCase("is_nil")) {
                if (!isNullFields)
                    return false;
            }
            
        }
        
        Logger.I(TAG, "Condition is passed");
        return true;
    }

	public void setField(String key, String value)
	{
		mFields.put(key,value);
	}
	
	public String getField(String key)
	{
		return mFields.get(key);
	}
	
	public void setId(String id) {
		this.id = id;
		setField(Phonebook.PB_ID, id);
	}
	
	public String id() {
		return id;
	}

}
