function isAndroidPlatform(){
    return ["ANDROID"] == (Rho.System.getPlatform());
}

function isApplePlatform(){
    return ["APPLE"] == (Rho.System.getPlatform());
}

function isWindowsMobilePlatform() {
    return "WINDOWS" == Rho.System.getPlatform();
}

function isWindowsDesktopPlatform(){
    return "WINDOWS_DESKTOP" == Rho.System.getPlatform();
}

function isAnyWindowsFamilyPlatform() {
    return ["WINDOWS", "WINDOWS_DESKTOP", "WP8"].indexOf(Rho.System.getPlatform()) != -1;
}

function isWindowsMobileOrWindowsDesktopPlatform() {
    return ["WINDOWS", "WINDOWS_DESKTOP"].indexOf(Rho.System.getPlatform()) != -1;
}

function isAnyButWindowsFamilyPlatform() {
    return !isAnyWindowsFamilyPlatform();
}

function isAnyButAppleAndWindowsMobilePlatform(){
    return ["APPLE", "WINDOWS"].indexOf(Rho.System.getPlatform()) == -1;
}

function isWindowsMobileOrAndroidPlatform() {
    return ["WINDOWS", "ANDROID"].indexOf(Rho.System.getPlatform()) != -1;
}

function isAnyButApplePlatform(){
    return ["APPLE"] != (Rho.System.getPlatform());
}

function isAndroidOrApplePlatform(){
    return ["ANDROID", "APPLE"].indexOf(Rho.System.getPlatform()) != -1;
}


