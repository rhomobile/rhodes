package rhomobile;

import net.rim.device.api.ui.MenuItem;
import net.rim.device.api.ui.UiApplication;
import net.rim.device.api.ui.component.LabelField;
import net.rim.device.api.ui.container.MainScreen;
import net.rim.device.api.ui.component.RichTextField;
import net.rim.device.api.ui.container.HorizontalFieldManager;
//import net.rim.device.api.ui.container.VerticalFieldManager;
//import net.rim.device.api.ui.component.RichTextField;
//import net.rim.device.api.ui.Manager;
import com.rho.RhoLogger;

public class LogScreen extends MainScreen {

	private int m_nCurLine = 0;
	private java.util.Timer m_timer = new java.util.Timer();
	private MyEdit m_logText;
	
	private MenuItem optionsItem = new MenuItem("Options", 100000, 10) {
		public void run() {
			LogOptionsScreen screen = new LogOptionsScreen();
	        UiApplication.getUiApplication().pushScreen(screen);
		}
	};
	private MenuItem clearItem = new MenuItem("Clear", 100000, 10) {
		public void run() {
			RhoLogger.clearLog();
			loadLogText();
			trackwheelRoll(m_nCurLine, 0, 0);
		}
	};
	private MenuItem refreshItem = new MenuItem("Refresh", 100000, 10) {
		public void run() {
			loadLogText();
			trackwheelRoll(m_nCurLine, 0, 0);
		}
	};
	private MenuItem sendItem = new MenuItem("Send", 100000, 10) {
		public void run() {
			try{
				com.rho.RhoConf.sendLog();
			}catch(Exception exc)
			{
			}
		}
	};
	private MenuItem copyLogItem = new MenuItem("Copy to sdcard", 100000, 10) {
		public void run() {
			com.rho.file.Jsr75File.copyRhoFileFromDeviceMemory(RhoLogger.LOGFILENAME);
		}
	};
		
	/*private MenuItem posItem = new MenuItem("Pos", 100000, 10) {
		public void run() {
				trackwheelRoll(m_nCurLine, 0, 0);
			}
		};*/
	
	private static class MyEdit extends RichTextField{
	};
	
	private class MyTimerTask extends  java.util.TimerTask {
		public void run() {
		synchronized(UiApplication.getEventLock()) {
				trackwheelRoll(m_nCurLine, 0, 0);
			}
		}
	};
	
	void findCurLine(String strLog){
        int nPos = RhoLogger.getLogTextPos();
        m_nCurLine = 0;
        
        for ( int nEndLine = strLog.indexOf('\n'); nEndLine >= 0 && nEndLine<nPos; 
        		nEndLine = strLog.indexOf('\n', nEndLine+1) ) 
        	m_nCurLine ++;
        
        if (m_nCurLine>0)
        	m_nCurLine--;
        
		/*for( int i = 0; i < nPos; i++ ){
			if ( strLog.charAt(i) == '\n' )
				m_nCurLine ++;
		}*/
	}
	
	public LogScreen() 
	{
		setTitle( new LabelField( "Log", LabelField.ELLIPSIS | LabelField.USE_ALL_WIDTH ) );
		
        addMenuItem(optionsItem);
        addMenuItem(clearItem);
        addMenuItem(refreshItem);
        addMenuItem(sendItem);
        addMenuItem(copyLogItem);
        
        m_logText = new MyEdit();
        m_logText.setEditable(false);
        loadLogText();
        
        HorizontalFieldManager hfm=new HorizontalFieldManager(HORIZONTAL_SCROLL);
        //VerticalFieldManager   vfm=new VerticalFieldManager(VERTICAL_SCROLL);        
        hfm.add(m_logText);
        //vfm.add(hfm);
        add(hfm); 

        m_timer.schedule(new MyTimerTask(), 2000);  
	}

	private void loadLogText(){
        String strLog = RhoLogger.getLogText();
        findCurLine(strLog);
        
        m_logText.setText(strLog);
	}
}
