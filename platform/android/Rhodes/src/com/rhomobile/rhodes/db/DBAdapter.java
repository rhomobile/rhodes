package com.rhomobile.rhodes.db;

import com.rho.db.IDBAdapter;
import com.rho.db.IDbCallback;
import com.rhomobile.rhodes.RhodesInstance;
import com.rhomobile.rhodes.db.ContentValues.ClientInfoContentValues;
import com.rhomobile.rhodes.db.ContentValues.ObjectValuesContentValues;
import com.rhomobile.rhodes.db.ContentValues.SourcesContentValues;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;

public class DBAdapter extends RubyBasic implements IDBAdapter {

	private static DBAdapter adapter = null; // instance of the adapter

	private RhoDB storage;

	private IDbCallback callback;

	public DBAdapter(RubyClass c) {
		super(c);
	}

	// @RubyAllocMethod
	public static DBAdapter alloc(RubyValue receiver) {
		if (adapter == null) {
			adapter = new DBAdapter(RubyRuntime.DBAdapterClass);
		}
		return adapter;
	}

	public void setDbCallback(IDbCallback callback) {
		this.callback = callback;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.rhomobile.rhodes.db.IDBAdapter#initialize(com.xruby.runtime.lang.RubyValue)
	 */
	public synchronized DBAdapter initialize(RubyValue v) {

		Integer version = null;
		try {
			version = Integer.parseInt(v != null && v != RubyConstant.QNIL ? v
					.toString().replace(".", "") : "");
		} catch (Exception e) {
		}
		
		if (storage == null)
			storage = new RhoDB(RhodesInstance.getInstance());

		try {
			storage.openDb(version);
		} catch (Exception e) {
			return null;
		}
		
		try {
		    
			if ( this.callback != null && storage.isNewVersion() ){
				this.callback.OnDeleteAllFromTable("client_info");
				this.callback.OnDeleteAllFromTable("object_values");
				this.callback.OnDeleteAllFromTable("sources");
			}
		}
		catch (Exception e) {
		}


		return this;
	}

	// @RubyLevelMethod(name="insertIntoTable")
	/*
	 * (non-Javadoc)
	 * 
	 * @see com.rhomobile.rhodes.db.IDBAdapter#insertIntoTable(com.xruby.runtime.lang.RubyValue,
	 *      com.xruby.runtime.lang.RubyValue)
	 */
	public synchronized RubyValue insertIntoTable(RubyValue table, RubyValue values) {

		if ( table!= null && table != RubyConstant.QNIL && values != null && values != RubyConstant.QNIL ){
			
			storage.executeSql(createInsertStatement(table, (RubyHash)values));
		}

		return new RubyArray();
	}

	private String createInsertStatement(RubyValue table, RubyHash values) 
	{
		String retval = "insert into " + table;

		String columns = "";
		String valuesStr = "";
		if (values != null) {
			RubyArray keys = values.keys();
			for (int i = 0; i < keys.size(); i++) {
				RubyValue key = keys.get(i);
				RubyValue val = values.get(key);
				String type = val.getRubyClass().getName();
				
				String escapedValue = "";
				if ( type.equals("String"))
					escapedValue = "'" + escapeSQL(val.asString()) + "'";
				else
					escapedValue = val.asString();
				
				valuesStr += " " + escapedValue + ( i < (keys.size() - 1) ? "," : "" );
				columns += " " + key + ( i < (keys.size() - 1) ? "," : "" );
			}
		}
		retval += " (" + columns + ") values (" + valuesStr + " )";

		return retval;
		
	}
	
	
	// @RubyLevelMethod(name="selectFromTable")
	/*
	 * (non-Javadoc)
	 * 
	 * @see com.rhomobile.rhodes.db.IDBAdapter#selectFromTable(com.xruby.runtime.builtin.RubyArray)
	 */
	public synchronized RubyValue selectFromTable(RubyArray args) {
		if (args.size() < 2) {
			new RubyException(
					"selectFromTable has 3 paramaters: tableName, attrib, where");
			return RubyConstant.QNIL;
		}
		return selectFromTable(args.get(0), args.get(1),
				(args.size() > 2 ? args.get(2) : null), (args.size() > 3 ? args
						.get(3) : null));
	}

	private String vals2str(RubyHash values, String delim) {
		String retval = "";

		if (values != null) {
			RubyArray keys = values.keys();
			for (int i = 0; i < keys.size(); i++) {
				RubyValue key = keys.get(i);
				RubyValue val = values.get(key);
				String type = val.getRubyClass().getName();
				
				String escapedValue = "";
				if ( type.equals("String"))
					escapedValue = "'" + escapeSQL(val.asString()) + "'";
				else
					escapedValue = val.asString();
				
				retval += " " + key + "=" + escapedValue + ( i < (keys.size() - 1) ? (delim != null ? delim : "") : "" );
			}
		}

		return retval;
	}
	
	private String escapeSQL( String val )
	{
		return val.replace("'", "''");
	}

	RubyValue selectFromTable(RubyValue table, RubyValue columns,
			RubyValue condition, RubyValue params) {

		String query = "";
		boolean count = false;
		boolean distinct = false;
		RubyValue orderBy = null;

		if (table != RubyConstant.QNIL && columns != RubyConstant.QNIL ){
			if (params != null) {
				try{
					distinct = ((RubyHash) params).get(ObjectFactory
						.createString("distinct")) != RubyConstant.QNIL;
				}
				catch(Exception e){
					distinct = false;
				}
				try {
					count = ((RubyHash) params).get(ObjectFactory
						.createString("count")) != RubyConstant.QNIL;
				}
				catch(Exception e){
					count = false;
				}
				
				try {
					orderBy = ((RubyHash) params).get(ObjectFactory
							.createString("order by"));
					if (orderBy == RubyConstant.QNIL)
						orderBy = null;
				}
				catch(Exception e){
					orderBy = null;
				}
				
				if (distinct) {
					query = "select distinct "
							+ ((RubyString) columns).asString() + " from "
							+ table 
							+ (( condition != null && condition != RubyConstant.QNIL) 
									?  " where (" + vals2str((RubyHash) condition, " AND") + ")" 
									: "") ;
				} else if (count) {
					query = "select count(*) from " + table 
							+ (( condition != null && condition != RubyConstant.QNIL) 
								? " where (" + vals2str((RubyHash) condition, " AND") + ")"
								: "");
					
				} else if (orderBy != null) {
					query = "select " + ((RubyString) columns).asString()
							+ " from " + table 
							+ (( condition != null && condition != RubyConstant.QNIL) 
									 ? " where (" + vals2str((RubyHash) condition, " AND") + ")" 
									 : "" ) 
							 + " order by " + orderBy.asString();
				} else {
					query = "select " + ((RubyString) columns).asString()
							+ " from " + table 
							+ (( condition != null && condition != RubyConstant.QNIL) 
								? " where (" + vals2str((RubyHash) condition, " AND") + ")"
								: "" );
				}
			}
		} else if (table != RubyConstant.QNIL && columns != RubyConstant.QNIL) {
			query = "select " + ((RubyString) columns).asString() + " from "
					+ table;
		}

		if (count)
			return storage.executeCountSql(query);
		else
			return storage.executeSql(distinct, table.asString(),
					((RubyString) columns).asString(),
					orderBy != null ? orderBy.asString() : null, 
							( condition != null && condition != RubyConstant.QNIL) 
								? vals2str( (RubyHash) condition, " AND")
								: null);
	}

	// @RubyLevelMethod(name="updateIntoTable")
	/*
	 * (non-Javadoc)
	 * 
	 * @see com.rhomobile.rhodes.db.IDBAdapter#updateIntoTable(com.xruby.runtime.builtin.RubyArray)
	 */
	public synchronized RubyValue updateIntoTable(RubyArray args) {
		if (args.size() != 3) {
			new RubyException(
					"updateIntoTable has 3 paramaters: tableName, mapValues, where");
			return RubyConstant.QNIL;
		}
		return updateIntoTable(args.get(0), args.get(1), args.get(2));
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see com.rhomobile.rhodes.db.IDBAdapter#updateIntoTable(com.xruby.runtime.lang.RubyValue,
	 *      com.xruby.runtime.lang.RubyValue, com.xruby.runtime.lang.RubyValue)
	 */
	public synchronized RubyValue updateIntoTable(RubyValue table,
			RubyValue values, RubyValue condition) {
		
		if ( table!= null && table != RubyConstant.QNIL && values != null && values != RubyConstant.QNIL ){
		
			String query = "update " + table + " set " + vals2str((RubyHash) values, " ,")
			+ (( condition != null && condition != RubyConstant.QNIL) 
				? " where (" + vals2str((RubyHash) condition, " AND") + ")"
				: "" );
			
			storage.executeSql(query);
		}
		
		return new RubyArray();
	}

	// @RubyLevelMethod(name="deleteAllFromTable")
	/*
	 * (non-Javadoc)
	 * 
	 * @see com.rhomobile.rhodes.db.IDBAdapter#deleteAllFromTable(com.xruby.runtime.lang.RubyValue)
	 */
	public synchronized RubyValue deleteAllFromTable(RubyValue table) {
		
		if ( table!= null && table != RubyConstant.QNIL ){

			if ( this.callback != null )
				this.callback.OnDeleteAllFromTable(table.toStr());

			String query = "delete * from " + table;
			storage.executeSql(query);
		}
		return new RubyArray();
	}

	// @RubyLevelMethod(name="deleteFromTable")
	/*
	 * (non-Javadoc)
	 * 
	 * @see com.rhomobile.rhodes.db.IDBAdapter#deleteFromTable(com.xruby.runtime.lang.RubyValue,
	 *      com.xruby.runtime.lang.RubyValue)
	 */
	public synchronized RubyValue deleteFromTable(RubyValue table,
			RubyValue condition) {
		
		if ( table!= null && table != RubyConstant.QNIL && condition != null && condition != RubyConstant.QNIL ){
			
			if ( this.callback != null )
			{
				RubyArray rows2Delete = (RubyArray)storage.executeSql(false, table.asString(), "*", null,
							( condition != null && condition != RubyConstant.QNIL) 
								? vals2str( (RubyHash) condition, " AND")
								: null);
				
				this.callback.OnDeleteFromTable(table.asString(), rows2Delete);
			}
			
			String query = "delete from " + table
			+ (( condition != null && condition != RubyConstant.QNIL) 
				? " where (" + vals2str((RubyHash) condition, " AND") + ")"
				: "" );
			
			storage.executeSql(query);
		}
		
		return new RubyArray();
	}

	// @RubyLevelMethod(name="close")
	/*
	 * (non-Javadoc)
	 * 
	 * @see com.rhomobile.rhodes.db.IDBAdapter#close()
	 */
	public RubyValue close() {
		if (storage != null) {
			storage.close();
			storage = null;
		}

		return RubyConstant.QNIL;
	}

	// @RubyLevelMethod(name="closed?")
	RubyValue isClosed() {
		if (storage != null && storage.isOpened())
			return RubyConstant.QFALSE;

		return RubyConstant.QTRUE;
	}

	public static void initMethods(RubyClass klass) {
		klass.defineMethod("initialize", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg,
					RubyBlock block) {
				return ((DBAdapter) receiver).initialize(arg);
			}
		});

		klass.defineMethod("deleteAllFromTable", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg,
					RubyBlock block) {
				return ((DBAdapter) receiver).deleteAllFromTable(arg);
			}
		});

		klass.defineMethod("insertIntoTable", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0,
					RubyValue arg1, RubyBlock block) {
				return ((DBAdapter) receiver).insertIntoTable(arg0, arg1);
			}
		});
		klass.defineMethod("selectFromTable", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args,
					RubyBlock block) {
				return ((DBAdapter) receiver).selectFromTable(args);
			}
		});
		klass.defineMethod("updateIntoTable", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args,
					RubyBlock block) {
				return ((DBAdapter) receiver).updateIntoTable(args);
			}
		});
		klass.defineMethod("deleteFromTable", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0,
					RubyValue arg1, RubyBlock block) {
				return ((DBAdapter) receiver).deleteFromTable(arg0, arg1);
			}
		});
		klass.defineMethod("close", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return ((DBAdapter) receiver).close();
			}
		});
		klass.defineMethod("closed?", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return ((DBAdapter) receiver).isClosed();
			}
		});

		klass.defineAllocMethod(new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return DBAdapter.alloc(receiver);
			}
		});
	}
}
