package rhomobile.camera;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.Vector;
import java.util.Date;
import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;

import com.rho.db.DBAdapter;
import rhomobile.RhodesApplication;
import rhomobile.Utilities;

import net.rim.blackberry.api.invoke.CameraArguments;
import net.rim.blackberry.api.invoke.Invoke;
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

import com.rho.SimpleFile;
import com.rho.RhoClassFactory;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.Version;

public class CameraScreen extends MainScreen {

	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Camera");
	
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
	
	private CameraFilesListener _fileListener = null;
	
	private boolean _cameraConnected;
	
	/**
	 * Constructor.
	 * @param raw A byte array representing an image.
	 */
	public CameraScreen(String callbackUrl)
	{
		_cameraConnected = false;
		
		//
		_callbackUrl = callbackUrl;

		//A reference to this object, to be used in listeners.
		_cameraScreen = this;

		setTitle( new LabelField( "Take picture", LabelField.ELLIPSIS | LabelField.USE_ALL_WIDTH ) );

        //Initialize the camera object and video field.
        initializeCamera();
        
        createUI();
	}
	
	public void invokeCloseScreen(boolean ok) {
		invokeCloseScreen(ok, null);
	}
	
	public void invokeCloseScreen(boolean ok, String fileName) {
		RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
		app.invokeLater(new CloseScreen(_cameraScreen, ok, fileName));
		app.requestForeground();
	}
	
	private class CloseScreen implements Runnable {
		private CameraScreen _screen;
		private boolean _ok;
		private String _fileName;
		
		CloseScreen(CameraScreen screen, boolean ok, String fileName) {
			_screen = screen;
			_ok = ok;
			_fileName = fileName;
		}

		public void run() {
			synchronized(UiApplication.getEventLock()) {
				if (_screen != null)
					_screen.closeScreen(_ok, _fileName);
			}
		}
	};
	
	private void closeScreen(boolean ok) {
		closeScreen(ok, null);
	}
	
