package com.rho.audiocapture;

class AudioCaptureSingleton extends AudioCaptureSingletonBase implements IAudioCaptureSingleton {

    public AudioCaptureSingleton(IAudioCaptureFactory factory) {
        super(factory);
    }

    @Override
    protected String getInitialDefaultID() {
        return "";
    }

    
}
