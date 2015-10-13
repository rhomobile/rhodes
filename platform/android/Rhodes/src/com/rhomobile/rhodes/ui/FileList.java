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

package com.rhomobile.rhodes.ui;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.util.Utils;

import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.GradientDrawable.Orientation;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.OrientationEventListener;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;

public class FileList extends BaseActivity implements OnClickListener{

	private static final String TAG = FileList.class.getSimpleName();
	
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
		
		setContentView(R.layout.directory_list);
		
		imagePreview = (ImageView) findViewById(R.id.preview);

		ListView filesList = (ListView) findViewById(R.id.filesList);

		files = getImages();
		List<String> names = new ArrayList<String>();
		Iterator<String> it = files.iterator();
		while (it.hasNext()) {
			names.add(Utils.getBaseName(it.next()));
		}
		filesList.setAdapter(new ArrayAdapter<String>(this, R.layout.file_row, names));

		Button okButton = (Button) findViewById(R.id.okButton);
		Button cancelButton = (Button) findViewById(R.id.cancelButton);

		TextView lookIn = (TextView) findViewById(R.id.lookIn);
		lookIn.setText("Look In: Gallery");

		okButton.setOnClickListener(this);
		cancelButton.setOnClickListener(this);
		
		filesList.setOnItemClickListener(new OnItemClickListener(){

			public void onItemClick(AdapterView<?> arg0, View arg1, int position, long arg3) {
				try {
					String file = files.get(position);
					Logger.D(TAG, "Selected file: " + file);
					int intDividerforwidth= getWindowManager().getDefaultDisplay().getWidth()/3;
					int intDividerforHeight=getWindowManager().getDefaultDisplay().getHeight()/4;
					if( getWindowManager().getDefaultDisplay().getHeight()<= getWindowManager().getDefaultDisplay().getWidth())
					{	
						intDividerforHeight=getWindowManager().getDefaultDisplay().getWidth()/4;
						intDividerforwidth=getWindowManager().getDefaultDisplay().getHeight()/3;
					}
					BitmapFactory.Options options = new BitmapFactory.Options();
					options.inSampleSize = 10;
					Bitmap bm = BitmapFactory.decodeFile(file, options);
					if (bm != null)
						bm = Bitmap.createScaledBitmap(bm, intDividerforHeight, intDividerforwidth, true);
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

    private void doCallback(int status, String filename) {
        Intent intent = new Intent();
        //try {
            //Uri dstUrl = getIntent().getParcelableExtra(MediaStore.EXTRA_OUTPUT);
            //String dstPath = dstUrl.getPath();
            if (filename != null && filename.length() > 0) {
                Logger.T(TAG, "Selected file: " + Uri.fromFile(new File(filename)));
                //Utils.copy(filename, dstPath);
                //Logger.T(TAG, "Copied file: " + dstPath);
                //intent.putExtra(MediaStore.EXTRA_OUTPUT, dstUrl);
                intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(new File(filename)));
            } else {
                intent.putExtra("cancel", "No input file name");
            }
//        } catch (IOException e) {
//            Logger.E(TAG, e);
//            intent.putExtra("error", e.getMessage());
//            status = RESULT_CANCELED;
//        }
        setResult(status, intent);
    }

    public void onClick(View view) {
        switch (view.getId()) {
        case R.id.okButton:
            doCallback(RESULT_OK, selectedFile);
            break;
        case R.id.cancelButton:
            doCallback(RESULT_CANCELED, null);
            break;
        }
        finish();
    }
}
