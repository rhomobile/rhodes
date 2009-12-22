package rhomobile.camera;

import java.util.Date;
import java.util.Vector;
//import java.util.Enumeration;

//import java.io.IOException;

import com.rho.db.DBAdapter;
import rhomobile.RhodesApplication;
import rhomobile.Utilities;

import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.FieldChangeListener;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.component.Dialog;
import net.rim.device.api.ui.component.ButtonField;
import net.rim.device.api.ui.component.LabelField;
import net.rim.device.api.ui.component.RichTextField;
import net.rim.device.api.ui.component.BitmapField;
import net.rim.device.api.ui.container.HorizontalFieldManager;
import net.rim.device.api.ui.container.MainScreen;
import net.rim.device.api.i18n.SimpleDateFormat;
import net.rim.device.api.io.http.HttpHeaders;
import net.rim.device.api.system.Bitmap;

import com.rho.Jsr75File;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

public class ImageBrowserScreen extends MainScreen {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("ImageBrowserScreen");
	
	private static final int IMAGE_SCALING = 7;
	
    /** A reference to the current screen for listeners. */
	private ImageBrowserScreen _imageBrowserScreen;

	/** List of images in the folder's */
	private Vector _images = new Vector();
	
	/** Currently displayed image */
	private int _currentImage = 0;
	
	/** Field to display image */
	BitmapField _imageField = new BitmapField();
	
	/** Callback URL */
	private String _callbackUrl;

	/**
	 * Constructor.
	 * @param raw A byte array representing an image.
	 */
	public ImageBrowserScreen(String callbackUrl)
	{
		//
		_callbackUrl = callbackUrl;

		//A reference to this object, to be used in listeners.
		_imageBrowserScreen = this;

		// "/sdcard/blackberry/pictures/"
        // "/store/home/user/pictures/"

		Jsr75File.listFolder("file:///SDCard/blackberry/pictures", _images); 
		Jsr75File.listFolder("file:///store/home/user/pictures", _images); 
		
		for (int index = 0; index < _images.size(); ) { 
			String fileName = (String) _images.elementAt(index);
			if ( fileName.endsWith(".jpeg") || fileName.endsWith(".jpg") || 
					 fileName.endsWith(".png") || fileName.endsWith(".gif") ) {
				index++;
			} else {
				_images.removeElementAt(index);
			}
		}		

		setTitle( new LabelField( "Choose picture", LabelField.ELLIPSIS | LabelField.USE_ALL_WIDTH ) );

        createUI();
	}

	private Bitmap loadBitmap(String uri) {
		Jsr75File file = new Jsr75File();
		Bitmap image = null;
		try {
			//open file
			file.open(uri, false, false);
			//load image bytes into memory
			byte raw[] = new byte[(int)file.length()];
			file.getInputStream().read(raw);
			//Convert the byte array to a Bitmap image.
			image = Bitmap.createBitmapFromBytes( raw, 0, -1, IMAGE_SCALING );
		} catch(Exception e) {
			LOG.ERROR("Can't load " + uri, e);
		} finally {
			try {
				file.close();
			} catch(Exception exc) {}
        }
		return image;
	}
	
    /**
     * Adds the BitmapField and the "Choose" button to the screen.
     */
    private void createUI()
    {
        //If the field was constructed successfully, create the UI.
        if(_images.size() > 0)
        {
        	HorizontalFieldManager hfm0 = new HorizontalFieldManager(Field.FIELD_HCENTER);
    		ButtonField nextButton = new ButtonField( "Next" );
    		nextButton.setChangeListener( new NextListener() );
    		ButtonField previousButton = new ButtonField( "Previous" );
    		previousButton.setChangeListener( new PreviousListener() );
    		hfm0.add(previousButton);
    		hfm0.add(nextButton);
        	add(hfm0);
        	
        	HorizontalFieldManager hfm1 = new HorizontalFieldManager( Field.FIELD_HCENTER );
        	Bitmap bmp = loadBitmap((String)_images.elementAt(0));
        	if ( bmp != null ) {
        		_imageField.setBitmap(bmp);
        	}
        	hfm1.add( _imageField );
        	add(hfm1);

            HorizontalFieldManager hfm = new HorizontalFieldManager(Field.FIELD_HCENTER);
    		ButtonField cancelButton = new ButtonField( "Cancel" );
    		cancelButton.setChangeListener( new CancelListener() );
    		ButtonField selectButton = new ButtonField( "Select" );
    		selectButton.setChangeListener( new SelectListener() );

    		hfm.add(selectButton);
    		hfm.add(cancelButton);

            //Add the FieldManager containing the button to the screen.
            add(hfm);   	
        }
        //If not, display an error message to the user.
        else
        {
            add( new RichTextField( "There is no images to choose..." ) );
        }
    }

