/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes.uri;

import java.io.File;
import java.net.URISyntaxException;

import com.rhomobile.rhodes.LocalFileProvider;
import com.rhomobile.rhodes.Logger;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.webkit.URLUtil;

public class LocalFileHandler implements UriHandler
{
    private static final String TAG = "LocalFileHandler";
    private Context ctx;
    
    public LocalFileHandler(Context c) {
        ctx = c;
    }

    public boolean handle(String url) throws URISyntaxException {

        if(!URLUtil.isFileUrl(url))
            return false;

        Logger.D(TAG, "This is 'file' URI, handle it: " + url);

        int intentFlags = 0;
        String path = Uri.parse(url).getPath();
        File file = new File(path);

        if(path.startsWith(LocalFileProvider.PATH_PREFIX))
        {
            url = LocalFileProvider.uriFromLocalFile(file).toString();
            intentFlags = Intent.FLAG_GRANT_READ_URI_PERMISSION;
        }

        Intent intent = Intent.parseUri(url, intentFlags);
        ctx.startActivity(Intent.createChooser(intent, "Open in..."));

        return true;
    }
    
}
