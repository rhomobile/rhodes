package com.rhomobile.rhodes.db;

import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.db.ContentValues.BaseContentValues;
import com.rhomobile.rhodes.db.ContentValues.ClientInfoContentValues;
import com.rhomobile.rhodes.db.ContentValues.ObjectValuesContentValues;
import com.rhomobile.rhodes.db.ContentValues.SourcesContentValues;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyValue;

import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteStatement;
import android.util.Log;

public class RhoDB {

	private static final int DATABASE_VERSION = 100;//1.0.0
	private static final String DATABASE_NAME = "rho_syncdb";
	private static final String LOG_TAG = "RhoDB";

	public static final String CLIENT_INFO_TABLE = "client_info";
	public static final String OBJECT_VALUES_TABLE = "object_values";
	public static final String SOURCES_TABLE = "sources";
	
	private SQLiteDatabase db;
	private Context ctx;
	private int dbVersion;

	public RhoDB(Context ctx) {
		this.ctx = ctx;
		dbVersion = DATABASE_VERSION;
	}

	public void openDb(Integer version) throws SQLException, Exception {
		try {
			db = ctx.openOrCreateDatabase(DATABASE_NAME, SQLiteDatabase.OPEN_READWRITE, null);
			
			if ( version != null )
				dbVersion = -26;//version.intValue();
			
			if (dbVersion != db.getVersion()) {
				db.close();
				ctx.deleteDatabase(DATABASE_NAME);
				createDb();
			}
		} catch (Exception e) {
			createDb();
		}
	}
	
	private void createDb() throws SQLException, Exception 
	{
		try {
			db = ctx.openOrCreateDatabase(DATABASE_NAME,
					SQLiteDatabase.OPEN_READWRITE | SQLiteDatabase.CREATE_IF_NECESSARY, 
					null);
			
			db.beginTransaction();
			
			String [] statments = ctx.getString(R.string.db_schema).split(";");
			for ( int i = 0; i < statments.length; i++) 
			{
				SQLiteStatement st = db.compileStatement(statments[i]);
				st.execute();
			}
			
			SQLiteStatement st = db.compileStatement(ctx.getString(R.string.deleteTrigger));
			st.execute();
			
			db.setVersion(dbVersion);
			
			db.setTransactionSuccessful();
		} 
		catch (SQLException e) {
			Log.e(LOG_TAG, e.getMessage());
			db = null;
			
			throw e;
		}
		catch (Exception e) {
			Log.e(LOG_TAG, e.getMessage());
			db = null;
			
			throw e;
		}
		finally {
			if ( db != null )
				db.endTransaction();
		}
	}
	
	public void beginTransaction()
	{
		if ( db == null )
			throw new SQLException("Database must be opened before begin transaction!");
		
		if ( !db.inTransaction() )
			db.beginTransaction();
	}

	public void endTransaction()
	{
		if ( db == null )
			throw new SQLException("Database must be opened before end transaction!");
		
		if ( db.inTransaction() )
			db.endTransaction();
	}
	
	public boolean insert(String table, BaseContentValues values)
	{
		if ( db == null )
			throw new SQLException("Database must be opened before insert new data!");
		
		beginTransaction();
		//if ( !db.inTransaction() )
		//	throw new SQLException("Transaction must be started before insert new data!");
		
		try
		{
			long result = db.insert(table, null, values.getValues());
			
			if ( result > 0 )
				db.setTransactionSuccessful();
			
			return result > 0;
		}
		catch (Exception e ){
			Log.e( LOG_TAG, e.getMessage());
		}
		finally {
			endTransaction();
		}
		
		return false;
	}
	
	public RubyValue executeSql( boolean distinct, String table, 
			String columnStr, String orderBy, String selection )
	{
		if ( db == null )
			throw new SQLException("Database must be opened before select any data!");
		
		RubyArray res = new RubyArray();
		
		try
		{
			String[] clnms = columnStr.split(",");
			
			Cursor c = db.query(distinct, table, clnms, selection, null, null, null, orderBy, null);
			
			try
			{
				int numRows = c.getCount();
				
				c.moveToFirst();
		        
		        String[] columns = c.getColumnNames();
		        for (int i = 0; i < numRows; ++i) {
		            
		        	RubyHash record = ObjectFactory.createHash();
		        	
		        	for ( int j = 0; j < columns.length; j++)
		        	{
		        		if ( !columnStr.trim().equals("*") )
		        		{
		        			boolean bSkip = false;
		        			for ( int k = 0; k < clnms.length; k++ )
		        			{
		        				if ( !columns[j].equalsIgnoreCase(clnms[k].trim()) )
		        				{
		        					bSkip = true;
		        					break;
		        				}
		        			}
		        			
		        			if ( bSkip )
		        				continue;
		        		}
		        		
		        		
		        		if ( table.equalsIgnoreCase(RhoDB.CLIENT_INFO_TABLE) )
			    			ClientInfoContentValues.db2hash(record, c, columns[j], j);
			    		else if ( table.equalsIgnoreCase(RhoDB.OBJECT_VALUES_TABLE) )
			    			ObjectValuesContentValues.db2hash(record, c, columns[j], j);
			    		else if ( table.equalsIgnoreCase(RhoDB.SOURCES_TABLE) )
			    			SourcesContentValues.db2hash(record, c, columns[j], j);
			    		else
			    			throw new RubyException("Unknown database name:" + table);
		        	}
		        	res.add(record);
		            c.moveToNext();
		        }
			}
			finally {
				c.close();
			}
		}
		catch ( Exception e ){
			Log.e( LOG_TAG, e.getMessage() );
		}

		return res;
	}
	
	public RubyValue executeCountSql( String sql )
	{
		if ( db == null )
			throw new SQLException("Database must be opened before select any data!");
		
		if ( sql == null || "".equals(sql))
			throw new SQLException("SQL query must be specified!");
		
		SQLiteStatement st = db.compileStatement(sql);
		long count = st.simpleQueryForLong();
		return ObjectFactory.createInteger(count);
	}
	
	public void executeSql( String sql )
	{
		if ( db == null )
			throw new SQLException("Database must be opened before select any data!");
		
		if ( sql == null || "".equals(sql))
			throw new SQLException("SQL query must be specified!");
		
		try {
			beginTransaction();
			
			db.execSQL(sql);
			
			db.setTransactionSuccessful();
		}
		finally {
			endTransaction();
		}
	}
	
	public void close()
	{
		if ( db != null )
			db.close();
		db = null;
	}
	
	public boolean isOpened()
	{
		if ( db != null )
			return db.isOpen();
		
		return false;
	}
}
