package com.rhomobile.rhodes.db;

import java.io.File;

import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.db.ContentValues.BaseContentValues;
import com.rhomobile.rhodes.db.ContentValues.ClientInfoContentValues;
import com.rhomobile.rhodes.db.ContentValues.ObjectValuesContentValues;
import com.rhomobile.rhodes.db.ContentValues.SourcesContentValues;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyValue;

import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteStatement;
import android.os.Environment;
import android.util.Log;

public class RhoDB extends SQLiteOpenHelper {

	private static final int DATABASE_VERSION = 100;//1.0.0
	
	private static final String DATABASE_NAME = "rho_sync.db";
	private static final String DATABASE_PATH_EXTERNAL = "/sdcard/rhomobile/rho_sync.db";
	private static final String DATABASE_PATH_LOCAL = "rho_sync.db";
	
	private static final String LOG_TAG = "RhoDB";

	public static final String CLIENT_INFO_TABLE = "client_info";
	public static final String OBJECT_VALUES_TABLE = "object_values";
	public static final String SOURCES_TABLE = "sources";
	
	private SQLiteDatabase db;
	private Context ctx;
	private int dbVersion;
	private String dbPathToUse;
	private boolean useLocal = false;
	private boolean newVersion = false;
	private boolean isInitializing = false;
	
	public RhoDB(Context ctx) {
		super(ctx, DATABASE_NAME, null, DATABASE_VERSION);
		
		this.ctx = ctx;
		dbVersion = DATABASE_VERSION;
		
		File sdcard = Environment.getExternalStorageDirectory();
		
		File destination = new File(sdcard, "rhomobile");
		if (destination.mkdir()) {
			Log.d("RhoDB", "Application data directory created");
		}
		
		if (destination.exists()) {
			useLocal = false;
			dbPathToUse = DATABASE_PATH_EXTERNAL;
		} else {
			useLocal = true;
			dbPathToUse = DATABASE_PATH_LOCAL;
		}
	}

	public void openDb(Integer version) throws SQLException, Exception {
		try {
			db = this.getWritableDatabase();//ctx.openOrCreateDatabase(DATABASE_NAME, SQLiteDatabase.OPEN_READWRITE, null);
			
			if ( version != null )
				dbVersion = version.intValue();
			
			if (dbVersion != db.getVersion()) {
				onCreate(db);
			}
		} catch (Exception e) {
			Log.e("RhoDB", e.getMessage());
		}
	}
	
	@Override
	public void onCreate(SQLiteDatabase db_)
	{
		try {
			newVersion = true;
			
			db = db_;
			
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
		}
		catch (Exception e) {
			Log.e(LOG_TAG, e.getMessage());
			db = null;
		}
		finally {
			if ( db != null )
				db.endTransaction();
		}
	}
	
	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
		Log.w("RhoDB", "Upgrading database from version " + oldVersion + " to " + newVersion
				+ ", which will destroy all old data");
		
		onCreate(db);
	}
	
	@Override
	public synchronized SQLiteDatabase getWritableDatabase() {
		if(useLocal) {
			return super.getWritableDatabase();
		}
	
		if (db != null && db.isOpen() && !db.isReadOnly()) {
			return db; // The database is already open for business
		}

		if (isInitializing) {
			throw new IllegalStateException("getWritableDatabase called recursively");
		}

		SQLiteDatabase mDatabase = null;
	
		try {
			isInitializing = true;
			mDatabase = SQLiteDatabase.openOrCreateDatabase(dbPathToUse, null);
			onOpen(mDatabase);
			return mDatabase;
		} finally {
			isInitializing = false;
		}
	}	
		
	public boolean isNewVersion() {
		return newVersion;
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
		catch ( Exception e ){
			Log.e("RhoDB", e.getMessage());
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
