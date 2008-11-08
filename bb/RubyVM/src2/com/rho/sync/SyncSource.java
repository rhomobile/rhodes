package com.rho.sync;

public class SyncSource {
	private int _sourceId;
	private String _sourceUrl;

	public SyncSource(String sourceUrl, int sourceId) {
		_sourceUrl = sourceUrl;
		_sourceId = sourceId;
	}

	public int get_sourceId() {
		return _sourceId;
	}

	public void set_sourceId(int id) {
		_sourceId = id;
	}

	public String get_sourceUrl() {
		return _sourceUrl;
	}

	public void set_sourceUrl(String url) {
		_sourceUrl = url;
	}
}
