package com.rhomobile.rhodes.db.ContentValues;

import android.database.Cursor;

import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyValue;

public class ObjectValuesContentValues extends BaseContentValues {

	private static final RubyString ATTRIB = ObjectFactory.createString("attrib");
	private static final RubyString OBJECT = ObjectFactory.createString("object");
	private static final RubyString VALUE = ObjectFactory.createString("value");
	private static final RubyString UPDATE_TYPE = ObjectFactory.createString("update_type");

	public ObjectValuesContentValues(RubyHash hash) {
		super(hash);
		
		RubyValue val = hash.getValue(ATTRIB);
        if ( val != RubyConstant.QNIL )
        	values.put(ATTRIB.asString(), val.toStr());
        
        val = hash.getValue(OBJECT);
        if ( val != RubyConstant.QNIL )
        	values.put(OBJECT.asString(), val.toStr());
        
        val = hash.getValue(VALUE);
        if ( val != RubyConstant.QNIL )
        	values.put(VALUE.asString(), val.toStr());

        val = hash.getValue(UPDATE_TYPE);
        if ( val != RubyConstant.QNIL )
        	values.put(UPDATE_TYPE.asString(), val.toStr());

        val = hash.getValue(TYPE);
        if ( val != RubyConstant.QNIL )
        	values.put(TYPE.asString(), val.toStr());
	}
	
	public static RubyHash db2hash(RubyHash res, Cursor c, String column, int columnIndex)
	{
		BaseContentValues.db2hash(res, c, column, columnIndex);
    	boolean bAll = column.equals(ALL);
    	
    	if ( bAll || column.equals(ATTRIB.asString()) )
    		res.add( ATTRIB, ObjectFactory.createString(c.getString(columnIndex)) );
    	if ( bAll || column.equals(OBJECT.asString()) )
    		res.add( OBJECT, ObjectFactory.createString(c.getString(columnIndex)) );
    	if ( bAll || column.equals(VALUE.asString()) )
    		res.add( VALUE, ObjectFactory.createString(c.getString(columnIndex)) );
    	if ( bAll || column.equals(UPDATE_TYPE.asString()) )
    		res.add( UPDATE_TYPE, ObjectFactory.createString(c.getString(columnIndex)) );
    	if ( bAll || column.equals(TYPE.asString()) )
    		res.add( TYPE, ObjectFactory.createString(c.getString(columnIndex)) );
    	
    	return res;
	}
}
