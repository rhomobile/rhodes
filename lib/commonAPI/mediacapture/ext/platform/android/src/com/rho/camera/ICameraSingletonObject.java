package com.rho.camera;

public interface ICameraSingletonObject extends ICameraSingleton {

    int getCameraCount();
    void setDefaultIndex(int id);
    ICameraObject createCameraObject(String id);
}
