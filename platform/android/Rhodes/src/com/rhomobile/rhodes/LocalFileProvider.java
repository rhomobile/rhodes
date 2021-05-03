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

package com.rhomobile.rhodes;

import java.io.File;
import java.util.List;

import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.ContextFactory;

import android.content.ContentProvider;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ResolveInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetFileDescriptor;
import android.database.Cursor;
import android.net.Uri;
import android.net.Uri.Builder;
import android.webkit.MimeTypeMap;
import android.os.ParcelFileDescriptor;
import java.io.FileNotFoundException;


public class LocalFileProvider extends ContentProvider
{
    private static final String TAG = LocalFileProvider.class.getSimpleName();
    private static String fullPathPrefix;
    private static String contentAuthority;

    private static String getPathPrefix() {
        if (fullPathPrefix == null) {
            fullPathPrefix = ContextFactory.getAppContext().getApplicationInfo().dataDir;
        }
        return fullPathPrefix;
    }


    //content authority for Rhodes app always equals package name. See manifest template.
    private static String getAuthority() {
        if ( null == contentAuthority ) {
            contentAuthority = ContextFactory.getAppContext().getPackageName();
        }

        return contentAuthority;
    }

    public static boolean isCorrectAuthority(String authority) {        
        return authority.equals(getAuthority());
    }
    
    public static Uri overrideUri(Uri uri) {
        if (!RhoConf.isExist("useAssetFS") || RhoConf.getBool("useAssetFS")) {
            return overrideSystemUri(uri);
       }
        return null;
    }

    //builds content:// URI from file:// URI
    public static Uri overrideSystemUri(Uri uri) {

        Uri overridedUri = null;

        String scheme = uri.getScheme();
        
        if (scheme != null && scheme.equals("file")) {
            String ssp = uri.getSchemeSpecificPart();
            String sspPrefix = "//" + getPathPrefix();

            //if requested path is inside app data dir
            if(ssp.startsWith(sspPrefix)) {
                //get content part
                String packagePath = ssp.substring(sspPrefix.length());

                Uri.Builder builder = new Uri.Builder();
                builder.scheme( ContentResolver.SCHEME_CONTENT );
                builder.authority( getAuthority() );
                builder.path( packagePath );
                builder.fragment( uri.getFragment() );

                overridedUri = builder.build();

                Logger.T(TAG, "Overriding URI: " + uri.toString() + " to " + overridedUri.toString() );                
            }
        }
 
        return overridedUri;
    }

    public static void grantPermissionOnSharingContentIntent(Context ctx, Intent intent, Uri uri){
        List<ResolveInfo> resInfoList = ctx.getPackageManager().queryIntentActivities(intent, PackageManager.MATCH_DEFAULT_ONLY);
        for (ResolveInfo resolveInfo : resInfoList) {
            String packageName = resolveInfo.activityInfo.packageName;
            ctx.grantUriPermission(packageName, uri, Intent.FLAG_GRANT_READ_URI_PERMISSION);
        }
    }

    public static boolean isHomeDir(Context ctx, String fileName){
        final File f = new File( ctx.getApplicationInfo().dataDir, "rhodata" );        
        final String filePath = new File(f,"apps").getPath();
        return fileName.startsWith(filePath) || fileName.startsWith("file://" + filePath);
    }
    
    
    public static void revokeUriPermissions( Context ctx ) {
        Uri.Builder builder = new Uri.Builder();

        builder.scheme( ContentResolver.SCHEME_CONTENT );
        builder.authority( getAuthority() );

        Uri uri = builder.build();
        
        Logger.I(TAG, "Revoke URI permissions: " + Uri.decode(uri.toString()));
        ctx.revokeUriPermission(uri, Intent.FLAG_GRANT_READ_URI_PERMISSION);
    }
    
    public static File fileFromUri(Uri uri) throws IllegalArgumentException {

        Logger.T(TAG, "fileFromUri: " + uri);

        String authority = uri.getAuthority();
        if(isCorrectAuthority(authority)) {
            return new File(getPathPrefix() + uri.getPath());
        }
        else {
            throw new IllegalArgumentException("Unknown URI authority: " + authority);
        }
    }
    
   @Override
    public AssetFileDescriptor openAssetFile(Uri uri, String mode) throws FileNotFoundException {
        Logger.T(TAG, "Opening asset: " + uri);
        
        if(mode.compareTo("r") != 0)
        {
            Logger.E(TAG, uri + " unacceptable mode: " + mode);
            throw new SecurityException("Unacceptable openFile mode: " + mode);
        }
        File path = fileFromUri(uri);
        
        AssetFileDescriptor fd = RhoFileApi.openAssetFd(path.getPath());

        if ( null == fd ) {
            return super.openAssetFile( uri, mode );
        }
        
        return fd;
    }

    @Override
    public ParcelFileDescriptor openFile(Uri uri, String mode)
        throws FileNotFoundException, SecurityException {

        Logger.T(TAG, "openFile: " + uri + " mode: " + mode );

        if(mode.compareTo("r") != 0)
        {
            throw new SecurityException("Unacceptable openFile mode: " + mode);
        }
        try {
            File path = fileFromUri(uri);

            Logger.D(TAG, "Opening content file: " + path.getPath());

            ParcelFileDescriptor fd = RhoFileApi.openParcelFd(path.getPath());
            if(fd == null) {
                throw new IllegalArgumentException();
            }

            return fd;

        }
        catch(IllegalArgumentException error) {
            FileNotFoundException fileError = new FileNotFoundException("Cannot assign file for URI: " + uri.toString());
            fileError.initCause(error);
            throw fileError;
        }      
    }

    @Override
    public boolean onCreate() {
        Logger.T(TAG, "OnCreate");

        return true;
    }

    @Override
    public int delete(Uri uri, String s, String[] as) {
        Logger.T(TAG, "delete: " + uri);

        throw new UnsupportedOperationException(
                "Delete operation is not supported by this provider");
    }

    @Override
    public String getType(Uri uri) {

        Logger.D(TAG, "Resolving content type - " + uri);

        String type = "";
        String ext = MimeTypeMap.getFileExtensionFromUrl(uri.getPath());
        if (ext == null || ext.length() == 0)
            throw new IllegalArgumentException("Unknown URI " + uri);

        type = MimeTypeMap.getSingleton().getMimeTypeFromExtension(ext);
        return type;
    }

    @Override
    public Uri insert(Uri uri, ContentValues contentvalues) {
        Logger.T(TAG, "insert: " + uri);

        throw new UnsupportedOperationException(
                "Insert operation is not supported by this provider");
    }

    @Override
    public Cursor query(Uri uri, String[] proj, String selection, String[] selectionArgs, String sortOrder) {
        Logger.T(TAG, "query: " + uri);

        // Some apps want to query some metadata for files being opened. So do not throw form here...  
        return null;
//        throw new UnsupportedOperationException(
//                "Query operation is not supported by this provider");
    }

    @Override
    public int update(Uri uri, ContentValues contentvalues, String s, String[] as) {
        Logger.T(TAG, "update: " + uri);

        throw new UnsupportedOperationException(
                "Update operation is not supported by this provider");
    }

}
