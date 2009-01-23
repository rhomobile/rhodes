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
 * The Class SyncOperation.
 */
public class SyncOperation {
	
	/** The _operation. */
	private String _operation;
	
	/** The _post body. */
	private String _postBody;

	/**
	 * Instantiates a new sync operation.
	 * 
	 * @param operation the operation
	 * @param object the object
	 */
	public SyncOperation(String operation, SyncObject object) {
		_operation = operation;
		_postBody = setSyncPostBody(object);
		System.out.println("Formatted post strig: " + _postBody);
	}

	/**
	 * Filter attrib.
	 * 
	 * @param buffer the buffer
	 * @param attrib the attrib
	 * @param value the value
	 */
	private void filterAttrib(StringBuffer buffer, String attrib, String value) {
		String attrPrefix = "attrvals[][";
		String attrSuffix = "]=";
		buffer.append(attrPrefix);
		buffer.append(attrib);
		buffer.append(attrSuffix);
		buffer.append(value);
	}

	/**
	 * Gets the _operation.
	 * 
	 * @return the _operation
	 */
	public String get_operation() {
		return _operation;
	}

	/**
	 * Gets the _post body.
	 * 
	 * @return the _post body
	 */
	public String get_postBody() {
		return _postBody;
	}

	/**
	 * Sets the _operation.
	 * 
	 * @param _operation the new _operation
	 */
	public void set_operation(String _operation) {
		this._operation = _operation;
	}

	/**
	 * Sets the _post body.
	 * 
	 * @param body the new _post body
	 */
	public void set_postBody(String body) {
		_postBody = body;
	}

	/**
	 * Sets the sync post body.
	 * 
	 * @param object the object
	 * 
	 * @return the string
	 */
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
}
