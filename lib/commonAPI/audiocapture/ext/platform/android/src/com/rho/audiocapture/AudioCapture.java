package com.rho.audiocapture;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import android.graphics.Rect;
import android.media.MediaRecorder;
import android.media.MediaRecorder.OnErrorListener;
import android.media.MediaRecorder.OnInfoListener;
import android.os.Environment;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;
import com.rhomobile.rhodes.extmanager.IRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoWebView;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

public class AudioCapture extends AudioCaptureBase implements IAudioCapture {
    
    private static final String TAG = AudioCapture.class.getSimpleName();
    
    private IMethodResult storedMethodResult=null;
    
    private String filepath=null;
    
    private Map<String, String> mActualPropertyMap = new HashMap<String, String>();
    
     private boolean stopApi=false;
    
    private AudioCaptureExt ext=null;
    private void initWithDefaultValues()
    {
    	ext=new AudioCaptureExt();
    	RhoExtManager.getInstance().registerExtension("audiocapture", ext);
    	
    	if(mActualPropertyMap.size()==0)
    	{
    		mActualPropertyMap.put("maxDuration", "20000");
    								
        	//mActualPropertyMap.put("fileName", "AudioCapture");
    		mActualPropertyMap.put("fileName", "");
        	mActualPropertyMap.put("encoder","AAC");
        	mActualPropertyMap.put("source", "mic");
    	}
    	
    }
    
   @Override
    public void getAllProperties(IMethodResult result) {
    	
    	//super.getAllProperties(result);
    	
    	
    	Map<String, Object> props = new HashMap<String, Object>();
    	Iterator entries = mActualPropertyMap.entrySet().iterator();
    	while (entries.hasNext()) {
    	  Entry thisEntry = (Entry) entries.next();
    	  String key = (String) thisEntry.getKey();
    	  Object value = thisEntry.getValue();
    	  props.put(key, value);
    	  
    	}
    	result.set(props);
    }
    
    @Override
    public void getEncoder(IMethodResult result) {
    	
    	//super.getEncoder(result);
    	
    	result.set(mActualPropertyMap.get("encoder"));
    	
    	
    	
    }
    
    @Override
    public void getFileName(IMethodResult result) {
    	
    	//super.getFileName(result);
    	
    	result.set(mActualPropertyMap.get("fileName"));
    }
    
    @Override
    public void getMaxDuration(IMethodResult result) {
    	
    	//super.getMaxDuration(result);
    	try{
    	result.set(Integer.parseInt(mActualPropertyMap.get("maxDuration")));
    	}
    	catch(NumberFormatException e)
    	{
    		System.out.println("getMaxDuration,error="+e.getMessage());
    		Logger.E(TAG, "Error in getMaxDuration= "+e.getMessage());
    		result.set(0);
    	}
    }
    
    @Override
    public void getProperties(List<String> arrayofNames, IMethodResult result) {
    	
    	//super.getProperties(arrayofNames, result);
    	
    	Map<String, Object> props = new HashMap<String, Object>();
        for (String name: arrayofNames)
        {
        	if(name.equalsIgnoreCase("maxDuration"))
        	{
        		try{
        		props.put(name,Integer.parseInt(mActualPropertyMap.get(name)) );
        		}
        		catch(NumberFormatException e)
        		{
        			System.out.println("getProperties()..error="+e.getMessage());
        			Logger.E(TAG, "Error in getProperties= "+e.getMessage());
        			props.put(name,0 );
        		}
        	}
        	else
                props.put(name,mActualPropertyMap.get(name) );
        }
        result.set(props);
    }
    
    @Override
    public void getProperty(String propertyName, IMethodResult result) {
    	
    	//super.getProperty(propertyName, result);
    	
    	try{
    	if(propertyName.equalsIgnoreCase("maxDuration"))
    		result.set(Integer.parseInt(mActualPropertyMap.get(propertyName)));
    	else
    	result.set(mActualPropertyMap.get(propertyName));
    	}
    	catch(NumberFormatException e)
    	{
    		Logger.E(TAG, "Error getProperty="+e.getMessage());
    		result.set(0);
    	}
    }
    
    @Override
    public void getSource(IMethodResult result) {
    	
    	//super.getSource(result);
    	
    	result.set(mActualPropertyMap.get("source"));
    }
    
