package com.rho.sync;

public class SyncObject {
	private int _primaryKey = -1;
	private String _attrib;
	private int _sourceId = -1;
	private String _object;
	private String _value;
	private String _createdAt;
	private String _updatedAt;
	private String _updateType;

	public SyncObject(String attrib, int sourceId, String object, String value,
			String updateType) {
		this._attrib = attrib;
		this._sourceId = sourceId;
		this._object = object;
		this._value = value;
		this._updateType = updateType;
	}

	public int getPrimaryKey() {
		return _primaryKey;
	}

	public void setPrimaryKey(int primaryKey) {
		this._primaryKey = primaryKey;
	}

	public String getAttrib() {
		return _attrib;
	}

	public void setAttrib(String attrib) {
		this._attrib = attrib;
	}

	public int getSourceId() {
		return _sourceId;
	}

	public void setSourceId(int sourceId) {
		this._sourceId = sourceId;
	}

	public String getObject() {
		return _object;
	}

	public void setObject(String object) {
		this._object = object;
	}

	public String getValue() {
		return _value;
	}

	public void setValue(String value) {
		this._value = value;
	}

	public String getCreatedAt() {
		return _createdAt;
	}

	public void setCreatedAt(String createdAt) {
		this._createdAt = createdAt;
	}

	public String getUpdatedAt() {
		return _updatedAt;
	}

	public void setUpdatedAt(String updatedAt) {
		this._updatedAt = updatedAt;
	}

	public String getUpdateType() {
		return _updateType;
	}

	public void setUpdateType(String updateType) {
		this._updateType = updateType;
	}
}
