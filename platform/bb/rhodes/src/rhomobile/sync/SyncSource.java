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

import com.rho.db.*;

/**
 * The Class SyncSource.
 */
public class SyncSource {
	
	/** The _source id. */
	private int _sourceId;
	
	/** The _source url. */
	private String _sourceUrl;

	private String _token = "";
	
	/**
	 * Instantiates a new sync source.
	 * 
	 * @param sourceUrl the source url
	 * @param sourceId the source id
	 */
	public SyncSource(IDBResult sources, int nItem) {
		_sourceUrl = sources.getString(nItem, SyncConstants.COL_SOURCEURL);
		_sourceId = sources.getInt(nItem, SyncConstants.COL_SOURCEID);
		_token = sources.getString(nItem, SyncConstants.COL_TOKEN);
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

	public String get_token() {
		return _token;
	}
	
	/*
	public void set_sourceId(int id) {
		_sourceId = id;
	}

	public void set_sourceUrl(String url) {
		_sourceUrl = url;
	}*/

	public void set_token(String _token) {
		this._token = _token;
	}
}
