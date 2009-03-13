package rhomobile.camera;

import java.util.Vector;
import java.util.Date;
import java.io.IOException;

import rhomobile.sync.SyncBlob;
import rhomobile.RhodesApplication;

import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.FieldChangeListener;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.component.Dialog;
import net.rim.device.api.ui.component.ButtonField;
import net.rim.device.api.ui.component.LabelField;
import net.rim.device.api.ui.component.ObjectChoiceField;
import net.rim.device.api.ui.component.RichTextField;
import net.rim.device.api.ui.container.HorizontalFieldManager;
import net.rim.device.api.ui.container.MainScreen;
import net.rim.device.api.i18n.SimpleDateFormat;
import net.rim.device.api.io.http.HttpHeaders;
import net.rim.device.api.util.StringUtilities;

import javax.microedition.media.Manager;
import javax.microedition.media.Player;
import javax.microedition.media.control.VideoControl;

import org.garret.perst.impl.Jsr75File;

public class CameraScreen extends MainScreen {

    /** The camera's video controller. */
    private VideoControl _videoControl;

    /** The field containing the feed from the camera. */
    private Field _videoField;

    /** A field which contains the current snapshot encoding. */
    private ObjectChoiceField _encodingField;
    
    /** An array of valid snapshot encodings. */
    private EncodingProperties[] _encodings;
    
    /** A button that captures a snapshot when pressed. */
    private ButtonField _photoButton;
    
    /** A reference to the current screen for listeners. */
	private CameraScreen _cameraScreen;

	/** Callback URL **/
	private String _callbackUrl;
	
	/**
	 * Constructor.
	 * @param raw A byte array representing an image.
	 */
	public CameraScreen(String callbackUrl)
	{
		//
		_callbackUrl = callbackUrl;

		//A reference to this object, to be used in listeners.
		_cameraScreen = this;

		setTitle( new LabelField( "Take picture", LabelField.ELLIPSIS | LabelField.USE_ALL_WIDTH ) );

        //Initialize the camera object and video field.
        initializeCamera();
	
        //Initialize the list of possible encodings.
        initializeEncodingList();
        
        //If the field was constructed successfully, create the UI.
        if(_videoField != null)
        {
            createUI();
        }
        //If not, display an error message to the user.
        else
        {
            add( new RichTextField( "Error connecting to camera." ) );
        }        
	}

//	String canonicalizeURL(String url) {
//		if (!url.startsWith("http://")) {
//			String slash = "/";
//			if (url.startsWith("\\") || url.startsWith("/")) {
//				slash = "";
//			}
//			return "http://localhost:8080" + slash + url;
//		}
//		return url;
//	}
    /**
     * Initializes the Player, VideoControl and VideoField.
     */
    private void initializeCamera()
    {
        try
        {
            //Create a player for the Blackberry's camera.
            Player player = Manager.createPlayer( "capture://video" );

            //Set the player to the REALIZED state (see Player docs.)
            player.realize();

            //Grab the video control and set it to the current display.
            _videoControl = (VideoControl)player.getControl( "VideoControl" );

            if (_videoControl != null)
            {
                //Create the video field as a GUI primitive (as opposed to a
                //direct video, which can only be used on platforms with
                //LCDUI support.)
                _videoField = (Field) _videoControl.initDisplayMode (VideoControl.USE_GUI_PRIMITIVE, "net.rim.device.api.ui.Field");
                //Display the video control
                _videoControl.setVisible(true);
            }

            //Set the player to the STARTED state (see Player docs.)
            player.start();
        }
        catch(Exception e)
        {
            Dialog.alert( "ERROR " + e.getClass() + ":  " + e.getMessage() );
        }
    }

    /**
     * Initialize the list of encodings.
     */
    private void initializeEncodingList()
    {
        try
        {
            //Retrieve the list of valid encodings.
            String encodingString = System.getProperty("video.snapshot.encodings");
            
            //Extract the properties as an array of words.
            String[] properties = StringUtilities.stringToKeywords(encodingString);
            
            //The list of encodings;
            Vector encodingList = new Vector();
            
            //Strings representing the four properties of an encoding as
            //returned by System.getProperty().
            String encoding = "encoding";
            String width = "width";
            String height = "height";
            String quality = "quality";
            
            EncodingProperties temp = null;
            
            for(int i = 0; i < properties.length ; ++i)
            {
                if( properties[i].equals(encoding))
                {
                    if(temp != null && temp.isComplete())
                    {
                        //Add a new encoding to the list if it has been
                        //properly set.
                        encodingList.addElement( temp );
                    }
                    temp = new EncodingProperties();
                    
                    //Set the new encoding's format.
                    ++i;
                    temp.setFormat(properties[i]);
                }
                else if( properties[i].equals(width))
                {
                    //Set the new encoding's width.
                    ++i;
                    temp.setWidth(properties[i]);
                }
                else if( properties[i].equals(height))
                {
                    //Set the new encoding's height.
                    ++i;
                    temp.setHeight(properties[i]);
                }
                else if( properties[i].equals(quality))
                {
                    //Set the new encoding's quality.
                    ++i;
                    temp.setQuality(properties[i]);
                }
            }
            
            //If there is a leftover complete encoding, add it.
            if(temp != null && temp.isComplete())
            {
                encodingList.addElement( temp );
            }
            
            //Convert the Vector to an array for later use.
            _encodings = new EncodingProperties[ encodingList.size() ];
            encodingList.copyInto((Object[])_encodings);
        }
        catch (Exception e)
        {
            //Something is wrong, indicate that there are no encoding options.
            _encodings = null;
        }
    }
    
