package com.rhomobile.rhodes.db.ContentValues;

import android.database.Cursor;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyValue;

public class SourcesContentValues extends BaseContentValues{

	public static final RubyString LAST_UPDATED = ObjectFactory.createString("last_updated");
    public static final RubyString LAST_INSERTED_SIZE = ObjectFactory.createString("last_inserted_size");
    public static final RubyString LAST_DELETED_SIZE = ObjectFactory.createString("last_deleted_size");
    public static final RubyString LAST_SYNC_DURATION = ObjectFactory.createString("last_sync_duration");
    public static final RubyString LAST_SYNC_SUCCESS = ObjectFactory.createString("last_sync_success");
    
	public SourcesContentValues(RubyHash hash) {
		super(hash);
		
		RubyValue val = hash.getValue(SOURCE_URL);
        if ( val != RubyConstant.QNIL )
        	values.put(SOURCE_URL.asString(), val.toStr());
        
        val = hash.getValue(SESSION);
        if ( val != RubyConstant.QNIL )
        	values.put(SESSION.asString(), val.toStr());
        
        val = hash.getValue(LAST_UPDATED);
        if ( val != RubyConstant.QNIL )
        	values.put(LAST_UPDATED.asString(), val.toInt());
        
        val = hash.getValue(LAST_INSERTED_SIZE);
        if ( val != RubyConstant.QNIL )
        	values.put(LAST_INSERTED_SIZE.asString(), val.toInt());
        
        val = hash.getValue(LAST_DELETED_SIZE);
        if ( val != RubyConstant.QNIL )
        	values.put(LAST_DELETED_SIZE.asString(), val.toInt());

        val = hash.getValue(LAST_SYNC_DURATION);
        if ( val != RubyConstant.QNIL )
        	values.put(LAST_SYNC_DURATION.asString(), val.toInt());
        
        val = hash.getValue(LAST_SYNC_SUCCESS);
        if ( val != RubyConstant.QNIL )
        	values.put(LAST_SYNC_SUCCESS.asString(), val.toInt());
	}

	public static RubyHash db2hash(RubyHash res, Cursor c, String column, int columnIndex)
	{
		BaseContentValues.db2hash(res, c, column, columnIndex);
    	boolean bAll = column.equals(ALL);
    	
		if ( bAll || column.equals(SOURCE_URL.asString()) )
    		res.add( SOURCE_URL, ObjectFactory.createString(c.getString(columnIndex)) );
    	if ( bAll || column.equals(SESSION.asString()) )
    		res.add( SESSION, ObjectFactory.createString(c.getString(columnIndex)) );
    	if ( bAll || column.equals(LAST_UPDATED.asString()) )
    		res.add( LAST_UPDATED, ObjectFactory.createInteger(c.getLong(columnIndex)) );
    	if ( bAll || column.equals(LAST_INSERTED_SIZE.asString()) )
    		res.add( LAST_INSERTED_SIZE, ObjectFactory.createInteger(c.getLong(columnIndex)) );
    	if ( bAll || column.equals(LAST_DELETED_SIZE.asString()) )
    		res.add( LAST_DELETED_SIZE, ObjectFactory.createInteger(c.getLong(columnIndex)) );
    	if ( bAll || column.equals(LAST_SYNC_DURATION.asString()) )
    		res.add( LAST_SYNC_DURATION, ObjectFactory.createInteger(c.getLong(columnIndex)) );
    	if ( bAll || column.equals(LAST_SYNC_SUCCESS.asString()) )
    		res.add( LAST_SYNC_SUCCESS, ObjectFactory.createInteger(c.getLong(columnIndex)) );
    	
		return res;
	}
}
