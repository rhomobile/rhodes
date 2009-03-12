package rhomobile.camera;

import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyOneArgMethod;
import com.xruby.runtime.lang.RubyValue;

import net.rim.device.api.ui.UiApplication;

public class Camera extends RubyBasic {

	Camera(RubyClass c) {
		super(c);
	}

	public static RubyValue take_picture(RubyValue arg) {
		String callback = arg.toStr();
		
		System.out.println("Calling take_picture");
		System.out.println("Callback: " + callback);
		
		UiApplication.getUiApplication().invokeLater(new Runnable(){
			public void run(){
				//Initialize the screen.
		        CameraScreen cameraScreen = new CameraScreen();
		        //Push this screen to display it to the user.
		        UiApplication.getUiApplication().pushScreen( cameraScreen );
			}
		});
        
		return RubyConstant.QNIL;
	}

	public static RubyValue choose_picture(RubyValue arg) {
//		String callback = arg.toStr();
		System.out.println("Calling choose_picture");
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
