package com.rho.audiocapture;

public class AudioCaptureFactory implements IAudioCaptureFactory {

    AudioCaptureSingleton mSingleton;
    AudioCapture mInstance;
    
    @Override
    public AudioCaptureSingleton getApiSingleton() {
        if (mSingleton == null)
            mSingleton = new AudioCaptureSingleton(this);
        return mSingleton;
    }

    @Override
    public AudioCapture getApiObject(String id) {
        if (mInstance == null)
            mInstance = new AudioCapture("id");
        return mInstance;
    }

}
