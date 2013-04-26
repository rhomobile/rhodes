package com.rho.camera;

public interface ICameraSingletonObject extends ICameraSingleton {

    int getCameraCount();
    void setDefaultID(int id);
    ICameraObject createCameraObject(String id);
}
