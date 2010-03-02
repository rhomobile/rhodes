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
import java.util.ArrayList;
import java.util.List;

import com.rhomobile.rhodes.AndroidR;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.Rhodes;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
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
	
	private Button okButton;
	private Button cancelButton;
	private TextView lookIn;
	private List<String> items = null;
	private String selectedFilePath = "";
	private ImageView imagePreview;
	private ListView filesList;
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);

		getWindow().setFlags(Rhodes.WINDOW_FLAGS, Rhodes.WINDOW_MASK);
		setContentView(AndroidR.layout.directory_list);

		imagePreview = (ImageView) findViewById(AndroidR.id.preview);

		filesList = (ListView) findViewById(AndroidR.id.filesList);
		
		fill(new File(Camera.BASE_CAMERA_DIR).listFiles());

		okButton = (Button) findViewById(AndroidR.id.okButton);
		cancelButton = (Button) findViewById(AndroidR.id.cancelButton);

		lookIn = (TextView) findViewById(AndroidR.id.lookIn);

		lookIn.setText("Look In: " + Camera.BASE_CAMERA_DIR);

		okButton.setOnClickListener(this);
		cancelButton.setOnClickListener(this);
		
		filesList.setOnItemClickListener(new OnItemClickListener(){

			public void onItemClick(AdapterView<?> arg0, View arg1, int position, long arg3) {
				int selectionRowID = position;
				try {
					File file = new File(Camera.BASE_CAMERA_DIR + "/" + items.get(selectionRowID));
					selectedFilePath = file.getAbsolutePath();
					
					BitmapFactory.Options options = new BitmapFactory.Options();
					options.inSampleSize = 10;
					Bitmap obm = BitmapFactory.decodeFile(selectedFilePath, options);
					Bitmap bm = Bitmap.createScaledBitmap(obm, 176, 144, true);
					imagePreview.setImageBitmap(bm);
				} catch (Exception e) {
					Logger.E(TAG, e.getMessage());
				}
			}
			
		});
	}

	public void onClick(View view) {
		switch (view.getId()) {
		case AndroidR.id.okButton:
			com.rhomobile.rhodes.camera.Camera.doCallback(this.selectedFilePath);
			finish();
			break;
		case AndroidR.id.cancelButton:
			selectedFilePath = "";
			com.rhomobile.rhodes.camera.Camera.doCallback(this.selectedFilePath);
			finish();
			break;
		}
	}
	
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

	public String getSelectedFilePath() {
		return selectedFilePath;
	}
}