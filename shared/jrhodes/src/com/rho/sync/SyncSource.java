package com.rho.sync;

/**
 * The Class SyncSource.
 */
public class SyncSource {
	
	/** The _source id. */
	private int _sourceId;
	
	/** The _source url. */
	private String _sourceUrl;

	/**
	 * Instantiates a new sync source.
	 * 
	 * @param sourceUrl the source url
	 * @param sourceId the source id
	 */
	public SyncSource(String sourceUrl, int sourceId) {
		_sourceUrl = sourceUrl;
		_sourceId = sourceId;
	}

	/**
	 * Gets the _source id.
	 * 
	 * @return the _source id
	 */
	public int get_sourceId() {
		return _sourceId;
	}

	/**
	 * Gets the _source url.
	 * 
	 * @return the _source url
	 */
	public String get_sourceUrl() {
		return _sourceUrl;
	}

	/**
	 * Sets the _source id.
	 * 
	 * @param id the new _source id
	 */
	public void set_sourceId(int id) {
		_sourceId = id;
	}

	/**
	 * Sets the _source url.
	 * 
	 * @param url the new _source url
	 */
	public void set_sourceUrl(String url) {
		_sourceUrl = url;
	}
}

