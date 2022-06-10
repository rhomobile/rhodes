package com.rhomobile.rhodes.uri;

import java.lang.String;

import java.io.File;

import android.support.v4.content.FileProvider;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;

import com.rhomobile.rhodes.Logger;

public class RhoFileProvider extends FileProvider {
	private static final String TAG = "RhoFileProvider"; 

	//ctx.getPackageName()
	public static Uri generateUri(Context ctx, String fileName){
		Uri uri = RhoFileProvider.getUriForFile(ctx, ctx.getPackageName() + ".rhofileprovider", new File(fileName));
		return uri;
	}



}