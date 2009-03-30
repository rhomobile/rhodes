package com.rhomobile.rhodes.db.ContentValues;

import android.database.Cursor;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyValue;

public class ClientInfoContentValues extends BaseContentValues {

	public static final RubyString CLIENT_ID = ObjectFactory.createString("client_id");
	
	public ClientInfoContentValues(RubyHash hash) {
		super(hash);
		
		RubyValue val = hash.getValue(CLIENT_ID);
        if ( val != RubyConstant.QNIL )
        	values.put(CLIENT_ID.asString(), val.toStr());
	}

	public static RubyHash db2hash(RubyHash res, Cursor c, String column, int columnIndex)
	{
		BaseContentValues.db2hash(res, c, column, columnIndex);
    	boolean bAll = column.equals(ALL);

    	if ( bAll || column.equals(CLIENT_ID.asString()) )
    		res.add( CLIENT_ID, ObjectFactory.createString(c.getString(columnIndex)) );
    	
		return res;
	}
	
}
