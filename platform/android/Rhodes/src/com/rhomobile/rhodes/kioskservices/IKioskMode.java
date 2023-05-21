package com.rhomobile.rhodes.kioskservices;

public interface IKioskMode {
    public void startKioskMode(boolean use_overlay);
    public void stopKioskMode();
    public boolean isKioskMode();
    public void startOverlay();
    public void stopOverlay();
    public boolean isAllPermissions();
    public void showPermissionsList();
}
