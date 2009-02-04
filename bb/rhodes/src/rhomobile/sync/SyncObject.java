/*
 *  rhodes
 *
 *  Copyright (C) 2008 Lars Burgess. All rights reserved.
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

import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.lang.RubyValue;

/**
 * The Class SyncObject.
 */
public class SyncObject {
	
	/* track the database operation to use */	
	private String _db_operation;
	
	/** The _primary key. */
	private int _primaryKey = -1;
	
	/** The _attrib. */
	private String _attrib;
	
	/** The _source id. */
	private int _sourceId = -1;
	
	/** The _object. */
	private String _object;
	
	/** The _value. */
	private String _value;

	/** The _update type. */
	private String _updateType;

	/**
	 * Instantiates a new sync object.
	 * 
	 * @param attrib the attrib
	 * @param sourceId the source id
	 * @param object the object
	 * @param value the value
	 * @param updateType the update type
	 */
	public SyncObject(String attrib, int sourceId, String object, String value,
			String updateType) {
		this._attrib = attrib;
		this._sourceId = sourceId;
		this._object = object;
		this._value = value;
		this._updateType = updateType;
	}

	/**
	 * Dehydrate.
	 * 
	 * @return the int
	 */
	public int insertIntoDatabase() {
		try {
			SyncUtil.adapter.insertIntoTable(SyncUtil
					.createString(SyncConstants.OBJECTS_TABLE), this
					.getHashFromValues());
		} catch (Exception e) {
			System.out.println("There was an error inserting the record: "
					+ e.getMessage());
			return SyncConstants.SYNC_OBJECT_ERROR;
		}
		return SyncConstants.SYNC_OBJECT_SUCCESS;
	}

	public void deleteFromDatabase() {
		RubyHash hash = SyncUtil.createHash();
		hash.add(SyncUtil.createString("id"), SyncUtil
				.createInteger(this.getPrimaryKey()));
		SyncUtil.adapter.deleteFromTable(SyncUtil
				.createString(SyncConstants.OBJECTS_TABLE), (RubyValue) hash);
	}
	
	/**
	 * Delete from database by source.
	 * 
	 * @param id the id
	 */
	public static void deleteFromDatabaseBySource(int id) {
		RubyHash hash = SyncUtil.createHash();
		hash.add(SyncUtil.createString("source_id"), SyncUtil
				.createInteger((long) id));
		SyncUtil.adapter.deleteFromTable(SyncUtil
				.createString(SyncConstants.OBJECTS_TABLE), (RubyValue) hash);
	}

	/**
	 * Gets the hash from values.
	 * 
	 * @return the hash from values
	 */
	private RubyHash getHashFromValues() {
		RubyHash hash = SyncUtil.createHash();
		hash.add(SyncUtil.createString("id"), SyncUtil
				.createInteger(this.getPrimaryKey()));
		hash.add(SyncUtil.createString("attrib"), SyncUtil
				.createString(this.getAttrib()));
		hash.add(SyncUtil.createString("source_id"), SyncUtil
				.createInteger((long) this.getSourceId()));
		hash.add(SyncUtil.createString("object"), SyncUtil
				.createString(this.getObject()));
		hash.add(SyncUtil.createString("value"), SyncUtil
				.createString(this.getValue()));
		hash.add(SyncUtil.createString("update_type"), SyncUtil
				.createString(this.getUpdateType()));
		
		return hash;
	}

	/**
	 * Gets the attrib.
	 * 
	 * @return the attrib
	 */
	public String getAttrib() {
		return _attrib;
	}

	/**
	 * Gets the object.
	 * 
	 * @return the object
	 */
	public String getObject() {
		return _object;
	}

	/**
	 * Gets the primary key.
	 * 
	 * @return the primary key
	 */
	public int getPrimaryKey() {
		return _primaryKey;
	}

	/**
	 * Gets the source id.
	 * 
	 * @return the source id
	 */
	public int getSourceId() {
		return _sourceId;
	}

	/**
	 * Gets the update type.
	 * 
	 * @return the update type
	 */
	public String getUpdateType() {
		return _updateType;
	}

	/**
	 * Gets the value.
	 * 
	 * @return the value
	 */
	public String getValue() {
		return _value == null ? "" : _value;
	}

	/**
	 * Sets the attrib.
	 * 
	 * @param attrib the new attrib
	 */
	public void setAttrib(String attrib) {
		this._attrib = attrib;
	}

	/**
	 * Sets the object.
	 * 
	 * @param object the new object
	 */
	public void setObject(String object) {
		this._object = object;
	}

	/**
	 * Sets the primary key.
	 * 
	 * @param primaryKey the new primary key
	 */
	public void setPrimaryKey(int primaryKey) {
		this._primaryKey = primaryKey;
	}

	/**
	 * Sets the source id.
	 * 
	 * @param sourceId the new source id
	 */
	public void setSourceId(int sourceId) {
		this._sourceId = sourceId;
	}

	/**
	 * Sets the update type.
	 * 
	 * @param updateType the new update type
	 */
	public void setUpdateType(String updateType) {
		this._updateType = updateType;
	}

	/**
	 * Sets the value.
	 * 
	 * @param value the new value
	 */
	public void setValue(String value) {
		this._value = value;
	}

	public String getDbOperation() {
		return _db_operation;
	}

	public void setDbOperation(String _db_operation) {
		this._db_operation = _db_operation;
	}
}
