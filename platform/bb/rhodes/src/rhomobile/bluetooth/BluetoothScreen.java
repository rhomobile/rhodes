package rhomobile.bluetooth;

import net.rim.device.api.ui.DrawStyle;
import net.rim.device.api.ui.Field;
import net.rim.device.api.ui.component.LabelField;
import net.rim.device.api.ui.component.SeparatorField;
import net.rim.device.api.ui.container.MainScreen;

public class BluetoothScreen extends MainScreen {
	
	public static int BLUETOOTH_SCREEN_TYPE_CLIENT = 1;
	public static int BLUETOOTH_SCREEN_TYPE_SERVER = 2;
	public static int BLUETOOTH_SCREEN_TYPE_UNSUPPORTED = 3;

	public interface BluetoothScreenCallback {
		void onBluetoothScreenCancel();
		void onBluetoothScreenSelect(int index);
	}
	
	private static String TITLE = "Bluetooth";
	private static String SELECT_DEVICE = "Please select device to connect :";
	private static String WAIT_FOR_CONNECT = "Waiting for a connection ...";
	private static String NO_DEVICES = "There are no paired devices for connect !";
	private static String UNSUPPORTED = "Bluetooth is not supported on this BlackBerry or simulator.";
		

	private BluetoothScreenCallback mCallback;
	private LabelField[] mFields;
	private boolean mIsClosedBySelect = false;
	
	
	public BluetoothScreen(int type, String[] devices, BluetoothScreenCallback callback) {
		mCallback = callback;
		mFields = new LabelField[0];
		setTitle(TITLE);
		if (type == BLUETOOTH_SCREEN_TYPE_UNSUPPORTED) {
			add(new LabelField(UNSUPPORTED));
			return;
		}
		if (type == BLUETOOTH_SCREEN_TYPE_SERVER) {
			add(new LabelField(WAIT_FOR_CONNECT));
			return;
		}
		if (type == BLUETOOTH_SCREEN_TYPE_CLIENT) {
			if (devices.length <= 0) {
				add(new LabelField(NO_DEVICES));
				return;
			}

			add(new LabelField(SELECT_DEVICE));
			add(new SeparatorField());
			int i;
			mFields = new LabelField[devices.length];
			for (i = 0; i < devices.length; i++) {
				LabelField lg = new LabelField(devices[i], Field.FOCUSABLE | Field.USE_ALL_WIDTH);
				mFields[i] = lg;
				add(lg);
			}
			add(new SeparatorField());
		}
	}
	
    /**
     * Overrides Screen.invokeAction().  Handles a trackball click and provides 
     * identical behavior to an ENTER keypress event.
     * 
     * @see net.rim.device.api.ui#invokeAction(int)
     */
    public boolean invokeAction(int action)
    {        
        switch(action)
        {
            case ACTION_INVOKE: // Trackball click.
            	int i;
            	for (i = 0; i < mFields.length; i++) {
            		if (mFields[i].isFocus()) {
            			if (mCallback != null) {
            				mCallback.onBluetoothScreenSelect(i);
            				mIsClosedBySelect = true;
            				BluetoothManager.rhoPopScreen(this);
            			}
            			return true; // We've consumed the event.
            		}
            	}
        }   
        return  super.invokeAction(action);
    }
	
    /**
     * Called when the main screen (and thus, the application) closes.  Removes
     * this screen as a low memory listener, and commits the order list to the
     * persistent store.
     * 
     * @see net.rim.device.api.ui.Screen#onClose()
     */
    public boolean onClose() 
    {
		if (mCallback != null) {
			if (!mIsClosedBySelect) {
				mCallback.onBluetoothScreenCancel();
			}
		}
		mIsClosedBySelect = false;
        return super.onClose();
    }
	

}