    /**
     * Adds the VideoField and the "Take Photo" button to the screen.
     */
    private void createUI()
    {
        //Add the video field to the screen.
        add(_videoField);

        //Initialize the button used to take photos.
        _photoButton = new ButtonField( "Take Photo" );
        _photoButton.setChangeListener( new SaveListener() );

		//Create the CANCEL button which returns the user to the main camera
		//screen without saving the picture.
		ButtonField cancelButton = new ButtonField( "Cancel" );
		cancelButton.setChangeListener( new CancelListener() );
		
		//The HorizontalFieldManager keeps the buttons in the center of
        //the screen.

        HorizontalFieldManager hfm = new HorizontalFieldManager(Field.FIELD_HCENTER);
        hfm.add(_photoButton);
		hfm.add(cancelButton);

        //Add the FieldManager containing the button to the screen.
        add(hfm);  
        
        //If there are encoding options available:
        if(_encodings != null)
        {
            //Add the field used to select the snapshot encoding to the screen.
            _encodingField = new ObjectChoiceField("Encoding: ", _encodings, 0);
            add(_encodingField);
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
	 * A wrapper for the various encoding properties available
	 * for use with the VideoControl.getSnapshot() method.
	 */
	public final class EncodingProperties
	{   
	    /** The file format of the picture. */
	    private String _format;

	    /** The width of the picture. */
	    private String _width;

	    /** The height of the picture. */
	    private String _height;

	    /** The quality of the picture. */
	    private String _quality;
	    
	    /** Booleans that indicate whether the values have been set. */
	    private boolean _formatSet;
	    private boolean _widthSet;
	    private boolean _heightSet;
	    private boolean _qualitySet;

	    /**
	     * Set the file format to be used in snapshots.
	     * @param format
	     */
	    public void setFormat(String format)
	    {
	        _format = format;
	        _formatSet = true;
	    }

	    public String getFormat()
	    {
	    	if (_formatSet)
	    		return _format;
	        return "image";
	    }
	    
	    /**
	     * Set the width to be used in snapshots.
	     * @param width
	     */
	    public void setWidth(String width)
	    {
	        _width = width;
	        _widthSet = true;
	    }

	    /**
	     * Set the height to be used in snapshots.
	     * @param height
	     */
	    public void setHeight(String height)
	    {
	        _height = height;
	        _heightSet = true;
	    }

	    /**
	     * Set the quality to be used in snapshots.
	     * @param quality
	     */
	    public void setQuality(String quality)
	    {
	        _quality = quality;
	        _qualitySet = true;
	    }

	    /**
	     * Return the encoding as a coherent String to be used in menus.
	     */
	    public String toString()
	    {
	        StringBuffer display = new StringBuffer();

	        display.append(_width);
	        display.append(" x ");
	        display.append(_height);
	        display.append(" ");
	        display.append(_format);
	        display.append(" (");
	        display.append(_quality);
	        display.append(")");

	        return display.toString();
	    }

	    /**
	     * Return the encoding as a properly formatted string to
	     * be used by the VideoControl.getSnapshot() method.
	     * @return The encoding expressed as a formatted string.
	     */
	    public String getFullEncoding()
	    {
	        StringBuffer fullEncoding = new StringBuffer();

	        fullEncoding.append("encoding=");
	        fullEncoding.append(_format);

	        fullEncoding.append("&width=");
	        fullEncoding.append(_width);

	        fullEncoding.append("&height=");
	        fullEncoding.append(_height);

	        fullEncoding.append("&quality=");
	        fullEncoding.append(_quality);

	        return fullEncoding.toString();
	    }
	    
	    /**
	     * Have all the fields been set?
	     * @return true if all fields have been set.
	     */
	    public boolean isComplete()
	    {
	        return _formatSet && _widthSet && _heightSet && _qualitySet;
	    }
	}

	/**
	 * A listener used for the "Take picture" button.
	 */
	private class SaveListener implements FieldChangeListener
	{
		private String makeFileName(String strExt)throws IOException {
			
			String fName = SyncBlob.makeBlobFolderName();
			SimpleDateFormat format = 
				new SimpleDateFormat("MMM_dd_yyyy_HH_mm_ss_zzz");
			fName += "image_" + format.format(new Date()) + strExt;

			return fName;
		}		
	    /**
	     * Save file, send notification, and return to the main camera screen.
	     */
		public void fieldChanged(Field field, int context)
		{
        	RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
    		HttpHeaders headers = new HttpHeaders();
    		headers.addProperty("Content-Type", "application/x-www-form-urlencoded");
			Jsr75File file = new Jsr75File();
			
            try
            {
                //A null encoding indicates that the camera should
                //use the default snapshot encoding.
                String encoding = null;
                String ext = ".";
                //If there are encoding options available:
                if( _encodings != null && _encodingField != null )
                {
                    //Use the user-selected encoding instead.
                    encoding = _encodings[_encodingField.getSelectedIndex()].getFullEncoding();
                    ext += _encodings[_encodingField.getSelectedIndex()].getFormat();
                }
                
            	String fname = makeFileName(ext);
            	file.open(fname, false, false);
            	
                //Retrieve the raw image from the VideoControl
            	byte[] image = _videoControl.getSnapshot(encoding);
            	//Write image data
            	file.getOutStream().write(image,0,image.length);
            	image = null;
            	
            	app.postUrl(_callbackUrl,  "status=ok&image_uri="+fname, headers);
            	
            } catch(Exception e) {
            	app.postUrl(_callbackUrl, "status=error&message=Error", headers);
            	Dialog.alert( "Error " + e.getClass() + ":  " + e.getMessage() );
            } finally {
    			try{
    				file.close();
    			}catch(Exception exc){}
    			
            	app.popScreen( _cameraScreen );            	
            }
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
    		app.postUrl(_callbackUrl, "status=cancel&message=User canceled operation", headers);
			app.popScreen( _cameraScreen );
		}
	}
}