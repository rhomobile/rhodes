package com.rho.sync;

public class SyncConstants {

	public static final String SYNC_FORMAT = "?format=json";

	public static final String UPDATE_TYPE_UPDATE = "updateobjects";
	public static final String UPDATE_TYPE_CREATE = "createobjects";
	public static final String UPDATE_TYPE_DELETE = "deleteobjects";
	
	public static final int MAX_SYNC_OBJECTS = 500;
	
	public static final int SYNC_OBJECT_SUCCESS = 0;
	public static final int SYNC_OBJECT_DUPLICATE = 1;
	public static final int SYNC_OBJECT_ERROR = 2;
	
	public static final int MAX_SINGLE_OP_SIZE = 200;
	public static final int SYNC_FETCH_CHANGES_OK = 0;
	public static final int SYNC_FETCH_CHANGES_ERROR = 1;
	public static final int SYNC_PUSH_CHANGES_OK = 0;
	public static final int SYNC_PUSH_CHANGES_ERROR = 1;
}
