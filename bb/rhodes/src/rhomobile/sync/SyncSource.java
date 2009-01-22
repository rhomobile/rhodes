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
