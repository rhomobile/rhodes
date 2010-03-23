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

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.Rhodes;
import com.rhomobile.rhodes.Utils;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
//import android.provider.MediaStore.Images.Media;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;

public class FileList extends Activity implements OnClickListener{

	private static final String TAG = "FileList";
	
	private String callbackUrl;
	private Button okButton;
	private Button cancelButton;
	private TextView lookIn;
	private List<String> items = null;
	private String selectedFile = "";
	private ImageView imagePreview;
	private ListView filesList;
	
	private static final String BASE_DIR = Environment.getExternalStorageDirectory() + "/DCIM/Camera";
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);

		getWindow().setFlags(Rhodes.WINDOW_FLAGS, Rhodes.WINDOW_MASK);
		setContentView(AndroidR.layout.directory_list);
		
		Bundle extras = getIntent().getExtras();
		callbackUrl = extras.getString(Camera.INTENT_EXTRA_PREFIX + "callback");

		imagePreview = (ImageView) findViewById(AndroidR.id.preview);

		filesList = (ListView) findViewById(AndroidR.id.filesList);
		
		// TODO: implement cursor reading
		fill(new File(BASE_DIR).listFiles());
		//Cursor cursor = getContentResolver().query(Media.EXTERNAL_CONTENT_URI, null, null, null, null);
		//fill(cursor);

		okButton = (Button) findViewById(AndroidR.id.okButton);
		cancelButton = (Button) findViewById(AndroidR.id.cancelButton);

		lookIn = (TextView) findViewById(AndroidR.id.lookIn);

		lookIn.setText("Look In: Gallery");

		okButton.setOnClickListener(this);
		cancelButton.setOnClickListener(this);
		
		filesList.setOnItemClickListener(new OnItemClickListener(){

			public void onItemClick(AdapterView<?> arg0, View arg1, int position, long arg3) {
				try {
					String file = items.get(position);
					Logger.D(TAG, "Selected file: " + file);
					
					BitmapFactory.Options options = new BitmapFactory.Options();
					options.inSampleSize = 10;
					String fullPath = BASE_DIR + "/" + file;
					Bitmap bm = BitmapFactory.decodeFile(fullPath, options);
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

	public void onClick(View view) {
		switch (view.getId()) {
		case AndroidR.id.okButton:
			try {
				String src = BASE_DIR + "/" + selectedFile;
				String dst = Camera.BASE_CAMERA_DIR + "/" + selectedFile;
				Utils.copy(src, dst);
				com.rhomobile.rhodes.camera.Camera.doCallback(callbackUrl, dst);
			} catch (IOException e) {
				Logger.E(TAG, e);
			}
			finish();
			break;
		case AndroidR.id.cancelButton:
			com.rhomobile.rhodes.camera.Camera.doCallback(callbackUrl, "");
			finish();
			break;
		}
	}
	
	/*
	private void fill(Cursor cursor) {
		if (!cursor.moveToFirst())
			return;
		
		int count = cursor.getColumnCount();
		for (int i = 0; i < count; ++i) {
			String name = cursor.getColumnName(i);
			Logger.D(TAG, "Name of " + i + " column: " + name);
		}
		
		do {
			String filename = cursor.getString(cursor.getColumnIndex(Media.TITLE));
			Logger.D(TAG, "Filename: " + filename);
		} while (cursor.moveToNext());
	}
	*/
	
	private void fill(File[] files) {
		items = new ArrayList<String>();
		if (files != null) {
			for (File file : files) {
				if (!file.isDirectory()) {
					
					boolean skip = false;
					
					if ( file.getName().indexOf(".png") == -1 && file.getName().indexOf(".jpg") == -1 )
						skip = true;
					
					if ( !skip )
						items.add(file.getName());
				}
			}
		}
		ArrayAdapter<String> fileList = new ArrayAdapter<String>(this,
				AndroidR.layout.file_row, items);
		
		filesList.setAdapter(fileList);
	}

}