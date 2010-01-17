package rhomobile.mapview;

import java.util.Hashtable;
import java.util.Vector;

import net.rim.device.api.ui.UiApplication;
import rhomobile.datetime.DateTimeScreen;

import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyOneArgMethod;
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
		private Hashtable _settings;
		private Vector _annotations;
		
		private static MapViewScreen screen = null;
		
		public CallMapViewScreen(String provider, Hashtable settings, Vector annotations) {
			_provider = provider;
			_settings = settings;
			_annotations = annotations;
		}

		public void run() {
			if (screen != null)
				return;
			//Initialize the screen.
			screen = new MapViewScreen(_provider, _settings, _annotations);
	        UiApplication.getUiApplication().pushModalScreen(screen);
	        screen = null;
		}
		
	};
	
	private static void show(String provider, Hashtable settings, Vector annotations) {
		UiApplication.getUiApplication().invokeLater(new CallMapViewScreen(provider, settings, annotations));
	}

	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("create", new RubyOneArgMethod() {

			protected RubyValue run(RubyValue receiver, RubyValue arg,
					RubyBlock block) {
				
				RubyHash settingsHash = null;
				RubyArray annotationsArray = null;
				
				Hashtable settings = new Hashtable();
				Vector annotations = new Vector();
				
				RubyHash hash = (RubyHash)arg;
				if (hash != null) {
					RubyArray arKeys = hash.keys();
					RubyArray arValues = hash.values();
					for (int i = 0; i != arKeys.size(); ++i) {
						String strKey = arKeys.get(i).toString();
						if (strKey.equals("settings"))
							settingsHash = (RubyHash)arValues.get(i);
						else if (strKey.equals("annotations"))
							annotationsArray = (RubyArray)arValues.get(i);
					}
				}
				
				if (settings != null) {
					RubyArray arKeys = settingsHash.keys();
					RubyArray arValues = settingsHash.values();
					for (int i = 0; i != arKeys.size(); ++i) {
						String strKey = arKeys.get(i).toString();
						RubyValue value = arValues.get(i);
						if (strKey.equals("map_type"))
							settings.put(strKey, value.toString());
						else if (strKey.equals("zoom_enabled"))
							settings.put(strKey, new Boolean(((RubyConstant)value).isTrue()));
						else if (strKey.equals("scroll_enabled"))
							settings.put(strKey, new Boolean(((RubyConstant)value).isTrue()));
						else if (strKey.equals("shows_user_location"))
							settings.put(strKey, new Boolean(((RubyConstant)value).isTrue()));
						else if (strKey.equals("region")) {
							RubyArray arr = (RubyArray)value;
							if (arr.size() == 4) {
								Hashtable region = new Hashtable();
								region.put("latitude", new Double(arr.get(0).toFloat()));
								region.put("longitude", new Double(arr.get(1).toFloat()));
								region.put("latDelta", new Double(arr.get(2).toFloat()));
								region.put("lonDelta", new Double(arr.get(3).toFloat()));
								settings.put(strKey, region);
							}
						}
					}
				}

				if (annotationsArray != null) {
					for (int i = 0; i != annotationsArray.size(); ++i) {
						Annotation annotation = new Annotation();
						RubyHash ann = (RubyHash)annotationsArray.get(i);
						RubyArray arKeys = ann.keys();
						RubyArray arValues = ann.values();
						for (int j = 0, lim = arKeys.size(); j < lim; ++j) {
							String strKey = arKeys.get(j).toString();
							RubyValue value = arValues.get(j);
							if (strKey.equals("latitude")) {
								if (annotation.coordinates == null)
									annotation.coordinates = new Annotation.Coordinates(value.toFloat(), 0);
								else
									annotation.coordinates.latitude = value.toFloat();
							}
							else if (strKey.equals("longitude")) {
								if (annotation.coordinates == null)
									annotation.coordinates = new Annotation.Coordinates(0, value.toFloat());
								else
									annotation.coordinates.longitude = value.toFloat();
							}
							else if (strKey.equals("title"))
								annotation.title = value.toString();
							else if (strKey.equals("subtitle"))
								annotation.subtitle = value.toString();
							else if (strKey.equals("street_address"))
								annotation.street_address = value.toString();
							else if (strKey.equals("resolved_address"))
								annotation.resolved_address = value.toString();
							else if (strKey.equals("url"))
								annotation.url = value.toString();
						}
						annotations.addElement(annotation);
					}
				}
				
				MapView.show("google", settings, annotations);
				
				return RubyConstant.QNIL;
			}
			
		});
	}
	
}