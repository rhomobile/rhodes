/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
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
package com.rhomobile.rhodes.camera;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhoActivity;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.Utils;

import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;

public class FileList extends RhoActivity implements OnClickListener{

	private static final String TAG = "FileList";
	
	private String callbackUrl;
	private List<String> files;
	private String selectedFile = "";
	private ImageView imagePreview;
	
	private ArrayList<String> getImages() {
		String[] proj = {MediaStore.Images.Media.DATA};
		Cursor cursor = MediaStore.Images.Media.query(getContentResolver(),
				MediaStore.Images.Media.EXTERNAL_CONTENT_URI, proj);
		
		ArrayList<String> files = new ArrayList<String>();
		
		int idx = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
		if (cursor.moveToFirst()) {
			do {
				String path = cursor.getString(idx);
				files.add(path);
			} while (cursor.moveToNext());
		}
		
		return files;
	}
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);

		getWindow().setFlags(RhodesService.WINDOW_FLAGS, RhodesService.WINDOW_MASK);
		setContentView(AndroidR.layout.directory_list);
		
		Bundle extras = getIntent().getExtras();
		callbackUrl = extras.getString(Camera.INTENT_EXTRA_PREFIX + "callback");

		imagePreview = (ImageView) findViewById(AndroidR.id.preview);

		ListView filesList = (ListView) findViewById(AndroidR.id.filesList);

		files = getImages();
		List<String> names = new ArrayList<String>();
		Iterator<String> it = files.iterator();
		while (it.hasNext())
			names.add(Utils.getBaseName(it.next()));
		filesList.setAdapter(new ArrayAdapter<String>(this,
				AndroidR.layout.file_row, names));
		

		Button okButton = (Button) findViewById(AndroidR.id.okButton);
		Button cancelButton = (Button) findViewById(AndroidR.id.cancelButton);

		TextView lookIn = (TextView) findViewById(AndroidR.id.lookIn);
		lookIn.setText("Look In: Gallery");

		okButton.setOnClickListener(this);
		cancelButton.setOnClickListener(this);
		
		filesList.setOnItemClickListener(new OnItemClickListener(){

			public void onItemClick(AdapterView<?> arg0, View arg1, int position, long arg3) {
				try {
					String file = files.get(position);
					Logger.D(TAG, "Selected file: " + file);
					
					BitmapFactory.Options options = new BitmapFactory.Options();
					options.inSampleSize = 10;
					Bitmap bm = BitmapFactory.decodeFile(file, options);
					if (bm != null)
						bm = Bitmap.createScaledBitmap(bm, 176, 144, true);
					if (bm != null) {
						imagePreview.setImageBitmap(bm);
						selectedFile = file;
					}
				} catch (Exception e) {
					Logger.E(TAG, e.getMessage());
				}
			}
			
		});
	}
	
	private void doCallback(String file) {
		try {
			String dst = null;
			if (file != null && file.length() > 0) {
				dst = RhodesService.getBlobPath() + "/" + Utils.getBaseName(file);
				Utils.copy(file, dst);
			}
			com.rhomobile.rhodes.camera.Camera.doCallback(callbackUrl, dst == null ? "" : dst);
		}
		catch (IOException e) {
			Logger.E(TAG, e);
		}
	}

	public void onClick(View view) {
		switch (view.getId()) {
		case AndroidR.id.okButton:
			doCallback(selectedFile);
			break;
		case AndroidR.id.cancelButton:
			doCallback(null);
			break;
		}
		finish();
	}
	
}