package rhomobile.camera;

import java.io.IOException;

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
	
	public CameraFilesListener(CameraScreen screen) {
		_screen = screen;
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
        		_screen.closeScreen(true, path);
        		_screen = null;
        	}
        }
        
        _lastUSN = nextUSN;
	}

}
