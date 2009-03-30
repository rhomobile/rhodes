package com.rhomobile.rhodes.db.ContentValues;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyValue;

import android.content.ContentValues;
import android.database.Cursor;

public class BaseContentValues {

	public static final RubyString ID = ObjectFactory.createString("id");
	public static final RubyString SOURCE_ID = ObjectFactory.createString("source_id");
	public static final RubyString SOURCE_URL = ObjectFactory.createString("source_url");
	public static final RubyString SESSION = ObjectFactory.createString("session");
	public static final RubyString TOKEN = ObjectFactory.createString("token");
	public static final RubyString TYPE = ObjectFactory.createString("type");
	
	public static final String ALL = "*";
	
	protected ContentValues values = new ContentValues();
	
	public BaseContentValues(RubyHash hash){
		RubyValue val = hash.getValue(ID);
        if ( val != RubyConstant.QNIL )
        	values.put(ID.asString(), ((long) val.toInt()) & 0xFFFFFFFFL );
        
        val = hash.getValue(SOURCE_ID);
        if ( val != RubyConstant.QNIL )
        	values.put(SOURCE_ID.asString(), val.toInt());
        
        val = hash.getValue(TOKEN);
        if ( val != RubyConstant.QNIL )
        	values.put(TOKEN.asString(), val.toString());
	}

	public ContentValues getValues() {
		return values;
	}
	
	public static RubyHash db2hash(RubyHash res, Cursor c, String column, int columnIndex){
    	boolean bAll = column.equals(ALL);
    	
    	if ( bAll || column.equals(SOURCE_ID.asString()) )
    		res.add( SOURCE_ID, ObjectFactory.createInteger(c.getLong(columnIndex)) );
    	if ( bAll || column.equals(ID.asString()) )
    		res.add( ID, ObjectFactory.createInteger(c.getLong(columnIndex)) );
    	if ( bAll || column.equals(TOKEN.asString()) )
    		res.add( TOKEN, ObjectFactory.createString(c.getString(columnIndex)) );
    	
    	return res;
    }
}