    @Override
    public void setEncoder(String encoder, IMethodResult result) {
    	
    	//super.setEncoder(encoder, result);
    	
    	mActualPropertyMap.put("encoder", encoder);
    	result.set(true);
    }
    
    @Override
    public void setFileName(String fileName, IMethodResult result) {
    	
    	//super.setFileName(fileName, result);
    	
    	mActualPropertyMap.put("fileName", fileName);
    	result.set(true);
    }
    
    @Override
    public void setMaxDuration(int maxDuration, IMethodResult result) {
    	
    	//super.setMaxDuration(maxDuration, result);
    	if(maxDuration<1000)
    		maxDuration=20000;
    	mActualPropertyMap.put("maxDuration", Integer.toString(maxDuration));
    	System.out.println("OLAA,setMaxDuration.....   maxDuration="+maxDuration);
    	result.set(true);
    }
    
    @Override
    public void setProperties(Map<String, String> propertyMap,
    		IMethodResult result) {
    	
    	//super.setProperties(propertyMap, result);
    	System.out.println("setProperties...propertyMap="+propertyMap);
    	mActualPropertyMap.putAll(propertyMap);
    	if(propertyMap.containsKey("maxDuration"))
    	{
    		int x=0;
    		try{
    			x=Integer.parseInt(propertyMap.get("maxDuration"));
    		}
			catch(NumberFormatException e)
			{
				x=0;
				Logger.E(TAG, "Error setProperties= "+e.getMessage());
				System.out.println("error="+e.getMessage());
				}
    		if(x<1000)
    			mActualPropertyMap.put("maxDuration", "20000");
    		else
    			mActualPropertyMap.put("maxDuration", Integer.toString(x));
    		System.out.println("OLAA,setProperties....   maxDuration="+x);
    	}
    	result.set(true);
    }
    
    @Override
    public void setProperty(String propertyName, String propertyValue,
    		IMethodResult result) {
    	
    	// super.setProperty(propertyName, propertyValue, result);
    	if(propertyName.equalsIgnoreCase("maxDuration"))
    	{
    		try{
    		if(Integer.parseInt(propertyValue)<1000)
    			mActualPropertyMap.put("maxDuration", "20000");
    		else
    			mActualPropertyMap.put(propertyName, propertyValue);
    		}
    		catch(NumberFormatException e)
    		{
    			System.out.println("Error="+e.getMessage());
    			Logger.E(TAG, "Error="+e.getMessage());
    			mActualPropertyMap.put("maxDuration", "20000");
    		}
    		System.out.println("OLAA,setProperty..   maxDuration="+propertyValue);
    	}
    	else
    		mActualPropertyMap.put(propertyName, propertyValue);
    	result.set(true);
    }
    
    @Override
    public void setSource(String source, IMethodResult result) {
    	
    	//super.setSource(source, result);
    	
    	mActualPropertyMap.put("source", source);
    	result.set(true);
    }
    
    
    
    
   
    
    private Map<String, String> makeActualPropertyMap(Map<String, String> props) {
       // mActualPropertyMap.putAll(getPropertiesMap());
        mActualPropertyMap.putAll(props);
        System.out.println("mActualPropertyMap="+mActualPropertyMap);
        
        for (Entry<String, String> entry : mActualPropertyMap.entrySet()) {
            System.out.println("entry= "+entry);
        }
      
        
        return mActualPropertyMap;
    }
    
    private Map<String, String> getActualPropertyMap() { return mActualPropertyMap; }
    private void clearActualPropertyMap() { mActualPropertyMap.clear(); }
    
    private MediaRecorder mRecorder;
    
    private MediaRecorder getRecorder() {
        if (mRecorder == null)
            {
        		mRecorder = new MediaRecorder();
        		
            }
        return mRecorder;
    }
    
    private void releaseRecorder() {
        if (mRecorder != null) {
            mRecorder.stop();
            mRecorder.release();
            mRecorder = null;
        }
    }
    
    private int getAudioSource(Map<String, String> props) {
        String sourceProp = props.get("source");
        int source;
        if(sourceProp == null || sourceProp.isEmpty()) {
            //source = MediaRecorder.AudioSource.DEFAULT;
        	source = MediaRecorder.AudioSource.MIC;
        } else if (sourceProp.equalsIgnoreCase(IAudioCaptureSingleton.MIC)) {
            source = MediaRecorder.AudioSource.MIC;
        
        } else {
            throw new RuntimeException("Unknown audio source: " + sourceProp);
        }
        return source;
    }

