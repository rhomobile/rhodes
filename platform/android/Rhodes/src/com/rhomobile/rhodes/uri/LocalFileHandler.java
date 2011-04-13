package com.rhomobile.rhodes.uri;

import java.io.File;
import java.net.URISyntaxException;

import com.rhomobile.rhodes.LocalFileProvider;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesService;

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

        if(!file.isFile())
            throw new IllegalArgumentException("Unknown file: " + path);

        if(path.startsWith(LocalFileProvider.PATH_PREFIX))
        {
            url = LocalFileProvider.uriFromLocalFile(file).toString();
            intentFlags = Intent.FLAG_GRANT_READ_URI_PERMISSION;
        }

        Intent intent = Intent.parseUri(url, intentFlags);
        Context ctx = RhodesService.getContext();

        ctx.startActivity(Intent.createChooser(intent, "Open in..."));

        return true;
    }
    
}