	private void closeScreen(boolean ok, String fileName) {
		if (_cameraScreen == null)
			return;
		
		RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
    	
    	if (_fileListener != null) {
    		app.removeFileSystemJournalListener(_fileListener);
    		_fileListener = null;
    	}
    	
		String body;
		if (ok) {
			boolean error = false;
			String fname = "";
			
			SimpleFile file = null;
			FileConnection fconnsrc = null;
			FileConnection fconndst = null;
	        try
	        {
	        	file = RhoClassFactory.createFile();
	        	
	        	if (fileName != null) {
	        		String ext = fileName.substring(fileName.lastIndexOf('.'));
	        		fname = makeFileName(ext);
					fconnsrc = (FileConnection)Connector.open(fileName, Connector.READ_WRITE);
					fconndst = (FileConnection)Connector.open(fname, Connector.READ_WRITE);
					if (fconndst.exists())
						fconndst.delete();
					fconndst.create();
					
					LOG.TRACE("should be readed " + fconnsrc.fileSize() + " bytes");
					
					InputStream src = fconnsrc.openInputStream();
					OutputStream dst = fconndst.openOutputStream();
					
					long fullLength = 0;
					
					byte[] buf = new byte[64*1024];
					int ret;
					while((ret = src.read(buf)) > 0) {
						fullLength += ret;
						dst.write(buf, 0, ret);
					}
					dst.flush();
					
					LOG.TRACE("readed: " + fullLength + " bytes");
				}
	        	else {
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
		            
		            fname = makeFileName(ext);
	        		
		        	file.open(fname, false, false);
		        	
		            //Retrieve the raw image from the VideoControl
		        	byte[] image = _videoControl.getSnapshot(encoding);
		        	//Write image data
		        	file.getOutStream().write(image,0,image.length);
		        	image = null;
	        	}
	
	        	String root = file.getDirPath("");
	        	fname = "/" + fname.substring(root.length());
	        	fname = Utilities.replaceAll(fname,"/","%2F");
	        } catch(Exception e) {
	        	error = true;
	        	LOG.ERROR(e);
	        	Dialog.alert( "Error " + e.getClass() + ":  " + e.getMessage() );
	        } finally {
				try{
					if ( file != null )
						file.close();
					
					if (fconnsrc != null)
						fconnsrc.close();
					if (fconndst != null)
						fconndst.close();
				}catch(Exception exc){}
	        }
	        if (error)
	        	body = "status=error&message=Error";
	        else
	        	body = "status=ok&image_uri=" + fname;
		}
		else
			body = "status=cancel&message=User cancelled operation";
	
		body += "&rho_callback=1";
		
		HttpHeaders headers = new HttpHeaders();
		headers.addProperty("Content-Type", "application/x-www-form-urlencoded");
		
		LOG.INFO("Callback with uri: " + _callbackUrl + ": " + body);
		app.postUrl(_callbackUrl, body, headers);
		app.popScreen(_cameraScreen);
		_cameraScreen = null;
	}
	
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
     * Initializes the Player, VideoControl and VideoField.
     */
    private void initializeCamera()
    {
    	LOG.TRACE("initializeCamera");
    	Player player = null;
    	try {
	        Version.SoftVersion ver = Version.getSoftVersion();
	        if ( ver.nMajor == 4 && ver.nMinor == 6 && ver.nMinor2 == 1)
	        {
	        	//http://rim.lithium.com/rim/board/message?board.id=java_dev&view=by_date_ascending&message.id=21891
	        	//takeSnapShot does not work on BlackBerry handheld software version 4.6.1:  
	        	//The only work around is to invoke the camera application and listen for an image 
	        	//being saved (would occur when the user takes a picture).  
	        }else
	        {
	    		// First of all, attempting to capture picture using MM API
	    		
	            //Create a player for the Blackberry's camera.
	            player = Manager.createPlayer( "capture://video" ); 
	            LOG.TRACE("Recording using MM API");
	        }
    	}
    	catch(Exception e) {
    		// Try to capture picture using BB Camera application
    		LOG.TRACE("Recording using BB Camera application");
    	}
    	
    	try
        {
    		if (player != null) {
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
	            
	            //Initialize the list of possible encodings.
	            initializeEncodingList();
    		}
    		else {
    			RhodesApplication app = RhodesApplication.getInstance();
    			_fileListener = new CameraFilesListener(this);
    			app.addFileSystemJournalListener(_fileListener);
    			
    			app.addActivateHook(new ActivateHook(_cameraScreen));
    			
    			CameraArguments vidargs = new CameraArguments();
            	Invoke.invokeApplication(Invoke.APP_TYPE_CAMERA, vidargs);
    		}
    		_cameraConnected = true;
        }
        catch(Exception e)
        {
        	LOG.ERROR("Camera initialization failed", e);
        	//Dialog.alert( "ERROR : can not connect to camera");
        }
    }
    
    private class ActivateHook extends RhodesApplication.ActivateHook {
    	private CameraScreen _screen;
    	public ActivateHook(CameraScreen screen) {
    		_screen = screen;
    	}
		public void run() {
			_screen.invokeCloseScreen(false);
		}
    };

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
        if (!_cameraConnected) {
        	add( new RichTextField( "Error connecting to camera." ) );
        	return;
        }
    	
        if (_videoField != null) {
	        //Add the video field to the screen.
	        add(_videoField);
	
	        //Initialize the button used to take photos.
	        _photoButton = new ButtonField( "Take Photo" );
	        _photoButton.setChangeListener( new SaveListener() );
    	}
        else {
        	add( new RichTextField( "Waiting for Camera start..." ) );
        }
	
		//Create the CANCEL button which returns the user to the main camera
		//screen without saving the picture.
		ButtonField cancelButton = new ButtonField( "Cancel" );
		cancelButton.setChangeListener( new CancelListener() );
		
		//The HorizontalFieldManager keeps the buttons in the center of
        //the screen.

        HorizontalFieldManager hfm = new HorizontalFieldManager(Field.FIELD_HCENTER);
        if (_photoButton != null)
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
		if (_fileListener != null) {
    		RhodesApplication.getInstance().removeFileSystemJournalListener(_fileListener);
    		_fileListener = null;
    	}
		
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
	public final static class EncodingProperties
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
	    /**
	     * Save file, send notification, and return to the main camera screen.
	     */
		public void fieldChanged(Field field, int context)
		{
        	_cameraScreen.closeScreen(true);
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
			_cameraScreen.closeScreen(false);
		}
	}
}