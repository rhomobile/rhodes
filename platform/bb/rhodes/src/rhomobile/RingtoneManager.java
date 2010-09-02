package rhomobile;

import j2me.util.HashSet;
import j2me.util.Iterator;

import java.io.IOException;
import java.util.Enumeration;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;
import javax.microedition.media.Manager;
import javax.microedition.media.Player;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.*;

public class RingtoneManager extends RubyBasic {

	//private static final String mediaDirProperties[] = {
	//		"fileconn.dir.tones",
	//		"fileconn.dir.memorycard.tones",
	//		"fileconn.dir.music",
	//		"fileconn.dir.memorycard.music"
	//};
	
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RingtoneManager");
	
	private static Player p = null;
	
	public RingtoneManager(RubyClass c) {
		super(c);
	}
	
	public static void play(String url) {
		stop();
		
		try {
			p = Manager.createPlayer(url);
			p.realize();
			p.prefetch();
			p.start();
		} catch (Exception e) {
			stop();
			LOG.ERROR("RingtoneManager", e);
		}
	}
	
	public static void stop() {
		if (p != null) {
			try {
				p.stop();
			} catch (Exception e) {
				LOG.ERROR("RingtoneManager", e);
			}
			p = null;
		}
	}

	public static void initMethods(RubyModule klass) {
		klass.getSingletonClass().defineMethod("get_all_ringtones", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				HashSet mediaDirs = new HashSet();
				mediaDirs.add("file:///store/home/user/ringtones/");
				mediaDirs.add("file:///store/home/user/music/");
				mediaDirs.add("file:///SDCard/blackberry/ringones/");
				mediaDirs.add("file:///SDCard/blackberry/music/");
				RubyHash tmp = new RubyHash();
				Iterator it = mediaDirs.iterator();
				while (it.hasNext()) {
					try {
						String mediaDir = (String)it.next();
						FileConnection dir = (FileConnection)Connector.open(mediaDir);
						if(dir.exists() && dir.isDirectory()) {
							Enumeration list = dir.list();
							while(list.hasMoreElements()) {
								String s = (String)list.nextElement();
								String name = s;
								int idx = s.lastIndexOf('.');
								if (idx > 0)
									name = name.substring(0, idx);
								String fileName = mediaDir + s;
								LOG.TRACE("RingtoneManager: retrieved ringtone: " + name + " => " + fileName);
								tmp.setValue(new RubyString(name), new RubyString(new String(fileName)));
							}
						}
					} catch(IOException e) {
						LOG.ERROR("RingtoneManager", e);
					}
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
