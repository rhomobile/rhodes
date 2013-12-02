package com.rho.audiocapture;

public class AudioCaptureFactory implements IAudioCaptureFactory {

    AudioCaptureSingleton mSingleton;
    
    @Override
    public AudioCaptureSingleton getApiSingleton() {
        if(mSingleton == null)
            mSingleton = new AudioCaptureSingleton();
        return mSingleton;
    }

    @Override
    public AudioCapture getApiObject(String id) {
        return null;
    }

}