    private int getAudioEncoder(Map<String, String> props) {
        String encoderProp = props.get("encoder");
        int encoder;
        if (encoderProp == null || encoderProp.isEmpty()) {
           // encoder = MediaRecorder.AudioEncoder.DEFAULT;
        	 encoder = MediaRecorder.AudioEncoder.AAC;
        } else if (encoderProp.equalsIgnoreCase(IAudioCaptureSingleton.ENCODER_AAC) /*|| encoderProp.equalsIgnoreCase(IAudioCaptureSingleton.ENCODER_AAC_LC)*/) {
            encoder = MediaRecorder.AudioEncoder.AAC;
        }
        else if (encoderProp.equalsIgnoreCase(IAudioCaptureSingleton.ENCODER_AMR_NB)) {
            encoder = MediaRecorder.AudioEncoder.AMR_NB;
        } else if (encoderProp.equalsIgnoreCase(IAudioCaptureSingleton.ENCODER_AMR_WB)) {
            encoder = MediaRecorder.AudioEncoder.AMR_WB;
        } else {
            throw new RuntimeException ("Unknown encoder: " + encoderProp);
        }
        return encoder;
    }
    
    private int getAudioMaxDuration(Map<String, String> props) {
    	System.out.println("*****Reached****, props="+props);
    	String maxDurationProp=null;
    	try{
    		maxDurationProp= props.get("maxDuration");
    	}
    	catch(Exception e)
    	{
    		System.out.println("OMG.error="+e.getMessage());
    	}
    	
        System.out.println("maxDurationProp="+maxDurationProp);
        int maxD=20000;
        if(maxDurationProp != null && maxDurationProp.length() != 0)
	        {
		        	try{
			        	maxD=Integer.parseInt(maxDurationProp);
			        }
			        catch(NumberFormatException e)
			        {
			        	System.out.println("error="+e.getMessage());
			        	Logger.E(TAG, "Error="+e.getMessage());
			        }
		        	if(maxD<1000)
		        	maxD=20000;//set to Default if it is less than 20000 mili sec
        }
        else
        	maxD=20000;
       // return (maxDurationProp != null && maxDurationProp.length() != 0) ? maxD : 20000;//20,000 milisec is the default value....
        return maxD;
        
    }
    
    private static int getOutputFormat(String path,int audioEncoder) {
        
        Logger.T(TAG, "Output format for: " + path);
        String ext ="";
        if(path.contains("."))
        { 
        	File file = new File(path);
        	String filename = file.getName();
        	ext= filename.substring(filename.lastIndexOf('.'));
        }
        
        if (ext.equalsIgnoreCase(".mp4")) {
            return MediaRecorder.OutputFormat.MPEG_4;
        } else if (ext.equalsIgnoreCase(".3gpp")) {
            return MediaRecorder.OutputFormat.THREE_GPP;
        } 
        
        else {
        	switch(audioEncoder)
        	{
        	case MediaRecorder.AudioEncoder.AAC:System.out.println("Inside-AAC"); return MediaRecorder.OutputFormat.MPEG_4;
        	case MediaRecorder.AudioEncoder.AMR_NB:System.out.println("Inside-AMR_NB");return MediaRecorder.OutputFormat.THREE_GPP;
        	case MediaRecorder.AudioEncoder.AMR_WB:System.out.println("Inside-AMR_WB");return MediaRecorder.OutputFormat.THREE_GPP;
        	 default:                        System.out.println("Inside-...");return MediaRecorder.OutputFormat.MPEG_4;
        	}
        	
            //throw new RuntimeException("Unsupported file format: " + ext);
        }
    }

    public AudioCapture(String id){ super(id);initWithDefaultValues();Logger.D(TAG, "Constructor"); }

    @Override
    public String getId() {
        return null;
    }

