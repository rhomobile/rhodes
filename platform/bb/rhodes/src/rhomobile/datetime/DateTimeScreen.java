package rhomobile.datetime;

import rhomobile.RhodesApplication;
import net.rim.device.api.i18n.DateFormat;
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
	
	/** A reference to the current screen for listeners. */
	private DateTimeScreen _dateTimeScreen;
	
	private DateField _dateTime;
	
	public Long retvalue;
	
	public DateTimeScreen(String title, long init)
	{
		//A reference to this object, to be used in listeners.
		_dateTimeScreen = this;
		
		retvalue = null;

		setTitle( new LabelField( title, LabelField.ELLIPSIS | LabelField.USE_ALL_WIDTH ) );
		//LabelField title = new LabelField( "Choose date/time", LabelField.ELLIPSIS | LabelField.USE_ALL_WIDTH );
		//add(title);
		
		_dateTime = new DateField("", init, DateFormat.DATE_DEFAULT);
		
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
        	retvalue = new Long(_dateTime.getDate());
        	app.popScreen( _dateTimeScreen );
		}
	}
	
	private class CancelListener implements FieldChangeListener
	{
		public void fieldChanged(Field field, int context)
		{
        	RhodesApplication app = (RhodesApplication)UiApplication.getUiApplication();
        	retvalue = null;
			app.popScreen( _dateTimeScreen );
		}
	}
}
