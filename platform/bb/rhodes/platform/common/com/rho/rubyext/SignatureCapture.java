package com.rho.rubyext;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhodesApp;
import com.rho.db.DBAdapter;
import com.rho.file.RhoFile;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyID;
import com.xruby.runtime.lang.RubyModule;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyTwoArgMethod;
import com.xruby.runtime.lang.RubyValue;

import net.rim.device.api.i18n.SimpleDateFormat;
import net.rim.device.api.system.Bitmap;
import net.rim.device.api.system.Display;
import net.rim.device.api.system.EncodedImage;
import net.rim.device.api.ui.Graphics;
import net.rim.device.api.ui.MenuItem;
import net.rim.device.api.ui.Ui;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.component.BitmapField;
import net.rim.device.api.ui.component.Dialog;
import net.rim.device.api.ui.component.Menu;
import net.rim.device.api.ui.container.MainScreen;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Date;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;

import rhomobile.Utilities;

public class SignatureCapture extends MainScreen 
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Alert");
	
	private static RhodesApp RHODESAPP(){ return RhodesApp.getInstance(); }
	
	public static final RubyID filePathID = RubyID.intern("filePath");
	
	private RubyHash m_hashParams;
	private String m_strFilePath = "";
	private boolean m_bIsCanceled = false;
	
	//Current Bitmap instance
	private Bitmap _bitmap;
	
	//A custom BitmapField for drawing a signature 
	private SignatureField _signatureField;

	boolean isCanceled(){ return m_bIsCanceled; }
	/**
	 * Constructor
	 */
	public SignatureCapture( RubyHash hashParams ) 
	{
		super();
		m_hashParams = hashParams;
			
		setTitle("Please Sign Your Name");
		//Ui.getUiEngineInstance().setAcceptableDirections(Display.DIRECTION_NORTH);
		try {
			
			//Get the local resource background image to initialize the signature field
			Class cl = Class.forName("com.rho.rubyext.SignatureCapture");
			InputStream is = cl.getResourceAsStream("/background.png");
			ByteArrayOutputStream bas = new ByteArrayOutputStream();
			byte[] buffer = new byte[1024];
			while (is.read(buffer) != -1) {
				bas.write(buffer, 0, buffer.length);
			}
			EncodedImage image = EncodedImage.createEncodedImage(bas.toByteArray(), 0, bas.size());
			//Set the background as the current bitmap
			_bitmap = image.getBitmap();
		} catch (IOException e) {
			LOG.ERROR("Save signature failed.", e);
		} catch (ClassNotFoundException e) {
			LOG.ERROR("Save signature failed.", e);
		}
		//Create a new SignatureField with current _bitmap instance
		_signatureField = new SignatureField(_bitmap);
		add(_signatureField);
	}

	private static String getHashStringValue(RubyHash hash, RubyID id)
    {
		if ( hash == null )
			return "";
		
    	RubyValue val = hash.get(id.toSymbol());
    	
    	return val != null && val != RubyConstant.QNIL ? val.toStr() : ""; 
    }
	
	private String getFilePath()throws Exception 
	{
		if ( m_strFilePath.length() > 0 )
			return m_strFilePath;
		
		m_strFilePath = getHashStringValue(m_hashParams, filePathID );
		if ( m_strFilePath.length() > 0 )
			return m_strFilePath;
		
		m_strFilePath = DBAdapter.makeBlobFolderName();
		SimpleDateFormat format = 
			new SimpleDateFormat("MMM_dd_yyyy_HH_mm_ss_zzz");
		
		String name = format.format(new Date());
		name = Utilities.replaceAll(name,"/","_");
		m_strFilePath += "image_" + name + ".png";
		
		return m_strFilePath;
	}
	
	/** 
	 * MenuItem to Capture the signature and save it to an image file
	 */
	private MenuItem _captureItem = new MenuItem("Capture", 200000, 10) {
		public void run() {
	        net.rim.device.api.system.PNGEncodedImage encodedImage = net.rim.device.api.system.PNGEncodedImage.encode(_bitmap);
			byte[] imageBytes = encodedImage.getData();
			try {
				String fileName = getFilePath();
				
				FileConnection fconn = (FileConnection) Connector.open(fileName);
				if (!fconn.exists())
					fconn.create();
				java.io.OutputStream outputStream = fconn.openOutputStream();
				outputStream.write(imageBytes);
		        outputStream.close();   
		        
				fconn.close(); 
			} catch (Exception e) {
				LOG.ERROR("Save signature failed.", e);
			}
			
			onClose();
		}
	};

	/**
	 * MenuItem to clear the signature field
	 */
	private MenuItem _clearItem = new MenuItem("Clear", 200000, 10) {
		public void run() {
			_signatureField.clear();
		}

	};

	/**
	 * MenuItem to close the application
	 */
	private MenuItem _closeItem = new MenuItem("Close", 200000, 10) {
		public void run() {
			m_bIsCanceled = true;
			onClose();
		}
	};

	/**
	 * Create the menu
	 */
	protected void makeMenu(Menu menu, int instance) {
		menu.add(_captureItem);
		menu.add(_clearItem);
		menu.add(_closeItem);
	}

	final class SignatureField extends BitmapField {

		private Graphics _graphics;
		
		/**
		 * Constructor
		 */
		public SignatureField(Bitmap b) {
			this.setBitmap(b);
			_graphics = new Graphics(b);
		}

		/**
		 * Handle touch events
		 */
		/*protected boolean touchEvent(TouchEvent message) {
			try {
				if (message.getEvent() == TouchEvent.MOVE) { //Move event fired
					//Get the move points
					int pointsSize = message.getMovePointsSize();
					
					if (pointsSize > 1)
					{
						int[] xPoints = new int[pointsSize];
						int[] yPoints = new int[pointsSize];
						message.getMovePoints(1, xPoints, yPoints, null);
						drawPath(xPoints,yPoints);
					}
				} else if (message.getEvent() == TouchEvent.GESTURE) { //Gesture event fired
					TouchGesture gesture = message.getGesture();
					if (gesture.getEvent() == TouchGesture.TAP) { //Tap Gesture
						//Since we have a tap only draw a single point
						int xPoint = message.getX(1);
						int yPoint = message.getY(1);
						drawPoint(xPoint,yPoint);
					} else if (gesture.getEvent() == TouchGesture.SWIPE) { //Swipe Gesture
						//Get the move points
						int pointsSize = message.getMovePointsSize();
						int[] xPoints = new int[pointsSize];
						int[] yPoints = new int[pointsSize];
						message.getMovePoints(1, xPoints, yPoints, null);
						drawPath(xPoints,yPoints);
					}
				}
			} catch (Throwable e) {
				throw new RuntimeException(e.toString());
			}
			return true;
		}*/

		/**
		 * Draw a path through the set of points
		 */
		private void drawPath(int[] xPoints, int[] yPoints) {
			int oldColor = _graphics.getColor();
			//Draw a path through the points 
			_graphics.setColor(0x000000);
			_graphics.drawPathOutline(xPoints,yPoints, null, null, false);
			_graphics.setColor(oldColor);
			//Repaint
			invalidate();
		}
		
		/**
		 * Draw a point
		 */
		private void drawPoint(int xPoint, int yPoint) {
			int oldColor = _graphics.getColor();
			_graphics.setColor(0x000000);
			_graphics.drawPoint(xPoint, yPoint);
			_graphics.setColor(oldColor);
			//Repaint
			invalidate();
		}
		
		/**
		 * Clear the field
		 */
		private void clear() {
			int oldColor = _graphics.getColor();
			_graphics.setColor(0xFFFFFF);
			_graphics.fillRect(0, 0, _bitmap.getWidth(), _bitmap.getHeight());
			_graphics.setColor(oldColor);
			invalidate();
		}
	}
	
	public static void initMethods(RubyModule klass) {
		klass.getSingletonClass().defineMethod("take", new RubyTwoArgMethod() 
		{
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) 
			{
				try {
					final String strCallback = arg0.toStr();
					final RubyHash hashParams = (RubyHash)arg1;
					
					LOG.INFO("take: " + strCallback);
					
					
					UiApplication.getUiApplication().invokeLater( new Runnable()
			        {
			            public void run()
			            {
			                try
			                {
			                	SignatureCapture oSignature = new SignatureCapture(hashParams);
			                	UiApplication.getUiApplication().pushModalScreen(oSignature);
			                	
			                	com.rho.FilePath oFilePath = new com.rho.FilePath( oSignature.getFilePath() );
			                	String strStatus = oSignature.isCanceled() ? "" : ( !RhoFile.isFileExist(oSignature.getFilePath()) ? "Error" : "");
			                	
			                	RHODESAPP().callSignatureCallback( strCallback, oFilePath.getBaseName(), strStatus, oSignature.isCanceled() );
			                }catch(Exception exc)
			                {
			                	LOG.ERROR("Display signature screen failed.", exc);
			                }
			            }
			        });
					
					return RubyConstant.QNIL;
				} catch(Exception e) {
					LOG.ERROR("take failed", e);
						throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
	}
}
