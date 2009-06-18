package rhomobile.datetime;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

import rhomobile.RhodesApplication;
import net.rim.device.api.i18n.DateFormat;
import net.rim.device.api.io.http.HttpHeaders;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.FieldChangeListener;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.component.ButtonField;
import net.rim.device.api.ui.component.DateField;
import net.rim.device.api.ui.component.LabelField;
import net.rim.device.api.ui.component.SeparatorField;
import net.rim.device.api.ui.container.HorizontalFieldManager;
import net.rim.device.api.ui.container.MainScreen;
import net.rim.device.api.ui.container.VerticalFieldManager;

public class DateTimeScreen extends MainScreen {
	
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("DateTimePicker");
	
	private String _callbackUrl;
	
	/** A reference to the current screen for listeners. */
	private DateTimeScreen _dateTimeScreen;
	
	private DateField _dateTime;
	
	public DateTimeScreen(String callback, String title, long init, DateFormat fmt)
	{
		_callbackUrl = callback;
		//A reference to this object, to be used in listeners.
		_dateTimeScreen = this;
		
		setTitle( new LabelField( title, LabelField.ELLIPSIS | LabelField.USE_ALL_WIDTH ) );
		
		_dateTime = new DateField("", init, fmt);
		
		ButtonField okButton = new ButtonField("OK");
		okButton.setChangeListener(new OkListener());
		ButtonField cancelButton = new ButtonField("Cancel");
		cancelButton.setChangeListener(new CancelListener());
		
		HorizontalFieldManager hfm = new HorizontalFieldManager(Field.FIELD_HCENTER);
		hfm.add(okButton);
		hfm.add(cancelButton);
		
		VerticalFieldManager vfm = new VerticalFieldManager(Field.FIELD_VCENTER);
		vfm.add(_dateTime);
		vfm.add(new SeparatorField());
		vfm.add(hfm);
		
		add(vfm);
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
	
	private class OkListener implements FieldChangeListener
	{
		public void fieldChanged(Field field, int context)
		{
			RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
			
			HttpHeaders headers = new HttpHeaders();
    		headers.addProperty("Content-Type", "application/x-www-form-urlencoded");
    		
    		// We need to divide returned value to 1000 because we send number of seconds
    		// but returned value is actually number of milliseconds since Epoch.
    		String body = "status=ok&result=" + _dateTime.getDate()/1000;
    		LOG.INFO("Callback with result: " + body);
			app.postUrl(_callbackUrl, body, headers);
			app.popScreen( _dateTimeScreen );
		}
	}
	
	private class CancelListener implements FieldChangeListener
	{
		public void fieldChanged(Field field, int context)
		{
			RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
			
			HttpHeaders headers = new HttpHeaders();
			headers.addProperty("Content-Type", "application/x-www-form-urlencoded");
			
			String body = "status=fail";
			LOG.INFO("Callback with result: " + body);
			
			app.postUrl(_callbackUrl, body, headers);	
        	app.popScreen( _dateTimeScreen );
		}
	}
}
