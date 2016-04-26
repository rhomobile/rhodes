/**
 * 
 */
package com.rho.rhoelements;
import java.io.File;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.R.string;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.CursorLoader;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.provider.MediaStore.MediaColumns;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.RadioButton;
import android.widget.TextView;
import android.provider.DocumentsContract;

/**
 * @author FPC843
 * This activity is responsible for creating RhoElements Home Screen shortcuts
 */
public class CreateShortcutActivity extends Activity
{
	Button mConfirmButton;
	Button mCancelButton;
	Button mIconBrowseButton;
	RadioButton mConfigPathRadioB;
	RadioButton mAppUrlRadioB;
	TextView mShortcutIconPath;
	EditText mShortcutUrlEditor;
	EditText mShortcutConfigPathEditor;
    EditText mShortcutNameEditor;
    private String selectedImagePath;
	private static final int SELECT_PICTURE = 1;
	private ImageView img;
	String mShortcutName = null;
	int imageWidth = 0;
	int imageHeight = 0;
	String confileFilePath =null;
	
	@Override
    protected void onCreate(Bundle savedInstanceState)
	{
        super.onCreate(savedInstanceState);
        setContentView(RhoExtManager.getResourceId("layout","createshortcutui"));
        
        mShortcutUrlEditor = (EditText) findViewById(RhoExtManager.getResourceId("id","editUrl"));
        mShortcutConfigPathEditor = (EditText) findViewById(RhoExtManager.getResourceId("id","editConfigPath"));
        mShortcutNameEditor = (EditText) findViewById(RhoExtManager.getResourceId("id","editName"));
        
        mConfirmButton = (Button) findViewById(RhoExtManager.getResourceId("id","bt_ok"));
        
        mShortcutIconPath = (TextView) findViewById(RhoExtManager.getResourceId("id","iconRes"));
        
        mIconBrowseButton = (Button) findViewById(RhoExtManager.getResourceId("id","iconButton"));
        
        mIconBrowseButton.setOnClickListener(new View.OnClickListener() {
        	
            public void onClick(View arg0) {
                Intent intent = new Intent(Intent.ACTION_PICK,
		                    android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
	        startActivityForResult(intent, SELECT_PICTURE);
            }
        });
        
        
        mConfirmButton.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0)
			{
				generateShortcut();
				if ( (mShortcutName == null) || (mShortcutName.length() == 0) )
					return;
				finish();
			}
		});
        mCancelButton = (Button) findViewById(RhoExtManager.getResourceId("id","bt_cancel"));
        mCancelButton.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View arg0)
			{
				finish();
			}
		});
        mConfigPathRadioB = (RadioButton) findViewById(RhoExtManager.getResourceId("id","rbConfigPath"));
        mConfigPathRadioB.setChecked(true);
        mAppUrlRadioB = (RadioButton) findViewById(RhoExtManager.getResourceId("id","rbAppUrl"));
        mConfigPathRadioB.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			
			@Override
			public void onCheckedChanged(CompoundButton arg0, boolean checked)
			{
				mAppUrlRadioB.setChecked(!checked);
				mShortcutUrlEditor.setEnabled(!checked);
				mShortcutConfigPathEditor.setEnabled(checked);
			}
		});
        mAppUrlRadioB.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
			
			@Override
			public void onCheckedChanged(CompoundButton arg0, boolean checked)
			{
				mConfigPathRadioB.setChecked(!checked);
				mShortcutConfigPathEditor.setEnabled(!checked);
				mShortcutUrlEditor.setEnabled(checked);
			}
		});
	}
	
 public void onActivityResult(int requestCode, int resultCode, Intent data) {
	        if (resultCode == RESULT_OK) {
	            if (requestCode == SELECT_PICTURE) {
	                Uri selectedImageUri = data.getData();
	                if (Build.VERSION.SDK_INT < 19){
	                   imageWidth =70;
	                    imageHeight =70;
	                }
	    		else if (Build.VERSION.SDK_INT >= 19){
	    			 imageWidth =100;
	                	 imageHeight =100;
	    			}
	    	        selectedImagePath = getRealPathFromURI(getApplicationContext(),selectedImageUri);
	                mShortcutIconPath.setText(selectedImagePath);
	                mShortcutIconPath.setMaxLines(2);
	            }
	        }
	    }

	 /**
	     * Returns Real Path of the image with respect to the Media query
	     * for below Kitkat platform
	     * @param context
	     * @return Media URI
	     */ 
	 
	 public  String getRealPathFromURI(Context context, Uri contentUri) {
		  String[] proj = { MediaStore.Images.Media.DATA };
		  String result = null;
		   
		  CursorLoader cursorLoader = new CursorLoader(
				  context, 
		    contentUri, proj, null, null, null);        
		  Cursor cursor = cursorLoader.loadInBackground();
		  
		  if(cursor != null){
		   int column_index = 
		     cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
		   cursor.moveToFirst();
		   result = cursor.getString(column_index);
		  }
		    
		  return result;  
	}
	 
	
	/**
     * Returns an Intent describing a RhoElements shortcut containing a URL
     *
     * @param result The result from the phone number picker
     * @return an Intent describing a phone number shortcut
     */
    private void generateShortcut()
    {
    	int error = 0;
    	String errorMessage = "";
    	if (mAppUrlRadioB.isChecked())
		{
    		if(mShortcutUrlEditor.getText().toString().length() == 0)
    		{
    			error = -1;
    			errorMessage = getString(RhoExtManager.getResourceId("string","shortcut_err_entervalidpath"));
    		}
    		else
    			error = 1;
		}
    	else if (mConfigPathRadioB.isChecked()) 
    	{
    		if (mShortcutConfigPathEditor.getText().toString().length() == 0)
    		{
    			error = -2;
    			errorMessage = getString(RhoExtManager.getResourceId("string","shortcut_err_entervalidurl"));
    		}
    		else
    			error = 2;
    	}
    	if (error < 0)
        {
        	AlertDialog alertDialog = new AlertDialog.Builder(this).create();
         	alertDialog.setTitle(getString(RhoExtManager.getResourceId("string","shortcut_err_dialogtitle")));
         	alertDialog.setMessage(errorMessage);
         	alertDialog.show();
         	return;
        }
    	if ( (mShortcutNameEditor != null) && (mShortcutNameEditor.length() > 0) )
        	mShortcutName = mShortcutNameEditor.getText().toString();
        else if (error == 1)
        	mShortcutName = mShortcutUrlEditor.getText().toString();
        else if (error == 2)
        	mShortcutName = mShortcutConfigPathEditor.getText().toString();
        Intent shortcutIntent = new Intent();
        shortcutIntent.setAction(ElementsCore.RHOELEMENTS_SHORTCUT_ACTION);
        shortcutIntent.addCategory(Intent.CATEGORY_DEFAULT);
        shortcutIntent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        shortcutIntent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        if (error == 1)
        {
        	shortcutIntent.setDataAndType(Uri.parse(mShortcutUrlEditor.getText().toString()), getString(string.shortcut_mime_url));
            shortcutIntent.putExtra("id", mShortcutUrlEditor.getText().toString().hashCode());
        }
        else if (error == 2)
        {
        	confileFilePath = mShortcutConfigPathEditor.getText().toString();
        	confileFilePath = confileFilePath.replace(" ", "%20");
        	shortcutIntent.setDataAndType(Uri.parse(confileFilePath.toString()), getString(string.shortcut_mime_configpath));
            shortcutIntent.putExtra("id", confileFilePath.toString().hashCode());
        }
        Intent addIntent = new Intent();
        addIntent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, shortcutIntent);
        addIntent.putExtra(Intent.EXTRA_SHORTCUT_NAME, mShortcutName);
          
        if(selectedImagePath!=null){
	        File fXmlFile = new File(selectedImagePath);
	
			if(fXmlFile.exists()){
				//Create ShortCut icon with userSpecific icon
			        Bitmap b=BitmapFactory.decodeFile(selectedImagePath);
			    Bitmap dstBitmap = Bitmap.createScaledBitmap(b, imageWidth, imageHeight, true);
		        	addIntent.putExtra(Intent.EXTRA_SHORTCUT_ICON, dstBitmap);
			}else{
				//Create ShortCut icon as application icon if the file not present at user location
				addIntent.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, Intent.ShortcutIconResource.fromContext(this, RhoExtManager.getResourceId("drawable","icon")));
			}
        }else{
        	addIntent.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, Intent.ShortcutIconResource.fromContext(this, RhoExtManager.getResourceId("drawable","icon")));
        }
       // addIntent.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, Intent.ShortcutIconResource.fromContext(this, RhoExtManager.getResourceId("drawable","icon")));
        addIntent.setAction("com.android.launcher.action.INSTALL_SHORTCUT");
        sendBroadcast(addIntent);
    }
}