	/**
	 * Handle trackball click events.
	 * @see net.rim.device.api.ui.Screen#invokeAction(int)
	 */
	protected boolean invokeAction(int action)
	{
		boolean handled = super.invokeAction(action);

		if(!handled)
		{
			switch(action)
			{
    			case ACTION_INVOKE: // Trackball click.
    			{
    				return true;
    			}
			}
		}
		return handled;
	}

	/**
	 * A listener used for the "Next" button.
	 */
	private class NextListener implements FieldChangeListener
	{
	    /**
	     * Return to the main camera screen.
	     */
		public void fieldChanged(Field field, int context)
		{
			if (_images.size() > 1) {
				_currentImage++;			
				if ( _currentImage >= _images.size() ) {
					_currentImage = 0;
				}
				Bitmap bmp = loadBitmap((String)_images.elementAt(_currentImage));
				if ( bmp != null ) {
					_imageField.setBitmap(bmp);
				}
			}
		}
	}

	/**
	 * A listener used for the "Previous" button.
	 */
	private class PreviousListener implements FieldChangeListener
	{
	    /**
	     * Return to the main camera screen.
	     */
		public void fieldChanged(Field field, int context)
		{
			if (_images.size() > 1) {
				_currentImage--;			
				if ( _currentImage < 0 ) {
					_currentImage = _images.size()-1;
				}
				Bitmap bmp = loadBitmap((String)_images.elementAt(_currentImage));
				if ( bmp != null ) {
					_imageField.setBitmap(bmp);
				}
			}
		}
	}
	
	/**
	 * A listener used for the "Select" button.
	 */
	private class SelectListener implements FieldChangeListener
	{
		private String makeFileName(String ext)throws Exception {

			String fullname = DBAdapter.makeBlobFolderName();
			SimpleDateFormat format =
				new SimpleDateFormat("MMM_dd_yyyy_HH_mm_ss_zzz");
			
			String name = format.format(new Date());
			name = Utilities.replaceAll(name,"/","_");
			fullname += "image_" + name + ext;

			return fullname;
		}
		
	    /**
	     * Save file, send notification, and return to the main camera screen.
	     */
		public void fieldChanged(Field field, int context)
		{
			if (_images.size() == 0) {
				return;
			}
				
        	RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
    		HttpHeaders headers = new HttpHeaders();
    		headers.addProperty("Content-Type", "application/x-www-form-urlencoded");
			Jsr75File srcFile = new Jsr75File();
			Jsr75File destFile = new Jsr75File();
			boolean error = false;
			String fname = "";
			
            try
            {
            	String imageName = (String)_images.elementAt(_currentImage);
            	int i = imageName.lastIndexOf('.');
                String ext = imageName.substring(i);

            	fname = makeFileName(ext);
            	LOG.INFO("Selected file new name: " + fname);

    			//open file
    			srcFile.open(imageName, false, false);
    			//load image bytes into memory
    			byte raw[] = new byte[(int)srcFile.length()];
            	srcFile.getInputStream().read(raw);
            	
            	destFile.open(fname, false, false);
            	//Write image data
            	destFile.getOutStream().write(raw,0,raw.length);
            	
            	String root = Jsr75File.getRhoPath();
            	fname = "/" + fname.substring(root.length());
            	fname = Utilities.replaceAll(fname,"/","%2F");
            } catch(Exception e) {
            	error = true;
            	Dialog.alert( "Error " + e.getClass() + ":  " + e.getMessage() );
            } finally {
    			try {
    				srcFile.close();
    			} catch(Exception exc) {}
    			try {
    				destFile.close();
    			} catch(Exception exc) {}
            }
            if (error) {
            	LOG.ERROR("Callback with error: status=error&message=Error");
            	app.postUrl(_callbackUrl, "status=error&message=Error&rho_callback=1", headers); 
            } else {
            	LOG.INFO("Callback with uri: status=ok&image_uri="+fname);
            	app.postUrl(_callbackUrl,  "status=ok&rho_callback=1&image_uri="+fname, headers); 
            }

        	app.popScreen( _imageBrowserScreen );
		}
	}

	/**
	 * A listener used for the "Cancel" button.
	 */
	private class CancelListener implements FieldChangeListener
	{
	    /**
	     * Return to the main camera screen.
	     */
		public void fieldChanged(Field field, int context)
		{
        	RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
    		HttpHeaders headers = new HttpHeaders();
    		headers.addProperty("Content-Type", "application/x-www-form-urlencoded");
    		app.postUrl(_callbackUrl, "status=cancel&rho_callback=1&message=User canceled operation", headers);
			app.popScreen( _imageBrowserScreen );
		}
	}
}