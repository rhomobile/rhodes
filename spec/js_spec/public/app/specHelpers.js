function isAndroidPlatform() {
    return ["ANDROID"] == (Rho.System.platform);
}

function isApplePlatform() {
    return ["APPLE"] == (Rho.System.platform);
}

function isWindowsMobilePlatform() {
    return "WINDOWS" == Rho.System.platform;
}

function isWindowsDesktopPlatform() {
    return "WINDOWS_DESKTOP" == Rho.System.platform;
}

function isAnyWindowsFamilyPlatform() {
    return ["WINDOWS", "WINDOWS_DESKTOP", "WP8"].indexOf(Rho.System.platform) != -1;
}

function isWindowsMobileOrWindowsDesktopPlatform() {
    return ["WINDOWS", "WINDOWS_DESKTOP"].indexOf(Rho.System.platform) != -1;
}

function isAnyButWindowsFamilyPlatform() {
    return !isAnyWindowsFamilyPlatform();
}

function isAnyButAppleAndWindowsMobilePlatform() {
    return ["APPLE", "WINDOWS"].indexOf(Rho.System.platform) == -1;
}

function isWindowsMobileOrAndroidPlatform() {
    return ["WINDOWS", "ANDROID"].indexOf(Rho.System.platform) != -1;
}

function isAnyButApplePlatform() {
    return ["APPLE"] != (Rho.System.platform);
}

function isAndroidOrApplePlatform() {
    return ["ANDROID", "APPLE"].indexOf(Rho.System.platform) != -1;
}


