package rhomobile.mapview;

import net.rim.device.api.ui.UiApplication;
import rhomobile.datetime.DateTimeScreen;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyTwoArgMethod;
import com.xruby.runtime.lang.RubyValue;
import com.xruby.runtime.lang.RubyVarArgMethod;

public class MapView extends RubyBasic {

	public MapView(RubyClass c) {
		super(c);
	}
	
	private static class CallMapViewScreen implements Runnable {

		private String _provider;
		private double _lat;
		private double _lon;
		private int _zoom;
		
		private static MapViewScreen screen = null;
		
		public CallMapViewScreen(String provider, double lat, double lon, int zoom) {
			_provider = provider;
			_lat = lat;
			_lon = lon;
			_zoom = zoom;
		}

		public void run() {
			if (screen != null)
				return;
			//Initialize the screen.
			screen = new MapViewScreen(_provider, _lat, _lon, _zoom);
	        UiApplication.getUiApplication().pushModalScreen(screen);
	        screen = null;
		}
		
	};
	
	private static void show(String provider, double lat, double lon, int zoom) {
		UiApplication.getUiApplication().invokeLater(new CallMapViewScreen(provider, lat, lon, zoom));
	}

	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("create", new RubyVarArgMethod() {

			protected RubyValue run(RubyValue receiver, RubyArray args, RubyBlock block) {

				final int ARGS = 3;
				if(args.size() != ARGS)
					throw new RubyException(RubyRuntime.ArgumentErrorClass,
							"in `" + this.getID() + "': wrong number of arguments (" + args.size() + " instead of " + ARGS + ")");
				
				double lat = args.get(0).toFloat();
				double lon = args.get(1).toFloat();
				int zoom = args.get(2).toInt();
				
				//String provider = "blackberry";
				String provider = "google";
				//String provider = "nonexistent";
				MapView.show(provider, lat, lon, zoom);
				
				return RubyConstant.QNIL;
			}
			
		});
	}
	
}