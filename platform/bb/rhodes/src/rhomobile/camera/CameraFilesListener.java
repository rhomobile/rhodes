package rhomobile.camera;

import java.io.IOException;
import java.util.Enumeration;
import java.util.Hashtable;

import javax.microedition.io.Connector;
import javax.microedition.io.file.FileConnection;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;

import net.rim.device.api.io.file.FileSystemJournal;
import net.rim.device.api.io.file.FileSystemJournalEntry;
import net.rim.device.api.io.file.FileSystemJournalListener;

public class CameraFilesListener implements FileSystemJournalListener {
	
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("CameraFilesListener");
	
	private long _lastUSN = 0;
	
	private CameraScreen _screen;
	
	private Hashtable _exists;
	
	private final String[] _folders = {
			"file:///store/home/user/pictures/",
			"file:///sdcard/blackberry/pictures/"
	};
	
	public CameraFilesListener(CameraScreen screen) {
		_screen = screen;
		_exists = new Hashtable();
		
		for(int i = 0; i < _folders.length; i++) {
			try {
				FileConnection fconn = (FileConnection)Connector.open(_folders[i], Connector.READ);
				if (!fconn.isDirectory())
					continue;
				Enumeration e = fconn.list();
				while(e.hasMoreElements()) {
					_exists.put(e.nextElement(), new Boolean(true));
				}
			} catch (IOException e) {
				LOG.ERROR("CameraFilesListener: " + _folders[i], e);
				continue;
			}
		}
	}

	public void fileJournalChanged() {
		long nextUSN = FileSystemJournal.getNextUSN();
        String msg = null;
        for (long lookUSN = nextUSN - 1; lookUSN >= _lastUSN && msg == null; --lookUSN) {
            FileSystemJournalEntry entry = FileSystemJournal.getEntry(lookUSN);
            if (entry == null) { // we didn't find an entry.
                break;
            }

            //check if this entry was added or deleted
            String path = entry.getPath();
            if (path == null)
            	continue;
            if (!path.startsWith("file://"))
            	path = "file://" + path;
            
        	int event = entry.getEvent();
        	LOG.TRACE("event: " + Integer.toString(event));
        	if (event != FileSystemJournalEntry.FILE_ADDED)
        		continue;
        	
        	String lpath = path.toLowerCase();
        	if (!lpath.startsWith("file:///sdcard/blackberry/pictures/") &&
        			!lpath.startsWith("file:///store/home/user/pictures/"))
        		continue;
        	
        	if (lpath.endsWith(".dat"))
        		continue;
        	
        	if (_exists.get(path) != null)
    			continue;
        	
        	if (_screen != null) {
				try {
					FileConnection fconn = (FileConnection)Connector.open(path, Connector.READ);
					boolean exists = fconn.exists();
					fconn.close();
					if (!exists)
	        			continue;
				} catch (IOException e) {
					continue;
				}
				_exists.put(path, new Boolean(true));
        		_screen.invokeCloseScreen(true, path);
        		_screen = null;
        	}
        }
        
        _lastUSN = nextUSN;
	}

}
