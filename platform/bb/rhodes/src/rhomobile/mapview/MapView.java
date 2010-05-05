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
import com.xruby.runtime.lang.RubySymbol;
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
						RubyValue key = arKeys.get(i);
						RubyValue value = arValues.get(i);
						if (key == null || value == null)
							continue;
						String strKey = key.toString();
						if (strKey.equals("settings")) {
							if (!(value instanceof RubyHash))
								throw new RubyException(RubyRuntime.ArgumentErrorClass,
										"Wrong 'settings' type, should be Hash");
							settingsHash = (RubyHash)value;
						}
						else if (strKey.equals("annotations")) {
							if (!(value instanceof RubyArray))
								throw new RubyException(RubyRuntime.ArgumentErrorClass,
										"Wrong 'annotations' type, should be Array");
							annotationsArray = (RubyArray)value;
						}
					}
				}
				
				if (settings != null) {
					RubyArray arKeys = settingsHash.keys();
					RubyArray arValues = settingsHash.values();
					for (int i = 0; i != arKeys.size(); ++i) {
						RubyValue key = arKeys.get(i);
						RubyValue value = arValues.get(i);
						if (key == null || value == null)
							continue;
						String strKey = key.toString();
						if (strKey.equals("map_type")) {
							String strValue = value.toString();
							if (!strValue.equals("roadmap") && !strValue.equals("satellite")
									&& !strValue.equals("terrain") && !strValue.equals("hybrid"))
								throw new RubyException(RubyRuntime.ArgumentErrorClass,
										"Wrong 'map_type' value: " + strValue);
							settings.put(strKey, strValue);
						}
						else if (strKey.equals("zoom_enabled"))
							settings.put(strKey, new Boolean(value.toString().equalsIgnoreCase("true")));
						else if (strKey.equals("scroll_enabled"))
							settings.put(strKey, new Boolean(value.toString().equalsIgnoreCase("true")));
						else if (strKey.equals("shows_user_location"))
							settings.put(strKey, new Boolean(value.toString().equalsIgnoreCase("true")));
						else if (strKey.equals("region")) {
							if (value instanceof RubyArray) {
								RubyArray arr = (RubyArray)value;
								if (arr.size() == 4) {
									Hashtable region = new Hashtable();
									double[] cs = {0.0, 0.0, 0.0, 0.0};
									for (int k = 0; k != 4; ++k) {
										String v = arr.get(k).toString();
										try {
											cs[k] = Double.parseDouble(v);
										}
										catch (NumberFormatException e) {
											throw new RubyException(RubyRuntime.ArgumentErrorClass,
													"Wrong region value: " + v + ", should be Float");
										}
									}
									region.put("latitude", new Double(cs[0]));
									region.put("longitude", new Double(cs[1]));
									region.put("latDelta", new Double(cs[2]));
									region.put("lonDelta", new Double(cs[3]));
									settings.put(strKey, region);
								}
							}
							else if (value instanceof RubyHash) {
								RubyHash hsh = (RubyHash)value;
								RubyArray hKeys = hsh.keys();
								RubyArray hValues = hsh.values();
								for (int j = 0; j < hKeys.size(); ++j) {
									RubyValue hKey = hKeys.get(j);
									RubyValue hValue = hValues.get(j);
									if (hKey == null || hValue == null)
										continue;
									
									String strHKey = hKey.toString();
									if (strHKey.equals("center")) {
										Annotation ann = new Annotation();
										ann.type = "center";
										ann.street_address = hValue.toString();
										annotations.addElement(ann);
									}
									else if (strHKey.equals("radius")) {
										String strHValue = hValue.toString();
										try {
											double radius = Double.parseDouble(strHValue);
											settings.put("radius", new Double(radius));
										}
										catch (NumberFormatException e) {
											throw new RubyException(RubyRuntime.ArgumentErrorClass,
													"Wrong 'radius' parameter: " + strHValue + ", should be Float");
										}
									}
								}
							}
						}
					}
				}

				if (annotationsArray != null) {
					for (int i = 0; i != annotationsArray.size(); ++i) {
						Annotation annotation = new Annotation();
						annotation.type = "ann";
						RubyValue val = annotationsArray.get(i);
						if (!(val instanceof RubyHash))
							throw new RubyException(RubyRuntime.ArgumentErrorClass,
									"Wrong annotation value type, should be Hash");
						RubyHash ann = (RubyHash)val;
						RubyArray arKeys = ann.keys();
						RubyArray arValues = ann.values();
						for (int j = 0, lim = arKeys.size(); j < lim; ++j) {
							RubyValue key = arKeys.get(j);
							RubyValue value = arValues.get(j);
							if (key == null || value == null ||
									key.equals(RubyConstant.QNIL) ||
									value.equals(RubyConstant.QNIL))
								continue;
							String strKey = key.toString();
							String strValue = value.toString();
							if (strKey.equals("latitude")) {
								double v;
								try {
									v = Double.parseDouble(strValue);
								}
								catch (NumberFormatException e) {
									throw new RubyException(RubyRuntime.ArgumentErrorClass,
											"Wrong 'latitude' parameter: " + strValue + ", should be Float");
								}
								if (annotation.coordinates == null)
									annotation.coordinates = new Annotation.Coordinates(v, 0);
								else
									annotation.coordinates.latitude = v;
							}
							else if (strKey.equals("longitude")) {
								double v;
								try {
									v = Double.parseDouble(strValue);
								}
								catch (NumberFormatException e) {
									throw new RubyException(RubyRuntime.ArgumentErrorClass,
											"Wrong 'longitude' parameter: " + strValue + ", should be Float");
								}
								if (annotation.coordinates == null)
									annotation.coordinates = new Annotation.Coordinates(0, v);
								else
									annotation.coordinates.longitude = v;
							}
							else if (strKey.equals("title"))
								annotation.title = strValue;
							else if (strKey.equals("subtitle"))
								annotation.subtitle = strValue;
							else if (strKey.equals("street_address"))
								annotation.street_address = strValue;
							else if (strKey.equals("resolved_address"))
								annotation.resolved_address = strValue;
							else if (strKey.equals("url"))
								annotation.url = strValue;
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