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

import java.lang.Exception;
import java.net.URISyntaxException;
import java.util.regex.Pattern;
import java.io.File;
import java.util.List;

import com.rhomobile.rhodes.LocalFileProvider;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoFileProvider;
import android.os.Build;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.webkit.URLUtil;
import android.webkit.MimeTypeMap;
import android.content.ClipData;
import android.content.ContentResolver;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;

public class LocalFileHandler implements UriHandler
{
    private static final String TAG = "LocalFileHandler";
    private static final Pattern mHtmlPattern = Pattern.compile(".*\\.(html|htm)$");
    private static final Pattern mImagePattern = Pattern.compile(".*\\.(jpg|jpeg|png|gif|svg)$");
    private Context ctx;
    
    public LocalFileHandler(Context c) {
        ctx = c;
    }

    private static boolean isHtmlUrl(String url) {
        Logger.T(TAG, "Looking for html extension: " + url);
        return mHtmlPattern.matcher(url).matches();
    }

    private static boolean isImageUrl(String url) {
        Logger.T(TAG, "Looking for image extension: " + url);
        return mImagePattern.matcher(url).matches();
    }

    public boolean handle(String url) throws URISyntaxException {

        if (!URLUtil.isFileUrl(url))
            return false;
        
        String noAnchorUrl = URLUtil.stripAnchor(url);
        Logger.D(TAG, "is Html Url: " + isHtmlUrl(noAnchorUrl));
        if (isHtmlUrl(noAnchorUrl)) {
            return false;
        }

        boolean isHomeDir = LocalFileProvider.isHomeDir(ctx, url);
        Logger.D(TAG, "isHomeDir is : " + isHomeDir);

        boolean isDbDir = LocalFileProvider.isDbDir(ctx, url);
        Logger.D(TAG, "isDbDir is : " + isDbDir);

        Logger.D(TAG, "Handle URI externally: " + url);

        Uri path = Uri.parse(url);
        Uri newUri = LocalFileProvider.overrideSystemUri(path);

        if(newUri != null) {
            url = Uri.decode(newUri.toString());
        }

        Intent intent = null;

        if (isDbDir) {
            Uri uri = Uri.parse(url);
            String mime = getMime(ctx, uri);
            intent = new Intent()
                .setDataAndType(newUri, mime)
                .addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
                .addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                
            intent.setClipData(ClipData.newUri(ctx.getContentResolver(), mime, uri));

            PackageManager pm = ctx.getPackageManager();
            List<ResolveInfo> targets = pm.queryIntentActivities(intent, PackageManager.MATCH_DEFAULT_ONLY);
            for (ResolveInfo ri : targets) {
                ctx.grantUriPermission(
                    ri.activityInfo.packageName,
                    newUri,
                    Intent.FLAG_GRANT_READ_URI_PERMISSION
                );
            }
        } else {
            intent = Intent.parseUri(url, Intent.FLAG_GRANT_READ_URI_PERMISSION);
        }

        intent.setAction(Intent.ACTION_VIEW);

        if (newUri != null && isHomeDir){
            LocalFileProvider.grantPermissionOnSharingContentIntent(ctx, intent, newUri);
        }

        if(newUri == null && url.contains("file://"))
        {    	   
    	    try{
        	    String extension = MimeTypeMap.getFileExtensionFromUrl(url);
        	    String mimeType = MimeTypeMap.getSingleton().getMimeTypeFromExtension(extension.toLowerCase());
        	    if(mimeType!=null && mimeType.contains("image"))
        	    	intent.setDataAndType(path, "image/*");
        	    else if(mimeType!=null && mimeType.contains("audio"))
        	    	intent.setDataAndType(path, "audio/*");
				else if(mimeType!=null && mimeType.contains("video"))
        	    	intent.setDataAndType(path, "video/*");	
        	    else if(mimeType!=null) {
                    intent.setDataAndType(path, mimeType); 
                } else {
        	    	intent.setDataAndType(path, "*/*");
                }
    	    } catch(Exception ex) {
    		    Logger.E(TAG, ex.getMessage());
    		    intent.setDataAndType(path, "*/*");
            }
               
    	
   	    }
        //ctx.startActivity(Intent.createChooser(intent, "Open in..."));

        if (isDbDir){
            ctx.startActivity(Intent.createChooser(intent, "Open in..."));
        } else {
            ctx.startActivity(intent);
        }
        return true;
    }
    
    private String getMime(Context ctx, Uri uri) {
        String mime = null;

        if (uri.getScheme().equals(ContentResolver.SCHEME_CONTENT)) {
            mime = ctx.getContentResolver().getType(uri);
        } else {
            String extension = MimeTypeMap.getFileExtensionFromUrl(uri.toString());
            if (extension != null) {
                mime = MimeTypeMap.getSingleton().getMimeTypeFromExtension(extension.toLowerCase());
            }
        }
        if (mime == null) {
            mime = "application/octet-stream";
        }
        return mime;
    }
}
