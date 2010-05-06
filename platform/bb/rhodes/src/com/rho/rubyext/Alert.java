package com.rho.rubyext;

import java.io.InputStream;
import java.util.Vector;

import javax.microedition.media.Player;

import rhomobile.RhodesApplication;

import net.rim.device.api.system.Application;
import net.rim.device.api.system.Bitmap;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.FieldChangeListener;
import net.rim.device.api.ui.Manager;
import net.rim.device.api.ui.component.BitmapField;
import net.rim.device.api.ui.component.ButtonField;
import net.rim.device.api.ui.component.Dialog;
import net.rim.device.api.ui.component.LabelField;
import net.rim.device.api.ui.container.HorizontalFieldManager;
import net.rim.device.api.ui.container.PopupScreen;
import net.rim.device.api.ui.container.VerticalFieldManager;

import com.rho.RhoClassFactory;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhodesApp;
import com.rho.file.SimpleFile;
import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;

public class Alert
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Alert");

	public static final RubyID titleID = RubyID.intern("title");
	public static final RubyID messageID = RubyID.intern("message");
	public static final RubyID iconID = RubyID.intern("icon");
	public static final RubyID callbackID = RubyID.intern("callback");
	public static final RubyID buttonsID = RubyID.intern("buttons");
	public static final RubyID buttonidID = RubyID.intern("id");
	
	public static final RubyID alertTypeID = RubyID.intern("alert");
	public static final RubyID questionTypeID = RubyID.intern("question");
	public static final RubyID infoTypeID = RubyID.intern("info");
	
    public static void showPopup(final String message) 
    {
    	Application.getApplication().invokeLater(new Runnable() {
            public void run() {    	
            	Application.getApplication().requestForeground();
            	Dialog.alert(message);
            }
    	});
    }

    private static String getHashStringValue(RubyHash hash, RubyID id)
    {
    	RubyValue val = hash.get(id.toSymbol());
    	
    	return val != null && val != RubyConstant.QNIL ? val.toStr() : ""; 
    }
    
    private static class AlertDialog extends PopupScreen {
    	
    	private static AlertDialog current = null;
    	
    	private HorizontalFieldManager hfm;
    	private String callback = null;
    	
    	public AlertDialog(String title, String msg, String icon, String c) {
    		super(new VerticalFieldManager(Manager.NO_VERTICAL_SCROLL | Manager.NO_VERTICAL_SCROLLBAR));
    		
    		callback = c;
    		
    		Manager mgr = new VerticalFieldManager(Manager.NO_VERTICAL_SCROLL | Manager.NO_VERTICAL_SCROLLBAR);
    		add(mgr);
    		
    		if (title != null) {
    			LabelField tf = new LabelField(title, Field.FIELD_HCENTER);
    			mgr.add(tf);
    		}
    		
    		HorizontalFieldManager lfm = new HorizontalFieldManager();
    		mgr.add(lfm);

    		Bitmap bmp = null;
    		if (icon != null) {
    			if (icon.equalsIgnoreCase(alertTypeID.toString()))
    				bmp = Bitmap.getPredefinedBitmap(Bitmap.EXCLAMATION);
    			else if (icon.equalsIgnoreCase(questionTypeID.toString()))
    				bmp = Bitmap.getPredefinedBitmap(Bitmap.QUESTION);
    			else if (icon.equalsIgnoreCase(infoTypeID.toString()))
    				bmp = Bitmap.getPredefinedBitmap(Bitmap.INFORMATION);
    			// TODO: load bitmap from file
    		}
			if (bmp != null) {
				BitmapField bf = new BitmapField(bmp, Field.FIELD_LEFT | Field.FIELD_VCENTER);
				lfm.add(bf);
			}

    		LabelField lf = new LabelField(msg, Field.FIELD_HCENTER | Field.FIELD_VCENTER);
    		lfm.add(lf);
    		
    		hfm = new HorizontalFieldManager();
    		mgr.add(hfm);
    	}
    	
    	private class Callback implements FieldChangeListener {

    		private String id;
    		private String text;
    		
    		public Callback(String i, String t) {
    			id = i;
    			text = t;
    		}
    		
			public void fieldChanged(Field field, int context) {
				try {
					RhodesApp.getInstance().callPopupCallback(callback, id, text);
				} catch (Exception e) {
					LOG.ERROR("show_popup callback failed.", e);
				}
				hide();
			}
    		
    	};
    	
    	public void addButton(String id, String text) {
    		ButtonField bf = new ButtonField(text);
    		bf.setChangeListener(new Callback(id, text));
    		hfm.add(bf);
    	}
    	
    	public void show() {
    		RhodesApplication app = (RhodesApplication)Application.getApplication();
    		app.requestForeground();
    		app.pushScreen(this);
    		current = this;
    	}
    	
    	public void hide() {
    		RhodesApplication app = (RhodesApplication)Application.getApplication();
    		app.popScreen(this);
    		current = null;
    	}
    	
    	private static void hidePopup() {
    		Application.getApplication().invokeLater(new Runnable() {
    			public void run() {
    				if (current == null)
    	    			return;
    				current.hide();
    				current = null;
    			}
    		});
    	}
    };
    
    private static class PopupHandler implements Runnable
    {
    	//String m_strMessage = "";
    	//String m_strCallback = "";
    	//Vector m_vecButtons = new Vector();
    	//Vector m_vecIDs = new Vector();
    	//private static final int atAlert = 1;
    	//private static final int atQuestion = 2;
    	//private static final int atInfo = 3;
    	//int      m_nType = 0;
    	
    	private AlertDialog dialog;
    	
    	PopupHandler(RubyHash hash)
    	{
    		String title = getHashStringValue(hash, titleID );
    		String msg = getHashStringValue(hash, messageID );
    		String icon = getHashStringValue(hash, iconID );
    		String callback = getHashStringValue(hash, callbackID );
    		dialog = new AlertDialog(title, msg, icon, callback);
    		
    		RubyValue valButtons = hash.get(buttonsID.toSymbol());
    		if ( valButtons != null && valButtons instanceof RubyArray )
    		{
    			RubyArray arButtons = (RubyArray)valButtons;
    			for ( int i = 0; i < arButtons.size(); i++ )
    			{
    				RubyValue valButton = arButtons.get(i);
    				if ( valButton != null && valButton instanceof RubyString )
    				{
    					String text = valButton.toStr();
    					dialog.addButton(text, text);
    				}else if ( valButton != null && valButton instanceof RubyHash )
    				{
    					String strLabel = getHashStringValue((RubyHash)valButton, titleID );
    					if ( strLabel.length() > 0 )
    					{
    						String strID = getHashStringValue((RubyHash)valButton, buttonidID );
    						if ( strID.length() > 0 )
    							dialog.addButton(strID, strLabel);
    						else
    							dialog.addButton(strLabel, strLabel);
    					}
    				}
    			}
    		}
    		
    		/*
    		RubyValue valIcon = hash.get(iconID.toSymbol());
    		if ( valIcon instanceof RubySymbol )
    		{
    			if ( valIcon == alertTypeID.toSymbol() )
    				m_nType = atAlert;
    			else if( valIcon == questionTypeID.toSymbol() )
    				m_nType = atQuestion;
    			else if( valIcon == infoTypeID.toSymbol() )
    				m_nType = atInfo;
    			
    		}else
    		{
    			if (m_vecButtons.size() > 0 )
    				m_nType = atQuestion;
    			else
    				m_nType = atInfo;
    		}
    		*/
    	}
    	
        public void run() {
        	dialog.show();
        	/*
        	Application.getApplication().requestForeground();
        	
        	switch( m_nType )
        	{
        	case atAlert:
        		Dialog.alert(m_strMessage);
        		break;
        	case atQuestion:
        		Object[] btns = new Object[m_vecButtons.size()];
        		m_vecButtons.copyInto(btns);
            	int nRes = Dialog.ask(m_strMessage,btns,0);
            	
            	try
            	{
	            	RhodesApp.getInstance().callPopupCallback(m_strCallback, (String)m_vecIDs.elementAt(nRes), 
	            			(String)m_vecButtons.elementAt(nRes));
            	}catch(Exception exc)
            	{
            		LOG.ERROR("show_popup callback failed.", exc);
            	}
        		break;
        	case atInfo:
            	Dialog.inform(m_strMessage);
        		break;
        		
        	}
        	*/
        }
    	
    }
    
    public static void vibrate(final String duration) 
    {
    	Application.getApplication().invokeLater(new Runnable() {
            public void run() {    	
		    	int dt = 2500;
		    	try {
		    		dt = Integer.parseInt(duration);
		    	} catch (NumberFormatException e) {    		
		    	}
		    	
		    	if (dt > 25500) dt = 25500;
		    	
		    	if (dt > 0) {
		    		net.rim.device.api.system.Alert.startVibrate(dt);
		    	}
            }
    	});
    }
	
    private static final String[][] filetypes = { {"mp3", "audio/mpeg"}, {"wav","audio/x-wav"} };
    private static String getTypeFromExt(String file_name) {
    	int pt = file_name.lastIndexOf('.');
    	if (pt<0) {
    		return filetypes[0][1];
    	}
    	String ext = file_name.substring(pt+1);
    	for (int cnt = filetypes.length - 1; cnt >= 0; --cnt) {
    		if(filetypes[cnt][0].equals(ext)) {
    			return filetypes[cnt][1];
    		}
    	}
    	return null;
    }
	
    public static void play_file(final String file_name, final String media_type) 
    {
    	Application.getApplication().invokeLater(new Runnable() 
    	{
            public void run() {            	
            	String type = media_type == null ? getTypeFromExt(file_name) : media_type;
            	if (type != null) {
            		LOG.INFO("File type: " + type);
            	} else {
            		LOG.ERROR("Error - can't play unknown file type");
            		return;
            	}
            	
            	String types[] = 
            		javax.microedition.media.Manager.getSupportedContentTypes(null);
            	for (int cnt = types.length - 1; cnt >= 0; --cnt) {
            		if (type.equals(types[cnt])) {
            			LOG.INFO( "Playing file " + file_name + " of type: " + types[cnt]);
            			
            			SimpleFile file = null;
            			try {
            				//retrieve the file
            				Class clazz = Class.forName("rhomobile.RhodesApplication");
            				file = RhoClassFactory.createFile();
            				String strClassName = file_name;
            				if ( !strClassName.startsWith("/apps") )
            					strClassName = "/apps" + file_name;
            				
            				InputStream is = file.getResourceAsStream(clazz.getClass(), strClassName);
            				//create an instance of the player from the InputStream
            				Player player = javax.microedition.media.Manager.createPlayer(is,type);
            				player.realize();
            				player.prefetch();
            				//start the player
            				player.start();
            	        } catch (Exception ex) { 
            	        	LOG.ERROR("Error playing " + file_name + " :" + ex.getMessage());
                        } finally {
                			try{
                				if ( file != null )
                					file.close();
                			}catch(Exception exc){}         	
                        }
            			return;
            		}
            	}
            	
            	LOG.ERROR("Error - media type " + type + " isn't supported.");
            }
    	});
    }
	
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("show_popup", new RubyOneArgMethod() 
		{
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) 
			{
				try {
					if ( arg0 instanceof RubyString )
					{
						String message = arg0.toString();
						Alert.showPopup(message);
					}else if ( arg0 instanceof RubyHash)
					{
					    	PopupHandler handler = new PopupHandler((RubyHash)arg0);
					    	Application.getApplication().invokeLater(handler);
					}
					else
						throw new RubyException(RubyRuntime.ArgumentErrorClass, "in Alert.show_popup: wrong argument type.Should be String or Hash");
					
					return RubyConstant.QNIL;
				} catch(Exception e) {
					LOG.ERROR("show_popup failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("hide_popup", new RubyNoArgMethod() {

			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try {
					AlertDialog.hidePopup();
					return RubyConstant.QNIL;
				}
				catch (Exception e) {
					LOG.ERROR("hide_popup failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
			
		});
		klass.getSingletonClass().defineMethod("vibrate", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) 
			{
				if ( args != null && args.size() > 1 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in Alert.vibrate: wrong number of arguments ( " + args.size() + " for " + 1 + " )");			
				
				try {
					String duration = "2500";
					if ((args != null) && (args.size() > 0))
						duration = args.get(0).toString();
					Alert.vibrate(duration);
					
					return RubyConstant.QNIL;
				} catch(Exception e) {
					LOG.ERROR("vibrate failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
		klass.getSingletonClass().defineMethod("play_file", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) 
			{
				if ( args.size() < 1 || args.size() > 2 )
					throw new RubyException(RubyRuntime.ArgumentErrorClass, 
							"in Alert.play_file: wrong number of arguments ( " + args.size() + " for " + 2 + " )");			
				
				try {
					String file_name = args.get(0).toString();
					String media_type = null;
					if ((args.size() > 1) && (args.get(1) != RubyConstant.QNIL))
						media_type = args.get(1).toString();
					Alert.play_file(file_name,media_type);
					
					return RubyConstant.QNIL;
				} catch(Exception e) {
					LOG.ERROR("play_file failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
	}
	
}