    private String getDefaultPath(Map<String,String> props)
    {
    	int audioEncoder=getAudioEncoder(props);
    	String defaultPath=Environment.getExternalStorageDirectory().getAbsolutePath().concat("/");
    	switch(audioEncoder)
    	{
    	case MediaRecorder.AudioEncoder.AAC: return defaultPath.concat("AudioCapture.mp4");
    	case MediaRecorder.AudioEncoder.AMR_NB:return defaultPath.concat("AudioCapture.3gpp");
    	case MediaRecorder.AudioEncoder.AMR_WB: return defaultPath.concat("AudioCapture.3gpp");
    	default:return defaultPath.concat("AudioCapture.mp4");
    	
    	}
    	
    }
    @Override
    public synchronized void start(Map<String, String> props, IMethodResult res) {
    	
    	System.out.println("Audio Capture Start is called");
    	Logger.D(TAG, "start+");
    	if(!res.hasCallback())
    		{
    			// Dont Do Anything...as CallBack is NOT provided...
    			Logger.I(TAG, "No CallBack is provided. Hence returning...");
    			return;
    		}
    	
    	
    	if(storedMethodResult!=null)
    	{
    		//Start is already triggered and it is running currently.... DOnt start it again...
    		Logger.I(TAG, "Start is already triggered and it is running currently.... DOnt start it again...");
    		return;
    	}
    	
    	
    	storedMethodResult=res;
    	System.out.println("Start storedMethodResult="+storedMethodResult);

    	
        try {
        	System.out.println("Start->1");
            Map<String, String> actualPropertyMap = makeActualPropertyMap(props);
            System.out.println("Start->2");
            String path = actualPropertyMap.get("fileName");
            
            System.out.println("Start->3");
            if (path == null || path.length() == 0 || path.equalsIgnoreCase("")) {
                Logger.E(TAG, "fileName property cannot be empty! Taking the default path...");
                //path=getDefaultPath(actualPropertyMap);//Default  file path concept is removed in ARB meeting...
                //throw new RuntimeException("fileName property is empty");
                Map<String, Object> tempprops = new HashMap<String, Object>();
                tempprops.put("status", "error");
                tempprops.put("message", "fileName property is NOT set");
                tempprops.put("fileName", "");
                if(storedMethodResult!=null)
                	{
                		storedMethodResult.set(tempprops);
                		System.out.println("Stop storedMethodResult="+storedMethodResult);
                	}
                storedMethodResult=null;
                return;
            }
           
            System.out.println("Start->4");

            int audioSource = getAudioSource(actualPropertyMap);
            System.out.println("Start->5");
            int maxDuration = getAudioMaxDuration(actualPropertyMap);
            System.out.println("Start->6");
            int audioEncoder = getAudioEncoder(actualPropertyMap);
            System.out.println("Start->7");
            int outputFormat = getOutputFormat(path,audioEncoder);
            if(!(path.contains("/")))
            		{
            			//path="/sdcard/".concat(path);
            			  path=Environment.getExternalStorageDirectory().getAbsolutePath().concat("/").concat(path);
            		}
            
            if(!(path.contains(".")))
            {
            		switch(outputFormat)
            		{
            		case MediaRecorder.OutputFormat.MPEG_4: path=path.concat(".mp4");break;
            		case MediaRecorder.OutputFormat.THREE_GPP: path=path.concat(".3gpp");break;
            		
            		}
            }
           
            System.out.println("outputFormat is ="+outputFormat);
            
            System.out.println("Start->8");
            
            Logger.T(TAG, "Start audio recording: source: " + audioSource + ", max duration: " + maxDuration + ", encoder: " + audioEncoder + ", format: " + outputFormat + ", path: " + path);
            
            MediaRecorder recorder = getRecorder();
            
            
            recorder.setOnInfoListener(new OnInfoListener() {
            	@Override
            	public void onInfo(MediaRecorder mr, int what, int extra) {     
            		switch (what)
            		{
            		case MediaRecorder.MEDIA_RECORDER_INFO_MAX_DURATION_REACHED:
            			System.out.println("Max Duration Reached");
            			Logger.I(TAG, "Max Duration has reached");
            			stop(null);
            			break;
            		case MediaRecorder.MEDIA_RECORDER_INFO_MAX_FILESIZE_REACHED:
            			System.out.println("Max FIle Size Reached");
            			stop(null);
            			break;
            		case MediaRecorder.MEDIA_RECORDER_INFO_UNKNOWN:
            			
            			break;        		
            		}
            	}
            });
            
            recorder.setOnErrorListener(new OnErrorListener() {
            	@Override
            	public void onError(MediaRecorder mr, int what, int extra) {
            		switch(what)
            		{
            		case MediaRecorder.MEDIA_RECORDER_ERROR_UNKNOWN:
            			
            			//cancel(null);
            			stopNsend("error");
            			break;
            		}
            	}
            });
            
            
            
            
            System.out.println("Start->9,,audioSource="+audioSource);
            try{
            recorder.setAudioSource(audioSource);
            }
            catch(Exception e)
            {
            	System.out.println("AudioSource error="+e.getMessage());
            	Logger.E(TAG, "AudioSource error="+e.getMessage());
            }
            
            System.out.println("Start->10");
            recorder.setMaxDuration(maxDuration);
            System.out.println("Start->11,maxDuration="+maxDuration+" outputFormat="+outputFormat);
            recorder.setOutputFormat(outputFormat);
            System.out.println("Start->12,outputFormat="+outputFormat);
            recorder.setOutputFile(path);
            filepath=path;//to be sent during callback
            System.out.println("Start->13,path="+path);
            recorder.setAudioEncoder(audioEncoder);
            System.out.println("Start->14,audioEncoder="+audioEncoder);
            
            recorder.prepare();
            System.out.println("Start->15");
            recorder.start();
            System.out.println("Start->16");
        } catch (IllegalStateException ex) {
        	System.out.println("Start->17,error="+ex.getMessage());
            throw new RuntimeException(ex);
        } catch (IOException ex) {
        	System.out.println("Start->18");
            throw new RuntimeException(ex);
        }
       
    }

