package com.rhomobile.rhodes.kioskservices;

public interface IKioskMode {
    public void startKioskMode(String password);
    public void stopKioskMode(String password);
    public boolean isKioskMode();
    public void startOverlay();
    public void stopOverlay();
    public boolean isAllPermissions();
    public void showPermissionsList();
}
