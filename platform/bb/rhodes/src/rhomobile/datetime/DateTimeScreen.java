package rhomobile.datetime;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

import rhomobile.Callback;
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
	private String _opaque;
	
	/** A reference to the current screen for listeners. */
	private DateTimeScreen _dateTimeScreen;
	
	private DateField _dateTimeField;
	private ButtonField _okButton;
	private ButtonField _cancelButton;
	
	public DateTimeScreen(String callback, String title, long init, DateFormat fmt, String opaque)
	{
		_callbackUrl = callback;
		_opaque = opaque;
		//A reference to this object, to be used in listeners.
		_dateTimeScreen = this;
		
		setTitle( new LabelField( title, LabelField.ELLIPSIS | LabelField.USE_ALL_WIDTH ) );
		
		_dateTimeField = new DateField("", init, fmt);
		
		_okButton = new ButtonField("OK");
		_okButton.setChangeListener(new OkListener());
		_cancelButton = new ButtonField("Cancel");
		_cancelButton.setChangeListener(new CancelListener());
		
		HorizontalFieldManager hfm = new HorizontalFieldManager(Field.FIELD_HCENTER);
		hfm.add(_okButton);
		hfm.add(_cancelButton);
		
		VerticalFieldManager vfm = new VerticalFieldManager(Field.FIELD_VCENTER);
		vfm.add(_dateTimeField);
		vfm.add(new SeparatorField());
		vfm.add(hfm);
		
		add(vfm);
	}
	
	private void setFieldsEditable(boolean v) {
		_okButton.setEditable(v);
		_cancelButton.setEditable(v);
		_dateTimeField.setEditable(v);
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
	
	private class RemoveScreen implements Callback
	{
		private RhodesApplication _app = null;
		private DateTimeScreen _screen = null;
		
		public RemoveScreen(RhodesApplication app, DateTimeScreen screen) {
			_app = app;
			_screen = screen;
		}
		
		public void run() {
			synchronized ( RhodesApplication.getEventLock() ) {
				_app.popScreen( _screen );
			}
		}
	};
	
	private class OkListener implements FieldChangeListener
	{
		public void fieldChanged(Field field, int context)
		{
			RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
			
			HttpHeaders headers = new HttpHeaders();
    		headers.addProperty("Content-Type", "application/x-www-form-urlencoded");
    		
    		// We need to divide returned value to 1000 because we send number of seconds
    		// but returned value is actually number of milliseconds since Epoch.
    		String body = "status=ok&result=" + _dateTimeField.getDate()/1000;
    		if (_opaque != null)
    			body += "&opaque=" + _opaque;
    		body += "&rho_callback=1";
    		
    		LOG.INFO("Callback with result: " + body);
    		
    		_dateTimeScreen.setFieldsEditable(false);
			app.postUrl(_callbackUrl, body, headers, new RemoveScreen(app, _dateTimeScreen));
		}
	}
	
	private class CancelListener implements FieldChangeListener
	{
		public void fieldChanged(Field field, int context)
		{
			RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
			
			HttpHeaders headers = new HttpHeaders();
			headers.addProperty("Content-Type", "application/x-www-form-urlencoded");
			
			String body = "status=cancel";
    		if (_opaque != null)
    			body += "&opaque=" + _opaque;
    		body += "&rho_callback=1";
    		
			LOG.INFO("Callback with result: " + body);
			
			_dateTimeScreen.setFieldsEditable(false);
			app.postUrl(_callbackUrl, body, headers, new RemoveScreen(app, _dateTimeScreen));
		}
	}
}
