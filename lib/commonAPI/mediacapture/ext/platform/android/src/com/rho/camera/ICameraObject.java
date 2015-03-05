package com.rho.camera;

import android.app.Activity;
import android.view.SurfaceHolder;

public interface ICameraObject extends ICamera {
    interface ISize {
        int getWidth();
        int getHeight();
        int D2();
    }
    void startPreview(SurfaceHolder sh);
    void stopPreview();
    ISize setPreviewSize(int width, int height);
	void setDisplayOrientation(int rotate);
    void doTakePicture(Activity previewActivity, int rotation);

}