    @Override
    public synchronized void stop(IMethodResult res) {
    	stopApi=true;
    	System.out.println("Audio Capture Stop is called");
        try {
            releaseRecorder();
        } finally {
          //  clearActualPropertyMap();
        	stopNsend("stop");
        }
        
    }

    @Override
    public synchronized void cancel(IMethodResult res) {
    	System.out.println("Audio Capture Cancel is called");
        try {
            releaseRecorder();
        } finally {
        	System.out.println("18");
            //String path = getActualPropertyMap().get("fileName");
            //System.out.println("19,path="+path);
            if (filepath != null && !filepath.isEmpty()) {
                File file = new File(filepath);
                if (file.exists()) {
                	 System.out.println("20,exists="+true);
                    boolean del=file.delete();
                    System.out.println("20,deletion happens or NOT="+true);
                }
            }
           // clearActualPropertyMap();
           stopNsend("cancel");
        }
    }
    private synchronized void stopNsend(String status)
    {
    	
    	 Map<String, Object> tempprops = new HashMap<String, Object>();
         
        
    	if(status.equalsIgnoreCase("stop"))
    	{
    		String filrProtocolPath="file://".concat(filepath);
    		tempprops.put("status", "ok");
    		tempprops.put("message", "");
    		tempprops.put("fileName", filrProtocolPath);
    	}
    	else if(status.equalsIgnoreCase("cancel"))
    	{
    		tempprops.put("status", "cancel");
    		tempprops.put("message", "");
    		tempprops.put("fileName", "");
    	}
    	else if(status.equalsIgnoreCase("error"))
    	{
    		
    		System.out.println("Audio Error Has Happened");
            try {
                releaseRecorder();
            } finally {
                //String path = getActualPropertyMap().get("fileName");
                if (filepath != null && !filepath.isEmpty()) {
									                    File file = new File(filepath);
									                    if (file.exists()) {
									                        				file.delete();
									                    					}
                									}
                
    		tempprops.put("status", "error");
    		tempprops.put("message", "Unknown Error");
    		tempprops.put("fileName", "");
    	}
            
    	}
    	 if(storedMethodResult!=null)
      	{
      		storedMethodResult.set(tempprops);
      		System.out.println("stopNsend storedMethodResult="+storedMethodResult);
      	}
      storedMethodResult=null;
    	
    }
class AudioCaptureExt extends AbstractRhoExtension
{

	@Override
	public boolean onBeforeNavigate(IRhoExtManager extManager, String url,
			IRhoWebView ext, boolean res) {
		// TODO Auto-generated method stub
	System.out.println("AbstractRhoExtension..onBeforeNavigate,stopApi="+stopApi);
		if(stopApi==false)
		cancel(null);
		
		stopApi=false;
		
		return false;
	}

	
	}

}
