package rhomobile;

import java.io.IOException;
import java.util.Enumeration;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;
import javax.microedition.media.Manager;
import javax.microedition.media.MediaException;
import javax.microedition.media.Player;

import net.rim.device.api.i18n.DateFormat;
import net.rim.device.api.i18n.SimpleDateFormat;
import rhomobile.datetime.DateTimePicker;

import com.xruby.runtime.builtin.RubyArray;
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

	private static final String mediaDirs[] = {
			System.getProperty("fileconn.dir.tones"),
			System.getProperty("fileconn.dir.memorycard.tones"),
			System.getProperty("fileconn.dir.music"),
			System.getProperty("fileconn.dir.memorycard.music")
	};
	
	private static Player p = null;
	
	public RingtoneManager(RubyClass c) {
		super(c);
	}
	
	public static void play(String url) throws MediaException, IOException {
		stop();
		
		p = Manager.createPlayer(url);
		p.realize();
		p.prefetch();
		p.start();
	}
	
	public static void stop() throws MediaException {
		if (p != null) {
			p.stop();
			p = null;
		}
	}

	public static void initMethods(RubyClass klass) {
		klass.getSingletonClass().defineMethod("get_all_ringtones", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				RubyHash tmp = new RubyHash();
				for(int i = 0; i < mediaDirs.length; i++) {
					try {
						FileConnection dir = (FileConnection)Connector.open(mediaDirs[i], Connector.READ);
						if(dir.exists() && dir.isDirectory()) {
							Enumeration list = dir.list();
							while(list.hasMoreElements()) {
								String s = (String)list.nextElement();
								String name = s;
								int idx = s.lastIndexOf('.');
								if (idx > 0)
									name = name.substring(0, idx);
								tmp.setValue(new RubyString(name), new RubyString(new String(mediaDirs[i] + s)));
							}
						}
					} catch(IOException e) {}
				}
				
				return tmp;
			}
		});
		
		klass.getSingletonClass().defineMethod("play", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg,
					RubyBlock block) {
				String fullName = arg.toStr();
				
				try {
					play(fullName);
				} catch(Exception e) {
					throw new RubyException(RubyRuntime.MediaErrorClass,
							"in `" + this.getID() + "': " + e.getMessage());
				}
					
				return RubyConstant.QNIL;
			}
		});
		
		klass.getSingletonClass().defineMethod("stop", new RubyNoArgMethod() {
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
