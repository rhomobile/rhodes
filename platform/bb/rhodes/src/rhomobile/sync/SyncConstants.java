/*
 *  rhodes
 *
 *  Copyright (C) 2008 Rhomobile, Inc. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package rhomobile.sync;

import com.xruby.runtime.builtin.*;
/**
 * The Class SyncConstants.
 */
public class SyncConstants {

	/** The Constant DEBUG. */
	public static final boolean DEBUG = false;
	
	/** The Constant SYNC_FORMAT. */
	public static final String SYNC_FORMAT = "?format=json";

	public static final String SYNC_ASK_ACTION = "/ask";
	
	// Table Names
	/** The Constant OBJECTS_TABLE. */
	public static final String OBJECTS_TABLE = "object_values";
	
	/** The Constant SOURCES_TABLE. */
	public static final String SOURCES_TABLE = "sources";

	public static final String CLIENT_INFO = "client_info";
	
	public static final String COL_ATTRIB = "attrib";
	public static final String COL_SOURCEID = "source_id";
	public static final String COL_OBJECT = "object";
	public static final String COL_VALUE = "value";
	public static final String COL_UPDATETYPE = "update_type";
	public static final String COL_ATTRIBTYPE = "attrib_type";
	public static final String COL_TOKEN = "token";
	public static final String COL_SOURCEURL = "source_url";
	public static final String COL_SESSION = "session";
	
	// Sync OP Types
	/** The Constant UPDATE_TYPE_UPDATE. */
	public static final String UPDATE_TYPE_UPDATE = "updateobjects";
	
	/** The Constant UPDATE_TYPE_CREATE. */
	public static final String UPDATE_TYPE_CREATE = "createobjects";
	
	/** The Constant UPDATE_TYPE_DELETE. */
	public static final String UPDATE_TYPE_DELETE = "deleteobjects";

	/** The Constant MAX_SYNC_OBJECTS. */
	public static final int MAX_SYNC_OBJECTS = 500;

	public static final int MAX_SYNC_TRY_COUNT = 3;
	public static final int SYNC_PAGE_SIZE     = 200;
	
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
