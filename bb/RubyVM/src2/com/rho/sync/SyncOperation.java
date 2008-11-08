package com.rho.sync;

public class SyncOperation {
	private String _operation;
	private SyncSource _source;
	private String _postBody;

	public SyncOperation(String operation, SyncSource source, String postBody) {
		_operation = operation;
		_source = source;
		_postBody = postBody;
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
