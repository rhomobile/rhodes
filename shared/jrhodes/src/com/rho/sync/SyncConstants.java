package com.rho.sync;

/**
 * The Class SyncConstants.
 */
public class SyncConstants {

	/** The Constant DEBUG. */
	public static final boolean DEBUG = false;
	
	/** The Constant SYNC_FORMAT. */
	public static final String SYNC_FORMAT = "?format=json";

	// Table Names
	/** The Constant OBJECTS_TABLE. */
	public static final String OBJECTS_TABLE = "object_values";
	
	/** The Constant SOURCES_TABLE. */
	public static final String SOURCES_TABLE = "sources";

	public static final String CLIENT_INFO = "client_info";
	
	// Sync OP Types
	/** The Constant UPDATE_TYPE_UPDATE. */
	public static final String UPDATE_TYPE_UPDATE = "updateobjects";
	
	/** The Constant UPDATE_TYPE_CREATE. */
	public static final String UPDATE_TYPE_CREATE = "createobjects";
	
	/** The Constant UPDATE_TYPE_DELETE. */
	public static final String UPDATE_TYPE_DELETE = "deleteobjects";

	/** The Constant MAX_SYNC_OBJECTS. */
	public static final int MAX_SYNC_OBJECTS = 500;

	// Database Codes
	/** The Constant SYNC_OBJECT_SUCCESS. */
	public static final int SYNC_OBJECT_SUCCESS = 0;
	
	/** The Constant SYNC_OBJECT_DUPLICATE. */
	public static final int SYNC_OBJECT_DUPLICATE = 1;
	
	/** The Constant SYNC_OBJECT_ERROR. */
	public static final int SYNC_OBJECT_ERROR = 2;

	// Remote Operation Codes
	/** The Constant MAX_SINGLE_OP_SIZE. */
	public static final int MAX_SINGLE_OP_SIZE = 200;
	
	/** The Constant SYNC_FETCH_CHANGES_OK. */
	public static final int SYNC_FETCH_CHANGES_OK = 0;
	
	/** The Constant SYNC_FETCH_CHANGES_ERROR. */
	public static final int SYNC_FETCH_CHANGES_ERROR = 1;
	
	/** The Constant SYNC_PUSH_CHANGES_OK. */
	public static final int SYNC_PUSH_CHANGES_OK = 0;
	
	/** The Constant SYNC_PUSH_CHANGES_ERROR. */
	public static final int SYNC_PUSH_CHANGES_ERROR = 1;
	
	/** The Constant SYNC_PROCESS_CHANGES_OK. */
	public static final int SYNC_PROCESS_CHANGES_OK = 0;
}

