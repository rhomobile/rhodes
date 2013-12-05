package com.rho.audiocapture;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import android.media.MediaRecorder;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;

public class AudioCapture extends AudioCaptureBase implements IAudioCapture {
    
    private static final String TAG = AudioCapture.class.getSimpleName();
    
    private Map<String, String> mActualPropertyMap = new HashMap<String, String>();
    
    private Map<String, String> makeActualPropertyMap(Map<String, String> props) {
        mActualPropertyMap.putAll(getPropertiesMap());
        mActualPropertyMap.putAll(props);
        return mActualPropertyMap;
    }

    private Map<String, String> getActualPropertyMap() { return mActualPropertyMap; }
    private void clearActualPropertyMap() { mActualPropertyMap.clear(); }

    private MediaRecorder mRecorder;
    
    private MediaRecorder getRecorder() {
        if (mRecorder == null)
            mRecorder = new MediaRecorder();
        return mRecorder;
    }
    
    private int getAudioSource(Map<String, String> props) {
        String sourceProp = props.get("source");
        int source;
        if(sourceProp == null || sourceProp.isEmpty()) {
            source = MediaRecorder.AudioSource.DEFAULT;
        } else if (sourceProp.equalsIgnoreCase(IAudioCaptureSingleton.MIC)) {
            source = MediaRecorder.AudioSource.MIC;
        } else if (sourceProp.equalsIgnoreCase(IAudioCaptureSingleton.CAMCORDER)) {
            source = MediaRecorder.AudioSource.CAMCORDER;
        } else if (sourceProp.equalsIgnoreCase(IAudioCaptureSingleton.CALL_FULLDUPLEX)) {
            source = MediaRecorder.AudioSource.VOICE_CALL;
        } else if (sourceProp.equalsIgnoreCase(IAudioCaptureSingleton.CALL_UPLINK)) {
            source = MediaRecorder.AudioSource.VOICE_UPLINK;
        } else if (sourceProp.equalsIgnoreCase(IAudioCaptureSingleton.CALL_DOWNLINK)) {
            source = MediaRecorder.AudioSource.VOICE_DOWNLINK;
        } else {
            throw new RuntimeException("Unknown audio source: " + sourceProp);
        }
        return source;
    }

    private int getAudioEncoder(Map<String, String> props) {
        String encoderProp = props.get("encoder");
        int encoder;
        if (encoderProp == null || encoderProp.isEmpty()) {
            encoder = MediaRecorder.AudioEncoder.DEFAULT;
        } else if (encoderProp.equalsIgnoreCase(IAudioCaptureSingleton.ENCODER_AAC) || encoderProp.equalsIgnoreCase(IAudioCaptureSingleton.ENCODER_AAC_LC)) {
            encoder = MediaRecorder.AudioEncoder.AAC;
        } else if (encoderProp.equalsIgnoreCase(IAudioCaptureSingleton.ENCODER_AAC_ELD)) {
            encoder = MediaRecorder.AudioEncoder.AAC_ELD;
        } else if (encoderProp.equalsIgnoreCase(IAudioCaptureSingleton.ENCODER_AAC_HE)) {
            encoder = MediaRecorder.AudioEncoder.HE_AAC;
        } else if (encoderProp.equalsIgnoreCase(IAudioCaptureSingleton.ENCODER_AMR_NB)) {
            encoder = MediaRecorder.AudioEncoder.AMR_NB;
        } else if (encoderProp.equalsIgnoreCase(IAudioCaptureSingleton.ENCODER_AMR_WB)) {
            encoder = MediaRecorder.AudioEncoder.AMR_WB;
        } else {
            throw new RuntimeException ("Unknown encoder: " + encoderProp);
        }
        return encoder;
    }
    
    private int getMaxDuration(Map<String, String> props) {
        String maxDurationProp = props.get("maxDuration");
        return (maxDurationProp != null && maxDurationProp.length() != 0) ? Integer.parseInt(maxDurationProp) : 0;
        
    }
    
    private static int getOutputFormat(String path) {
        
        Logger.T(TAG, "Output format for: " + path);
        
        File file = new File(path);
        String filename = file.getName();
        String ext = filename.substring(filename.lastIndexOf('.'));

        if (ext.equalsIgnoreCase(".mp4")) {
            return MediaRecorder.OutputFormat.MPEG_4;
        } else if (ext.equalsIgnoreCase(".3gpp")) {
            return MediaRecorder.OutputFormat.THREE_GPP;
        } else {
            throw new RuntimeException("Unsupported file format: " + ext);
        }
    }

    public AudioCapture(String id){ super(id); }

    @Override
    public String getId() {
        return null;
    }

    @Override
    public synchronized void start(Map<String, String> props, IMethodResult res) {

        try {
            Map<String, String> actualPropertyMap = makeActualPropertyMap(props);

            String path = actualPropertyMap.get("fileName");
            if (path == null || path.length() == 0) {
                Logger.E(TAG, "fileName property cannot be empty!");
                throw new RuntimeException("fileName property is empty");
            }

            MediaRecorder recorder = getRecorder();
            recorder.setAudioSource(getAudioSource(actualPropertyMap));
            recorder.setMaxDuration(getMaxDuration(actualPropertyMap));
            recorder.setOutputFormat(getOutputFormat(path));
            recorder.setOutputFile(path);
            recorder.setAudioEncoder(getAudioEncoder(actualPropertyMap));
            recorder.prepare();
            recorder.start();
        } catch (IllegalStateException ex) {
            throw new RuntimeException(ex);
        } catch (IOException ex) {
            throw new RuntimeException(ex);
        }
       
    }

    @Override
    public synchronized void stop(IMethodResult res) {
        MediaRecorder recorder = getRecorder();
        recorder.stop();
        recorder.release();
        clearActualPropertyMap();
    }

    @Override
    public synchronized void cancel(IMethodResult res) {
        try {
            MediaRecorder recorder = getRecorder();
            recorder.stop();
            recorder.release();
        } finally {
            String path = getActualPropertyMap().get("fileName");
            if (path != null && !path.isEmpty()) {
                File file = new File(path);
                if (file.exists()) {
                    file.delete();
                }
            }
        }
    }

}
