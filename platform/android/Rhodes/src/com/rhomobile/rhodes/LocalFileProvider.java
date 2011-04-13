package com.rhomobile.rhodes;

import java.io.File;
import java.io.FileNotFoundException;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.database.Cursor;
import android.net.Uri;
import android.os.ParcelFileDescriptor;
import android.webkit.MimeTypeMap;


public class LocalFileProvider extends ContentProvider
{
    private static final String TAG = LocalFileProvider.class.getSimpleName();
    public static final String PATH_PREFIX = "/data/data/";
    public static final String PROTOCOL_PREFIX = "content://";

    public static boolean isCorrectAuthority(String authority)
    {
//        Logger.D(TAG, "Comparing authorities: "
//                    + RhodesService.getInstance().getClass().getPackage().getName()
//                    + " vs. " + authority);
//        return authority.equals(RhodesService.getInstance().getClass().getPackage().getName());
        return true;
    }
    
    public static Uri uriFromLocalFile(File file)
    {
        String path = file.getAbsolutePath();
        String url = PROTOCOL_PREFIX +
            path.substring(PATH_PREFIX.length(), path.length());
        return Uri.parse(url);
    }
    
    public static File fileFromUri(Uri uri) throws IllegalArgumentException
    {
        String authority = uri.getAuthority();
        if(isCorrectAuthority(authority))
            return new File(PATH_PREFIX + authority + uri.getPath());
        else throw new IllegalArgumentException("Unknown URI authority: " + authority);
    }
    
    @Override
    public ParcelFileDescriptor openFile(Uri uri, String mode)
        throws FileNotFoundException, SecurityException
    {
        Logger.D(TAG, "Opening content: " + uri);
        
        if(!mode.equals("r"))
        {
            throw new SecurityException("Unacceptable openFile mode: " + mode);
        }
        
        try {
            File file = fileFromUri(uri);
            
            Logger.D(TAG, "Opening content file: " + file.getAbsolutePath());
            
            return ParcelFileDescriptor.open(file, ParcelFileDescriptor.MODE_READ_ONLY);
        } catch(IllegalArgumentException error)
        {
            FileNotFoundException fileError = new FileNotFoundException("Cannot assign file for URI: " + uri.toString());
            fileError.initCause(error);
            throw fileError;
        }
    }

    @Override
    public boolean onCreate() {
        return true;
    }

    @Override
    public int delete(Uri uri, String s, String[] as) {
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
        throw new UnsupportedOperationException(
                "Insert operation is not supported by this provider");
    }

    @Override
    public Cursor query(Uri uri, String[] as, String s, String[] as1, String s1) {
        throw new UnsupportedOperationException(
                "Query operation is not supported by this provider");
    }

    @Override
    public int update(Uri uri, ContentValues contentvalues, String s, String[] as) {
        throw new UnsupportedOperationException(
                "Update operation is not supported by this provider");
    }

}
