package com.rhomobile.rhodes;

import android.content.Context;
import android.database.Cursor;
import android.media.Ringtone;
import android.net.Uri;
import android.util.Log;

import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.RubyBasic;
import com.xruby.runtime.lang.RubyBlock;
import com.xruby.runtime.lang.RubyClass;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyException;
import com.xruby.runtime.lang.RubyNoArgMethod;
import com.xruby.runtime.lang.RubyOneArgMethod;
import com.xruby.runtime.lang.RubyRuntime;
import com.xruby.runtime.lang.RubyValue;

public class RingtoneManager extends RubyBasic {

	private static android.media.RingtoneManager manager = null;
	private static Context context = null;
	private static Ringtone current = null;
	
	public RingtoneManager(RubyClass c) {
		super(c);
	}
	
	private static void init() {
		if (context == null)
			context = RhodesInstance.getInstance().getApplicationContext();
		if (manager == null)
			manager = new android.media.RingtoneManager(context);
	}
	
	public static void play(String url) {
		init();
		
		stop();
		
		Log.d("RingtoneManager", "play");
		current = android.media.RingtoneManager.getRingtone(context, Uri.parse(url));
		current.play();
	}
	
	public static void stop() {
		init();
		
		Log.d("RingtoneManager", "stop");
		
		if (current != null) {
			current.stop();
			current = null;
		}
	}
	
	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("get_all_ringtones", new RubyNoArgMethod() {
			@Override
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				init();
				
				RubyHash tmp = new RubyHash();
				
				Log.d("RingtoneManager", "Retrieve all ringtones");
				Cursor cursor = manager.getCursor();
				if (cursor.moveToFirst()) {
					for(int i = 0; i < cursor.getCount(); i++) {
						String name = manager.getRingtone(i).getTitle(context);
						String value = manager.getRingtoneUri(i).toString();
						Log.d("RingtoneManager", "Retrieved ringtone '" + name + "'");
						tmp.setValue(new RubyString(name), new RubyString(value));
					}
				}
				Log.d("RingtoneManager", "All ringtones retrieved");
				
				return tmp;
			}
		});
		
		klass.getSingletonClass().defineMethod("play", new RubyOneArgMethod() {
			@Override
			protected RubyValue run(RubyValue receiver, RubyValue arg,
					RubyBlock block) {
				try {
					String fullName = arg.toStr();
					play(fullName);
				} catch (Exception e) {
					throw new RubyException(RubyRuntime.MediaErrorClass,
							"in `" + this.getID() + "': " + e.getMessage());
				}
				
				return RubyConstant.QNIL;
			}
		});
		
		klass.getSingletonClass().defineMethod("stop", new RubyNoArgMethod() {
			@Override
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				try {
					stop();
				} catch (Exception e) {
					throw new RubyException(RubyRuntime.MediaErrorClass,
							"in `" + this.getID() + "': " + e.getMessage());
				}
				
				return RubyConstant.QNIL;
			}
		});
	}

}
