package rhomobile.camera;

import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyOneArgMethod;
import com.xruby.runtime.lang.RubyValue;

import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.container.MainScreen;

class CallCameraScreen implements Runnable {
	public static final int CAMERA_SCREEN = 1;
	public static final int IMAGE_BROWSER_SCREEN = 2;
	
	private String _callback;
	private int _screenType;
	
	CallCameraScreen(int screenType, String callback) {
		_screenType = screenType;
		_callback = callback;
	}
	public void run(){
		//Initialize the screen.
		MainScreen screen;
		if (_screenType == CAMERA_SCREEN)
        	screen = new CameraScreen(_callback);
		else
			screen = new ImageBrowserScreen(_callback);		
        //Push this screen to display it to the user.
        UiApplication.getUiApplication().pushScreen(screen);
	}
}

public class Camera extends RubyBasic {

	Camera(RubyClass c) {
		super(c);
	}
	
	public static RubyValue take_picture(RubyValue arg) {
		String callback = arg.toStr();
		
		UiApplication.getUiApplication().invokeLater(
				new CallCameraScreen(CallCameraScreen.CAMERA_SCREEN,callback));
        
		return RubyConstant.QNIL;
	}

	public static RubyValue choose_picture(RubyValue arg) {
		String callback = arg.toStr();

		UiApplication.getUiApplication().invokeLater(
				new CallCameraScreen(CallCameraScreen.IMAGE_BROWSER_SCREEN,callback));

		return RubyConstant.QNIL;
	}
	
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("take_picture", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				return Camera.take_picture(arg0);
			}
		});		
		klass.getSingletonClass().defineMethod("choose_picture", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) {
				return Camera.choose_picture(arg0);
			}
		});		
	}
}
