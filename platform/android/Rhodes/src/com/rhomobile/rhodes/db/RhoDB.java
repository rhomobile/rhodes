package com.rhomobile.rhodes.db;

import java.io.File;

import com.rho.db.DBException;
import com.rho.db.IDBResult;
import com.rhomobile.rhodes.AndroidR;
import android.content.Context;
import android.database.Cursor;
import android.database.DatabaseUtils;
import android.database.SQLException;
import android.database.sqlite.SQLiteConstraintException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteStatement;
import android.os.Environment;
import android.util.Log;

public class RhoDB extends SQLiteOpenHelper {

	private static final int DATABASE_VERSION = 1;// 1.0.0

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

	public RhoDB(Context ctx, String dbPath, String dbName) {
		super(ctx, dbName, null, DATABASE_VERSION);

		this.ctx = ctx;
		dbVersion = DATABASE_VERSION;

		File destination = new File(dbPath);
		if (destination.mkdir()) {
			Log.d("RhoDB", "Database data directory created");
		}

		if (destination.exists()) {
			useLocal = false;
			dbPathToUse = dbPath + dbName;
		} else {
			useLocal = true;
			dbPathToUse = dbName;
		}
	}

	public void open() throws DBException {
		try {
			db = this.getWritableDatabase();
		} catch (Exception e) {
			Log.e("RhoDB", e.getMessage());
			throw new DBException(e);
		}
	}

	public void loadSchema(String schema) throws DBException {
		try {
			newVersion = true;

			db.beginTransaction();

			String[] statements = schema.split(";");
			for (int i = 0; i < statements.length; i++) {
				SQLiteStatement st = db.compileStatement(statements[i]);
				st.execute();
			}

			statements = ctx.getString(AndroidR.string.db_schema).split(";");
			for (int i = 0; i < statements.length; i++) {
				SQLiteStatement st = db.compileStatement(statements[i]);
				st.execute();
			}
			
			SQLiteStatement stTrigger = db.compileStatement(ctx
					.getString(AndroidR.string.deleteTrigger));
			stTrigger.execute();
			
			stTrigger = db.compileStatement(ctx.
					getString(AndroidR.string.insertTrigger));
			stTrigger.execute();

			db.setVersion(dbVersion);

			db.setTransactionSuccessful();
		} catch (Exception e) {
			Log.e(LOG_TAG, e.getMessage());
			throw new DBException(e);
		} finally {
			if (db != null)
				db.endTransaction();
		}
	}

	@Override
	public synchronized SQLiteDatabase getWritableDatabase() {
		if (useLocal) {
			return super.getWritableDatabase();
		}

		if (db != null && db.isOpen() && !db.isReadOnly()) {
			return db; // The database is already open for business
		}

		if (isInitializing) {
			throw new IllegalStateException(
					"getWritableDatabase called recursively");
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

	public void beginTransaction() {
		if (db == null)
			throw new SQLException(
					"Database must be opened before begin transaction!");

		if (!db.inTransaction())
			db.beginTransaction();
	}

	public void endTransaction() {
		if (db == null)
			throw new SQLException(
					"Database must be opened before end transaction!");

		if (db.inTransaction()) {
			db.setTransactionSuccessful();
			db.endTransaction();
		}
	}
	
	public void rollbackTransaction() {
		if (db == null)
			throw new SQLException(
					"Database must be opened before rollback transaction");
		
		if (db.inTransaction()) {
			db.endTransaction();
		}
	}
	
	public IDBResult executeSQL(String strStatement, Object[] values) throws DBException {
		return executeSQL(strStatement, values, false);
	}

	public IDBResult executeSQL(String strStatement, Object[] values, boolean bReportNonUnique)
			throws DBException {

		if (db == null)
			throw new DBException(new SQLException(
					"Database must be opened before insert new data!"));

		String[] params = null;
		if (values != null && values.length > 0) {
			params = new String[values.length];

			for (int i = 0; i < values.length; i++) {
				bindParams(params, i, values[i]);
			}
		}

		Cursor cursor = null;
		try {
			cursor = db.rawQuery(strStatement, params);
			SqliteDBResult result = new SqliteDBResult();
			result.copy(cursor);
			return result;
		}
		catch (SQLiteConstraintException e) {
			if (bReportNonUnique)
				return new SqliteDBResult(true);
			Log.e(LOG_TAG, "SQL error", e);
			return null;
		}
		catch (Exception e) {
			Log.e(LOG_TAG, "SQL error", e);
			return null;
		}
		finally {
			if (cursor != null)
				cursor.close();
		}
	}

	public void bindParams(String[] params, int i, Object value) {
		if (value == null) {
			params[i] = "null";
		} else if (value instanceof Double || value instanceof Float) {
			params[i] = new Double(((Number) value).doubleValue()).toString();
		} else if (value instanceof Number) {
			params[i] = new Long(((Number) value).longValue()).toString();
		} else if (value instanceof Boolean) {
			Boolean bool = (Boolean) value;
			params[i] = bool ? "1" : "0";
		} else {
			params[i] = value.toString();
		}
	}

	public void close() {
		if (db != null)
			db.close();
		db = null;
	}

	public boolean isOpen() {
		if (db != null)
			return db.isOpen();

		return false;
	}

	@Override
	public void onCreate(SQLiteDatabase arg0) {

	}

	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
	}

}
