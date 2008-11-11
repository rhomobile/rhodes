package com.rho.sync;

public class SyncOperation {
	private String _operation;
	private SyncSource _source;
	private String _postBody;

	public SyncOperation(String operation, SyncSource source, SyncObject object) {
		_operation = operation;
		_source = source;
		_postBody = setSyncPostBody(object);
		System.out.println("Formatted post strig: " + _postBody);
	}

	private String setSyncPostBody(SyncObject object) {
		StringBuffer body = new StringBuffer();
		this.filterAttrib(body, "attrib", object.getAttrib());

		if (object.getObject() != null) {
			body.append("&");
			this.filterAttrib(body, "object", object.getObject());
		}

		if (object.getValue() != null) {
			body.append("&");
			this.filterAttrib(body, "value", object.getValue());
		}
		return body.toString();
	}

	private void filterAttrib(StringBuffer buffer, String attrib, String value) {
		String attrPrefix = "attrvals[][";
		String attrSuffix = "]=";
		buffer.append(attrPrefix);
		buffer.append(attrib);
		buffer.append(attrSuffix);
		buffer.append(value);
	}

	public String get_operation() {
		return _operation;
	}

	public void set_operation(String _operation) {
		this._operation = _operation;
	}

	public SyncSource get_source() {
		return _source;
	}

	public void set_source(SyncSource _source) {
		this._source = _source;
	}

	public String get_postBody() {
		return _postBody;
	}

	public void set_postBody(String body) {
		_postBody = body;
	}
}
