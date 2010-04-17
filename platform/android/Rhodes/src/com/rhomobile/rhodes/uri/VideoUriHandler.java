/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 2.0
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
package com.rhomobile.rhodes.uri;

import com.rhomobile.rhodes.Logger;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.webkit.MimeTypeMap;

public class VideoUriHandler implements UriHandler {

	private static final String TAG = "VideoUriHandler";
	
	private Context ctx;
	
	public VideoUriHandler(Context c) {
		ctx = c;
	}

	public boolean handle(String url) {
		String ext = MimeTypeMap.getFileExtensionFromUrl(url);
		if (ext == null || ext.length() == 0)
			return false;
		
		MimeTypeMap mt = MimeTypeMap.getSingleton();
		String mimeType = mt.getMimeTypeFromExtension(ext);
		if (mimeType == null)
			return false;
		
		if (!mimeType.startsWith("video/"))
			return false;
		
		Logger.D(TAG, "This is video url, handle it");
		
		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.setDataAndType(Uri.parse(url), mimeType);
		ctx.startActivity(Intent.createChooser(intent, "Choose video player"));
		return true;
	}

}
